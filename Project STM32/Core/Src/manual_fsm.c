/*
 * manual_fsm.c
 *
 *  Created on: Dec 8, 2024
 *      Author: tungn
 */



#include "manual_fsm.h"

enum FSM_STATE fsmState = FSM_INIT;

void fsmInit(void) {
    fsmReInit(FSM_INIT);
}

void fsmReInit(enum FSM_STATE state) {
    switch (state) {
        case FSM_INIT:
            lcd_greeting();

            fsmState = READING_FSM_RUN;
            break;

        case READING_FSM_RUN:
        	reading_fsm_run();
            break;

        case DISPLAY_RAIN_DATA:
            display_rain_data();
            break;

        default:
            break;
    }
}


void fsmRun(void) {
    switch (fsmState) {
        case READING_FSM_RUN:
            if (buttonPressed(0)==0) {
                fsmReInit(DISPLAY_RAIN_DATA);

            }
            break;

        case DISPLAY_RAIN_DATA:

            if (buttonPressed(0)==1) {
                fsmReInit(READING_FSM_RUN);

            }
            break;

        default:
            break;
    }
}
