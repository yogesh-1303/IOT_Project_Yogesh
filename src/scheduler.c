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

// enumeration for various events on interrupt
typedef enum {

  evtNOEvent=0,
  evtComp1Event=1,
  evtUFEvent=2,
  evtI2CdoneEvent=3,
  evtConnection_Opened=4,
  evtProcedure_Completed=5,
  evtConnection_Closed=6,
  totalevents
}event;

// enumeration for various states of Si7021 Temperature Reading
typedef enum uint32_t{
  IDLE_State,
  POWERON_State,
  I2Cwrite_State,
  I2Cread_State,
  POWERDOWN_State,
  STARTCLIENT_State,
  SERVICE_DISCOVERY_State,
  CHARACTERISTIC_DISCOVERY_State,
  NOTIFICATION_RECEIVED_State,
  CLOSECLIENT_State
}state_t;


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




void Si7021_state_machine(sl_bt_msg_t *evt){

  // Enable measurement only if connection is opened and disable measurement when closed
  ble_data_struct_t* data = getBleDataPtr();

  if (data->enable_measurement == true){

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

               //Enable_si7021(false);

               process_temp_si7021();

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
        static state_t next_state = STARTCLIENT_State;

        current_state = next_state;


         switch(current_state){

            case STARTCLIENT_State:{

                   next_state = STARTCLIENT_State;

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

                       next_state = SERVICE_DISCOVERY_State;

                       displayPrintf(DISPLAY_ROW_CONNECTION,"Handling Indication");

                   }
                   break;
                  }


            case SERVICE_DISCOVERY_State:{

              next_state = SERVICE_DISCOVERY_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){

                  ble_data_struct_t* data = getBleDataPtr();

                  sc = sl_bt_gatt_discover_characteristics_by_uuid(data->new_connection,
                                                                      data->service,
                                                                      sizeof(thermo_char),
                                                                     thermo_char);
                  if (sc != SL_STATUS_OK) {

                     LOG_ERROR("Error in discovering health service\n\r");
                     break;
                 }


                  next_state = CHARACTERISTIC_DISCOVERY_State;

              }


              break;
            }


            case CHARACTERISTIC_DISCOVERY_State:{

              next_state = CHARACTERISTIC_DISCOVERY_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){

                  ble_data_struct_t* data = getBleDataPtr();

                  sc = sl_bt_gatt_set_characteristic_notification(data->new_connection,
                                                                    data->characteristic,
                                                                    sl_bt_gatt_indication);
                  if (sc != SL_STATUS_OK) {

                     LOG_ERROR("Error in discovering health service\n\r");
                     break;
                 }


                  next_state = NOTIFICATION_RECEIVED_State;


              }

              break;
            }

            case NOTIFICATION_RECEIVED_State:{

              next_state = NOTIFICATION_RECEIVED_State;

              if (evt->data.evt_system_external_signal.extsignals == evtProcedure_Completed){

                  next_state = NOTIFICATION_RECEIVED_State;


              }

             if(evt->data.evt_system_external_signal.extsignals == evtConnection_Closed){

                  next_state = STARTCLIENT_State;

              }

              break;
            }

            case CLOSECLIENT_State:{

             next_state = STARTCLIENT_State;

              break;
            }



            default: break;


        }




    }











}

