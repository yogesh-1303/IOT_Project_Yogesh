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


void schedulerSetCOMP1Event();

void schedulerSetUFEvent();

uint32_t getNextEvent();


#endif /* SRC_SCHEDULER_H_ */
