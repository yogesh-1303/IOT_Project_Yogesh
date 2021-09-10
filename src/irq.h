/*
 * irq.h - Header file for implementation of LETIMER IRQ Handler
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#ifndef SRC_IRQ_H_
#define SRC_IRQ_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "log.h"
#include "src/gpio.h"
#include "em_letimer.h"
#include "efr32bg13p632f512gm48.h"



/* Function that Implements LETIMER0 IRQ Handler
 * PARAMETERS : NONE
 * RETURNS    : NONE
 */
void LETIMER0_IRQHandler(void);



#endif /* SRC_IRQ_H_ */
