/*
 * i2c.h
 *
 *  Created on: 15-Sep-2021
 *      Author: Shrikant
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "em_gpio.h"
#include "src/timers.h"
#include "app.h"
#include "em_i2c.h"
#include <sl_i2cspm.h>
#include "em_letimer.h"
#include "efr32bg13p632f512gm48.h"


void I2C_init(void);

uint8_t* I2C_Read_Si7021(void);

bool I2C_Write_Si7021(void);

uint16_t read_temp_si7021(void);

#endif /* SRC_I2C_H_ */
