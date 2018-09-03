#include "Btn_controller.h"
#include "GlobalVariables.h"

/*
 * init the state machine
 */
void init_btn_controller(Btn_controller* btnCtrl)
{
    //set the inital state
    btnCtrl->sm_btn = ST_BTN_WAITING;
    btnCtrl->old_sm_btn = btnCtrl->sm_btn;
}

/*
 * process event of the state machine
 */
void state_machine_btn(Btn_controller* btnCtrl, Event ev)
{
    //Store the old state
    btnCtrl->old_sm_btn = btnCtrl->sm_btn;

    //on transition actions
    switch (btnCtrl->sm_btn) {
    //no body is using the button
    case ST_BTN_WAITING:
        if(ev == EV_BTN_PRESSED)
        {
            btnCtrl->sm_btn = ST_BTN_WAITING;
        }
        break;

    // somebody pressed the button : wait a bit to make sure no rebound happens
    case ST_BTN_PRESSED:
        if(ev == EV_BTN_TIMEOUT)
        {
            btnCtrl->sm_btn = ST_BTN_CONFIRMED;
        }
        break;
    //event has been launched, back to unpressed state
    case ST_BTN_CONFIRMED:
        if(ev == EV_BTN_GOTO_WAIT)
        {
            btnCtrl->sm_btn = ST_BTN_WAITING;
        }
        break;
    default:
        break;
    }

    //on entry actions
    if(btnCtrl->old_sm_btn != btnCtrl->sm_btn)
    {
        switch (btnCtrl->sm_btn) {
        case ST_BTN_WAITING:
            //nothing to do there for the moment
            break;
        case ST_BTN_PRESSED:
            //launch timer
            launch_timer();
            break;
        case ST_BTN_CONFIRMED:
            //throw event
            Event btnConfirmed = EV_BTN_TIMEOUT;
            XF_pushEvent(btnConfirmed, false);
            break;
        default:
            break;
        }
    }
}

/*
 * start the timer to avoid any rebound
 */
void launch_timer()
{
    XF_scheduleTimer()
}
