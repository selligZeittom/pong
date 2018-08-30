#include "Game_controller.h"

void init_Game_controller(Game_controller* gameCtrl)
{
    gameCtrl->sm_mode = ST_ON;
    gameCtrl->old_sm_mode = gameCtrl->sm_mode;
    
    gameCtrl->sm_on = ST_ON_MENU;
    gameCtrl->old_sm_on = gameCtrl->sm_on;
    
    gameCtrl->sm_game = ST_ON_GAME_WAIT;
    gameCtrl->old_sm_game = gameCtrl->sm_game;
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
void state_machine_game(Game_controller* gameCtrl, Event ev)
{
    
}