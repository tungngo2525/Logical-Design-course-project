/*
 * rain_sensor.h
 *
 *  Created on: Dec 10, 2024
 *      Author: tungn
 */

#ifndef INC_RAIN_SENSOR_H_
#define INC_RAIN_SENSOR_H_

#include <stdio.h>
#include "main.h"
#include "i2c-lcd.h"
#include "software_timer.h"
#include "stm32f1xx_hal.h"
#include <stdint.h>

extern uint16_t rain_measurements[2];

void rain_sensor_read(uint16_t* value);
void init_read(void);
void get_rain_values(uint16_t* rain_mm, uint16_t* rain_mm_x10);
void display_rain_data(void);

#endif /* INC_RAIN_SENSOR_H_ */
