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
  evtPushbuttonPB0PressEvent=7,
  evtPushbuttonPB0ReleaseEvent=8,
  evtPushbuttonPB1PressEvent=9,
  evtPushbuttonPB1ReleaseEvent=10,
  totalevents
}event;

// enumeration for various states of Si7021 Temperature Reading
typedef enum uint32_t{
  IDLE_State,
  POWERON_State,
  I2Cwrite_State,
  I2Cread_State,
  POWERDOWN_State,
  STARTCLIENTtempservice_tempState,
  STARTCLIENTtempservice_State,
  TEMP_SERVICE_DISCOVERY_State,
  TEMP_CHARACTERISTIC_DISCOVERY_State,
  STARTCLIENTbuttonservice_State,
  BUTTON_SERVICE_DISCOVERY_State,
  BUTTON_CHARACTERISTIC_DISCOVERY_State,
  CLOSECLIENT_State
}state_t;


void schedulerSetNOEvent();

void schedulerSetI2CdoneEvent();

void schedulerSetCOMP1Event();

void schedulerSetUFEvent();

void schedulerSetConnection_OpenedEvent();

void schedulerSetProcedure_CompletedEvent();

void schedulerSetConnection_ClosedEvent();

void schedulerSetPushbuttonPB0PressEvent();

void schedulerSetPushbuttonPB0ReleaseEvent();

void schedulerSetPushbuttonPB1PressEvent();

void schedulerSetPushbuttonPB1ReleaseEvent();

void Si7021_state_machine(sl_bt_msg_t *evt);

void discovery_state_machine(sl_bt_msg_t *evt);

#endif /* SRC_SCHEDULER_H_ */
