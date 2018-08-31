/* 
 * File:   spi.h
 * Author: yan.michello
 *
 * Created on 31. août 2018, 10:25
 */

#ifndef SPI_H
#define	SPI_H

#include <xc.h>             // XC8 General Include File
#include <stdint.h>         // usage of standard types
#include <stdbool.h>        // usage of boolean types

//------------------------------------------------------
// SPI pins
//------------------------------------------------------
#define SPIclk          TRISC3
#define MISO            TRISC4
#define MOSI            TRISC5

//---------------------------------------------------------
// SPI mode
//----------------------------------------------------
#define SPIclkSelectBit CKE


/******************************************************************************/
/* FUNCTION : tscInit                                                         */
/* INPUT		: -                                                   */
/* OUTPUT		: -                                                   */
/******************************************************************************/
/* COMMENTS     : Initialise SPI                             */
/******************************************************************************/
void SPIInit(void);

#endif	/* SPI_H */

