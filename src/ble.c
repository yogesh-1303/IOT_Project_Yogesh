/*
 * ble.c
 * Attributes - soc_thermometer.c
 *              Prof.David Sluiter Lecture 10 & 11
 *
 */

#include "ble.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG (1)

#include "src/log.h"

#define SL_BT_HT_TEMPERATURE_TYPE          (2)         ///SL_BT_HT_TEMPERATURE_TYPE_BODY

#define MEASUREMENT_INTERVAL               (2)

#define PASSIVE_SCANNING                   (0)

#define SCANNING_INTERVAL                  (80)

#define SCANNING_WINDOW                    (40)

#if (DEVICE_IS_BLE_SERVER == 0)
// Health Thermometer service UUID defined by Bluetooth SIG
static const uint8_t thermo_service[2] = { 0x09, 0x18 };
// Temperature Measurement characteristic UUID defined by Bluetooth SIG
static const uint8_t thermo_char[2] = { 0x1c, 0x2a };
#endif

// BLE private data
ble_data_struct_t ble_data = {.temp_measure_status=false,.temp_type_status=false,.temp_interval_status=false,.enable_measurement=false};


ble_data_struct_t* getBleDataPtr(){

  return (&ble_data);

}

#if (DEVICE_IS_BLE_SERVER == 0)
// -----------------------------------------------
// Private function, original from Dan Walkes. I fixed a sign extension bug.
// We'll need this for Client A7 assignment to convert health thermometer
// indications back to an integer. Convert IEEE-11073 32-bit float to signed integer.
// -----------------------------------------------
static int32_t FLOAT_TO_INT32(const uint8_t *value_start_little_endian)
{
    uint8_t signByte = 0;
    int32_t mantissa;
    // input data format is:
    // [0] = flags byte
    // [3][2][1] = mantissa (2's complement)
    // [4] = exponent (2's complement)
    // BT value_start_little_endian[0] has the flags byte
    int8_t exponent = (int8_t)value_start_little_endian[4];
    // sign extend the mantissa value if the mantissa is negative
    if (value_start_little_endian[3] & 0x80) { // msb of [3] is the sign of the mantissa
    signByte = 0xFF;
    }
    mantissa = (int32_t) (value_start_little_endian[1] << 0) |
    (value_start_little_endian[2] << 8) |
    (value_start_little_endian[3] << 16) |
    (signByte << 24) ;
    // value = 10^exponent * mantissa, pow() returns a double type
    return (int32_t) (pow(10, exponent) * mantissa);
} // FLOAT_TO_INT32
#endif


