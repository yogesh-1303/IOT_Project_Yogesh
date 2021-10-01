/*
 * ble.c
 *
 *  Created on: 01-Oct-2021
 *      Author: Shrikant
 */

#include "ble.h"

// BLE private data
ble_data_struct_t ble_data;

ble_data_struct_t* getBleDataPtr(){

  return (&ble_data);

}


void handle_ble_event(sl_bt_msg_t *evt){

  sl_status_t sc;

  switch(SL_BT_MSG_ID(evt->header)){

    case sl_bt_evt_system_boot_id:{

      // Extract unique ID from BT Address.
      sc = sl_bt_system_get_identity_address(&ble_data.myAddress, &ble_data.myAddressType);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("\nError in getting identity");
          break;
      }

      //LOG_INFO("\nIdentity Retrieved");
      // Create an advertising set.
      sc = sl_bt_advertiser_create_set(&ble_data.advertisingSetHandle);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("\nError in advertising");
          break;
      }

      // Set advertising interval to 100ms.
       sc = sl_bt_advertiser_set_timing(
             ble_data.advertisingSetHandle, // advertising set handle
              400, // min. adv. interval (milliseconds * 1.6)
              400, // max. adv. interval (milliseconds * 1.6)
              0,   // adv. duration
              0);  // max. num. adv. events
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("\nError in setting advertising time");
           break;
       }

       sc = sl_bt_advertiser_start(
              ble_data.advertisingSetHandle,
              sl_bt_advertiser_general_discoverable,
              sl_bt_advertiser_connectable_scannable);
       if (sc != SL_STATUS_OK) {
           LOG_ERROR("\nError in starting advertising");
           break;
       }
      break;
    }

    case sl_bt_evt_connection_opened_id:{

      sc =  sl_bt_advertiser_stop(ble_data.advertisingSetHandle);
      if (sc != SL_STATUS_OK) {
          LOG_ERROR("\nError in stopping advertising");
          break;
      }

      sc = sl_bt_connection_set_parameters ( (evt->data.evt_connection_opened.connection),
      60,
      60,
      4,
      600,
      0xffff,
      0xffff);

      if (sc != SL_STATUS_OK) {
          LOG_ERROR("\nError in setting connection parameters");
          break;
      }
      break;
    }


    case sl_bt_evt_connection_closed_id:{

      sc = sl_bt_advertiser_start(
                    ble_data.advertisingSetHandle,
                    sl_bt_advertiser_general_discoverable,
                    sl_bt_advertiser_connectable_scannable);
             if (sc != SL_STATUS_OK) {
                 LOG_ERROR("\nError in starting advertising");
                 break;
             }


      break;
    }


    case  sl_bt_evt_connection_parameters_id:{

      // LOG_INFO("");

      break;
    }

    case sl_bt_evt_gatt_server_characteristic_status_id:{





      break;
    }


    case sl_bt_evt_gatt_server_indication_timeout_id:{

      break;
    }


  }


}
