/*
 * accelerometer_app.h
 *
 *  Created on: Dec 2, 2017
 *      Author: stheven
 */

#ifndef _ACCELEROMETER_APP_H_
#define _ACCELEROMETER_APP_H_

#include <includes.h>
#include "LIS2DH12_ACC_driver_HL.h"

int accelerometer_init(void);

void accelerometer_enable_notification(void);

void accelerometer_disable_notification(void);

#endif /* _ACCELEROMETER_APP_H_ */
