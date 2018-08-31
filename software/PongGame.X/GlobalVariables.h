/* 
 * File:   GlobalVariables.h
 * Author: Gilles Mottiez
 *
 * Created on 31. août 2018, 10:34
 */

#ifndef GLOBALVARIABLES_H
#define	GLOBALVARIABLES_H

#include "xf.h"

//events for the button state machine
const Event EV_BTN_PRESSED = 0;
const Event EV_BTN_TIMEOUT = 1;
const Event EV_BTN_GOTO_WAIT = 2;

//events for the tsc state machine
const Event EV_TIMEOUT_TSC = 10;
const Event EV_GOTO_TSC_WAIT = 11;

//events for the mode state machine
const Event EV_ON = 20;
const Event EV_OFF = 21;

//events for the mode ON state machine
const Event EV_GAME = 30;
const Event EV_MENU = 31;

//events for the mode Menu state machine
const Event EV_LEVEL_SW = 40;
const Event EV_CTRL_SW = 41;
const Event EV_GOTO_MENU_WAIT = 42;

//events for the mode Game screen state machine
const Event EV_TIMEOUT_SCREEN = 50;
const Event EV_GOTO_SCREEN_WAIT = 51;

//events for the mode Game racket state machine
const Event EV_TIMEOUT_AI = 60;
const Event EV_TIMEOUT_USER = 61;
const Event EV_GOTO_RACKET_WAIT = 62;

//events for the mode Game ball state machine
const Event EV_TIMEOUT_BALL = 70;
const Event EV_CHECK_RACKET = 71;
const Event EV_MOVE = 72;
const Event EV_GAME_OVER = 73;
const Event EV_GOTO_BALL_WAIT = 74;


//events for the accelerometer state machine
const Event EV_TIMEOUT_ACC = 80;
const Event EV_GOTO_ACC_WAIT = 81;


#endif	/* GLOBALVARIABLES_H */

