/*
 * i2c.c
 *
 *  Created on: 15-Sep-2021
 *      Author: Shrikant
 */

#include "i2c.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG (1)

#include "src/log.h"

#define SI70_I2C_ADDR (0x40)

#define MEASURE_TEMP_CMD (0xF3)

void I2C_init(void){

  uint32_t i2c_freq=0;


  I2CSPM_Init_TypeDef i2cspm = {

      .i2cClhr = i2cClockHLRStandard,
      .i2cMaxFreq = I2C_FREQ_STANDARD_MAX,
      .i2cRefFreq = 0,
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


  uint8_t *temp_data=(uint8_t*)malloc(sizeof(uint8_t)*2);

  I2C_TransferReturn_TypeDef check_transfer;

  I2C_TransferSeq_TypeDef read = {

      .addr = SI70_I2C_ADDR<<1,
      .flags = I2C_FLAG_READ,
      .buf[0].data = temp_data,
      .buf[0].len = sizeof(temp_data),


  };

  check_transfer=I2CSPM_Transfer(I2C0,&read);

  while(check_transfer == i2cTransferNack);

  return temp_data;

}


bool I2C_Write_Si7021(void){


  uint8_t command = MEASURE_TEMP_CMD;

  I2C_TransferReturn_TypeDef check_transfer;

  I2C_TransferSeq_TypeDef write ={

      .addr = SI70_I2C_ADDR<<1,
      .flags = I2C_FLAG_WRITE,

      .buf[0].data = &command,
      .buf[0].len = 1,

  };

  check_transfer=I2CSPM_Transfer(I2C0,&write);

  //timerWaitUs(10800);

  LOG_INFO("\rWriting to I2C");

  switch(check_transfer){

    case i2cTransferInProgress:{

      break;
    }

    case i2cTransferDone:{

      return true;
      break;

    }

    case i2cTransferNack:{

      LOG_ERROR("\rNACK Received");
      break;
    }

    case i2cTransferBusErr:{
      LOG_ERROR("\rBus Error");
       break;
     }

    case i2cTransferArbLost:{

      LOG_ERROR("\rArbitration lost");
        break;
      }

    case i2cTransferUsageFault:{

      LOG_ERROR("\rUsage Fault");
        break;
      }

    case i2cTransferSwFault:{

      LOG_ERROR("\rSw Fault");
      break;
      }

    default:{
      break;
    }

  }

  return false;

}

uint16_t read_temp_si7021(void){

  uint8_t *temp_d;

  uint16_t temp = 0;

  uint16_t celsius = 0;

  GPIO_DriveStrengthSet(gpioPortD, gpioDriveStrengthWeakAlternateStrong);

  GPIO_PinModeSet(gpioPortD, 15, gpioModePushPull, false);

  GPIO_PinOutSet(gpioPortD,15);

  timerWaitUs(80000);


  if(I2C_Write_Si7021() == true){

      timerWaitUs(10800);

      temp_d = I2C_Read_Si7021();

      temp=(256*temp_d[0])+temp_d[1];

      GPIO_PinOutClear(gpioPortD,15);


     // Convert Temperarure Code to degree Celsius

      celsius = ((175.52*(temp))/65535)  - 46.85;

      // Free allocated buffer
      free(temp_d);

      LOG_INFO("\r Current Temperature : %d",(int32_t)celsius);

      return (celsius);

  }

  return 0;

}


