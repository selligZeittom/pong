/******************************************************************************/
/* FILENAME	: stmpe811.c                                                      */
/*----------------------------------------------------------------------------*/
/* GOAL		: Offers the high level function for the touch control              */
/******************************************************************************/
#include <pic18.h>
#include <stdint.h>
#include <stdio.h>
#include "tsc.h"
#include "lcd_highlevel.h"
#include "lcd_lowlevel.h"
#include "application.h"
tsc_t tsc;                  // global structure for tsc status
volatile uint8_t dummy;

/******************************************************************************/
/* FUNCTION : tscInit                                                         */
/* INPUT		: -                                                               */
/* OUTPUT		: -                                                               */
/******************************************************************************/
/* COMMENTS     : Initialise the TSC controller                               */
/******************************************************************************/
void TscInit(void)
{
  uint8_t temp8[2];

  TRISC4 = 1;               // MISO for SPI is input
  nCS_TSC = 1;              // tsc controller chip select
  DIR_nCS_TSC = 0;          // set CS as output
  DIR_INT_TSC = 1;
  TRISC3 = 0;               // clock for SPI is output
  TRISC5 = 0;               // MOSI for SPI is output
  CKE1 = 0;                  // mode of data transfer
  //CKP1 = 1;                  // used with tsc controller
  SSPCON1 = 0b00100010;     // SSPEN & CKP & speed select (fosc/64) 1MHz
  SSPEN1 = 1;                // enable SPI
RESTART_SPI:
  //----------------------------------------------------------------------------
  // software initialisation
  //----------------------------------------------------------------------------
  TscWrite (STMPE811_SPI_CFG, 1, STMPE811_SPI_MODE_0 | STMPE811_AUTO_INCR);
  TscRead(STMPE811_CHIP_ID,2,temp8);  // read chip ID

  if(temp8[0] != 0x08)      // if not good -> big problem
  {
    CKE1 = 1;
    goto RESTART_SPI;
//    for(;;){}
  }
  if(temp8[1] != 0x11)      // if not good -> big problem
  {
    for(;;){}
  }
  // Enable TSC,and set TSC operating mode is X, Y only.
  TscWrite (STMPE811_TSC_CTRL, 1, STMPE811_TSC_CTRL_EN| STMPE811_TSC_CTRL_XY);
  // Enable TSC and ADC
  TscWrite (STMPE811_SYS_CTRL2, 1, STMPE811_CTRL2_TS_OFF |
                                   STMPE811_CTRL2_GPIO_OFF);
  // Enable Touch detect, FIFO interrupt.
  TscWrite (STMPE811_INT_EN, 1, STMPE811_INT_EN_TOUCHDET |
                                STMPE811_INT_EN_TH |
                                STMPE811_INT_EN_FIFOOFLOW);
  // To select sample time, bit number and ADC reference.
  TscWrite (STMPE811_ADC_CTRL1, 1, ((0x6 & STMPE811_ADC_CTRL1_SAMPLE_M)
                                   << STMPE811_ADC_CTRL1_SAMPLE_S )|
                                   STMPE811_ADC_CTRL1_MOD);
  // To select the ADC clock speed is 3.25 MHz.
  TscWrite (STMPE811_ADC_CTRL2, 1, STMPE811_ADC_CTRL2_CLK1);
  // To set Average, Touch Detect Delay and Setting Time setting.
  TscWrite (STMPE811_TSC_CFG, 1,
            STMPE811_TSC_CFG_AVE_8 |
           ((0x7 & STMPE811_TSC_CFG_DELAY_M) << STMPE811_TSC_CFG_DELAY_S)|
           ((0x1 & STMPE811_TSC_CFG_SETTING_M)
           << STMPE811_TSC_CFG_SETTING_S));
  // Set FIFO threshold level.
  TscWrite (STMPE811_FIFO_TH, 1, 0x01);
  // Reset FIFO.
  TscWrite (STMPE811_FIFO_STA, 1, STMPE811_FIFO_STA_RESET);
  // Write 0x00 to put the FIFO back into operation mode.
  TscWrite (STMPE811_FIFO_STA, 1, 0x00);
  // Set the data format for Z value.
  TscWrite (STMPE811_TSC_FRACTION_Z, 1,
                 (0x07 * STMPE811_FRACTION_Z_M) << STMPE811_FRACTION_Z_S);
  // Set the driving capability of the device for touchscreen controller pins.
  TscWrite (STMPE811_TSC_I_DRIVE, 1, STMPE811_TSC_I_DRIVE_50);
  // Set tracking index, set touchscreen controller op. mode and enable TSC

  TscWrite (STMPE811_TSC_CTRL, 1,STMPE811_TSC_CTRL_EN |STMPE811_TSC_CTRL_XY);
  // Clear all the interrupt status.
  TscWrite (STMPE811_INT_STA, 1, 0xFF);
  // Set interrupt mode and enable interrupt.
  TscWrite (STMPE811_INT_CTRL, 1, STMPE811_INT_TYPE|
                   STMPE811_INT_GLOBAL);
  INTE_TSC = 1;
  INTEDG_TSC = 0; // interrupts on falling edge
}

