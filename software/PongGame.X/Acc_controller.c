#include <xc.h>             // XC8 General Include File
#include <stdint.h>         // usage of standard types
#include <stdbool.h>        // usage of boolean types
#include "Acc_controller.h"
#include "Game_controller.h"

/******************************************************************************/
/* FUNCTION : tscInit                                                         */
/* INPUT		: -                                                   */
/* OUTPUT		: -                                                   */
/******************************************************************************/
/* COMMENTS     : Initialise Accelerometer                            */
/******************************************************************************/
void AccInit(Acc_controller* accCtrl)
{
    //init states machine
    accCtrl->sm_acc = ST_ACC_WAIT;
    accCtrl->old_sm_acc = accCtrl->sm_acc;
    
    nCS_ACC = 1; // chip select
    DIR_nCS_ACC = 0 ;   // set nCS in output
    DIR_INT_ACC = 1 ;   // set accelerometer interrupt in input
}

/*
 * process events for the accelerometer
 */
void state_machine_accelerometer(Acc_controller* accCtrl, Event ev)
{
    //store old state
    accCtrl->old_sm_acc = accCtrl->sm_acc;
    
    //on transition
    switch (accCtrl->sm_acc) {
    case ST_ACC_WAIT:
        if(ev == EV_TIMEOUT_ACC)
        {
            accCtrl->sm_acc = ST_ACC_UPDATE;
        }
        break;
    case ST_ACC_UPDATE:
        if(ev == EV_GOTO_ACC_WAIT)
        {
            accCtrl->sm_acc = ST_ACC_WAIT;
        }
        break;
    default:
        break;
    }

    //on entry actions
    if(accCtrl->sm_acc != accCtrl->old_sm_acc)
    {
        switch (accCtrl->sm_acc) {
        case ST_ACC_WAIT:
            //nothing to do there right now
            break;
        case ST_ACC_UPDATE:
            //get the new values from the register of the accelerometer
            acc_update();
            
            //throw event to go back in wait mode
            XF_pushEvent(EV_GOTO_ACC_WAIT, false);
            break;
        default:
            break;
        }
    }
}

/*
 * read new values for accelerometer
 */
 void acc_update()
 {

 }
