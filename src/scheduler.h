/*
 * scheduler.h
 *
 *  Created on: 14-Sep-2021
 *      Author: Shrikant
 */

#ifndef SRC_SCHEDULER_H_
#define SRC_SCHEDULER_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "app.h"
#include "log.h"
#include "em_letimer.h"
#include "efr32bg13p632f512gm48.h"


void schedulerSetCOMP1Event();

void schedulerSetUFEvent();

uint32_t getNextEvent();


#endif /* SRC_SCHEDULER_H_ */
