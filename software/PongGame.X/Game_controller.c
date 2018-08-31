#include "Game_controller.h"

void init_Game_controller(Game_controller* gameCtrl)
{
    //init state machine of the mode ON or OFF
    gameCtrl->sm_mode = ST_ON;
    gameCtrl->old_sm_mode = gameCtrl->sm_mode;
    
    //init the state machine when it's ON
    gameCtrl->sm_on = ST_MENU;
    gameCtrl->old_sm_on = gameCtrl->sm_on;

    //init the state machine for the screen update
    gameCtrl->sm_screen = ST_SCREEN_WAIT;
    gameCtrl->old_sm_screen = gameCtrl->sm_screen;

    //init the state machine for the racket update
    gameCtrl->sm_racket = ST_RACKET_WAIT;
    gameCtrl->old_sm_racket = gameCtrl->sm_racket;

    //init the state machine for the ball update
    gameCtrl->sm_ball = ST_BALL_WAIT;
    gameCtrl->old_sm_ball = gameCtrl->old_sm_ball;
}
void state_machine_mode(Game_controller* gameCtrl, Event ev)
{
    //Store the old state
    gameCtrl->old_sm_mode = gameCtrl->sm_mode;
    
    switch (gameCtrl->sm_mode)
    {
        case ST_OFF:
            if(ev == EV_BTN_IS_PRESSED)
            {
                gameCtrl->sm_mode = ST_ON;
            }
            break;
        case ST_ON:
            if(ev == EV_BTN_IS_PRESSED)
            {
                gameCtrl->sm_mode = ST_OFF;
            }
            break;
        default:
            break;
    }
}
void state_machine_on(Game_controller* gameCtrl, Event ev)
{
    
}
void state_machine_screen(Game_controller* gameCtrl, Event ev)
{
    
}
void state_machine_racket(Game_controller* gameCtrl, Event ev)
{
    
}
void state_machine_ball(Game_controller* gameCtrl, Event ev)
{
    
}
