/*
 * irq.c
 *
 *  Created on: 07-Sep-2021
 *      Author: Shrikant
 */

#include "irq.h"

void LETIMER0_IRQHandler(void){


  if (LETIMER0->IF & 0x01){

      //gpioLed0SetOn();

      gpioLed0SetOff();

  }

  else if (LETIMER0->IF & 0x02){

      gpioLed0SetOn();

  }

  else if (LETIMER0->IF & 0x04){

      gpioLed0SetOff();

  }

  LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP0 |LETIMER_IF_COMP1 |LETIMER_IF_UF);


}