void transmit_tempdata(sl_bt_msg_t *evt,uint16_t attribute){

  sl_status_t sc;

  // Update our local GATT DB and send indication if enabled for the characteristic// -------------------------------------------------------------------
  uint8_t   htm_temperature_buffer[5];   // Stores the temperature data in the Health Thermometer (HTM) format.// format of the buffer is: flags_byte + 4-bytes of IEEE-11073 32-bit float
  uint8_t   *p = htm_temperature_buffer; // Pointer to HTM temperature buffer needed for converting values to bitstream.
  uint32_t  htm_temperature_flt;         // Stores the temperature data read from the sensor in the IEEE-11073 32-bit float format
  uint8_t   flags = 0x00;                // HTM flags set as 0 for Celsius, no time stamp and no temperature type.// "bitstream" refers to the order of bytes and bits sent. byte[0] is sent first, followed by byte[1]...
  UINT8_TO_BITSTREAM(p, flags); // put the flags byte in first, "convert" is a strong word, it places the byte into the buffer// Convert sensor data to IEEE-11073 32-bit floating point format.
  htm_temperature_flt = UINT32_TO_FLOAT(read_data*1000, -3); // Convert temperature to bitstream and place it in the htm_temperature_buffer
  UINT32_TO_BITSTREAM(p, htm_temperature_flt);

  // client characteristic configuration changed by remote GATT client
 if (sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

     // Check if notifications and indications are not disabled
     if(((sl_bt_gatt_client_config_flag_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags) != sl_bt_gatt_disable){
     // -------------------------------// Write our local GATT DB// -------------------------------
     sc = sl_bt_gatt_server_write_attribute_value(attribute,0,5,&htm_temperature_buffer[0]);
     if (sc != SL_STATUS_OK)
       {
         LOG_ERROR("sl_bt_gatt_server_write_attribute_value() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
       }


     sc = sl_bt_gatt_server_send_indication(evt->data.evt_gatt_server_characteristic_status.connection,evt->data.evt_gatt_server_characteristic_status.characteristic,5,&htm_temperature_buffer[0]);

     if (sc != SL_STATUS_OK)
     {
        LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
     }

     else {
         ble_data_struct_t* data = getBleDataPtr();
         data->temp_measure_status = true;
     }


     //Display Temperature on LCD
     displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp=%d",read_data);
  }

     else if (sl_bt_gatt_disable == ((sl_bt_gatt_client_config_flag_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags)){

         displayPrintf(DISPLAY_ROW_TEMPVALUE,"");

     }

 }

 // Confirmation of indication received from remove GATT client
 else if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

          ble_data_struct_t* data = getBleDataPtr();
          data->temp_measure_status = false;

 }

}

void transmit_temptype(sl_bt_msg_t *evt,uint16_t attribute){

  sl_status_t sc;

  uint8_t temperature_type = SL_BT_HT_TEMPERATURE_TYPE;


  if (sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

      // Check if notifications and indications are not disabled
      if(((sl_bt_gatt_client_config_flag_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags) != sl_bt_gatt_disable){

      // -------------------------------// Write our local GATT DB// -------------------------------
      sc = sl_bt_gatt_server_write_attribute_value(attribute,0,sizeof(temperature_type),&temperature_type);

      if (sc != SL_STATUS_OK)
       {
          LOG_ERROR("sl_bt_gatt_server_write_attribute() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
       }

       sc = sl_bt_gatt_server_send_indication(evt->data.evt_gatt_server_characteristic_status.connection,evt->data.evt_gatt_server_characteristic_status.characteristic,sizeof(temperature_type),&temperature_type);

       if (sc != SL_STATUS_OK)
       {
          LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
       }

       else {
           ble_data_struct_t* data = getBleDataPtr();
           data->temp_type_status = true;
       }
   }

}

  // Confirmation of indication received from remove GATT client
   else if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

        ble_data_struct_t* data = getBleDataPtr();
        data->temp_type_status = false;
   }
}


void transmit_tempinterval(sl_bt_msg_t *evt,uint16_t attribute){

  sl_status_t sc;

  uint16_t measurement_interval = MEASUREMENT_INTERVAL;

  if (sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

      // Check if notifications and indications are not disabled
      if(((sl_bt_gatt_client_config_flag_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags) != sl_bt_gatt_disable){

          // -------------------------------// Write our local GATT DB// -------------------------------
        sc = sl_bt_gatt_server_write_attribute_value(attribute,0,sizeof(measurement_interval),(uint8_t *)&measurement_interval);

        if (sc != SL_STATUS_OK)
         {
            LOG_ERROR("sl_bt_gatt_server_write_attribute() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
         }

         sc = sl_bt_gatt_server_send_indication(evt->data.evt_gatt_server_characteristic_status.connection,evt->data.evt_gatt_server_characteristic_status.characteristic,sizeof(measurement_interval),(uint8_t *)&measurement_interval);

         if (sc != SL_STATUS_OK)
         {
            LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
         }

         else {
             ble_data_struct_t* data = getBleDataPtr();
             data->temp_interval_status = true;
         }
     }

 }

    // confirmation of indication received from remove GATT client
     else if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {
              ble_data_struct_t* data = getBleDataPtr();
              data->temp_interval_status = false;
   }
}






void handle_ble_event(sl_bt_msg_t *evt){

  sl_status_t sc;

  switch(SL_BT_MSG_ID(evt->header)){

    case sl_bt_evt_system_boot_id:{

#if (DEVICE_IS_BLE_SERVER == 1)

      char string_buffer[50];

      // Initialize LCD Display
      displayInit();

      displayPrintf(DISPLAY_ROW_NAME,"SERVER");


      // Extract unique ID from BT Address.
      sc = sl_bt_system_get_identity_address(&ble_data.myAddress, &ble_data.myAddressType);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("Error in getting identity\n\r");
          break;
      }

      sprintf(string_buffer,"%x:%x:%x:%x:%x:%x",ble_data.myAddress.addr[0],ble_data.myAddress.addr[1],ble_data.myAddress.addr[2],ble_data.myAddress.addr[3],ble_data.myAddress.addr[4],ble_data.myAddress.addr[5]);
      displayPrintf(DISPLAY_ROW_BTADDR,"%s", string_buffer);

      displayPrintf(DISPLAY_ROW_ASSIGNMENT,"A7");

      //LOG_INFO("\nIdentity Retrieved");
      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&ble_data.advertisingSetHandle);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("\nError in advertising\n\r");
          break;
      }

      // Set advertising interval to 100ms.
       sc = sl_bt_advertiser_set_timing(
             ble_data.advertisingSetHandle, // advertising set handle
              400, // min. adv. interval (milliseconds * 1.6)
              400, // max. adv. interval (milliseconds * 1.6)
              0,   // adv. duration
              0);  // max. num. adv. events
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in setting advertising time\n\r");
           break;
       }

       /*Security Manager*/


       sc = sl_bt_sm_delete_bondings();
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in deleting bonds\n\r");
           break;
       }


       sc = sl_bt_sm_configure(0x0f,sl_bt_sm_io_capability_displayyesno);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in configuring security manager\n\r");
           break;
       }



       sc = sl_bt_advertiser_start(
              ble_data.advertisingSetHandle,
              sl_bt_advertiser_general_discoverable,
              sl_bt_advertiser_connectable_scannable);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in starting advertising\n\r");
           break;
       }

       //DISPLAY_ROW_CONNECTION
       displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising");



#endif

      // CLient Implememntation

#if (DEVICE_IS_BLE_SERVER == 0)

       char string_buffer[50];


       // Initialize LCD Display
       displayInit();

       displayPrintf(DISPLAY_ROW_NAME,"CLIENT");


       // Extract unique ID from BT Address.
       sc = sl_bt_system_get_identity_address(&ble_data.myAddress, &ble_data.myAddressType);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in getting identity\n\r");
           break;
       }

       sprintf(string_buffer,"%x:%x:%x:%x:%x:%x",ble_data.myAddress.addr[0],ble_data.myAddress.addr[1],ble_data.myAddress.addr[2],ble_data.myAddress.addr[3],ble_data.myAddress.addr[4],ble_data.myAddress.addr[5]);
       displayPrintf(DISPLAY_ROW_BTADDR,"%s", string_buffer);

       displayPrintf(DISPLAY_ROW_ASSIGNMENT,"A7");

       // Configure for passive scanning
       sc = sl_bt_scanner_set_mode(sl_bt_gap_1m_phy, PASSIVE_SCANNING);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in setting scanning mode\n\r");
           break;
       }

       sc = sl_bt_scanner_set_timing(sl_bt_gap_1m_phy,SCANNING_INTERVAL, SCANNING_WINDOW);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in setting scanning timing\n\r");
           break;
       }


       sc = sl_bt_connection_set_default_parameters(60,60,4,300,0,0xffff);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in setting parameters\n\r");
           break;
       }

       sc = sl_bt_scanner_start(sl_bt_gap_1m_phy,sl_bt_scanner_discover_observation);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in starting scanning\n\r");
           break;
       }

       //DISPLAY_ROW_CONNECTION
       displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");