/******************************************************************************/
/* FUNCTION : tscPowerOff                                                     */
/* INPUT		: -                                                               */
/* OUTPUT		: -                                                               */
/******************************************************************************/
/* COMMENTS : Turn off the TSC controller                                     */
/*            Do not forget to power off the TSC chip                         */
/*            Control the interrupt too, if used                              */
/******************************************************************************/
void tscPowerOff(void)
{
  SSPEN1 = 0;
  LATC3 = 0;
  LATC5 = 0;
  TRISC4 = 0;
  LATC4 = 0;
  nCS_TSC = 0;
}

/******************************************************************************/
/* FUNCTION     : TscWrite                                                    */
/* INPUT        : ucReg is internal register address                          */
/*                ucNum is the number of bytes to write (max 4)               */
/*                ulVal is th data to write (max 32 bits)                     */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Write a data on the touchscreen controller                  */
/******************************************************************************/
void TscWrite(uint8_t ucReg, uint8_t ucNum, uint32_t ulVal)
{
  //SSPCON1 = 0b00110010;     // SSPEN & CKP & speed select (fosc/64) 1MHz

  nCS_TSC = 0;                  // chip select

  SSPBUF = ucReg & 0x7F;        // register address and WRITE operation
  dummy = SSPBUF;
  while(SSPIF==0){};            // wait end of transfer
  SSPIF = 0;
  (void)SSPBUF;
  while(ucNum--)                // for each byte
  {
    SSPBUF = (ulVal & 0xFF);    // send data
    dummy = SSPBUF;
    while(SSPIF==0){};          // wait termination
    SSPIF = 0;
    ulVal >>= 8;                // get next
   (void)SSPBUF;
  }
  nCS_TSC = 1;                  // unselect chip
}

/******************************************************************************/
/* FUNCTION     : TscRead                                                     */
/* INPUT        : ucReg is internal register address                          */
/*                ucNum is the number of bytes to read                        */
/*                pucBuffer is th pointer to data will be placed              */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Reads data from the touchscreen controller                  */
/******************************************************************************/
void TscRead(uint8_t ucReg, uint8_t ucNum, uint8_t* pucBuffer)
{
  //SSPCON1 = 0b00110010;     // SSPEN & CKP & speed select (fosc/64) 1MHz
  nCS_TSC = 0;                    // chip select
  SSPBUF = ucReg | 0x80;          // register address READ OPERATION
  dummy = SSPBUF;
  while(SSPIF==0){};
  SSPIF = 0;
  (void)SSPBUF;
  while(ucNum--)                  // for number of bytes selected
  {
    SSPBUF = 0xFF;                // dummy write to read last address
    dummy = SSPBUF;
    while(SSPIF==0){};
    SSPIF = 0;
    * pucBuffer = SSPBUF;         // place readed val on destination
    pucBuffer++;                  // increment destination
  }
  nCS_TSC = 1;                    // unselect chip
}

