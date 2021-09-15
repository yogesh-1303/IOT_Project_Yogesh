/*
 * i2c.c
 *
 *  Created on: 15-Sep-2021
 *      Author: Shrikant
 */

#include "i2c.h"

#define SI70_I2C_ADDR (0x40)

#define MEASURE_TEMP_CMD (0xF3)

void I2C_init(void){

  uint32_t i2c_freq=0;


  I2CSPM_Init_TypeDef i2cspm = {

      .i2cClhr = i2cClockHLRStandard,
      .i2cMaxFreq = 0,
      .i2cRefFreq = I2C_FREQ_STANDARD_MAX,
      .port = I2C0,
      .portLocationScl = 14u,
      .portLocationSda = 16u,
      .sclPin = 10u,
      .sclPort = gpioPortC,
      .sdaPin = 11u,
      .sdaPort = gpioPortC,

      };

  I2CSPM_Init(&i2cspm);

  i2c_freq=I2C_BusFreqGet(I2C0);

  i2c_freq++;

}


uint8_t* I2C_Read_Si7021(void){

  //static uint8_t temp_data[1];
  //static uint8_t temp_data[2];

  uint8_t *temp_data=(uint8_t*)malloc(sizeof(uint8_t)*2);

  I2C_TransferReturn_TypeDef check_transfer;

  I2C_TransferSeq_TypeDef read = {

      //.addr = SI70_I2C_ADDR,
      .addr = (0x40)<<1,
      .flags = I2C_FLAG_READ,
      .buf[0].data = temp_data,
      .buf[0].len = sizeof(temp_data),


  };

  while((check_transfer=I2CSPM_Transfer(I2C0,&read)) == i2cTransferNack);

  temp_data[0] = (read.buf[0].data[0]);

  temp_data[1] = (read.buf[0].data[1]);

  return temp_data;

}


bool I2C_Write_Si7021(void){

  int test=0;

  //uint8_t command = MEASURE_TEMP_CMD;
  uint8_t command = 0xF3;

  I2C_TransferReturn_TypeDef check_transfer;

  I2C_TransferSeq_TypeDef write ={

      //.addr = SI70_I2C_ADDR,
      .addr = (0x40)<<1,
      .flags = I2C_FLAG_WRITE,

      .buf[0].data = &command,
      .buf[0].len = 1,

  };

  check_transfer=I2CSPM_Transfer(I2C0,&write);


  timerWaitUs(10800);


  //while((check_transfer=I2CSPM_Transfer(I2C0,&write)) == i2cTransferNack);

  /*if (check_transfer == i2cTransferDone){
      return true;
  }

  else return false;*/

  switch(check_transfer){

    case i2cTransferInProgress:{
      test++;
      break;
    }

    case i2cTransferDone:{
      test++;
      return true;
      break;
    }

    case i2cTransferNack:{
      test++;
      break;
    }

    case i2cTransferBusErr:{
       LOG_INFO("Here");
       break;
     }

    case i2cTransferArbLost:{
      test++;
        break;
      }

    case i2cTransferUsageFault:{
      test++;
        break;
      }

    case i2cTransferSwFault:{
      test++;
        break;
      }

    default:{
      test++;
      break;
    }

  }

  return false;

}

uint8_t read_temp_si7021(void){

  //uint8_t* temp_data;

  uint8_t *temp_d;

  uint8_t temp =0;

  GPIO_DriveStrengthSet(gpioPortD, gpioDriveStrengthWeakAlternateStrong);

  GPIO_PinModeSet(gpioPortD, 15, gpioModePushPull, false);

  GPIO_PinOutSet(gpioPortD,15);

  timerWaitUs(80000);


  if(I2C_Write_Si7021() == true){

      temp_d = I2C_Read_Si7021();

  }

  temp=temp_d[0]+temp_d[1];

  GPIO_PinOutClear(gpioPortD,15);

 // GPIO_PinOutSet(gpioPortD,15);


  return (temp);

}