#endif
      break;
    }

    case sl_bt_evt_connection_opened_id:{

#if (DEVICE_IS_BLE_SERVER == 1)

      ble_data_struct_t* data = getBleDataPtr();
      data->enable_measurement = true;

      displayPrintf(DISPLAY_ROW_CONNECTION,"Connected");

      //LOG_INFO("Connection Open\n\r");

      sc =  sl_bt_advertiser_stop(ble_data.advertisingSetHandle);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("Error in stopping advertising\n\r");
          break;
      }

      sc = sl_bt_connection_set_parameters ( (evt->data.evt_connection_opened.connection),
      60,
      60,
      4,
      300,
      0,
      0xffff);

      if (sc != SL_STATUS_OK) {

          //LOG_ERROR("Error in setting connection parameters\n\r");
      }
#endif

#if (DEVICE_IS_BLE_SERVER == 0)

      //DISPLAY_ROW_CONNECTION
      displayPrintf(DISPLAY_ROW_CONNECTION,"Connected");

      ble_data.new_connection = evt->data.evt_connection_opened.connection;
      ble_data.newconnection_status =  true;

      // Set event
      schedulerSetConnection_OpenedEvent();

#endif
      break;
    }


    case sl_bt_evt_connection_closed_id:{

#if (DEVICE_IS_BLE_SERVER == 1)

      ble_data_struct_t* data = getBleDataPtr();

      // Close connection only if there is no indication in flight

      while( data->temp_measure_status==true || data->temp_type_status == true ||data->temp_interval_status  == true );

      // Disable temperature measurement by switching off temperarture state machine
      data->enable_measurement = false;

      //LOG_INFO("Connection Closed\n\r");

      displayPrintf(DISPLAY_ROW_CONNECTION,"");
      displayPrintf(DISPLAY_ROW_TEMPVALUE,"");
      displayPrintf(DISPLAY_ROW_CONNECTION,"Advertising");

      sc = sl_bt_advertiser_start(
                    ble_data.advertisingSetHandle,
                    sl_bt_advertiser_general_discoverable,
                    sl_bt_advertiser_connectable_scannable);
             if (sc != SL_STATUS_OK) {
                 LOG_ERROR("Error in starting advertising\n\r");
                 break;
             }

      sc = sl_bt_sm_delete_bondings();
              if (sc != SL_STATUS_OK) {
                  LOG_ERROR("Error in deleting bonds\n\r");
                  break;
              }

#endif

#if (DEVICE_IS_BLE_SERVER == 0)

     // Set event
     schedulerSetConnection_ClosedEvent();


     displayPrintf(DISPLAY_ROW_CONNECTION,"");
     displayPrintf(DISPLAY_ROW_TEMPVALUE,"");
     displayPrintf(DISPLAY_ROW_BTADDR2,"");
     displayPrintf(DISPLAY_ROW_CONNECTION,"Discovering");


     sc = sl_bt_scanner_start(sl_bt_gap_1m_phy,sl_bt_scanner_discover_generic);
     if (sc != SL_STATUS_OK) {
        LOG_ERROR("Error in starting scanning\n\r");
        break;
     }



#endif

      break;
    }

