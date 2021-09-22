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
#include "app.h"
#include "em_letimer.h"
#include "efr32bg13p632f512gm48.h"

// enumeration for various events on interrupt
typedef enum {

  evtNOEvent=0,
  evtComp1Event=1,
  evtUFEvent=2,
  evtI2CdoneEvent=3,
  evtI2CwriteEvent=4,
  evtSetWaitTempReadyEvent=5,
  evtI2CreadEvent=6,
  evtI2CprocesstempEvent=7,
  evtI2CpowerdownEvent=8,
  totalevents
}event;

void schedulerSetNOEvent();

void schedulerSetI2CdoneEvent();

void schedulerSetI2CwriteEvent();

void schedulerSetI2CreadEvent();

void schedulerSetWaitTempReadyEvent();

void schedulerSetI2CprocesstempEvent();

void schedulerSetI2CpowerdownEvent();

void schedulerSetCOMP1Event();

void schedulerSetUFEvent();

uint32_t getNextEvent();


#endif /* SRC_SCHEDULER_H_ */
