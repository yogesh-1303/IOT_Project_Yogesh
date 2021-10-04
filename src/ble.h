/*
 * ble.h
 *
 *  Created on: 01-Oct-2021
 *      Author: Shrikant
 */

#ifndef SRC_BLE_H_
#define SRC_BLE_H_

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <sl_bt_api.h>
#include "app.h"
#include "em_gpio.h"
#include "efr32bg13p632f512gm48.h"
#include "autogen/gatt_db.h"

#define UINT8_TO_BITSTREAM(p, n)     { *(p)++ = (uint8_t)(n); }
#define UINT32_TO_BITSTREAM(p, n)    { *(p)++ = (uint8_t)(n); *(p)++ = (uint8_t)((n) >> 8); \
                                     *(p)++ = (uint8_t)((n) >> 16); *(p)++ = (uint8_t)((n) >> 24); }
#define UINT32_TO_FLOAT(m, e)        (((uint32_t)(m) & 0x00FFFFFFU) | (uint32_t)((int32_t)(e) << 24))



// BLE Data Structure, save all of our private BT data in here.
// Modern C (circa 2021 does it this way)
// typedef ble_data_struct_t is referred to as an anonymous struct definition
typedef struct {

  // values that are common to servers and clients
  bd_addr myAddress;
  uint8_t myAddressType;

  // values unique for server
  // The advertising set handle allocated from Bluetooth stack.
  uint8_t advertisingSetHandle;
  bool temp_measure_status;
  bool temp_type_status;
  bool temp_interval_status;

   // values unique for client

} ble_data_struct_t;

void transmit_tempdata(sl_bt_msg_t *evt,uint16_t attribute);

void transmit_temptype(sl_bt_msg_t *evt,uint16_t attribute);

void transmit_tempinterval(sl_bt_msg_t *evt,uint16_t attribute);

ble_data_struct_t* getBleDataPtr(void);

void handle_ble_event(sl_bt_msg_t *evt);


#endif /* SRC_BLE_H_ */
