/*
 * irq.c
 *
 *  Created on: 07-Sep-2021
 *      Author: Shrikant
 */

#include "irq.h"

void LETIMER0_IRQHandler(void){

  /*
  if (LETIMER0->IF & LETIMER_IF_COMP0){

      gpioLed0SetOn();

      //gpioLed0SetOff();

  }*/

  if (LETIMER0->IF & LETIMER_IF_COMP1){

      gpioLed0SetOff();

  }

  else if (LETIMER0->IF & LETIMER_IF_UF){

      gpioLed0SetOn();

  }

  LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP0 |LETIMER_IF_COMP1 |LETIMER_IF_UF);


}
