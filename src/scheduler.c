/*
 * scheduler.c
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#include "scheduler.h"



// global variable for checking triggered event
uint32_t my_events;

void schedulerSetI2CdoneEvent(){

   CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   my_events=evtI2CdoneEvent;

   CORE_EXIT_CRITICAL();


}
/*
void schedulerSetI2CwriteEvent(){

   CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   my_events=evtI2CwriteEvent;

   CORE_EXIT_CRITICAL();


}

void schedulerSetI2CreadEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  my_events=evtI2CreadEvent;

  CORE_EXIT_CRITICAL();



}

void schedulerSetWaitTempReadyEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  my_events=evtSetWaitTempReadyEvent;

  CORE_EXIT_CRITICAL();

  void schedulerSetI2CprocesstempEvent(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  my_events=evtI2CprocesstempEvent;

  CORE_EXIT_CRITICAL();

}

void schedulerSetI2CpowerdownEvent(){


  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();

  my_events=evtI2CpowerdownEvent;

  CORE_EXIT_CRITICAL();



}

}*/


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

    /*case evtI2CwriteEvent:{

      evt=4;
      break;

    }

    case evtSetWaitTempReadyEvent:{

      evt=5;
      break;

    }

    case evtI2CreadEvent:{

      evt=6;
      break;

    }

    case evtI2CprocesstempEvent:{

      evt=7;
      break;

    }

    case evtI2CpowerdownEvent:{

      evt=8;
      break;

    }*/

    default: break;

  }

   CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   my_events=evtNOEvent;

   CORE_EXIT_CRITICAL();

   return evt;


}


