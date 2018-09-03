/* 
 * File:   Acc_controller.h
 * Author: yan.michello
 *
 * Created on 31. août 2018, 09:23
 */

#ifndef ACC_CONTROLLER_H
#define	ACC_CONTROLLER_H

#include <xc.h>             // XC8 General Include File
#include <stdint.h>         // usage of standard types
#include <stdbool.h>        // usage of boolean types
#include "xf.h"
#include "GlobalVariables.h"

typedef struct Accelerometer        // accelerometer structure
{
    int16_t xAcc;               // value of acceleration in X
    int16_t yAcc;               // value of acceleration in Y
    int16_t zAcc;               // value of acceleration in Z
} Accelerometer;

//states for the accelerometer's machine
typedef enum SM_ACC
{
    ST_ACC_WAIT,
    ST_ACC_UPDATE,
} SM_ACC;

//structure for the double switch pattern
typedef struct Acc_controller
{
    SM_ACC sm_acc, old_sm_acc;
}Acc_controller;

//-----------------------------------------------------------------
// Accelerometer Registers
//-----------------------------------------------------------------
#define CTRL_REG1                 0b00100000
#define OUT_X_L                   0b00101000
#define OUT_X_H                   0b00101001
#define OUT_Y_L                   0b00101010
#define OUT_Y_H                   0b00101011
#define OUT_Z_L                   0b00101100
#define OUT_Z_H                   0b00101101

//-----------------------------------------------------------------
// Accelerometer Initiation
//-----------------------------------------------------------------
#define nCS_ACC                 LATC7
#define DIR_nCS_ACC             TRISC7
#define DIR_INT_ACC             TRISB2

//-----------------------------------------------------------------
// SPI bit
//-----------------------------------------------------------------
#define READbit                 1
#define MnSbit                  0

/******************************************************************************/
/* FUNCTION : accInit                                                         */
/* INPUT		: -                                                   */
/* OUTPUT		: -                                                   */
/******************************************************************************/
/* COMMENTS     : Initialise Accelerometer                            */
/******************************************************************************/
void AccInit(Acc_controller* accCtrl);

/*
 * process events for the accelerometer
 */
void state_machine_accelerometer(Acc_controller* accCtrl, Event ev);

/*
 * read new values for accelerometer
 */
 void acc_update();

#endif	/* ACC_CONTROLLER_H */

