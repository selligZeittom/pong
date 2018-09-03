/* 
 * File:   Btn_controller.h
 * Author: Gilles Mottiez
 *
 * Created on 2. septembre 2018, 21:05
 */

#ifndef BTN_CONTROLLER_H
#define	BTN_CONTROLLER_H

#include "xf.h"
#include "GlobalVariables.h"

typedef enum SM_BTN
{
    ST_BTN_WAITING,
    ST_BTN_PRESSED,
    ST_BTN_CONFIRMED,
            
}SM_BTN;

typedef struct Btn_controller
{
    SM_BTN sm_btn, old_sm_btn;
}Btn_controller;

/*
 * init the state machine
 */
void init_btn_controller(Btn_controller* btnCtrl);

/*
 * process event of the state machine
 */
void state_machine_btn(Btn_controller* btnCtrl, Event ev);

/*
 * start the timer to avoid any rebound
 */
void launch_timer();

#endif	/* BTN_CONTROLLER_H */

