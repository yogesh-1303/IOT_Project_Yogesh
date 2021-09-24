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

// Master mode transfer message structure
I2C_TransferSeq_TypeDef transferSequence;

// Variable for temperature commands
uint8_t cmd_data;

// Variable for temperature in celsius
uint16_t read_data;

// Buffer for storing read data
uint8_t *temp_buffer;

void I2C_init(void){

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

  //i2c_freq=I2C_BusFreqGet(I2C0);


}


void I2C_Read_Si7021(void){

  I2C_TransferReturn_TypeDef check_transfer;

  // Allocate Memory for read buffer to store temperature data
  temp_buffer=(uint8_t*)malloc(sizeof(uint8_t)*2);


  I2C_init();

  transferSequence.addr = SI70_I2C_ADDR<<1;             // Si7021 I2C Address
  transferSequence.flags = I2C_FLAG_READ;               // Set transfer flas as Read
  transferSequence.buf[0].data = temp_buffer,           // Assign buffer to store read data
  transferSequence.buf[0].len = sizeof(temp_buffer),    // Assign size of the buffer

  // Enable I2C Interrupt
  NVIC_EnableIRQ(I2C0_IRQn);

  // Prepare and Start I2C Transfer
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

  transferSequence.addr = SI70_I2C_ADDR<<1;                // Si7021 I2C Address
  transferSequence.flags = I2C_FLAG_WRITE;                 // Set transfer flas as write
  transferSequence.buf[0].data = &cmd_data;                // Assign address of Temperature command
  transferSequence.buf[0].len = sizeof(cmd_data);          // Assign size

  // Enable I2C Interrupt
  NVIC_EnableIRQ(I2C0_IRQn);

  // Prepare and Start I2C Transfer
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


  uint16_t temp = 0;

  uint16_t celsius = 0;

  // Combine 8 bit words by left shiffting MSB by 8
  temp=(256*temp_buffer[0])+temp_buffer[1];

  // Convert Temperarure Code to degree Celsius
  celsius = ((175.72*(temp))/65535)  - 46.85;

  // Free allocated buffer
  free(temp_buffer);

  read_data=celsius;

  // LOG the temperature
  LOG_INFO("Current Temperature : %d\r",(int32_t)read_data);


}


void I2C0_IRQHandler(void){

  I2C_TransferReturn_TypeDef check_transfer;

  // Continue the initiated I2C transfer
  check_transfer = I2C_Transfer(I2C0);

  // Check status of transfer
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


