/* 
 * File:   Ball_manager.h
 * Author: Gilles Mottiez
 *
 * Created on 31. août 2018, 15:24
 */

#ifndef BALL_MANAGER_H
#define	BALL_MANAGER_H
#include <xc.h>             // XC8 General Include File
#include <stdint.h>         // usage of standard types
#include <stdbool.h>        // usage of boolean types
#include "xf.h"
#include "GlobalVariables.h"

typedef struct Ball
{
    uint16_t xPos, yPos;
    
} Ball;

//variables for the trajectory
uint8_t step;
uint8_t alpha; //from 0 to 179, means from 0 to 359 degree, step is 2 degree


void ball_init(Ball* ball);
void compute_ballPosition();
void set_xPos(uint16_t new_xPos);
void set_yPos(uint16_t new_yPos);

#endif

