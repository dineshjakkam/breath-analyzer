/*
 * services.h
 *
 *  Created on: May 21, 2020
 *      Author: venkatadineshjakkampudi
 */

#ifndef INC_SERVICES_H_
#define INC_SERVICES_H_

#include "bluenrg_aci_const.h"
#include <stdint.h>
#include <stdbool.h>

void read_data(uint8_t *, uint8_t);
void set_notification_pending(void);
void set_connection_handle(uint16_t);
void send_notification(void);

tBleStatus add_breath_analyzer_service(void);

uint16_t get_connection_handle(void);

bool is_ba_notification_attribute(uint16_t);

#endif /* INC_SERVICES_H_ */
