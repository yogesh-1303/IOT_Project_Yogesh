/*
 * irq.c -  Source file for implementation of LETIMER0 IRQ Handler
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#include "irq.h"

/*Macro definition to select appropriate clock frequency macro depending on energy mode*/

#if(LOWEST_ENERGY_MODE==3)
#define CLOCK_FREQ (1000)
#else
#define CLOCK_FREQ (32768)
#endif

// Macro definition for Clock Prescaler
#define PRESCALER (4)
#define ACTUAL_CLK_FREQ ((CLOCK_FREQ) / (PRESCALER))

uint32_t rollover_count=0;



/* Function that Implements LETIMER0 IRQ Handler
 * PARAMETERS : NONE
 * RETURNS    : NONE
 */
void LETIMER0_IRQHandler(void){

  // Check if COMP1 Interrupt flag is set
  if (LETIMER0->IF & LETIMER_IF_COMP1){

      // Set the appropriate event in scheduler
      schedulerSetCOMP1Event();

      /*Disable COMP1 Interrupt*/
      LETIMER_IntDisable(LETIMER0, LETIMER_IF_COMP1);

  }

  // Check if Underflow Interrupt flag is set
  else if (LETIMER0->IF & LETIMER_IF_UF){

      // Set the appropriate event in scheduler
      schedulerSetUFEvent();

      // Increament underflow count
      rollover_count++;


  }

  // Clear Interrupt Sorces
  LETIMER_IntClear(LETIMER0, LETIMER_IF_COMP0 |LETIMER_IF_COMP1 |LETIMER_IF_UF);


}

uint32_t letimerMilliseconds(){


  uint32_t milli_sec;
  uint32_t current_count = LETIMER_CounterGet(LETIMER0);

  milli_sec = (rollover_count * LETIMER_PERIOD_MS) + ((current_count *1000)/ACTUAL_CLK_FREQ);

  return milli_sec;

}




