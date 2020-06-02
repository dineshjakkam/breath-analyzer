/*
 * callbacks.c
 *
 *  Created on: May 22, 2020
 *      Author: venkatadineshjakkampudi
 */

#include "callbacks.h"
#include "services.h"
#include "app_ble.h"
#include "adc.h"
#include "bluenrg_gap_aci.h"
#include "bluenrg_gatt_aci.h"

#include<stdbool.h>
#include<stdlib.h>

static volatile uint8_t CONNECTED = FALSE;
static volatile uint8_t NOTIFICATION_ENABLED = FALSE;

void cb_on_gap_connection_complete(uint8_t *, uint16_t);
void cb_on_gap_disconnection_complete(void);
void cb_on_read_request(uint16_t);
void cb_on_attribute_modified(uint16_t, uint16_t, uint8_t []);

uint8_t is_connected(void);


/*
 * @ brief return the status of connection
 * @retvalue True if connected, else False
 */
uint8_t is_connected(void){
	return CONNECTED;
}


/*
 * @ brief return the status of notifications
 */
uint8_t is_notification_enabled(void){
	return NOTIFICATION_ENABLED;
}

/*
 * @brief Call back called on successful connection complete
 * @param peer_addr An array that contains peer address
 * @param handle connection handle
 */
void cb_on_gap_connection_complete(uint8_t peer_addr[], uint16_t handle){
	CONNECTED = TRUE;
	set_connection_handle(handle);
	reset_connectable_status();
}


/*
 * @brief Call back called on successful disconnection
 * 			reset the active connection flags to make
 * 			BLE device to broadcast discovery packets
 * 			again
 */
void cb_on_gap_disconnection_complete(void){
	CONNECTED = FALSE;
	set_connection_handle(0);
	set_connectable_status();
}


/*
 * @brief This is the call back called on attribute modification
 * 			from the connected device e.g., enabling notifications,
 * 			writing from the connected device etc..
 * @param handle The characteristic handle to compare against
 * @param len The length of the data arrived at radio
 * @param data The data arrived
 * @retvalue None
 */
void cb_on_attribute_modified(uint16_t handle, uint16_t len, uint8_t data[]){
	if(is_ba_notification_attribute(handle)){
		if(data[0] == 0x01)
			NOTIFICATION_ENABLED = TRUE;
		else
			NOTIFICATION_ENABLED = FALSE;
	}
}

/*
 * @brief This is call back called through interrupt on push button pressed
 * 	On PB pressed clear the max detected alcohol levels to enable for further
 * 	reading
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	clear_detected_alcohol_levels();
}


