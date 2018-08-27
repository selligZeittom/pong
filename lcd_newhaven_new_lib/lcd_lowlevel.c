/******************************************************************************/
/* FILENAME	: lcd_lowlevel.c                                                  */
/*----------------------------------------------------------------------------*/
/* GOAL		: Offers the low level function for the LCD control                 */
/******************************************************************************/
#include <pic18.h>
#include <stdint.h>
#include "lcd_lowlevel.h"

/******************************************************************************/
/* FUNCTION : LCD_Lowlevel_Init                                               */
/* INPUT		: -                                                               */
/* OUTPUT		:                                                                 */
/******************************************************************************/
/* COMMENTS : Initialize ports, reset and so ...                              */
/******************************************************************************/
void LCD_Lowlevel_Init(void)
{
  uint32_t i;
  DIR_LCD_nCS = 0;
  DIR_LCD_nRD = 0;
  DIR_LCD_nWR = 0;
  DIR_LCD_DnC = 0;
  DIR_LCD_nRES = 0;
  DIR_LCD_DATA_BUS = 0x00;

	LCD_nRES = 0;			// Reset the LCD-MODULE
	LCD_nCS = 0;			// chip select
	LCD_nRD = 1;			// Disable the Read mode
	LCD_nWR = 1;			// Disable the Write mode
	LCD_DnC = 1;			// Enable the Data mode

  for(i=0;i<12;i++) // delay 120 ms
  {
    __delay_ms(10);
  }  
	LCD_nRES = 1;			// Disable the Reset
  for(i=0;i<12;i++) // delay 120 ms
  {
    __delay_ms(10);
  }  
}

/******************************************************************************/
/* FUNCTION : LCD_PowerOff                                                    */
/* INPUT		: -                                                               */
/* OUTPUT		:                                                                 */
/******************************************************************************/
/* COMMENTS : Turns off LCD and all I/O pins ...                              */
/*            Do not forget to power off the LCD !!!                          */
/******************************************************************************/
void LCD_PowerOff(void)
{
  LCD_BACKLIGHT = 1;  // turn off backlight
}
/******************************************************************************/
/* FUNCTION : lcd_read                                                        */
/* INPUT		: posX the x position of pixel to read                            */
/*            posY the y position of pixel to read                            */
/* OUTPUT		: the color read                                                  */
/******************************************************************************/
/* COMMENTS : Reads a pixel from the selected lcd memory position             */
/******************************************************************************/
uint16_t LCD_Read(uint16_t posX, uint16_t posY)
{
	uint16_t temp16;
  uint16_t color;

  //----------------------------------------------------------------------------
  LCD_Cmd(COLUMN_ADDRESS);      // column address set
  LCD_Data(posX>>8);            // start address
  LCD_Data(posX);               //
  LCD_Data((LCD_WIDTH-1) >> 8); // end address
  LCD_Data((LCD_WIDTH-1));      //
  //----------------------------------------------------------------------------
  LCD_Cmd(PAGE_ADDRESS);        // page address set
  LCD_Data(posY>>8);            // start address
  LCD_Data(posY);               // 
  LCD_Data((LCD_HEIGHT-1) >> 8); // end address
  LCD_Data((LCD_HEIGHT-1));     //
  //----------------------------------------------------------------------------
  LCD_Cmd(0x2E);                // read data command
	DIR_LCD_DATA_BUS = 0xFF;			// LCD_DATA_BUS as input
	LCD_nRD = 0;                  // Dummy read
  NOP();
	LCD_nRD = 1;                  //
	DIR_LCD_DATA_BUS = 0xFF;			// LCD_DATA_BUS as input
	LCD_nRD = 0;                  // Enable the Read mode
  NOP();
	temp16 = LCD_DATA_BUS;        // get red byte
	LCD_nRD = 1;                  // Disable the Read mode
  temp16 = temp16 >> 3;
  color = temp16 << 11;
	LCD_nRD = 0;                  // Enable the Read mode
  NOP();
	temp16 = LCD_DATA_BUS;        // get green byte
	LCD_nRD = 1;                  // Disable the Read mode
  temp16 = temp16 >> 2;
  color |= temp16 << 5;
	LCD_nRD = 0;                  // Enable the Read mode
  NOP();
	temp16 |= LCD_DATA_BUS;       // get blue
	LCD_nRD = 1;                  // Disable the Read mode
  temp16 = temp16 >> 3;
  color |= temp16;
	DIR_LCD_DATA_BUS = 0x00;			// LCD_DATA_BUS as output (default)
	return color;
}
