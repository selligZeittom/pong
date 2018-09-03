#include "TS_controller.h"

/*
 * init the tsc controller machine state
 */
void init_tsc_controller(TS_controller* tsCtrl)
{
    //Set the initial state
    tsCtrl->sm_tsc = ST_TSC_WAIT;
    tsCtrl->old_sm_tsc = tsCtrl->sm_tsc;
}

/*
 * process event of the state machine
 */
void state_machine_tsc(TS_controller* tsCtrl, Event ev)
{
    //store the old state
    tsCtrl->old_sm_tsc = tsCtrl->sm_tsc;

    //on transition actions
    switch (tsCtrl->sm_tsc) {
    case ST_TSC_WAIT:
        if(ev == EV_TIMEOUT_TSC)
        {
            tsCtrl->sm_tsc = ST_TSC_UPDATE;
        }
        break;
    case ST_TSC_UPDATE:
        if(ev == EV_GOTO_TSC_WAIT)
        {
            tsCtrl->sm_tsc = ST_TSC_WAIT;
        }
        break;
    default:
        break;
    }
    
    //on entry actions
    if(tsCtrl->sm_tsc != tsCtrl->old_sm_tsc)
    {
        switch (tsCtrl->sm_tsc) {
        case ST_TSC_WAIT:
            //nothing to do there now
            break;
        case ST_TSC_UPDATE:
            //go and get the new values of the tsc
            tsc_update();
            
            //throw event to go back to wait mode
            XF_pushEvent(EV_GOTO_TSC_WAIT, false);
            break;
        default:
            break;
        }
    }
}

/*
 * get the new values form the tsc
 */
void tsc_update()
{

}
