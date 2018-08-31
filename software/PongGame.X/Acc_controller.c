#include <xc.h>             // XC8 General Include File
#include <stdint.h>         // usage of standard types
#include <stdbool.h>        // usage of boolean types
#include "Acc_controller.h"

/******************************************************************************/
/* FUNCTION : tscInit                                                         */
/* INPUT		: -                                                   */
/* OUTPUT		: -                                                   */
/******************************************************************************/
/* COMMENTS     : Initialise Accelerometer                            */
/******************************************************************************/
void AccInit(void){
    nCS_ACC = 1; // chip select
    DIR_nCS_ACC = 0 ;   // set nCS in output
    DIR_INT_ACC = 1 ;   // set accelerometer interrupt in input
}
