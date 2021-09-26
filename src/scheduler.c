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

   my_events=evtI2CdoneEvent;

   CORE_EXIT_CRITICAL();


}


void schedulerSetCOMP1Event(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  my_events=evtComp1Event;

  CORE_EXIT_CRITICAL();

}



void schedulerSetUFEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  my_events=evtUFEvent;

  CORE_EXIT_CRITICAL();

}

void schedulerSetNOEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  my_events=evtNOEvent;

  CORE_EXIT_CRITICAL();

}

uint32_t getNextEvent(){

  uint32_t evt=0;

  switch(my_events){

    case evtComp1Event:{

      evt=1;
      break;

    }

    case evtUFEvent:{

      evt=2;
      break;

    }

    case evtI2CdoneEvent:{

      evt=3;
      break;

    }


    default: break;

  }

   CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   my_events=evtNOEvent;

   CORE_EXIT_CRITICAL();

   return evt;


}


void State_Machine(void){

   uint32_t event;
   state_t current_state ;
   static state_t next_state = IDLE_State;

   current_state = next_state;

   event=getNextEvent();

   switch(current_state){

     case IDLE_State:{
      next_state = IDLE_State;

      if (event == evtUFEvent){

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

      if (event == evtComp1Event){

          I2C_Write_Si7021();
          next_state = I2Cwrite_State;

          sl_power_manager_add_em_requirement(EM1);

      }
      break;
     }

     case I2Cwrite_State:{

      next_state = I2Cwrite_State;

      if (event == evtI2CdoneEvent){

          NVIC_DisableIRQ(I2C0_IRQn);

          sl_power_manager_remove_em_requirement(EM1);

          timerWaitUs_irq(10800);
          next_state = I2Cread_State;

      }
      break;
     }

     case I2Cread_State:{

        next_state = I2Cread_State;

        if (event == evtComp1Event){

            I2C_Read_Si7021();

            next_state = POWERDOWN_State;

            sl_power_manager_add_em_requirement(EM1);


        }
        break;
       }

     case POWERDOWN_State:{

       next_state = POWERDOWN_State;

       if (event == evtI2CdoneEvent){

        NVIC_DisableIRQ(I2C0_IRQn);

        sl_power_manager_remove_em_requirement(EM1);

        Enable_si7021(false);

        process_temp_si7021();

        next_state = IDLE_State;

       }

        break;
       }


     default: break;


   }





}


