/*
 * timers.c
 *
 *  Created on: 06-Sep-2021
 *      Author: Shrikant
 */

#include "timers.h"

/*
#if(LOWEST_ENERGY_MODE==3)
#define CLOCK_FREQ (1000)
#else
#define CLOCK_FREQ (32768)
#endif*/

#define CLOCK_FREQ (32768)

#define PRESCALER (4)
#define ACTUAL_CLK_FREQ (CLOCK_FREQ/PRESCALER)

#define COMPARE1_VALUE ((LETIMER_PERIOD_MS*ACTUAL_CLK_FREQ)/1000)

#define COMPARE2_VALUE (COMPARE1_VALUE-((LETIMER_ON_TIME_MS*ACTUAL_CLK_FREQ)/1000))
//#define COMPARE2_VALUE ((LETIMER_ON_TIME_MS*ACTUAL_CLK_FREQ)/1000)


uint32_t temp;

void Timer_init(){


  const LETIMER_Init_TypeDef LEINIT = {
    .enable   = false,
    .debugRun = false,
    .comp0Top = true,
    .bufTop   = false,
    .out0Pol  = 0,
    .out1Pol  = 0,
    .ufoa0    = letimerUFOANone,
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

  LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP0 |LETIMER_IEN_COMP1 |LETIMER_IEN_UF);

}

