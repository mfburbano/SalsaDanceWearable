/*
 * ble_custom_service.c
 *
 *  Created on: Dec 3, 2017
 *      Author: stheven
 */

#include <ble_custom_service.h>


// Implement service
static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;
static uint16_t m_service_handle;
static uint8_t m_uuid_type;
static ble_uuid_t m_service_uuid;

// Implement option characteristic
static ble_gatts_char_handles_t m_char_handle_option;
static uint8_t m_char_value_option;

// Implement accel data characteristic
static ble_gatts_char_handles_t m_char_handle_accel_data;
static accel_data_char_t m_char_value_accel_data;


static uint32_t init_characteristic(char* char_name,
			uint16_t service_handle,
			uint16_t uuid,
			uint8_t uuid_type,
			uint8_t properties,
			uint16_t max_len,
			uint8_t* data_buffer,
			ble_gatts_char_handles_t* m_char_handle)
{
	uint32_t result;
	ble_gatts_char_md_t char_md             = {{0}};
	ble_gatts_attr_t    attr_char_value     = {0};
	ble_gatts_attr_md_t cccd_md				= {{0}};
	ble_gatts_attr_md_t attr_md             = {{0}};
	ble_uuid_t          char_uuid;


	cccd_md.vloc                = BLE_GATTS_VLOC_STACK;
	cccd_md.vlen = 0;
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);

	char_md.char_props.read = (READ_PERMITION & properties);
	char_md.char_props.write = (WRITE_PERMITION & properties) >> 1;
	char_md.char_props.write_wo_resp = (WRITE_NO_RESPONSE_PERMITION & properties) >> 2;
	char_md.char_props.notify = (NOTIFY_PERMITION & properties) >> 3;
	char_md.char_props.indicate = (INDICATE_PERMITION & properties) >> 4;


	if(char_md.char_props.notify || char_md.char_props.indicate)
	{
		char_md.p_cccd_md = &cccd_md;
	}
	else
	{
		char_md.p_cccd_md = NULL;
	}

	char_uuid.type = uuid_type;
	char_uuid.uuid = uuid;

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

	//attr_md.vloc    = BLE_GATTS_VLOC_STACK;
	attr_md.vloc    = BLE_GATTS_VLOC_USER;
	attr_md.vlen = 0;

	attr_char_value.p_uuid    = &char_uuid;
	attr_char_value.p_attr_md = &attr_md;
	attr_char_value.max_len   = max_len;
	attr_char_value.init_len = max_len;
	attr_char_value.p_value   = data_buffer;

	result = sd_ble_gatts_characteristic_add(service_handle,
									&char_md,
									&attr_char_value,
									m_char_handle);
	NRF_LOG_INFO("init_characteristic uuid %x result %d (%s) size %d\r\n", uuid, result,(uint32_t)char_name, max_len);
	return result;

}

uint32_t custom_service_init(){
	NRF_LOG_INFO("%s\r\n",__FUNCTION__);
	uint32_t err_code;
	m_conn_handle = BLE_CONN_HANDLE_INVALID;

	BLE_UUID_BLE_ASSIGN(m_service_uuid, CUSTOM_SERVICE_BLE_UUID);

	err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
										&m_service_uuid,
										&m_service_handle);
	APP_ERROR_CHECK(err_code);

	const ble_uuid128_t base_uuid128 =
	{
		{
			0x73, 0x65, 0x69, 0x73, 0x6d, 0x69, 0x63, 0x5f,
			0x61, 0x63, 0x63, 0x5f, 0x64, 0x65, 0x74, 0x20
		}
	};
	err_code = sd_ble_uuid_vs_add(&base_uuid128, &m_uuid_type);
	APP_ERROR_CHECK(err_code);

	init_characteristic("option",
						m_service_handle,
						CUSTOM_SERVICE_BLE_CHAR_OPTION_UUID,
						m_uuid_type,
						WRITE_NO_RESPONSE_PERMITION|NOTIFY_PERMITION,
						sizeof(m_char_value_option),
						(uint8_t *)&m_char_value_option,
						&m_char_handle_option);

	init_characteristic("Accelerometer data",
						m_service_handle,
						CUSTOM_SERVICE_BLE_CHAR_ACCEL_DATA_UUID,
						m_uuid_type,
						NOTIFY_PERMITION,
						sizeof(m_char_value_accel_data),
						(uint8_t *)&m_char_value_accel_data,
						&m_char_handle_accel_data);

}



