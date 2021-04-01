/*
 * accelerometer_app.c
 *
 *  Created on: Dec 2, 2017
 *      Author: stheven
 */


#include "accelerometer_app.h"

ACCELERO_Data_t ACCELERO_Data;
LIS2DH12_Data_t LIS2DH12_0_Data;
DrvContextTypeDef accelHandle;
#define APP_TIMER_PRESCALER			0
#define ACCEL_PULLING_INTERVAL		APP_TIMER_TICKS(20, APP_TIMER_PRESCALER)

APP_TIMER_DEF(m_accel_pulling_data_id);
bool accelerometer_notification_flag = false;
accel_data_char_t accelerometer_data_char;
uint8_t accelerometer_counter_data_char = 0;

typedef struct
{
  int16_t AXIS_X;
  int16_t AXIS_Y;
  int16_t AXIS_Z;
} SensorAxesInt16_t;


static void accel_pulling_data_handler(){
	//NRF_LOG_INFO("%s\r\n",__FUNCTION__);
	SensorAxes_t accelAxes32;
	SensorAxesInt16_t accelAxes16;

	LIS2DH12_Drv.Get_Axes(&accelHandle, &accelAxes32);
	accelAxes16.AXIS_X = (int16_t)accelAxes32.AXIS_X;
	accelAxes16.AXIS_Y = (int16_t)accelAxes32.AXIS_Y;
	accelAxes16.AXIS_Z = (int16_t)accelAxes32.AXIS_Z;

	if (accelerometer_counter_data_char > 2 ){
		notify_accel_data(accelerometer_data_char);
		memset(&accelerometer_data_char,0,sizeof(accelerometer_data_char));
		accelerometer_counter_data_char = 0;
	}
	accelerometer_data_char.accelx[accelerometer_counter_data_char] = accelAxes16.AXIS_X;
	accelerometer_data_char.accely[accelerometer_counter_data_char] = accelAxes16.AXIS_Y;
	accelerometer_data_char.accelz[accelerometer_counter_data_char] = accelAxes16.AXIS_Z;
	NRF_LOG_DEBUG("Accel X:%d Y:%d Z:%d\r\n",accelerometer_data_char.accelx[accelerometer_counter_data_char],
											accelerometer_data_char.accely[accelerometer_counter_data_char],
											accelerometer_data_char.accelz[accelerometer_counter_data_char]);

	accelerometer_counter_data_char++;
}

int accelerometer_init(void){
	NRF_LOG_INFO("%s\r\n",__FUNCTION__);
	uint32_t err_code;

	/* Setup sensor handle. */
    accelHandle.who_am_i      = LIS2DH12_WHO_AM_I;
    accelHandle.ifType        = 1; /* I2C interface */
    accelHandle.spiDevice     = SPIM0_SS_PIN;
    accelHandle.instance      = 0;
    accelHandle.isInitialized = 0;
    accelHandle.isEnabled     = 1;
    accelHandle.isCombo       = 0;
    accelHandle.pData         = ( void * )&ACCELERO_Data;
    accelHandle.pVTable       = ( void * )&LIS2DH12_Drv;
    accelHandle.pExtVTable    = ( void * )&LIS2DH12_ExtDrv;

    ACCELERO_Data.pComponentData = ( void * )&LIS2DH12_0_Data;
    ACCELERO_Data.pExtData       = 0;
    LIS2DH12_Drv.Init(&accelHandle);

    /* Set high resolution mode. */
    if ( LIS2DH12_SetMode( ( void * )&accelHandle, LIS2DH12_HIGH_RES ) == MEMS_ERROR )
    {
    	NRF_LOG_ERROR("LIS2DH12_SetMode");
      return COMPONENT_ERROR;
    }

    err_code = app_timer_create(&m_accel_pulling_data_id, APP_TIMER_MODE_REPEATED, accel_pulling_data_handler);
    APP_ERROR_CHECK(err_code);

    return 0;
}

void accelerometer_enable_notification(){
	app_timer_start(m_accel_pulling_data_id, ACCEL_PULLING_INTERVAL, NULL);
}

void accelerometer_disable_notification(){
	memset(&accelerometer_data_char,0,sizeof(accelerometer_data_char));
	accelerometer_counter_data_char = 0;
	app_timer_stop(m_accel_pulling_data_id);
}
