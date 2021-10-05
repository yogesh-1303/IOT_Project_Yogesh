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

// Variable to enable or disable measuremnt from Si7021
uint8_t enable_measurement = 0;

// BLE private data
ble_data_struct_t ble_data = {.temp_measure_status=false,.temp_type_status=false,.temp_interval_status=false};


ble_data_struct_t* getBleDataPtr(){

  return (&ble_data);

}




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


     // -------------------------------// Write our local GATT DB// -------------------------------
     sc = sl_bt_gatt_server_write_attribute_value(attribute,0,5,&htm_temperature_buffer[0]);
     if (sc != SL_STATUS_OK)
       {
         LOG_ERROR("sl_bt_gatt_server_write_attribute_value() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
       }


     if(sl_bt_gatt_server_indication == (sl_bt_gatt_server_client_configuration_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags){

     sc = sl_bt_gatt_server_send_indication(evt->data.evt_gatt_server_characteristic_status.connection,evt->data.evt_gatt_server_characteristic_status.characteristic,5,&htm_temperature_buffer[0]);

     if (sc != SL_STATUS_OK)
     {
        LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
     }

     else {
         ble_data_struct_t* data = getBleDataPtr();
         data->temp_measure_status = true;
     }
    }

 }

 // confirmation of indication received from remove GATT client
 else if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

     if(sl_bt_gatt_server_indication == (sl_bt_gatt_server_client_configuration_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags){
          sc = sl_bt_gatt_server_send_indication(evt->data.evt_gatt_server_characteristic_status.connection,evt->data.evt_gatt_server_characteristic_status.characteristic,5,&htm_temperature_buffer[0]);

          if (sc != SL_STATUS_OK)
          {
             LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04r\n\r", (unsigned int) sc);
          }

          else {

              ble_data_struct_t* data = getBleDataPtr();
              data->temp_measure_status = false;
     }
 }



}
}

void transmit_temptype(sl_bt_msg_t *evt,uint16_t attribute){

  sl_status_t sc;

  uint8_t temperature_type = SL_BT_HT_TEMPERATURE_TYPE;

  if (sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

      // -------------------------------// Write our local GATT DB// -------------------------------
      sc = sl_bt_gatt_server_write_attribute_value(attribute,0,sizeof(temperature_type),&temperature_type);

      if (sc != SL_STATUS_OK)
       {
          LOG_ERROR("sl_bt_gatt_server_write_attribute() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
       }


      if(sl_bt_gatt_server_indication == (sl_bt_gatt_server_client_configuration_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags){
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

  // confirmation of indication received from remove GATT client
   else if (sl_bt_gatt_server_confirmation == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

       if(sl_bt_gatt_server_indication == (sl_bt_gatt_server_client_configuration_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags){

           sc = sl_bt_gatt_server_send_indication(evt->data.evt_gatt_server_characteristic_status.connection,evt->data.evt_gatt_server_characteristic_status.characteristic,sizeof(temperature_type),&temperature_type);


            if (sc != SL_STATUS_OK)
            {
               LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
            }

            else {

                ble_data_struct_t* data = getBleDataPtr();
                data->temp_type_status = false;

            }
       }
   }


}


void transmit_tempinterval(sl_bt_msg_t *evt,uint16_t attribute){

  sl_status_t sc;

  uint16_t measurement_interval = MEASUREMENT_INTERVAL;

  if (sl_bt_gatt_server_client_config == (sl_bt_gatt_server_characteristic_status_flag_t)evt->data.evt_gatt_server_characteristic_status.status_flags) {

        // -------------------------------// Write our local GATT DB// -------------------------------
        sc = sl_bt_gatt_server_write_attribute_value(attribute,0,sizeof(measurement_interval),(uint8_t *)&measurement_interval);

        if (sc != SL_STATUS_OK)
         {
            LOG_ERROR("sl_bt_gatt_server_write_attribute() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
         }


        if(sl_bt_gatt_server_indication == (sl_bt_gatt_server_client_configuration_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags){
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

         if(sl_bt_gatt_server_indication == (sl_bt_gatt_server_client_configuration_t)evt->data.evt_gatt_server_characteristic_status.client_config_flags){

             sc = sl_bt_gatt_server_send_indication(evt->data.evt_gatt_server_characteristic_status.connection,evt->data.evt_gatt_server_characteristic_status.characteristic,sizeof(measurement_interval),(uint8_t *)&measurement_interval);

              if (sc != SL_STATUS_OK)
              {
                 LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\n\r", (unsigned int) sc);
              }

              else {

                  ble_data_struct_t* data = getBleDataPtr();
                  data->temp_interval_status = false;
              }

              }
         }
}






void handle_ble_event(sl_bt_msg_t *evt){

  sl_status_t sc;

  switch(SL_BT_MSG_ID(evt->header)){

    case sl_bt_evt_system_boot_id:{

      // Extract unique ID from BT Address.
      sc = sl_bt_system_get_identity_address(&ble_data.myAddress, &ble_data.myAddressType);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("Error in getting identity\n\r");
          break;
      }

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

       sc = sl_bt_advertiser_start(
              ble_data.advertisingSetHandle,
              sl_bt_advertiser_general_discoverable,
              sl_bt_advertiser_connectable_scannable);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("Error in starting advertising\n\r");
           break;
       }
      break;
    }

    case sl_bt_evt_connection_opened_id:{

      enable_measurement = 1;

      LOG_INFO("Connection Open\n\r");

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
      break;
    }


    case sl_bt_evt_connection_closed_id:{

      ble_data_struct_t* data = getBleDataPtr();

      // Close connection only if there is no indication in flight

      while( data->temp_measure_status==true || data->temp_type_status == true ||data->temp_interval_status  == true );

      enable_measurement = 0;
      LOG_INFO("Connection Closed\n\r");

      sc = sl_bt_advertiser_start(
                    ble_data.advertisingSetHandle,
                    sl_bt_advertiser_general_discoverable,
                    sl_bt_advertiser_connectable_scannable);
             if (sc != SL_STATUS_OK) {
                 LOG_ERROR("Error in starting advertising\n\r");
                 break;
             }


      break;
    }


    case  sl_bt_evt_connection_parameters_id:{


      LOG_INFO("Connection params: connection=%d, interval=%d, latency=%d, timeout=%d, securitymode=%d",
               (int) (evt->data.evt_connection_parameters.connection),
               (int) (evt->data.evt_connection_parameters.interval*1.25),
               (int) (evt->data.evt_connection_parameters.latency),
               (int) (evt->data.evt_connection_parameters.timeout*10),
               (int) (evt->data.evt_connection_parameters.security_mode) );


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


  }


}
