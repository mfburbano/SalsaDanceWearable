/* Copyright (c) 2014 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/** @file
 *
 * @defgroup ble_sdk_app_beacon_main main.c
 * @{
 * @ingroup ble_sdk_app_beacon
 * @brief Beacon Transmitter Sample Application main file.
 *
 * This file contains the source code for an Beacon transmitter sample application
 * with variable data updated based on advertising inverval.
 */

#include <includes.h>


#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */
#pragma pack(pop)   /* restore original alignment from stack */

/**@brief Function for doing power management.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    uint32_t err_code;
    // Initialize.
    err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    // Start execution.
    NRF_LOG_INFO("\r\n\r\n\r\n\r\n-----------------------------------------------\r\n");
    NRF_LOG_INFO("BLE Beacon started\r\n");

    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

    bsp_board_leds_init();

    // Initialize BLE
    ble_stack_init();
    gap_params_init();
    advertising_init();
    gatt_init();
    services_init();
    conn_params_init();

    // Initialize accelerometer
    accelerometer_init();

    for (int i = 0; i < 5; ++i) {
        bsp_board_led_on(0);
        nrf_delay_ms(50);
        bsp_board_led_off(0);
        nrf_delay_ms(50);
	}

    advertising_start();

    // Enter main loop.
    for (;; )
    {

        if (NRF_LOG_PROCESS() == false)
        {
        	power_manage();
        }
    }
}


/**
 * @}
 */
