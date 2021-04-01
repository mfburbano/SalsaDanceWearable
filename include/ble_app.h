/*
 * ble_app.h
 *
 *  Created on: Dec 2, 2017
 *      Author: stheven
 */

#ifndef _BLE_APP_H_
#define _BLE_APP_H_

#include <includes.h>

#define CENTRAL_LINK_COUNT              0                                 /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT           1                                 /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define DEVICE_NAME 					"Thesis"
#define APP_ADV_INTERVAL                 1000									/**< The advertising interval (in units of 0.625 ms. This value corresponds to 187.5 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       0										/**< The advertising timeout in units of seconds. */

#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(50, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.4 seconds). */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(70, UNIT_1_25_MS)            /**< Maximum acceptable connection interval (0.65 second). */
#define SLAVE_LATENCY                    0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  // Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds).
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) // Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds).
#define MAX_CONN_PARAMS_UPDATE_COUNT     3                                           // Number of attempts before giving up the connection parameter negotiation.

#define APP_FEATURE_NOT_SUPPORTED       BLE_GATT_STATUS_ATTERR_APP_BEGIN + 2        /**< Reply when unsupported features are requested. */


#define IS_SRVC_CHANGED_CHARACT_PRESENT 0                                 /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define APP_CFG_NON_CONN_ADV_TIMEOUT    0                                 /**< Time for which the device must be advertising in non-connectable mode (in seconds). 0 disables timeout. */
#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(100, UNIT_0_625_MS) /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). */

#define APP_BEACON_INFO_LENGTH          0x17                              /**< Total length of information advertised by the Beacon. */
#define APP_ADV_DATA_LENGTH             0x15                              /**< Length of manufacturer specific data in the advertisement. */
#define APP_DEVICE_TYPE                 0x02                              /**< 0x02 refers to Beacon. */
#define APP_MEASURED_RSSI               0xC3                              /**< The Beacon's measured RSSI at 1 meter distance in dBm. */
#define APP_COMPANY_IDENTIFIER          0x0507                            /**< Company identifier for Nordic Semiconductor ASA. as per www.bluetooth.org. */
#define APP_MAJOR_VALUE                 0x01, 0x02                        /**< Major value used to identify Beacons. */
#define APP_MINOR_VALUE                 0x03, 0x04                        /**< Minor value used to identify Beacons. */
#define APP_BEACON_UUID                 0x01, 0x12, 0x23, 0x34, \
                                        0x45, 0x56, 0x67, 0x78, \
                                        0x89, 0x9a, 0xab, 0xbc, \
                                        0xcd, 0xde, 0xef, 0xf0            /**< Proprietary UUID for Beacon. */

#define DEAD_BEEF                       0xDEADBEEF                        /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */


void ble_stack_init(void);
void gap_params_init(void);
void advertising_init(void);
void gatt_init(void);
void advertising_start(void);
void conn_params_init(void);
void services_init(void);
void on_adv_evt(ble_adv_evt_t ble_adv_evt);


#endif /* _BLE_APP_H_ */
