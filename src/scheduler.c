/*
 * scheduler.c
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#include "scheduler.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG (1)

#include "src/log.h"


// Health Thermometer service UUID defined by Bluetooth SIG
static const uint8_t thermo_service[2] = { 0x09, 0x18 };
// Temperature Measurement characteristic UUID defined by Bluetooth SIG
static const uint8_t thermo_char[2] = { 0x1c, 0x2a };


static const uint8_t button_service[16] = {0x89,0x62,0x13,0x2d,0x2a,0x65,0xec,0x87,0x3e,0x43,0xc8,0x38,0x01,0x00,0x00,0x00};

static const uint8_t button_char[16] = {0x89,0x62,0x13,0x2d,0x2a,0x65,0xec,0x87,0x3e,0x43,0xc8,0x38,0x02,0x00,0x00,0x00};


// global variable for checking triggered event
uint32_t my_events;



void schedulerSetI2CdoneEvent(){

   CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   sl_bt_external_signal(evtI2CdoneEvent);

   CORE_EXIT_CRITICAL();


}


void schedulerSetCOMP1Event(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtComp1Event);

  CORE_EXIT_CRITICAL();

}



void schedulerSetUFEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtUFEvent);

  CORE_EXIT_CRITICAL();

}

void schedulerSetConnection_OpenedEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtConnection_Opened);

  CORE_EXIT_CRITICAL();

}


void schedulerSetProcedure_CompletedEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtProcedure_Completed);

  CORE_EXIT_CRITICAL();

}

void schedulerSetConnection_ClosedEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtConnection_Closed);

  CORE_EXIT_CRITICAL();

}




void schedulerSetNOEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtNOEvent);

  CORE_EXIT_CRITICAL();

}

void schedulerSetPushbuttonPB0PressEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtPushbuttonPB0PressEvent);

  CORE_EXIT_CRITICAL();

}

void schedulerSetPushbuttonPB0ReleaseEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtPushbuttonPB0ReleaseEvent);

  CORE_EXIT_CRITICAL();


}



void schedulerSetPushbuttonPB1PressEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtPushbuttonPB1PressEvent);

  CORE_EXIT_CRITICAL();

}

void schedulerSetPushbuttonPB1ReleaseEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtPushbuttonPB1ReleaseEvent);

  CORE_EXIT_CRITICAL();


}



void Si7021_state_machine(sl_bt_msg_t *evt){

  // Enable measurement only if connection is opened and disable measurement when closed
  ble_data_struct_t* data = getBleDataPtr();

  if(data->connection_status && (data->pushbutton_indication_status || data->temp_measure_indication_status )){

  //if (data->enable_measurement == true){

      if (SL_BT_MSG_ID(evt->header) == sl_bt_evt_system_external_signal_id){

         state_t current_state ;
         static state_t next_state = IDLE_State;

         current_state = next_state;


         switch(current_state){

            case IDLE_State:{

              next_state = IDLE_State;

              if (evt->data.evt_system_external_signal.extsignals == evtUFEvent){

                   //LOG_INFO("POWER ON SENSOR @\r");

                  // Enable Si7021 by setting its enable signal high
                  Enable_si7021(true);

                  // Wait for Power up time
                  timerWaitUs_irq(80000);

                  next_state = POWERON_State;

               }
               break;
              }

            case POWERON_State:{

               next_state = POWERON_State;

               if (evt->data.evt_system_external_signal.extsignals == evtComp1Event){

                   I2C_Write_Si7021();
                   next_state = I2Cwrite_State;

                   sl_power_manager_add_em_requirement(EM1);

               }
               break;
              }

            case I2Cwrite_State:{

               next_state = I2Cwrite_State;

               if (evt->data.evt_system_external_signal.extsignals == evtI2CdoneEvent){

                   NVIC_DisableIRQ(I2C0_IRQn);

                   sl_power_manager_remove_em_requirement(EM1);

                   timerWaitUs_irq(10800);
                   next_state = I2Cread_State;

               }
               break;
              }

            case I2Cread_State:{

                 next_state = I2Cread_State;

                 if (evt->data.evt_system_external_signal.extsignals == evtComp1Event){

                     I2C_Read_Si7021();

                     next_state = POWERDOWN_State;

                     sl_power_manager_add_em_requirement(EM1);


                 }
                 break;
                }

            case POWERDOWN_State:{

                next_state = POWERDOWN_State;

                if (evt->data.evt_system_external_signal.extsignals == evtI2CdoneEvent){

                 NVIC_DisableIRQ(I2C0_IRQn);

                 sl_power_manager_remove_em_requirement(EM1);

                 process_temp_si7021();

                 transmit_tempdata();

                 next_state = IDLE_State;

                }

                 break;
                }


              default: break;
         }
      }
  }

}


void discovery_state_machine(sl_bt_msg_t *evt){

    sl_status_t sc;

    if (SL_BT_MSG_ID(evt->header) == sl_bt_evt_system_external_signal_id){


        state_t current_state ;
        static state_t next_state = STARTCLIENTtempservice_State;

        current_state = next_state;


         switch(current_state){

            case STARTCLIENTtempservice_State:{

              next_state = STARTCLIENTtempservice_State;

               if (evt->data.evt_system_external_signal.extsignals == evtConnection_Opened){

                   ble_data_struct_t* data = getBleDataPtr();

                   // Discover Health Thermometer service on the responder device
                   sc = sl_bt_gatt_discover_primary_services_by_uuid(data->new_connection,
                                                                           sizeof(thermo_service),
                                                                           thermo_service);

                   if (sc != SL_STATUS_OK) {
                       LOG_ERROR("Error in discovering health service\n\r");
                       break;
                   }

                   next_state = TEMP_SERVICE_DISCOVERY_State;


               }
               break;
              }


            case TEMP_SERVICE_DISCOVERY_State:{

              next_state = TEMP_SERVICE_DISCOVERY_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){

                  ble_data_struct_t* data = getBleDataPtr();

                  sc = sl_bt_gatt_discover_characteristics_by_uuid(data->new_connection,
                                                                      data->temperature_service,
                                                                      sizeof(thermo_char),
                                                                     thermo_char);
                  if (sc != SL_STATUS_OK) {

                     LOG_ERROR("Error in discovering health service\n\r");
                     break;
                 }


                  next_state = TEMP_CHARACTERISTIC_DISCOVERY_State;

              }


              break;
            }


            case TEMP_CHARACTERISTIC_DISCOVERY_State:{

              next_state = TEMP_CHARACTERISTIC_DISCOVERY_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){

                  ble_data_struct_t* data = getBleDataPtr();

                  sc = sl_bt_gatt_set_characteristic_notification(data->new_connection,
                                                                    data->temperature_characteristic,
                                                                    sl_bt_gatt_indication);
                  if (sc != SL_STATUS_OK) {

                     LOG_ERROR("Error in discovering health service\n\r");
                     break;
                 }


                  displayPrintf(DISPLAY_ROW_CONNECTION,"Handling Indication");

                  next_state = STARTCLIENTbuttonservice_State;


              }

              break;
            }



            case STARTCLIENTbuttonservice_State:{

              next_state = STARTCLIENTbuttonservice_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){


                  ble_data_struct_t* data = getBleDataPtr();

                 // Discover Health Thermometer service on the responder device
                 sc = sl_bt_gatt_discover_primary_services_by_uuid(data->new_connection,
                                                                         sizeof(button_service),
                                                                         button_service);

                 if (sc != SL_STATUS_OK) {
                     LOG_ERROR("Error in discovering health service\n\r");
                     break;
                 }

                 next_state = BUTTON_SERVICE_DISCOVERY_State;
              }

              break;

            }

            case BUTTON_SERVICE_DISCOVERY_State:{

              next_state = BUTTON_SERVICE_DISCOVERY_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){

              ble_data_struct_t* data = getBleDataPtr();

               sc = sl_bt_gatt_discover_characteristics_by_uuid(data->new_connection,
                                                                   data->button_service,
                                                                   sizeof(button_char),
                                                                  button_char);
               if (sc != SL_STATUS_OK) {

                  LOG_ERROR("Error in discovering health service\n\r");
                  break;
              }


               next_state = BUTTON_CHARACTERISTIC_DISCOVERY_State;

            }

              break;
            }


            case BUTTON_CHARACTERISTIC_DISCOVERY_State:{

              next_state = BUTTON_CHARACTERISTIC_DISCOVERY_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){

                  ble_data_struct_t* data = getBleDataPtr();

                  sc = sl_bt_gatt_set_characteristic_notification(data->new_connection,
                                                                    data->button_characteristic,
                                                                    sl_bt_gatt_indication);
                  if (sc != SL_STATUS_OK) {

                     LOG_ERROR("Error in discovering health service\n\r");
                     break;
                 }


                  displayPrintf(DISPLAY_ROW_CONNECTION,"Handling Indication");

                  next_state = CLOSECLIENT_State;


              }

              break;
            }

            case CLOSECLIENT_State:{


              next_state = CLOSECLIENT_State;

              if(evt->data.evt_system_external_signal.extsignals == evtConnection_Closed){

                  next_state = STARTCLIENTtempservice_State;

                 }


              break;
            }



            default: break;


        }

    }

}

