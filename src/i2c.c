/*
 * i2c.c - Source file Containing implementation of I2C functionality
 * Attributes - Prof.David Sluiter IOT and Embedded Firmware Lecture 5 & Lecture 6
 *
 */

#include "i2c.h"

// Include logging for this file
#define INCLUDE_LOG_DEBUG (1)

#include "src/log.h"

// Si7021 I2C Address
#define SI70_I2C_ADDR (0x40)

// Measure temperature No Hold Master Mode
#define MEASURE_TEMP_CMD (0xF3)

I2C_TransferSeq_TypeDef transferSequence;
uint8_t cmd_data;
uint16_t read_data;

uint8_t *temp_data;

void I2C_init(void){

  uint32_t i2c_freq=0;

  // Assign values for I2C init
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

  // Initialize I2C0
  I2CSPM_Init(&i2cspm);

  i2c_freq=I2C_BusFreqGet(I2C0);


}


void I2C_Read_Si7021(void){

  I2C_TransferReturn_TypeDef check_transfer;

  // Allocate Memory for read buffer to store temperature data
  temp_data=(uint8_t*)malloc(sizeof(uint8_t)*2);


  I2C_init();

  transferSequence.addr = SI70_I2C_ADDR<<1;
  transferSequence.flags = I2C_FLAG_READ;
  transferSequence.buf[0].data = temp_data,
  transferSequence.buf[0].len = sizeof(temp_data),

  NVIC_EnableIRQ(I2C0_IRQn);

  check_transfer = I2C_TransferInit(I2C0, &transferSequence);


  // Map Return status of I2C transfer
  if (check_transfer < 0){

      switch(check_transfer){

         case i2cTransferNack:{
           LOG_ERROR("\n\rNACK Received");
           break;
         }

         case i2cTransferBusErr:{
           LOG_ERROR("\n\rBus Error");
            break;
          }

         case i2cTransferArbLost:{

           LOG_ERROR("\n\rArbitration lost");
             break;
           }

         case i2cTransferUsageFault:{

           LOG_ERROR("\n\rUsage Fault");
             break;
           }

         case i2cTransferSwFault:{

           LOG_ERROR("\n\rSw Fault");
           break;
           }

         default:{

           break;
         }

       }

  }


}


void I2C_Write_Si7021(void){

  I2C_TransferReturn_TypeDef check_transfer;

  I2C_init();

  // Measure temperature No Hold Master Mode command
  cmd_data = MEASURE_TEMP_CMD;

  transferSequence.addr = SI70_I2C_ADDR<<1;
  transferSequence.flags = I2C_FLAG_WRITE;
  transferSequence.buf[0].data = cmd_data;
  transferSequence.buf[0].len = sizeof(cmd_data);

  NVIC_EnableIRQ(I2C0_IRQn);

  check_transfer = I2C_TransferInit(I2C0, &transferSequence);


  // Map Return status of I2C transfer
  if (check_transfer < 0){

      switch(check_transfer){

         case i2cTransferNack:{
           LOG_ERROR("\n\rNACK Received");
           break;
         }

         case i2cTransferBusErr:{
           LOG_ERROR("\n\rBus Error");
            break;
          }

         case i2cTransferArbLost:{

           LOG_ERROR("\n\rArbitration lost");
             break;
           }

         case i2cTransferUsageFault:{

           LOG_ERROR("\n\rUsage Fault");
             break;
           }

         case i2cTransferSwFault:{

           LOG_ERROR("\n\rSw Fault");
           break;
           }

         default:{

           break;
         }

       }

  }


}


void Enable_si7021(bool state){

  // Set Enable Pin high
  if (state == true){

      GPIO_PinOutSet(gpioPortD,15);

  }

  // Set Enable Pin low
  else if (state == false){

      GPIO_PinOutClear(gpioPortD,15);

  }


}

void process_temp_si7021(void){

  // Temporary pointer to point to buffer and read from
  uint8_t *temp_d;

  uint16_t temp = 0;

  uint16_t celsius = 0;



      // Combine 8 bit words by left shiffting MSB by 8
      temp=(256*temp_data[0])+temp_data[1];

     // Convert Temperarure Code to degree Celsius
      celsius = ((175.72*(temp))/65535)  - 46.85;

      // Free allocated buffer
      free(temp_data);

      read_data=celsius;

      // LOG the temperature
      LOG_INFO("Current Temperature : %d\r",(int32_t)read_data);

      // Set temperature after converting temp code to celsius



}


void I2C0_IRQHandler(void){

  I2C_TransferReturn_TypeDef check_transfer;

  check_transfer = I2C_Transfer(I2C0);

  if (check_transfer == i2cTransferDone) {

      schedulerSetI2CdoneEvent();


  }

  if (check_transfer < 0){

      switch(check_transfer){

         case i2cTransferNack:{
           LOG_ERROR("\n\rNACK Received");
           break;
         }

         case i2cTransferBusErr:{
           LOG_ERROR("\n\rBus Error");
            break;
          }

         case i2cTransferArbLost:{

           LOG_ERROR("\n\rArbitration lost");
             break;
           }

         case i2cTransferUsageFault:{

           LOG_ERROR("\n\rUsage Fault");
             break;
           }

         case i2cTransferSwFault:{

           LOG_ERROR("\n\rSw Fault");
           break;
           }

         default:{

           break;
         }

       }


  }





}


