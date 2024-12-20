/*
 * uart_reading,c
 *
 *  Created on: Dec 8, 2024
 *      Author: tungn
 */



#include "uart_reading.h"

extern UART_HandleTypeDef huart2;

extern I2C_HandleTypeDef hi2c1;

//Globally use in main to take input
uint8_t buffer_byte;
uint8_t buffer[MAX_BUFFER_SIZE];
uint8_t index_buffer = 0;
uint8_t buffer_flag = 0;

//Locally use in uart_reading
uint8_t cmdParserStatus = INIT_UART;
uint8_t cmd_data[MAX_CMD_SIZE];
uint8_t cmd_index = 0;
uint8_t cmd_flag = INIT_UART;

int isCmdEqualToRST(uint8_t str[]){
	int flag = 0;
	if (str[0] == 'R') flag = 1;
	else flag = 0;
	return flag;
}

int isCmdEqualToCAP(uint8_t str[]){
	int flag = 0;
	if (str[0] == 'C') flag = 1;
	else flag = 0;
	return flag;
}

void cmd_parser_fsm(){
	switch(cmdParserStatus){
		case INIT_UART:
			if (buffer_byte == '!') cmdParserStatus = READING;
			break;
		case READING:
			if (buffer_byte != '!' && buffer_byte != '#'){
				cmd_data[cmd_index] = buffer_byte;
				cmd_index++;
			}
			else if (buffer_byte == '!'){
				cmdParserStatus = READING;
			}
			else if (buffer_byte == '#'){
				cmdParserStatus = STOP;
				cmd_index = 0;
			}
			break;
		case STOP:
			if (isCmdEqualToRST(cmd_data)==1) cmd_flag = isRST;
			else if (isCmdEqualToCAP(cmd_data)==1) cmd_flag = isCAP;
			else return;
			cmdParserStatus = INIT_UART;
			break;
		default:
			break;
	}
}

void uart_control_fsm() {
    switch (cmd_flag) {
        case INIT_UART:
            cmd_flag = AUTO;
            timerSet(1,2);
            break;

        case AUTO:
            if (timerFlags[2] == 1) {
                HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, RESET);
                timerSet(300,2);
            }
            reading_fsm_run();
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, SET);
            break;

        case isCAP:
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, RESET);

            timerSet(1000,4);
            cmd_flag = WAIT;
            break;

        case WAIT:
            HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, RESET);
            if (timerFlags[4] == 1) cmd_flag = INIT_UART;
            break;

        case isRST:
            cmd_flag = INIT_UART;
            break;

        default:
            break;
    }
}


void Scan_Addr() {
    char info[] = "\r\n\r\nScanning I2C bus...\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)info, strlen(info), HAL_MAX_DELAY);

    HAL_StatusTypeDef res;
    for(uint16_t i = 0; i < 128; i++) {
        res = HAL_I2C_IsDeviceReady(&hi2c1, i << 1, 1, 10);
        if(res == HAL_OK) {
            char msg[64];
            snprintf(msg, sizeof(msg), "0x%02X", i);
            HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
        } else {
            HAL_UART_Transmit(&huart2, (uint8_t*)".", 1, HAL_MAX_DELAY);
        }
    }
}
void send_data(void) {

	uint16_t rain_mm ;
	uint16_t rain_mm_x10;
	get_rain_values(&rain_mm, &rain_mm_x10);
    float temp = (float)(value_x10[1] / 10) + (float)(value_x10[1] % 10) / 10.0f;;
    float humidity = (float)(value_x10[0] / 10) + (float)(value_x10[0] % 10) / 10.0f;
    float rain = (float)rain_mm + (float)(rain_mm_x10)/1000.0f;

    uint8_t temp_bytes[4];
    uint8_t humidity_bytes[4];
    uint8_t rain_bytes[4];

    memcpy(temp_bytes, &temp, sizeof(temp));
    memcpy(humidity_bytes, &humidity, sizeof(humidity));
    memcpy(rain_bytes, &rain, sizeof(rain));

    HAL_UART_Transmit(&huart2, temp_bytes, sizeof(temp_bytes), 500);
    HAL_UART_Transmit(&huart2, humidity_bytes, sizeof(humidity_bytes), 500);
    HAL_UART_Transmit(&huart2, rain_bytes, sizeof(rain_bytes), 500);
}


void commandParser(void){

	  if (buffer_flag == 1){
		  cmd_parser_fsm();
		  buffer_flag = 0;
	  }
	  if(timerFlags[3] == 1){

		  uart_control_fsm();
	  }
}