#if (DEVICE_IS_BLE_SERVER == 1)
    case  sl_bt_evt_connection_parameters_id:{

      /*
      LOG_INFO("Connection params: connection=%d, interval=%d, latency=%d, timeout=%d, securitymode=%d",
               (int) (evt->data.evt_connection_parameters.connection),
               (int) (evt->data.evt_connection_parameters.interval*1.25),
               (int) (evt->data.evt_connection_parameters.latency),
               (int) (evt->data.evt_connection_parameters.timeout*10),
               (int) (evt->data.evt_connection_parameters.security_mode) );
       */

      break;
    }

    case sl_bt_evt_gatt_server_characteristic_status_id:{

      if  (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_measurement){

          transmit_tempdata(evt,gattdb_temperature_measurement);

      }

      if  (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_temperature_type ){

          transmit_temptype(evt,gattdb_temperature_type);


      }

      if  (evt->data.evt_gatt_server_characteristic_status.characteristic == gattdb_measurement_interval){

          transmit_tempdata(evt,gattdb_measurement_interval);

      }

      break;
    }


    case sl_bt_evt_gatt_server_indication_timeout_id:{

      ble_data_struct_t* data = getBleDataPtr();
      if (data->temp_measure_status == true){

          LOG_ERROR("Error in indication\n\r");
      }

      if (data->temp_type_status == true){

          LOG_ERROR("Error in indication\n\r");
      }
      else if (data->temp_interval_status == true){

           LOG_ERROR("Error in indication\n\r");
       }


      break;
    }

    case sl_bt_evt_system_soft_timer_id:{

      displayUpdate();

      break;

    }

    case sl_bt_evt_sm_confirm_bonding_id:{

      sc = sl_bt_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection,0x01);

      if (sc != SL_STATUS_OK) {
         LOG_ERROR("Error in confirming bonding\n\r");
         break;
      }

      ble_data.bond_connection = evt->data.evt_sm_confirm_bonding.connection;

      break;
    }

    case sl_bt_evt_sm_confirm_passkey_id:{

      displayPrintf(DISPLAY_ROW_PASSKEY,"Passkey %d",evt->data.evt_sm_confirm_passkey.passkey);


      break;
    }

    case sl_bt_evt_system_external_signal_id:{

      if( evt->data.evt_system_external_signal.extsignals == evtPushbuttonEvent){

          sc = sl_bt_sm_passkey_confirm(ble_data.bond_connection,1);
          if (sc != SL_STATUS_OK) {
             LOG_ERROR("Error in confirming passkey\n\r");
             break;
          }

      }


      break;
    }

    case sl_bt_evt_sm_bonded_id:{

      displayPrintf(DISPLAY_ROW_CONNECTION,"Bonded");
      displayPrintf(DISPLAY_ROW_PASSKEY,"");

      break;
    }

