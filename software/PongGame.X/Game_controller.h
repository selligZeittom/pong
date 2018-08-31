/* 
 * File:   Game_controller.h
 * Author: Gilles Mottiez
 *
 * Created on 30. août 2018, 21:38
 */

#ifndef GAME_CONTROLLER_H
#define	GAME_CONTROLLER_H

#include "xf.h"
#include "GlobalVariables.h"

typedef enum SM_MODE
{
    ST_ON,
    ST_OFF,
} SM_MODE;

typedef enum SM_ON
{
    ST_MENU,
    ST_GAME,
} SM_ON;

typedef enum SM_MENU
{
    ST_MENU_WAIT,
    ST_MENU_CTRL_SW,
    ST_MENU_LEVEL_SW,
} SM_MENU;

typedef enum SM_SCREEN
{
    ST_SCREEN_WAIT,
    ST_SCREEN_UPDATE,
} SM_SCREEN;

typedef enum SM_RACKET
{
    ST_RACKET_WAIT,
    ST_RACKET_USER,
    ST_RACKET_AI,
} SM_RACKET;

typedef enum SM_BALL
{
    ST_BALL_WAIT,
    ST_BALL_CHECK_Y,
    ST_BALL_CHECK_X,
    ST_BALL_MOVE,
    ST_BALL_GAME_OVER,
} SM_BALL;


/* 
 * this is the struct to use to control the game
 *
 * first state machine : ON or OFF
 * second state machine : MENU or GAME in ON state
 * third state machine : game sequences
 */
typedef struct Game_controller
{
    SM_MODE sm_mode, old_sm_mode;
    SM_ON sm_on, old_sm_on;
    SM_MENU sm_menu, old_sm_menu;
    SM_SCREEN sm_screen, old_sm_screen;
    SM_RACKET sm_racket, old_sm_racket;
    SM_BALL sm_ball, old_sm_ball;
}Game_controller;

/*initialization
 * 
 * put the states machines in their starting state
 */
void init_Game_controller(Game_controller* gameCtrl);


void state_machine_mode(Game_controller* gameCtrl, Event ev);
void state_machine_on(Game_controller* gameCtrl, Event ev);
void state_machine_menu(Game_controller* gameCtrl, Event ev);
void state_machine_screen(Game_controller* gameCtrl, Event ev);
void state_machine_racket(Game_controller* gameCtrl, Event ev);
void state_machine_ball(Game_controller* gameCtrl, Event ev);
#endif	/* GAME_CONTROLLER_H */

