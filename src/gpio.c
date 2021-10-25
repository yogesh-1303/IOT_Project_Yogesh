/*
  gpio.c
 
   Created on: Dec 12, 2018
       Author: Dan Walkes
   Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

   March 17
   Dave Sluiter: Use this file to define functions that set up or control GPIOs.

 */




#include "gpio.h"





// Set GPIO drive strengths and modes of operation
void gpioInit()
{

  // Student Edit:

  //GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_DriveStrengthSet(LED0_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(LED0_port, LED0_pin, gpioModePushPull, false);

  //GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthStrongAlternateStrong);
  GPIO_DriveStrengthSet(LED1_port, gpioDriveStrengthWeakAlternateWeak);
  GPIO_PinModeSet(LED1_port, LED1_pin, gpioModePushPull, false);

 // Enable Pin of Si7021 Sensor
  GPIO_PinModeSet(gpioPortD, 15, gpioModePushPull, false);

  // Enable Pin of Si7021 Sensor
  GPIO_PinModeSet(EXTCOMIN_port, EXTCOMIN_pin, gpioModePushPull, false);

  // Set PB0 as input
  GPIO_PinModeSet(PUSHBUTTON_port,PUSHBUTTON_pin,gpioModeInputPullFilter, 1);


  // Enable GPIO interrupts
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  GPIO_ExtIntConfig(PUSHBUTTON_port, PUSHBUTTON_pin,PUSHBUTTON_pin, true, true, true);



} // gpioInit()


void gpioLed0SetOn()
{
  GPIO_PinOutSet(LED0_port,LED0_pin);
}


void gpioLed0SetOff()
{
  GPIO_PinOutClear(LED0_port,LED0_pin);
}


void gpioLed1SetOn()
{
  GPIO_PinOutSet(LED1_port,LED1_pin);
}


void gpioLed1SetOff()
{
  GPIO_PinOutClear(LED1_port,LED1_pin);
}


void gpioSetDisplayExtcomin(bool state){

  if (state == true){

      GPIO_PinOutSet(EXTCOMIN_port,EXTCOMIN_pin);

  }

  else if (state == false){

      GPIO_PinOutClear(EXTCOMIN_port,EXTCOMIN_pin);

  }

}



