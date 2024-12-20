/*
 * input_button.c
 *
 *  Created on: Dec 15, 2024
 *      Author: tungn
 */


#include "input_button.h"



#define DEBOUNCE_DELAY 50

uint8_t buttonPressed(uint8_t button)
{
    static uint8_t lastState[1] = {1};
    uint8_t currentState;

    if (button == 0)
    {
        currentState = HAL_GPIO_ReadPin(GPIOA, BUTTON_0_Pin);

        if (currentState != lastState[button]) {
            HAL_Delay(DEBOUNCE_DELAY);
            currentState = HAL_GPIO_ReadPin(GPIOA, BUTTON_0_Pin);
        }
        lastState[button] = currentState;
        return currentState;
    }
    return 1;
}



