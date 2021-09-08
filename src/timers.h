/*
 * timers.h
 *
 *  Created on: 06-Sep-2021
 *      Author: Shrikant
 */

#ifndef SRC_TIMERS_H_
#define SRC_TIMERS_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "app.h"
#include "log.h"
#include "em_letimer.h"
#include "efr32bg13p632f512gm48.h"


void Timer_init();

void Timer_load();

void Timer_Onoff(bool state);

void Timer_Printcount();

void Timer_InterruptEnable();

#endif /* SRC_TIMERS_H_ */