/******************************************************************************/
/* FUNCTION     : TscUpdate                                                   */
/* INPUT        : -                                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : After an interrupt, update the current tsc values           */
/******************************************************************************/
void TscUpdate(void)
{
  uint8_t intStat;
  uint8_t tempTbl[2];
  int32_t xPos, yPos;

  TscRead(STMPE811_INT_STA, 1, &intStat);// get interrupts status
  if(intStat & 0x01)                // touch detect event (press or release)
  {
    TscRead(STMPE811_TSC_CTRL, 1, tempTbl); // read press or released
    if(tempTbl[0] & 0x80)           // press info
    {
    }
    else                            // touch released
    {
      tsc.released = 1;             // set released
      tsc.pressed = 0;              // and not more pressed
    }
  }
  TscRead(STMPE811_FIFO_STA, 1, tempTbl);       // get fifo status
  if((intStat & 0x02)||(tempTbl[0] == 0x10))    // data in fifo ?
  {
    tsc.pressed = 1;                            // set pressed
    tsc.released = 0;                           // set released
#if defined(HORIZONTAL_NORMAL) || defined(HORIZONTAL_INVERTED)
    TscRead(0x4D, 2, tempTbl);                  // read Y position
    yPos = ((uint16_t)tempTbl[0] << 8) | tempTbl[1];
    TscRead(0x4F, 2, tempTbl);                  // read X position
    xPos = ((uint16_t)tempTbl[0] << 8) | tempTbl[1];
#endif
#if defined(VERTICAL_NORMAL) || defined(VERTICAL_INVERTED)
    TscRead(0x4D, 2, tempTbl);                  // read X position
    xPos = ((uint16_t)tempTbl[0] << 8) | tempTbl[1];
    TscRead(0x4F, 2, tempTbl);                  // read Y position
    yPos = ((uint16_t)tempTbl[0] << 8) | tempTbl[1];
#endif
    tsc.posY = ((yPos *(LCD_HEIGHT-1)) / (MAX_Y_MEAS-MIN_Y_MEAS))-(((LCD_HEIGHT-1) * MIN_Y_MEAS) / (MAX_Y_MEAS-MIN_Y_MEAS));
    tsc.posX = ((xPos *(LCD_WIDTH-1)) / (MAX_X_MEAS-MIN_X_MEAS))-(((LCD_WIDTH-1) * MIN_X_MEAS) / (MAX_X_MEAS-MIN_X_MEAS));
//    tsc.posX = xPos;
//    tsc.posY = yPos;
  }
  TscWrite(STMPE811_INT_STA, 1, 0xFF);   // clear all interrupts
}

/******************************************************************************/
/* FUNCTION     : TSC_ButtonExecute                                           */
/* INPUT        : button descriptor                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Update the button style and execute functions if any        */
/******************************************************************************/
void TSC_ButtonExecute(btn_t * button, uint8_t index)
{
      if(tsc.pressed == 1)
      {
        if(LCD_InButton(button,tsc.posX,tsc.posY) == 1)
        // EVENT BUTTON PRESSED
        {
          if(button->fpPress != NULL)
            button->fpPress(button->index);
        }
        if(button->pressed == 0)
        {
          if(LCD_InButton(button,tsc.posX,tsc.posY) == 1)
          {
            button->pressed = 1;
            LCD_ButtonUpdate(button);

          }
        }
        else
        {
          if(LCD_InButton(button,tsc.posX,tsc.posY) == 0)
          {
            if(button->fpOut != NULL)
                button->fpOut(button->index);
            button->pressed = 0;
            LCD_ButtonUpdate(button);
          }
        }
      }
      if(tsc.released == 1)
      {
        if(LCD_InButton(button,tsc.posX,tsc.posY) == 1)
        {
          tsc.released = 0;
          button->pressed = 0;
          LCD_ButtonUpdate(button);
          if(button->fpReleased != NULL)
            button->fpReleased(button->index);
          // EVENT BUTTON RELEASED

        }
      }
}

/******************************************************************************/
/* FUNCTION     : TSC_SliderExecute                                           */
/* INPUT        : slider descriptor                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Update the slider and execute functions if any              */
/******************************************************************************/
void TSC_SliderExecute(sld_t * slider)
{
  if(tsc.pressed == 1)
  {
    if(LCD_InSlider(slider,tsc.posX,tsc.posY) == 1)
    // EVENT BUTTON PRESSED
    {
      slider->value = ((tsc.posX - slider->posX) * slider->steps)/ slider->width;
      LCD_SliderUpdate(slider);
      if(slider->fpPress != NULL)
        slider->fpPress();
    }
  }
}
