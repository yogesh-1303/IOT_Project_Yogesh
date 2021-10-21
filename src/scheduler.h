/*
 * scheduler.h
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sl_bt_api.h>
#include "app.h"
#include "em_letimer.h"
#include "efr32bg13p632f512gm48.h"

// enumeration for various events on interrupt
typedef enum {

  evtNOEvent=0,
  evtComp1Event=1,
  evtUFEvent=2,
  evtI2CdoneEvent=3,
  evtConnection_Opened=4,
  evtProcedure_Completed=5,
  evtConnection_Closed=6,
  evtPushbuttonEvent=7,
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
  CLOSECLIENT_State
}state_t;


void schedulerSetNOEvent();

void schedulerSetI2CdoneEvent();

void schedulerSetCOMP1Event();

void schedulerSetUFEvent();

void schedulerSetConnection_OpenedEvent();

void schedulerSetProcedure_CompletedEvent();

void schedulerSetConnection_ClosedEvent();

void schedulerSetPushbuttonEvent();

void Si7021_state_machine(sl_bt_msg_t *evt);

void discovery_state_machine(sl_bt_msg_t *evt);

#endif /* SRC_SCHEDULER_H_ */
