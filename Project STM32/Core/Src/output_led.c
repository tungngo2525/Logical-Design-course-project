/*
 * output_led.c
 *
 *  Created on: Dec 8, 2024
 *      Author: tungn
 */

#include "output_led.h"
void turnOn_RGB(){
	HAL_GPIO_WritePin(LED_RGB_GPIO_Port, LED_RGB_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RGB1_GPIO_Port, LED_RGB1_Pin, GPIO_PIN_SET);
}
void turnOn_RGB1(){
	HAL_GPIO_WritePin(LED_RGB_GPIO_Port, LED_RGB_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RGB1_GPIO_Port, LED_RGB1_Pin, GPIO_PIN_SET);
}
void turnOn_RGB2(){
	HAL_GPIO_WritePin(LED_RGB_GPIO_Port, LED_RGB_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RGB1_GPIO_Port, LED_RGB1_Pin, GPIO_PIN_RESET);
}


void turnOff_RGB(){
	HAL_GPIO_WritePin(LED_RGB_GPIO_Port, LED_RGB_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LED_RGB1_GPIO_Port, LED_RGB1_Pin, GPIO_PIN_RESET);
}

