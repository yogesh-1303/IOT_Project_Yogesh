/*
 * scheduler.c
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#include "scheduler.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG (1)

#include "src/log.h"

// enumeration for various events on interrupt
typedef enum {

  evtNOEvent=0,
  evtComp1Event=1,
  evtUFEvent=2,
  evtI2CdoneEvent=3,
  totalevents
}event;

// enumeration for various states of Si7021 Temperature Reading
typedef enum uint32_t{
  IDLE_State,
  POWERON_State,
  I2Cwrite_State,
  I2Cread_State,
  POWERDOWN_State,
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

void schedulerSetNOEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  sl_bt_external_signal(evtNOEvent);

  CORE_EXIT_CRITICAL();

}




void state_machine(sl_bt_msg_t *evt){

  if (SL_BT_MSG_ID(evt->header) == sl_bt_evt_system_external_signal_id){

  switch(evt->data.evt_system_external_signal.extsignals){

     case IDLE_State:{

       //LOG_INFO("POWER ON SENSOR @\r");

       // Enable Si7021 by setting its enable signal high
       Enable_si7021(true);

       // Wait for Power up time
       timerWaitUs_irq(80000);

       break;
     }

     case POWERON_State:{

       I2C_Write_Si7021();
       sl_power_manager_add_em_requirement(EM1);

       break;
     }

     case I2Cwrite_State:{

       NVIC_DisableIRQ(I2C0_IRQn);

       sl_power_manager_remove_em_requirement(EM1);

       timerWaitUs_irq(10800);

       break;
     }

     case I2Cread_State:{

       I2C_Read_Si7021();
       sl_power_manager_add_em_requirement(EM1);

       break;

     }

     case POWERDOWN_State:{

        NVIC_DisableIRQ(I2C0_IRQn);

        sl_power_manager_remove_em_requirement(EM1);

        Enable_si7021(false);

        process_temp_si7021();


        break;
       }


     default: break;


   }

  }

}


