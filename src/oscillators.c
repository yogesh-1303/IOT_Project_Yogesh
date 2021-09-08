#include "oscillators.h"

/*
#if(LOWEST_ENERGY_MODE==3)
#define OSC_TYPE (cmuOsc_ULFRCO)
#define OSC_SEL (cmuSelect_ULFRCO)
#else
#define OSC_TYPE (cmuOsc_LFXO)
#define OSC_SEL (cmuSelect_LFXO)
#endif*/

#define OSC_TYPE (cmuOsc_LFXO)
#define OSC_SEL (cmuSelect_LFXO)

void Init_CMU(void){

  // Enable the oscillator
  //CMU_OscillatorEnable(cmuOsc_LFXO,true,true);
  CMU_OscillatorEnable(OSC_TYPE,true,true);

  // Select the reference oscillator
  //CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
  CMU_ClockSelectSet(cmuClock_LFA,OSC_SEL);

}

void Configure_CMU(void){

  CMU_ClockEnable(cmuClock_LFA,true);

  // Prescaling of clock
  CMU_ClockDivSet(cmuClock_LFA,4);

  // Enabling the peripheral clock
  CMU_ClockEnable(cmuClock_LETIMER0,true);


}