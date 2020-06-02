/*
 * services.c
 *
 *  Created on: May 21, 2020
 *      Author: venkatadineshjakkampudi
 */

#include "app_ble.h"
#include "bluenrg_gap.h"
#include "hci_const.h"
#include "bluenrg_hal_aci.h"
#include "hci.h"
#include "hci_le.h"
#include "bluenrg_gatt_aci.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_utils.h"
#include "services.h"
#include "callbacks.h"
#include "adc.h"
#include "main.h"

charactFormat charFormat;

const uint8_t breath_analyzer_service_uuid[16] = {0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe2, 0x11, 0x9e, 0xb1, 0xdf, 0xf2, 0x73, 0xd9};
const uint8_t ba_notification_charc_uuid[16] = {0x66, 0x9a, 0x0c, 0x20, 0x00, 0x08, 0x96, 0x9e, 0xe1, 0x11, 0x9e, 0xb1, 0xdf, 0xf2, 0x73, 0xd9};


static uint16_t breathAnalyzerServHandle, baCharcHandle, connectionHandle;

volatile static uint8_t LED_STATUS = 0;
volatile static uint8_t NOTIFICATION_PENDING = FALSE;


/*
 * @brief The service that sends notification to the connected
 *	mobile device when an alcohol is detected
 *
 */
tBleStatus add_breath_analyzer_service(void){
	tBleStatus ret;
	ret = aci_gatt_add_serv(UUID_TYPE_128,
			breathAnalyzerServHandle,
			PRIMARY_SERVICE,
			0x07,
			&breathAnalyzerServHandle);

	//characteristic that send notification on PB press
	ret = aci_gatt_add_char(breathAnalyzerServHandle,
			UUID_TYPE_128,
			ba_notification_charc_uuid,
			20,
			CHAR_PROP_NOTIFY,
			ATTR_PERMISSION_NONE,
			0,
			16,
			1,
			&baCharcHandle);

	return ret;
}


/*
 *  @brief set/reset connection handle on successful completion
 *  		of either the events. Will be called from call_backs.c
 *  @param handle Handle to the connection on successful connection
 *  				0 on successful disconnection
 */
void set_connection_handle(uint16_t handle){
	connectionHandle = handle;
}


/*
 * @brief Set the flag to true on alcohol detection to send out
 * 	notifications to connected mobile device
 */
void set_notification_pending(void){
	NOTIFICATION_PENDING = TRUE;
}


/*
 *  @brief return connection handle cached on successful
 *  		connection setup
 *  @retvalue connection handle
 */
uint16_t get_connection_handle(void){
	return connectionHandle;
}


/*
 * @brief Checks if the attribute change is corresponding to notification
 * 			characteristic
 * @param handle Handle corresponding to breath analyzer notification charac
 * @retvalue returns bool corresponding to comparison
 */
bool is_ba_notification_attribute(uint16_t handle){
	return (handle == (baCharcHandle+2));
}


/*
 *  @brief Send out notification on push button press
 */
void send_notification(void){
	if(is_notification_enabled() && NOTIFICATION_PENDING){
		uint8_t value = get_no_of_beers();
		aci_gatt_update_char_value(breathAnalyzerServHandle, baCharcHandle, 0, 1, (uint8_t *)&value);
		HAL_GPIO_WritePin(GreenLED_GPIO_Port, GreenLED_Pin, GPIO_PIN_RESET);
		NOTIFICATION_PENDING = FALSE;
	}
}
