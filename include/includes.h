/*
 * includes.h
 *
 *  Created on: Dec 3, 2017
 *      Author: stheven
 */

#ifndef _INCLUDES_H_
#define _INCLUDES_H_

#include <stdint.h>
#include <string.h>

#include <nrf_log.h>
#include "nrf_log_ctrl.h"
#include "ble_radio_notification.h"
#include <nordic_common.h>
#include <nrf.h>
#include <app_error.h>
#include <ble.h>
#include <ble_hci.h>
#include <ble_srv_common.h>
#include <ble_advdata.h>
#include <ble_advertising.h>
#include <ble_dis.h>
#include <ble_conn_params.h>
#include <softdevice_handler.h>
#include <app_timer.h>
#include <bsp.h>
#include <nrf_delay.h>
#include <bsp_btn_ble.h>
#include <nrf_ble_gatt.h>
#include "nrf_drv_gpiote.h"
#include <ble_conn_state.h>

#include "ble_app.h"
#include "accelerometer_app.h"
#include "ble_custom_service.h"

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            6                                           /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                           /**< Size of timer operation queues. */


#endif /* _INCLUDES_H_ */