//Events callback functions

/**@brief Function for handling the BLE_GAP_EVT_CONNECTED event.
 *
 * @details Handles BLE_GAP_EVT_CONNECTED event from the BLE stack.
 *
  * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_evt_t * p_ble_evt) {
	m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
	NRF_LOG_DEBUG("%s\r\n",(uint32_t)__FUNCTION__);
}


/**@brief Function for handling the BLE_GAP_EVT_DISCONNECTED event.
 *
 * @details Handles BLE_GAP_EVT_DISCONNECTED event from the BLE stack.
 *
  * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_disconnect(ble_evt_t * p_ble_evt) {
	UNUSED_PARAMETER(p_ble_evt);
	m_conn_handle = BLE_CONN_HANDLE_INVALID;
	NRF_LOG_DEBUG("%s\r\n",(uint32_t)__FUNCTION__);
}

/**@brief Function for handling the BLE_GATTS_EVT_WRITE event.
 *
 * @details Handles BLE_GATTS_EVT_WRITE event from the BLE stack.
 *
  * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_ble_write(ble_evt_t * p_ble_evt) {
	ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

	NRF_LOG_DEBUG("%s\r\n",(uint32_t)__FUNCTION__);
	//Number of tags
	if (p_evt_write->handle == m_char_handle_option.value_handle){
		NRF_LOG_DEBUG("Custom Service: m_char_handle_option.value_handle = 0x%x\r\n", p_evt_write->data[0]);
	}

	// Notification flag (accelerometer data)
	else if (p_evt_write->handle == m_char_handle_accel_data.cccd_handle){
		NRF_LOG_DEBUG("Custom Service:m_char_value_accel_data.cccd_handle = 0x%x\r\n", p_evt_write->data[0]);
		if(p_evt_write->data[0])
			accelerometer_enable_notification();
		else
			accelerometer_disable_notification();

	}

}


/**@brief Function for handling the HVC event.
 *
 * @details Handles HVC events from the BLE stack.
 *
  * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_hvc(ble_evt_t * p_ble_evt)
{
	// ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

	// Check if write event is performed on our characteristic or the CCCD
	//if (p_evt_write->handle == p_mesh_service->msn_handle.value_handle)
	//	msn_handle_evt(p_mesh_service, p_ble_evt);
	NRF_LOG_DEBUG("%s\r\n",(uint32_t)__FUNCTION__);
}



/**@brief Function for handling the QGuard custom service event.
 *
 * @details Handles CUSTOM service events from the BLE stack.
 *
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
void custom_service_on_evt(ble_evt_t * p_ble_evt) {
	switch (p_ble_evt->header.evt_id) {
	case BLE_GAP_EVT_CONNECTED:
		on_connect(p_ble_evt);
		break;

	case BLE_GAP_EVT_DISCONNECTED:
		on_disconnect(p_ble_evt);
		break;

	case BLE_GATTS_EVT_WRITE:
		on_ble_write(p_ble_evt);
		break;
    case BLE_GATTS_EVT_HVC:
        on_hvc(p_ble_evt);
        break;

	default:
		// No implementation needed.
		break;
	}
}


void notify_accel_data(accel_data_char_t _accel_data){
    // Send value if connected and notifying
	NRF_LOG_DEBUG("%s\r\n",(uint32_t)__FUNCTION__);

   	m_char_value_accel_data = _accel_data;
   	uint16_t len = sizeof(_accel_data);
   	ble_gatts_hvx_params_t hvx_params;
   	memset(&hvx_params, 0, sizeof(hvx_params));
   	hvx_params.handle = m_char_handle_accel_data.value_handle;
   	hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
   	hvx_params.offset = 0;
   	hvx_params.p_len = &len;
   	hvx_params.p_data = (uint8_t *) &m_char_value_accel_data;

   	sd_ble_gatts_hvx(m_conn_handle, &hvx_params);
}
