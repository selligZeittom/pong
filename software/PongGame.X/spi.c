/******************************************************************************/
/* GOAL		: Set SPI            */
/******************************************************************************/
#include <pic18.h>
#include <stdint.h>
#include <stdio.h>
#include "spi.h"

/******************************************************************************/
/* FUNCTION : tscInit                                                         */
/* INPUT		: -                                                   */
/* OUTPUT		: -                                                   */
/******************************************************************************/
/* COMMENTS     : Initialise SPI                             */
/******************************************************************************/
void SPIInit(void){
  TRISC4 = 1;               // MISO for SPI is input
  TRISC3 = 0;               // clock for SPI is output
  TRISC5 = 0;               // MOSI for SPI is output
  CKE = 0;                  // mode of data transfer (mode 3)
  SSPCON1 = 0b00110010;     // SSPEN & CKP & speed select (fosc/64) 1MHz
  SSPEN = 1;                // enable SPI
}