#include "oscillators.h"

void Init_CMU(void){

  // Enable the oscillator
  CMU_OscillatorEnable(cmuOsc_LFXO,true,true);

  // Select the reference oscillator
  CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);

}

void Configure_CMU(void){

  CMU_ClockEnable(cmuClock_LFA,true);

  // Prescaling of clock
  CMU_ClockDivSet(cmuClock_LFA,4);

  // Enabling the peripheral clock
  CMU_ClockEnable(cmuClock_LETIMER0,true);


}
