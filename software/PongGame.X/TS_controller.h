/* 
 * File:   TS_controller.h
 * Author: Gilles Mottiez
 *
 * Created on 2. septembre 2018, 20:35
 */

#ifndef TS_CONTROLLER_H
#define	TS_CONTROLLER_H

#include "xf.h"

//define states of the machine
typedef enum SM_TSC
{
    ST_TSC_WAIT,
    ST_TSC_UPDATE,
};

//controller for the double switch pattern
typedef struct TS_controller
{
    SM_TSC sm_tsc, old_sm_tsc;
} TS_controller;

/*
 * init the tsc controller machine state
 */
void init_tsc_controller(TS_controller* tsCtrl);

/*
 * process event of the state machine
 */
void state_machine_tsc(TS_controller* tsCtrl, Event ev);

/*
 * get the new values form the tsc
 */
void tsc_update();
#endif	/* TS_CONTROLLER_H */

