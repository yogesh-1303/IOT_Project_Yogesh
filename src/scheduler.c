/*
 * scheduler.c
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#include "scheduler.h"

// enumeration for various events on interrupt
typedef enum {

  evtNOEvent=0,
  evtComp1Event=1,
  evtUFEvent=2,
  totalevents
}event;

// global variable for checking triggered event
uint32_t my_events;

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

    default: break;

  }

   CORE_DECLARE_IRQ_STATE;

   CORE_ENTER_CRITICAL();

   my_events=evtNOEvent;

   CORE_EXIT_CRITICAL();

   return evt;


}


