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


void schedulerSetNOEvent();

void schedulerSetI2CdoneEvent();

void schedulerSetCOMP1Event();

void schedulerSetUFEvent();

void schedulerSetConnection_OpenedEvent();

void schedulerSetProcedure_CompletedEvent();

void schedulerSetConnection_ClosedEvent();

void Si7021_state_machine(sl_bt_msg_t *evt);

void discovery_state_machine(sl_bt_msg_t *evt);

#endif /* SRC_SCHEDULER_H_ */
