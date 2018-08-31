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

typedef struct Accelerometer        // accelerometer structure
{
    int16_t xAcc;               // value of acceleration in X
    int16_t yAcc;               // value of acceleration in Y
    int16_t zAcc;               // value of acceleration in Z
} Accelerometer;

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

//-----------------------------------------------------------------
// SPI bit
//-----------------------------------------------------------------
#define READbit                 1
#define MnSbit                  0

#endif	/* ACC_CONTROLLER_H */

