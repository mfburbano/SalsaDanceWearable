/*
 * ble_custom_service.h
 *
 *  Created on: Dec 3, 2017
 *      Author: stheven
 */

#ifndef _BLE_CUSTOM_SERVICE_H_
#define _BLE_CUSTOM_SERVICE_H_

#include <includes.h>

#define READ_PERMITION					0x01
#define WRITE_PERMITION					0x02
#define WRITE_NO_RESPONSE_PERMITION		0X04
#define NOTIFY_PERMITION				0x08
#define INDICATE_PERMITION				0x10

// This is a 16-bit UUID.
#define CUSTOM_SERVICE_BLE_UUID                 0x9800

#define CUSTOM_SERVICE_BLE_CHAR_OPTION_UUID 			CUSTOM_SERVICE_BLE_UUID | 0x0001
#define CUSTOM_SERVICE_BLE_CHAR_ACCEL_DATA_UUID 		CUSTOM_SERVICE_BLE_UUID | 0x0002

/**
 *
 */
typedef struct {
	int16_t accelx[3];
	int16_t accely[3];
	int16_t accelz[3];
} __attribute__ ((aligned))accel_data_char_t;


uint32_t custom_service_init();
void notify_accel_data(accel_data_char_t _accel_data);

#endif /* _BLE_CUSTOM_SERVICE_H_ */
