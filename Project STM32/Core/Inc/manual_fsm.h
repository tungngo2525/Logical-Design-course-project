/*
 * manual_fsm.h
 *
 *  Created on: Dec 15, 2024
 *      Author: tungn
 */

#ifndef MANUAL_FSM_H_
#define MANUAL_FSM_H_
#include "main.h"
#include "input_button.h"
#include "software_timer.h"
#include <string.h>
#include "dht20.h"
#include "rain_sensor.h"


enum FSM_STATE {
	FSM_INIT,
    READING_FSM_RUN,
    DISPLAY_RAIN_DATA
};


void fsmInit(void);
void fsmReInit(enum FSM_STATE state);
void fsmRun(void);

#endif /* MANUAL_FSM_H_ */

