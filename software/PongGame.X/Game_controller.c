#include "Game_controller.h"

/*initialization
 *
 * put the states machines in their starting state
 */
void init_Game_controller(Game_controller* gameCtrl)
{
    //init state machine of the mode ON or OFF
    gameCtrl->sm_mode = ST_ON;
    gameCtrl->old_sm_mode = gameCtrl->sm_mode;
    
    //init the state machine when it's ON
    gameCtrl->sm_on = ST_MENU;
    gameCtrl->old_sm_on = gameCtrl->sm_on;

    //init the state machine menu
    gameCtrl->sm_menu = ST_MENU_WAIT;
    gameCtrl->old_sm_menu = gameCtrl->sm_menu;

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

/*
 * process event of the mode ON - OFF state machine
 */
void state_machine_mode(Game_controller* gameCtrl, Event ev)
{
    //Store the old state
    gameCtrl->old_sm_mode = gameCtrl->sm_mode;
    
    //on transition
    switch (gameCtrl->sm_mode)
    {
    case ST_OFF:
        //from OFF to ON
        if(ev == EV_ON)
        {
            gameCtrl->sm_mode = ST_ON;
        }
        break;
    case ST_ON:
        //from OFN to OFF
        if(ev == EV_OFF)
        {
            gameCtrl->sm_mode = ST_OFF;
        }
        break;
    default:
        break;
    }
    
    //on entry actions
    if(gameCtrl->sm_mode != gameCtrl->old_sm_on)
    {
        switch (gameCtrl->sm_mode)
        {
        case ST_OFF:
            //put the pic in sleep mode, sleep well !
            sleep_mode();
            break;
        case ST_ON:
            //wake up the pic, he slept enough...
            wake_up();
            break;
        default:
            break;
        }
    }
}

/*
 * process event of the ON state machine
 */
void state_machine_on(Game_controller* gameCtrl, Event ev)
{
    //Store the old state
    gameCtrl->old_sm_on = gameCtrl->sm_on;

    switch (gameCtrl->sm_on) {
    case ST_MENU:
        //from menu to game
        if(ev == EV_GAME)
        {
            gameCtrl->sm_on = ST_GAME;
        }
        break;
    case ST_GAME:
        //from game to menu
        if(ev == EV_MENU)
        {
            gameCtrl->sm_on = ST_MENU;
        }
        break;
    default:
        break;
    }

    //on entry actions
    if(gameCtrl->sm_on != gameCtrl->old_sm_on)
    {
        switch (gameCtrl->sm_on) {
        case ST_MENU:
            //display the menu
            display_menu();
            break;
        case ST_GAME:
            //nothing to do for the moment
            break;
        default:
            break;
        }
    }
}

/*
 * process event of the MENU state machine
 */
void state_machine_menu(Game_controller* gameCtrl, Event ev)
{
    //Store the old state
    gameCtrl->old_sm_menu = gameCtrl->sm_menu;

    switch (gameCtrl->sm_menu) {
    case ST_MENU_WAIT:
        //from menu_wait to level_switch
        if(ev == EV_LEVEL_SW)
        {
            gameCtrl->sm_menu = ST_MENU_LEVEL_SW;
        }

        //from menu_wait to ctrl_switch
        if(ev == EV_CTRL_SW)
        {
            gameCtrl->sm_menu = ST_MENU_CTRL_SW;
        }
        break;

    case ST_MENU_CTRL_SW:
        //from ctrl_switch to menu_wait
        if(ev == EV_GOTO_MENU_WAIT)
        {
            gameCtrl->sm_menu = ST_MENU_WAIT;
        }
        break;
    case ST_MENU_LEVEL_SW:
        //from level_switch to menu_wait
        if(ev == EV_GOTO_MENU_WAIT)
        {
            gameCtrl->sm_menu = ST_MENU_WAIT;
        }
        break;
    default:
        break;
    }

    //on entry actions
    if(gameCtrl->sm_menu != gameCtrl->old_sm_menu)
    {
        switch (gameCtrl->sm_menu) {
        case ST_MENU_WAIT:
            //display menu
            display_menu();
            break;

        case ST_MENU_CTRL_SW:
            //switch the control : accelerometer or touch screen
            switch_control();
            break;
        case ST_MENU_LEVEL_SW:
            //switch the level
            switch_level();
            break;
        default:
            break;
        }
    }
}

/*
 * process event of the SCREEN state machine
 */
void state_machine_screen(Game_controller* gameCtrl, Event ev)
{
    //Store the old state
    gameCtrl->old_sm_screen = gameCtrl->sm_screen;

    switch (gameCtrl->sm_screen) {
    case ST_SCREEN_WAIT:
        //update periodically the screen
        if(ev == EV_TIMEOUT_SCREEN)
        {
            gameCtrl->sm_screen == ST_SCREEN_UPDATE;
        }
        break;
    case ST_SCREEN_UPDATE:
        //go back to wait mode
        if(ev == EV_GOTO_SCREEN_WAIT)
        {
            gameCtrl->sm_screen = ST_SCREEN_WAIT;
        }
        break;
    default:
        break;
    }

    //on entry actions
    if(gameCtrl->sm_screen != gameCtrl->old_sm_screen)
    {
        switch (gameCtrl->sm_screen) {
        case ST_SCREEN_WAIT:
            //nothing to do there for the moment
            break;
        case ST_SCREEN_UPDATE:
            //refresh screen with the new logic values
            update_screen();
            break;
        default:
            break;
        }
    }
}

/*
 * process event of the RACKET state machine
 */
void state_machine_racket(Game_controller* gameCtrl, Event ev)
{
    //Store the old state
    gameCtrl->old_sm_racket = gameCtrl->sm_racket;

    switch (gameCtrl->sm_racket) {
    case ST_RACKET_WAIT:
        //from wait to AI's racket
        if(ev == EV_TIMEOUT_AI)
        {
            gameCtrl->sm_racket = ST_RACKET_AI;
        }

        //from wait to user's racket
        if(ev == EV_TIMEOUT_USER)
        {
            gameCtrl->sm_racket = ST_RACKET_USER;
        }
        break;
    case ST_RACKET_USER:
        //from racket to wait
        if(ev == EV_GOTO_RACKET_WAIT)
        {
            gameCtrl->sm_racket = ST_RACKET_WAIT;
        }
        break;
    case ST_RACKET_AI:
        //from racket to wait
        if(ev == EV_GOTO_RACKET_WAIT)
        {
            gameCtrl->sm_racket = ST_RACKET_WAIT;
        }
        break;
    default:
        break;
    }

    //on entry actions
    if(gameCtrl->sm_racket != gameCtrl->old_sm_racket)
    {
        switch (gameCtrl->sm_racket) {
        case ST_RACKET_WAIT:
            //nothing to do there for the moment
            break;
        case ST_RACKET_USER:
            //move the user's racket
            move_user_racket();
            break;
        case ST_RACKET_AI:
            //move AI's racket
            move_AI_racket();
            break;
        default:
            break;
        }
    }
}

/*
 * process event of the BALL state machine
 */
void state_machine_ball(Game_controller* gameCtrl, Event ev)
{
    //Store the old state
    gameCtrl->old_sm_ball = gameCtrl->sm_ball;

    switch (gameCtrl->sm_ball) {
    case ST_BALL_WAIT:
        //from ball_wait to ball_check_y
        if(ev == EV_TIMEOUT_BALL)
        {
            gameCtrl->sm_ball = ST_BALL_CHECK_Y;
        }
        break;

    case ST_BALL_CHECK_Y:
        /* means that the ball arrived out of limits
         * and should be on a racket
         */
        if(ev == EV_CHECK_RACKET)
        {
            gameCtrl->sm_ball = ST_BALL_CHECK_X;
        }

        /* means that the ball isn't arrived to
         * the limits of screen
         */
        if(ev == EV_MOVE)
        {
            gameCtrl->sm_ball = ST_BALL_MOVE;
        }
        break;

    case ST_BALL_CHECK_X:
        //if the ball is not on a racket
        if(ev == EV_GAME_OVER)
        {
            gameCtrl->sm_ball = ST_BALL_GAME_OVER;
        }

        //if the ball is on the racket
        if(ev == EV_MOVE)
        {
            gameCtrl->sm_ball = ST_BALL_MOVE;
        }
        break;

    case ST_BALL_MOVE:
        //after the ball move, go to wait mode again
        if(ev == EV_GOTO_BALL_WAIT)
        {
            gameCtrl->sm_ball = ST_BALL_WAIT;
        }
        break;

    case ST_BALL_GAME_OVER:
        //if the game is over, go back to menu
        if(ev == EV_MENU)
        {
            gameCtrl->sm_ball = ST_BALL_WAIT;
            //gameCtrl->sm_on = ST_MENU;
        }
        break;

    default:
        break;
    }

    //on entry actions
    if(gameCtrl->sm_ball != gameCtrl->old_sm_ball)
    {
        switch (gameCtrl->sm_ball) {
        case ST_BALL_WAIT:
            //nothing to do there for the moment
            break;

        case ST_BALL_CHECK_Y:
            //compare ball's y pos to the limits
//            check_Y_ball();
            break;

        case ST_BALL_CHECK_X:
            //compare rackets's X pos to ball's X pos
//            check_X_ball();
            break;

        case ST_BALL_MOVE:
            //compute the trajectory
//            compute_movement_ball();
            break;

        case ST_BALL_GAME_OVER:
            //reset the game. byyyyyyyye
            reset_game();
            break;

        default:
            break;
        }
    }
}

/*
 * display the menu
 */
void display_menu()
{
    
}

/*
 * switch the level setting
 */
void switch_level()
{
    
}

/*
 * switch control panel : tsc or accelerometer
 */
void switch_control()
{
    
}

/*
 * update the screen with the new logic values
 */
void update_screen()
{
    
}

/*
 * move AI's racket
 */
void move_AI_racket()
{
    
}

/*
 * move_user_racket
 */
void move_user_racket()
{
    
}

/*
 * check the Y position of the ball
 */
void check_Y_ball(uint16_t y_ball)
{
    
}

/*
 * check the X position of the ball, it should be on a racket...
 */
void check_X_ball(uint16_t x_ball, uint16_t x_racket)
{
    
}

/*
 * compute the new coordinates
 * it depends on the trajectory of the ball and the wall on the side
 */
void compute_movement_ball(uint16_t x_ball)
{
    
}

/*
 * put the pic in sleep mode and all the peripherals
 */
void sleep_mode()
{
    
}

/*
 * wake up the pic !
 */
void wake_up()
{

}

/*
 * reset the data of the game
 */
void reset_game()
{

}
