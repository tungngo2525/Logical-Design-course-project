/*
 * dht20.h
 *
 *  Created on: Dec 22, 2024
 *      Author: tungn
 */

#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include <stdio.h>
#include "main.h"
#include "i2c-lcd.h"
#include "software_timer.h"
#include "output_led.h"
#include "uart_reading.h"

#define INIT 0
#define READ 1

extern uint16_t value_x10[2];
extern char temp[20],humid[20];

void dht20_init(void);

void dht20_reset(uint8_t);

void dht20_read(uint16_t*);

void dht20_start(void);

void init_reading(void);
void display_dht20_data(void);


void reading_fsm_run(void);
void notification_lights(void);
void tung(void);
#endif /* INC_DHT20_H_ */
