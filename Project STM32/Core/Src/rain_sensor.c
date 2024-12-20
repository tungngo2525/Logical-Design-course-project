/*
 * rain_sensor.c
 *
 *  Created on: Dec 8, 2024
 *      Author: tungn
 */



#include <stdint.h>
#include <stdio.h>
#include "rain_sensor.h"
#include "i2c-lcd.h"
#include <string.h>

extern ADC_HandleTypeDef hadc1;

uint32_t rain_value = 0;
uint16_t rain_measurements[2];


void rain_sensor_read(uint16_t* value) {

    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    rain_value = HAL_ADC_GetValue(&hadc1);

    if (rain_value == 0xFFFF) {
        rain_value = 0;
    }
    uint32_t rain_mm_value = (rain_value * 100) / 4095;
    value[0] = rain_mm_value;

    value[1] = (rain_mm_value * 10);
}


void init_read(void) {

    rain_sensor_read(rain_measurements);
}


void get_rain_values(uint16_t* rain_mm, uint16_t* rain_mm_x10) {
    *rain_mm = rain_measurements[0];
    *rain_mm_x10 = rain_measurements[1];
}

void display_rain_data(void) {
    uint16_t rain_mm, rain_mm_x10;
    lcd_clear_display();

    init_read();
    get_rain_values(&rain_mm, &rain_mm_x10);


    char rain_level[32];
    if (rain_mm < 20) {
        strcpy(rain_level, "  Mua: Mua nho");
    } else if (rain_mm <= 50) {
        strcpy(rain_level, "  Mua: Mua vua");
    } else {
        strcpy(rain_level, "  Mua: Mua to");
    }


    lcd_clear_display();
    lcd_goto_XY(0, 0);
    lcd_send_string(rain_level);


    char rain_amount[32];
    sprintf(rain_amount, "rain: %d.%01d mm", rain_mm, rain_mm_x10 / 10);

    lcd_goto_XY(1, 0);
    lcd_send_string(rain_amount);
    HAL_Delay(300);
}
