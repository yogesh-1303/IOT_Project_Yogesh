/*
 * irq.h
 *
 *  Created on: 07-Sep-2021
 *      Author: Shrikant
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


void LETIMER0_IRQHandler(void);



#endif /* SRC_IRQ_H_ */
