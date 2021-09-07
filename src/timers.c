/*
 * timers.c
 *
 *  Created on: 06-Sep-2021
 *      Author: Shrikant
 */

#include "timers.h"

#define LETIMER_ON_PERIOD_MS (175)
#define LETIMER_PERIOD_MS    (22)

#define PRESCALER (4)
#define CLK_FREQ (32768/PRESCALER)

#define COMPARE1_VALUE (0xFFFF)
#define COMPARE2_VALUE ((LETIMER_ON_PERIOD_MS*CLK_FREQ)/1000)


uint32_t temp;

void Timer_init(){

 /*
  LEINIT->enable = false;
  LEINIT->debugRun = true;
  //LEINIT->rtcComp0Enable = false;
  LEINIT->comp0Top = false;
  LEINIT->bufTop = false;
  LEINIT->out0Pol = 0;
  LEINIT->out1Pol = 0;
  LEINIT->ufoa0 = letimerUFOANone;
  LEINIT->ufoa1 = letimerUFOANone;
  LEINIT->repMode = letimerRepeatFree;
  LEINIT->topValue = 0xFFFF;
  //LEINIT->topValue = 8192;

  LETIMER_Init(LETIMER0,LEINIT);*/

  const LETIMER_Init_TypeDef LEINIT = {
    .enable         = false,
    .debugRun       = true,
    .comp0Top       = false,
    .bufTop         = false,
    .out0Pol        = 0,
    .out1Pol        = 0,
    .ufoa0          = letimerUFOANone,
    .ufoa1    = letimerUFOANone,
    .repMode  = letimerRepeatFree,
    .topValue = 0xFFFF,
    };

    /* Initialize LETIMER */
    LETIMER_Init(LETIMER0, &LEINIT);



}

void Timer_load(){

   LETIMER_CompareSet(LETIMER0,0,COMPARE1_VALUE);

   LETIMER_CompareSet(LETIMER0,1,COMPARE2_VALUE);

}

void Timer_Onoff(bool state){

  if (state == true){

      LETIMER_Enable(LETIMER0,true);

  }

  else if(state == false){

      LETIMER_Enable(LETIMER0,false);
  }

}

void Timer_Printcount(){


  temp=LETIMER_CounterGet(LETIMER0);


}

void Timer_InterruptEnable(){

  LETIMER_IntEnable(LETIMER0,LETIMER_IF_COMP0 |LETIMER_IF_COMP1 |LETIMER_IF_UF);

}

