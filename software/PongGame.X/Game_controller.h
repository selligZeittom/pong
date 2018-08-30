/* 
 * File:   Game_controller.h
 * Author: Gilles Mottiez
 *
 * Created on 30. août 2018, 21:38
 */

#ifndef GAME_CONTROLLER_H
#define	GAME_CONTROLLER_H

#include "xf.h"

typedef enum SM_MODE
{
    ST_ON,
    ST_OFF,
} SM_MODE;

typedef enum SM_ON
{
    ST_ON_MENU,
    ST_ON_GAME,
} SM_ON;

typedef enum SM_GAME
{
    ST_ON_GAME_WAIT,
    ST_ON_GAME_UPDATE_SCREEN,
    ST_ON_GAME_TIMEOUT_BALL,
    ST_ON_GAME_TIMEOUT_MOVE_X,
    ST_ON_GAME_TIMEOUT_AI,
    ST_ON_GAME_CHECK_RACKET,
    ST_ON_GAME_MOVE_BALL,
    ST_ON_GAME_OVER,
} SM_GAME;

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
    SM_GAME sm_game, old_sm_game;
}Game_controller;


void init_Game_controller(Game_controller* gameCtrl);
void state_machine_mode(Game_controller* gameCtrl, Event ev);
void state_machine_on(Game_controller* gameCtrl, Event ev);
void state_machine_game(Game_controller* gameCtrl, Event ev);
#endif	/* GAME_CONTROLLER_H */