#endif

//
#if (DEVICE_IS_BLE_SERVER == 0)

    case sl_bt_evt_scanner_scan_report_id:{

      char server_string[50];

      bd_addr server_addr = SERVER_BT_ADDRESS;
      ble_data.serverAddress = server_addr;

      uint8_t compare_check = 0;
      for(int i=0;i<6;i++){

          if (ble_data.serverAddress.addr[i] == evt->data.evt_scanner_scan_report.address.addr[i]){

              compare_check++;

          }

      }

      if (compare_check == 6){

          sc =  sl_bt_scanner_stop();

          if (sc != SL_STATUS_OK) {
              LOG_ERROR("Error in stopping scanner\n\r");
              break;
          }



          sprintf(server_string,"%x:%x:%x:%x:%x:%x",ble_data.serverAddress.addr[0],ble_data.serverAddress.addr[1],ble_data.serverAddress.addr[2],ble_data.serverAddress.addr[3],ble_data.serverAddress.addr[4],ble_data.serverAddress.addr[5]);
          displayPrintf(DISPLAY_ROW_BTADDR2,"%s", server_string);

          // address
          sc = sl_bt_connection_open(evt->data.evt_scanner_scan_report.address,
                                                evt->data.evt_scanner_scan_report.address_type,
                                                sl_bt_gap_1m_phy,
                                                NULL);

          if (sc != SL_STATUS_OK) {
              LOG_ERROR("Error in opening connection\n\r");
              break;
          }

      }

      break;
    }

    case  sl_bt_evt_gatt_service_id:{

      if(evt->data.evt_gatt_service.uuid.data[0] ==  thermo_service[0] && evt->data.evt_gatt_service.uuid.data[1] ==  thermo_service[1]  ){

         ble_data.service = evt->data.evt_gatt_service.service;

      }


      break;
    }


    case  sl_bt_evt_gatt_characteristic_id:{

      if(evt->data.evt_gatt_characteristic.uuid.data[0] ==  thermo_char[0] && evt->data.evt_gatt_characteristic.uuid.data[1] ==  thermo_char[1]  ){

          ble_data.characteristic = evt->data.evt_gatt_characteristic.characteristic;

      }

      break;
    }


    case sl_bt_evt_gatt_procedure_completed_id:{

      ble_data.procedure_completion = evt->data.evt_gatt_procedure_completed.connection;

      // Set Event
      schedulerSetProcedure_CompletedEvent();

      break;
    }


    case sl_bt_evt_gatt_characteristic_value_id:{

      uint8_t* data = &(evt->data.evt_gatt_characteristic_value.value.data[0]);

      int32_t temperature = FLOAT_TO_INT32(data);
      LOG_INFO("TEMP====%d",temperature);
      //Display Temperature on LCD
      displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp=%d",temperature);

      sc = sl_bt_gatt_send_characteristic_confirmation(evt->data.evt_gatt_characteristic_value.connection);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("Error in sending confirmation\n\r");
          break;
      }



      break;
    }
#endif
  }

}
