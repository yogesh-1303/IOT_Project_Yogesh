/*
 * timers.c - Source file for implementation of CMU functionality
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 *
 */

#include "timers.h"

// Include logging for this file
//#define INCLUDE_LOG_DEBUG 1

/*Macro definition to select appropriate clock frequency macro depending on energy mode*/

#if(LOWEST_ENERGY_MODE==3)
#define CLOCK_FREQ (1000)
#else
#define CLOCK_FREQ (32768)
#endif

// Macro definition for Clock Prescaler
#define PRESCALER (4)
#define ACTUAL_CLK_FREQ ((CLOCK_FREQ) / (PRESCALER))

// Macro definition for Compare Register 1 Value
#define COMPARE0_VALUE ((LETIMER_PERIOD_MS*ACTUAL_CLK_FREQ)/1000)

// Macro definition for Compare Register 2 Value
#define COMPARE1_VALUE (COMPARE0_VALUE-((LETIMER_ON_TIME_MS*ACTUAL_CLK_FREQ)/1000))
//#define COMPARE2_VALUE ((LETIMER_ON_TIME_MS*ACTUAL_CLK_FREQ)/1000)

#define MICROSEC_PER_SEC (1000000)

/* Function that initializes LETIMER by setting various bit field in LETIMER0_CTRL register
 * PARAMETERS : NONE
 * RETURNS    : NONE
 *
 */
void Timer_init(){


  const LETIMER_Init_TypeDef LEINIT = {
    .enable   = false,                       //Don't start counting upon init
    .debugRun = true,                       //Freeze timer in debug
    .comp0Top = true,                        //Load comp0 as top value
    .bufTop   = false,                       //Write comp0 only by software
    .out0Pol  = 0,                           //Low polarity of output1
    .out1Pol  = 0,                           //Low polarity of output2
    .ufoa0    = letimerUFOANone,             //No action on underflow
    .ufoa1    = letimerUFOANone,             //No action on underflow
    .repMode  = letimerRepeatFree,           //Stop timer by software
    .topValue = 65535,
    };

    /* Initialize LETIMER */
    LETIMER_Init(LETIMER0, &LEINIT);



}

/* Function that loads vale into comp0 & comp1 register
 * PARAMETERS : NONE
 * RETURNS    : NONE
 *
 */
void Timer_load(){

   // Load Value into compare register 0
   LETIMER_CompareSet(LETIMER0,0,COMPARE0_VALUE);

   // Load Value into compare register 1
   LETIMER_CompareSet(LETIMER0,1,COMPARE1_VALUE);

}

/* Function to enable/disable the timer
 * PARAMETERS : true to enable,false to disable
 * RETURNS    : NONE
 *
 */
void Timer_Onoff(bool state){

  if (state == true){

      // Start Timer
      LETIMER_Enable(LETIMER0,true);

  }

  else if(state == false){

      // Stop Timer
      LETIMER_Enable(LETIMER0,false);
  }

}

/* Function to print LETIMER0 counter value
 * PARAMETERS : NONE
 * RETURNS    : NONE
 *
 */
void Timer_Printcount(){


  //uint32_t temp;

  // Log Timer Count for debug purpose
  //temp=LETIMER_CounterGet(LETIMER0);
  //LOG_INFO("\nCurrent Timer Count is %d",(uint32_t)temp);


}

/* Function to enable particular interrupts of LETIMER0
 * PARAMETERS : NONE
 * RETURNS    : NONE
 *
 */
void Timer_InterruptEnable(){

  // Enable Comp0 Comp1 and Underflow Interrupts
  LETIMER_IntEnable(LETIMER0,LETIMER_IEN_COMP0 |LETIMER_IEN_COMP1 |LETIMER_IEN_UF);

}

void timerWaitUs(uint32_t us_wait){

  uint32_t compare0_value = (LETIMER_PERIOD_MS*ACTUAL_CLK_FREQ)/1000;

  uint32_t total_ticks;

  // To avoid unsigned 32 bit overflow for too large delays
  if(us_wait >= MICROSEC_PER_SEC ){

      uint32_t sec_wait=us_wait/MICROSEC_PER_SEC;

      total_ticks = (sec_wait*(ACTUAL_CLK_FREQ));

  }

  else {

      total_ticks = ((us_wait*(ACTUAL_CLK_FREQ))/MICROSEC_PER_SEC);

  }


  uint32_t now_count;
  uint32_t current_count;

  // Range Check for total ticks
  if(total_ticks > (COMPARE0_VALUE)){

      //LOG_ERRO("Out of Range Value\r");
  }

  now_count=LETIMER_CounterGet(LETIMER0);


  // Roll over logic
  if(now_count < total_ticks){


      // Let timer count till underflow
      // while((current_count=LETIMER_CounterGet(LETIMER0)) < now_count);
      while((current_count=LETIMER_CounterGet(LETIMER0)) != 0);


      // Continue counting for remainin ticks after underflow
      while((current_count=LETIMER_CounterGet(LETIMER0)) >= ((compare0_value)-(total_ticks-now_count)));


  }

  else {

      // Continue counting till the calculated ticks
      while((current_count=LETIMER_CounterGet(LETIMER0)) >= (now_count-total_ticks));

  }

}

