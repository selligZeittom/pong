/******************************************************************************/
/* FILENAME	: lcd_highlevel.c                                                 */
/*----------------------------------------------------------------------------*/
/* GOAL		: Offers the high level function for the LCD control                */
/******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "lcd_highlevel.h"
#include "lcd_lowlevel.h"


/******************************************************************************/
/* FUNCTION     : lcd_init                                                    */
/* INPUT		: -                                                               */
/* OUTPUT		: -                                                               */
/******************************************************************************/
/* COMMENTS     : INITALISE THE LCD-MODULE                                    */
/******************************************************************************/
void LCD_Init(void)
{
  uint8_t i; 

#if DISPLAY_NHD == NHD_24_SF
  LCD_Lowlevel_Init();          // ports initialisation
  LCD_Cmd(CMD_SOFTRESET);       // soft reset
  __delay_ms(10);               // delay 10 ms
  LCD_Cmd(CMD_DISPOFF);         //display off
  LCD_Cmd(CMD_SLPOUT);          //exit SLEEP mode
  LCD_Cmd(CMD_PWR_CTRL_A);      //power control A
  LCD_Data(0x39);
  LCD_Data(0x2C);
  LCD_Data(0x00);
  LCD_Data(0x34);
  LCD_Data(0x02);
  LCD_Cmd(CMD_PWR_CTRL_B);      //power control B
  LCD_Data(0x00);
  LCD_Data(0x81);
  LCD_Data(0x30);
  LCD_Cmd(CMD_PWR_CTRL_1);
  LCD_Data(0x26);               //power control 1
  LCD_Data(0x04);               //second param for ILI9340 (ignored by ILI9341)
  LCD_Cmd(CMD_PWR_CTRL_2);
  LCD_Data(0x11);               //power control 2
  LCD_Cmd(CMD_VCOM_CTRL_1);
  LCD_Data(0x35);
  LCD_Data(0x3E);               //VCOM control 1
  LCD_Cmd(CMD_MEM_ACC_CTRL);
  LCD_Data(0xA8);               //memory access control = BGR and flip col&add
  LCD_Cmd(CMD_FRAME_RATE_CTRL);
  LCD_Data(0x00);
  LCD_Data(0x18);               //frame rate control
  LCD_Cmd(CMD_DISP_FUNC_CTRL);
  LCD_Data(0x0A);
  LCD_Data(0xA2);               //display function control
  LCD_Cmd(CMD_VCOM_CTRL_2);
  LCD_Data(0xBE);               //VCOM control 2
  LCD_Cmd(CMD_PIXEL_FORMAT);
  LCD_Data(0x55);               //pixel format = 16 bit per pixel
  LCD_Cmd(GAMMA_CONTROL);       //3g damma control
  LCD_Data(0x02);               //off
  LCD_Cmd(GAMMA_CURVE);         //gamma curve 3
  LCD_Data(0x01);
  LCD_Cmd(COLUMN_ADDRESS);
  LCD_Data(0x00);               //column address set
  LCD_Data(0x00);               //start 0x0000
  LCD_Data(0x00);
  LCD_Data(0xEF);               //end 0x00EF
  LCD_Cmd(PAGE_ADDRESS);
  LCD_Data(0x00);               //page address set
  LCD_Data(0x00);               //start 0x0000
  LCD_Data(0x01);
  LCD_Data(0x3F);               //end 0x003F
  LCD_Cmd(CMD_MEM_ACC_CTRL);
  LCD_Data(CMD_MEM_ACC_DATA);
  LCD_Cmd(CMD_PIXEL_FORMAT);
  LCD_Data(0xC5);
  LCD_Cmd(CMD_DISPON);          //display ON
  LCD_BACKLIGHT_DIR = 0;
  LCD_BACKLIGHT = 0;
  for(i=0;i<10;i++)             // delay 100 ms
  {
    __delay_ms(10);
  }
  #elif DISPLAY_NHD == NHD_24_CF
  LCD_Lowlevel_Init();          // ports initialisation
  LCD_Cmd(CMD_SLPOUT);          //exit SLEEP mode
  for(i=0;i<10;i++)             // delay 100 ms
  {
    __delay_ms(10);
  }
  LCD_Cmd(CMD_MEM_ACC_CTRL);
  LCD_Data(CMD_MEM_ACC_DATA);               
  LCD_Cmd(CMD_PIXEL_FORMAT);
  LCD_Data(0x55);               //pixel format = 16 bit per pixel
  LCD_Cmd(CMD_PORCH_SETTING);
  LCD_Data(0x0C);
  LCD_Data(0x0C);
  LCD_Data(0x33);
  LCD_Data(0x33);
  LCD_Cmd(CMD_GATE_CTRL);
  LCD_Data(0x35);
  LCD_Cmd(CMD_VCOM_SETTING);
  LCD_Data(0x2B);
  LCD_Cmd(CMD_LCM_CTRL);
  LCD_Data(0x2C);
  LCD_Cmd(CMD_VDVVRH_EN);
  LCD_Data(0x01);
  LCD_Data(0xFF);
  LCD_Cmd(CMD_VRHS_SET);
  LCD_Data(0x11);
  LCD_Cmd(CMD_VDVS_SET);
  LCD_Data(0x20);
  LCD_Cmd(CMD_FRAME_RATE_CTRL);
  LCD_Data(0x09);
  LCD_Cmd(CMD_PWR_CTRL_1);
  LCD_Data(0xA4);               //power control 1
  LCD_Data(0xA1);               //second param for ILI9340 (ignored by ILI9341)
  LCD_Cmd(CMD_POSVOL_GAM_CTRL);
  LCD_Data(0xD0);
  LCD_Data(0x00);
  LCD_Data(0x05);
  LCD_Data(0x0E);
  LCD_Data(0x15);
  LCD_Data(0x0D);
  LCD_Data(0x37);
  LCD_Data(0x43);
  LCD_Data(0x47);
  LCD_Data(0x09);
  LCD_Data(0x15);
  LCD_Data(0x12);
  LCD_Data(0x16);
  LCD_Data(0x19);
  LCD_Cmd(CMD_NEGVOL_GAM_CTRL);
  LCD_Data(0xD0);
  LCD_Data(0x00);
  LCD_Data(0x05);
  LCD_Data(0x0D);
  LCD_Data(0x0C);
  LCD_Data(0x06);
  LCD_Data(0x2D);
  LCD_Data(0x44);
  LCD_Data(0x40);
  LCD_Data(0x0E);
  LCD_Data(0x1C);
  LCD_Data(0x18);
  LCD_Data(0x16);
  LCD_Data(0x19);
  LCD_Cmd(COLUMN_ADDRESS);
  LCD_Data(0x00);               //column address set
  LCD_Data(0x00);               //start 0x0000
  LCD_Data(0x00);
  LCD_Data(0xEF);               //end 0x00EF
  LCD_Cmd(PAGE_ADDRESS);
  LCD_Data(0x00);               //page address set
  LCD_Data(0x00);               //start 0x0000
  LCD_Data(0x01);
  LCD_Data(0x3F);     
  LCD_Cmd(GAMMA_CURVE);         //gamma curve 3
  LCD_Data(0x01);
  LCD_Cmd(CMD_DISPON);          //display ON
  LCD_BACKLIGHT_DIR = 0;
  LCD_BACKLIGHT = 0;
  #elif DISPLAY_NHD == 18
  LCD_Lowlevel_Init();          // ports initialisation
  LCD_Cmd(CMD_SLPOUT);
  for(i=0;i<10;i++)             // delay 100 ms
  {
    __delay_ms(10);
  }
  LCD_Cmd(GAMMA_CURVE);
  LCD_Data(4);
  LCD_Cmd(GAMMA_CONTROL);
  LCD_Data(0x00);
  LCD_Cmd(CMD_FRAME_RATE_CTRL);
  LCD_Data(0x0A);
  LCD_Data(0x14);
  LCD_Cmd(CMD_PWR_CTRL_1);
  LCD_Data(0x0A);
  LCD_Data(0x00);
  LCD_Cmd(CMD_PWR_CTRL_2);
  LCD_Data(0x02);
  LCD_Cmd(CMD_VCOM_CTRL_1);
  LCD_Data(0x2F);
  LCD_Data(0x3E);
  LCD_Cmd(CMD_VCOM_CTRL_2);
  LCD_Data(0x40);
  LCD_Cmd(COLUMN_ADDRESS);
  LCD_Data(0x00);
  LCD_Data(0x00);
  LCD_Data(0x00);
  LCD_Data(0x7F);
  LCD_Cmd(PAGE_ADDRESS);
  LCD_Data(0x00);
  LCD_Data(0x00);
  LCD_Data(0x00);
  LCD_Data(0x9F);
  LCD_Cmd(CMD_MEM_ACC_CTRL);
  LCD_Data(CMD_MEM_ACC_DATA);
  LCD_Cmd(CMD_PIXEL_FORMAT);
  LCD_Data(0xC5);
  LCD_Cmd(CMD_DISPON);
  LCD_Fill(BLACK);
  LCD_BACKLIGHT_DIR = 0;
  LCD_BACKLIGHT = 1;
#endif
}



/******************************************************************************/
/* FUNCTION     : LCD_Fill                                                    */
/* INPUT		: color The color to fill                                         */
/* OUTPUT		: -                                                               */
/******************************************************************************/
/* COMMENTS     : Fill the screen with selected color      					   			  */
/******************************************************************************/
void LCD_Fill(uint16_t color)
{
  uint16_t	column;		// column counter
  uint16_t	page;     // pages counter
  uint8_t colorH, colorL;
  colorH = color >> 8;
  colorL = color;
  //--------------------------------------------------------------------------
  LCD_Cmd(COLUMN_ADDRESS);            // column address set
  LCD_Data(0x00);                     // start address
  LCD_Data(0x00);                     // is 0
  LCD_Data((LCD_WIDTH-1) >> 8);       // end address
  LCD_Data((LCD_WIDTH-1));            //
  //--------------------------------------------------------------------------
  LCD_Cmd(PAGE_ADDRESS);              // page address set
  LCD_Data(0x00);                     // start address
  LCD_Data(0x00);                     // is 0
  LCD_Data((LCD_HEIGHT-1) >> 8);      // end address
  LCD_Data((LCD_HEIGHT-1));           //
  //--------------------------------------------------------------------------
  LCD_Cmd(MEMORY_WRITE);              // write data command
  for(column=0; column<LCD_HEIGHT; column++)
  {
    for(page=0; page<LCD_WIDTH; page++)
    {
      LCD_DATA_BUS = colorH;
      LCD_nWR = 0;                    // two access for 16 bits color
      LCD_nWR = 1;
      LCD_DATA_BUS = colorL;
      LCD_nWR = 0;
      LCD_nWR = 1;
    }
  }
}

/******************************************************************************/
/* FUNCTION     : LCD_SetPixel(PosX,PosY)                                     */
/* INPUT        : posX (0-319), posY (0-239)                                  */
/* OUTPUT       : -                                                           */
/* COMMENTS     : THIS FUNCTION WRITE A PIXEL ON THE DISPLAY                  */
/******************************************************************************/
void LCD_SetPixel(uint16_t posX,uint16_t posY, uint16_t color)
{
  //----------------------------------------------------------------------------
  LCD_Cmd(COLUMN_ADDRESS);            // column address set
  LCD_Data(posX>>8);                  // start address
  LCD_Data(posX);                     //
  LCD_Data((LCD_WIDTH-1) >> 8);       // end address
  LCD_Data((LCD_WIDTH-1));            //
  //----------------------------------------------------------------------------
  LCD_Cmd(PAGE_ADDRESS);              // page address set
  LCD_Data(posY>>8);                  // start address
  LCD_Data(posY);                     // is 0
  LCD_Data((LCD_HEIGHT-1) >> 8);      // end address
  LCD_Data((LCD_HEIGHT-1));           //
  //----------------------------------------------------------------------------
  LCD_Cmd(MEMORY_WRITE);              // write data command
  LCD_Data(color>>8);
  LCD_Data(color);
}

/******************************************************************************/
/* FUNCTION     : Create a slider                                             */
/* INPUT        : posX, posY, width, height: slider coordinates               */
/*                sldColor, bgColor, borderColor: the slider colors           */
/*                cursorWidth : width of the cursor (if 0, progress bar)      */
/*                steps : the number of steps in the slider                   */
/*                fpPress: pointer to function action to do                   */
/*                slider: the slider descriptor                               */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Creates a button without to draw it                         */
/*                If function are not used, place NULL on them                */
/******************************************************************************/
void LCD_SliderCreate(uint16_t posX, uint16_t posY,
                      uint16_t width, uint16_t height,
                      uint16_t sldColor, uint16_t bgColor, uint16_t borderColor,
                      uint16_t cursorWidth,
                      uint16_t steps,
                      void (* fpPress)(),
                      sld_t * slider)
{
  slider->posX = posX;
  slider->posY = posY;
  slider->height = height;
  slider->width = width;
  slider->sldColor = sldColor;
  slider->bgColor = bgColor;
  slider->borderColor = borderColor;
  slider->cursorWidth = cursorWidth;
  slider->fpPress = fpPress;
  slider->steps = steps;
  slider->value = 0;
  slider->oldValue = 0;
}

/******************************************************************************/
/* FUNCTION     : Redraw the slider (value has changed)                       */
/* INPUT        : button - The buttons descriptor                             */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Redraw button borders for pressed or released state         */
/******************************************************************************/
void LCD_SliderUpdate(sld_t * slider)
{
  uint16_t x1,x2,y1,y2;

  //----------------------------------------------------------------------------
  // clear last slider
  if(slider->cursorWidth != 0)
  {
    x1 = slider->posX + 1 + (((slider->width-slider->cursorWidth) * slider->oldValue)/ slider->steps);
    x2 = x1 + slider->cursorWidth-2;
    y1 = slider->posY + 2;
    y2 = slider->posY + slider->height - 2;
  }
  else
  {
    x1 = slider->posX + 1;
    x2 = slider->posX - 1 + ((slider->width * slider->oldValue)/ slider->steps);
    y1 = slider->posY + 2;
    y2 = slider->posY + slider->height - 2;
  }
  LCD_DrawRect(x1,
               y1,
               x2,
               y2,
               1, slider->bgColor);
  slider->oldValue = slider->value;
  //----------------------------------------------------------------------------
  // progress bar
  if(slider->cursorWidth == 0)
  {
    x1 = slider->posX + 1;
    x2 = slider->posX - 1 + ((slider->width * slider->value)/ slider->steps);
    y1 = slider->posY + 2;
    y2 = slider->posY + slider->height - 2;
  }
  //----------------------------------------------------------------------------
  // standard slider slider
  else
  {
    x1 = slider->posX + 1 + (((slider->width-slider->cursorWidth) * slider->value)/ slider->steps);
    x2 = x1 + slider->cursorWidth-2;
    y1 = slider->posY + 2;
    y2 = slider->posY + slider->height - 2;
  }
  LCD_DrawRect(x1, y1, x2, y2, 1, slider->sldColor);
}
/******************************************************************************/
/* FUNCTION     : Draw the slider                                             */
/* INPUT        : slider - The slider descriptor                              */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Draw the slider, needs to be called once                    */
/******************************************************************************/
void LCD_SliderDraw(sld_t * slider)
{
  uint16_t x1,x2,y1,y2;

  x1 = slider->posX;
  x2 = slider->posX + slider->width;
  y1 = slider->posY;
  y2 = slider->posY + slider->height;
  LCD_DrawRect(x1, y1, x2, y2, 1, slider->bgColor);
  LCD_DrawRect(x1, y1, x2, y2, 0, slider->borderColor);
  LCD_SliderUpdate(slider);
}

/******************************************************************************/
/* FUNCTION     : Create a button                                             */
/* INPUT        : posX, posY, width, height: button coordonates               */
/*                txtColor, bgColor: the button colors                        */
/*                textOrBmp The text could not start with 'BM' -> it's a bmp  */
/*                font - the selected font for the text                       */
/*                fpPress, fpReleased: pointer to function action to do       */
/*                button: the button descriptor                               */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Creates a button without to draw it                         */
/*                If function are not used, place NULL on them                */
/******************************************************************************/
void LCD_ButtonCreate(uint16_t posX, uint16_t posY,
                      uint16_t width, uint16_t height,
                      uint16_t txtColor, uint16_t bgColor,
                      const uint8_t * textOrBmp, FONT_INFO * font,
                      void (*fpPress)(uint8_t), void (* fpReleased)(uint8_t), void (* fpOut)(uint8_t),
                      btn_t * button,
                      uint8_t index)
{
  button->posX = posX;
  button->posY = posY;
  button->height = height;
  button->width = width;
  button->txtColor = txtColor;
  button->bgColor = bgColor;
  button->text = textOrBmp;
  button->font = font;
  button->fpPress = fpPress;
  button->fpReleased = fpReleased;
  button->fpOut = fpOut;
  button->index = index;
  button->pressed = 0;
}

/******************************************************************************/
/* FUNCTION     : Redraw the button status (pressed or released)              */
/* INPUT        : button - The buttons descriptor                             */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Redraw button borders for pressed or released state         */
/******************************************************************************/
void LCD_ButtonUpdate(btn_t * button)
{
  uint16_t x1,x2,y1,y2;
  uint16_t colorOut, colorIn;

  x1 = button->posX;
  x2 = button->posX + button->width;
  y1 = button->posY;
  y2 = button->posY + button->height;

  if(button->pressed == 0)  // button not pressed
  {
    colorOut = button->txtColor;
    colorIn = button->bgColor;
  }
  else
  {

    colorOut = LCD_Read(x1,y2+1);
    colorIn = button->txtColor;
  }
  if(button->bgColor != button->txtColor)
  {

    LCD_DrawRect(x1+3, y2+1, x2+3, y2+3, 1, colorOut); // bottom
    LCD_DrawRect(x2+1, y1+3, x2+3, y2+3, 1, colorOut); // right
    LCD_DrawRect(x1+1, y1+1, x2-1, y1+2, 1, colorIn);  // top
    LCD_DrawRect(x1+1, y1+1, x1+2, y2-1, 1, colorIn);  // left
  }
}

/******************************************************************************/
/* FUNCTION     : Draw the button                                             */
/* INPUT        : button - The buttons descriptor                             */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Draw the button, needs to be called once                    */
/******************************************************************************/
void LCD_ButtonDraw(btn_t * button)
{
  uint16_t x1,x2,y1,y2;
  uint16_t xText,yText;
  uint8_t i, tempChar;
  uint16_t width=0;
  BMP_STRUCT *bmpPtr;

  x1 = button->posX;
  x2 = button->posX + button->width;
  y1 = button->posY;
  y2 = button->posY + button->height;
  if(button->bgColor != button->txtColor)
  {
    LCD_DrawRect(x1, y1, x2, y2, 1, button->bgColor);
    LCD_DrawRect(x1, y1, x2, y2, 0, button->txtColor);
  }
  //----------------------------------------------------------------------------
  // check if is a bitmap button
  //----------------------------------------------------------------------------
  if((button->text[0] == 'B')&&(button->text[1] == 'M'))
  {
    bmpPtr = (BMP_STRUCT *)button->text;
    yText =  button->height / 2 + button->posY - (bmpPtr->biHeight / 2);
    xText =   button->width / 2 + button->posX - (bmpPtr->biWidth / 2);
    LCD_Bitmap((const uint8_t *)bmpPtr,xText,yText);
  }
  //----------------------------------------------------------------------------
  // is a text button
  //----------------------------------------------------------------------------
  else
  {
    yText =  button->height / 2 + button->posY - (button->font->height / 2);
    for(i=0;i<strlen(button->text);i++)
    {
      tempChar = (button->text[i]) - button->font->start_char;      // get char of table
      width += button->font->descriptor[tempChar].width+2;  // width of char
    }
    xText =   button->width / 2 + button->posX - (width / 2);
    LCD_DrawText(button->text, button->font,A_LEFT,
          xText, yText, button->txtColor,  button->bgColor);
  }
  LCD_ButtonUpdate(button);
}

/******************************************************************************/
/* FUNCTION     : Check if the coordinates are in the button                  */
/* INPUT        : button - The buttons descriptor                             */
/*                posX, posY                                                  */
/* OUTPUT       : true or false                                               */
/* COMMENTS     : Typically used for touchscreen test                         */
/******************************************************************************/
uint8_t LCD_InButton(btn_t * button, uint16_t posX, uint16_t posY)
{
  //----------------------------------------------------------------------------
  if(posX < button->posX) // x check
  {
    return 0;             // not in button
  }
  //----------------------------------------------------------------------------
  if(posX > (button->posX + button->width)) // x check
  {
    return 0;             // not in button
  }
  //----------------------------------------------------------------------------
  if(posY < button->posY) // x check
  {
    return 0;             // not in button
  }
  //----------------------------------------------------------------------------
  if(posY > (button->posY + button->height)) // x check
  {
    return 0;             // not in button
  }
  return 1;               // in button !
}

/******************************************************************************/
/* FUNCTION     : Check if the coordinates are in the slider                  */
/* INPUT        : slider - The slider descriptor                              */
/*                posX, posY                                                  */
/* OUTPUT       : true or false                                               */
/* COMMENTS     : Typically used for touchscreen test                         */
/******************************************************************************/
uint8_t LCD_InSlider(sld_t * slider, uint16_t posX, uint16_t posY)
{
  //----------------------------------------------------------------------------
  if(posX < slider->posX) // x check
  {
    return 0;             // not in button
  }
  //----------------------------------------------------------------------------
  if(posX > (slider->posX + slider->width)) // x check
  {
    return 0;             // not in button
  }
  //----------------------------------------------------------------------------
  if(posY < slider->posY) // x check
  {
    return 0;             // not in button
  }
  //----------------------------------------------------------------------------
  if(posY > (slider->posY + slider->height)) // x check
  {
    return 0;             // not in button
  }
  return 1;               // in button !
}

/******************************************************************************/
/* FUNCTION     : LCD_DrawRect                                                */
/* INPUT        : posX1, position in X for the first point                    */
/*                posY1, position in Y for the first point                    */
/*                posX2, position in X for the second point                   */
/*                posY2, position in Y for the second point                   */
/*                color, 1 = black rectangle, 0= white rectangle              */
/*                fill, 1 = filled, 0 = not filled                            */
/* OUTPUT       : -                                                           */
/* COMMENTS     : THIS FUNCTION DRAW A RECTANGLE ON THE DISPLAY               */
/*                fill define when the rectangle is filled or not (1 or 0)    */
/*                color	define the rectangle color                            */
/******************************************************************************/
void LCD_DrawRect(uint16_t posX1,uint16_t posY1, uint16_t posX2,
		uint16_t posY2,uint8_t fill,uint16_t color)
{
  uint16_t i;
  uint16_t j;
  uint16_t temp;
  uint8_t colorH,colorL;

  colorH = color>>8;
  colorL = color;
  //----------------------------------------------------------------------------
  // check swap left-right, top-bottom
  //----------------------------------------------------------------------------
  if(posX1 > posX2)
  {
    temp = posX1;
    posX1 = posX2;
    posX2 = temp;
  }
  if(posY1 > posY2)
  {
    temp = posY1;
    posY1 = posY2;
    posY2 = temp;
  }
  //----------------------------------------------------------------------------
  if(fill==1)								// if the rectangle is filled
  {
    //--------------------------------------------------------------------------
    LCD_Cmd(COLUMN_ADDRESS);              // column address set
    LCD_Data(posX1>>8);                   // start address
    LCD_Data(posX1);                      //
    LCD_Data(posX2>>8);                   // end address
    LCD_Data(posX2);                      //
    //--------------------------------------------------------------------------
    LCD_Cmd(PAGE_ADDRESS);                // page address set
    LCD_Data(posY1>>8);                   // start address
    LCD_Data(posY1);                      // is 0
    LCD_Data((LCD_HEIGHT-1) >> 8);        // end address
    LCD_Data((LCD_HEIGHT-1));             //
    //--------------------------------------------------------------------------
    LCD_Cmd(MEMORY_WRITE);                // write data
    for(i=posX1;i<=posX2;i++)
    {
      for(j=posY1;j<=posY2;j++)
      {                                   // write a filled rectangle
        LCD_Data(colorH);
        LCD_Data(colorL);
      }
    }
  }
  //----------------------------------------------------------------------------
  else                                  // if the rectangle is not filled
  {
    //--------------------------------------------------------------------------
    LCD_Cmd(COLUMN_ADDRESS);            // column address set
    LCD_Data(posX1>>8);                 // start address is X1
    LCD_Data(posX1);                    //
    LCD_Data(posX2>>8);                 // end address is X2
    LCD_Data(posX2);                    //
    //--------------------------------------------------------------------------
    LCD_Cmd(PAGE_ADDRESS);              // page address set
    LCD_Data(posY1>>8);                 // start address
    LCD_Data(posY1);                    // is Y1
    LCD_Data((LCD_HEIGHT-1) >> 8);      // end address
    LCD_Data((LCD_HEIGHT-1));           //
    //--------------------------------------------------------------------------
    LCD_Cmd(MEMORY_WRITE);              // write data

    for(i=posX1;i<=posX2;i++)           // write one horizontal line
    {
      LCD_Data(colorH);
      LCD_Data(colorL);
    }
    //--------------------------------------------------------------------------
    LCD_Cmd(PAGE_ADDRESS);              // page address set
    LCD_Data(posY2>>8);                 // start address
    LCD_Data(posY2);                    // is 0
    LCD_Cmd(MEMORY_WRITE);              // page address set
    for(i=posX1;i<=posX2;i++)           // write second horizontal line
    {
      LCD_Data(colorH);
      LCD_Data(colorL);
    }
    //--------------------------------------------------------------------------
    LCD_Cmd(COLUMN_ADDRESS);            // column address set
    LCD_Data(posX1>>8);                 // start address is X1
    LCD_Data(posX1);                    //
    LCD_Data(posX1>>8);                 // end address is same (width = 1)
    LCD_Data(posX1);                    //
    //--------------------------------------------------------------------------
    LCD_Cmd(PAGE_ADDRESS);              // page address set
    LCD_Data(posY1>>8);                 // start address
    LCD_Data(posY1);                    // is 0
    LCD_Data((LCD_HEIGHT-1) >> 8);      // end address
    LCD_Data((LCD_HEIGHT-1));           //
    LCD_Cmd(MEMORY_WRITE);              // write data
    for(i=posY1;i<=posY2;i++)           // write one vertical line
    {
      LCD_Data(colorH);
      LCD_Data(colorL);
    }
    //--------------------------------------------------------------------------
    LCD_Cmd(COLUMN_ADDRESS);            // column address set
    LCD_Data(posX2>>8);                 // start address is X2
    LCD_Data(posX2);                    //
    LCD_Data(posX2>>8);                 // end address is same (width = 1)
    LCD_Data(posX2);                    //
    LCD_Cmd(MEMORY_WRITE);              // write data
    for(i=posY1;i<=posY2;i++)           // write second vertical line
    {
      LCD_Data(colorH);
      LCD_Data(colorL);
    }
  }
}

/******************************************************************************/
/* FUNCTION : LCD_DrawText                                                    */
/* INPUT		: * msg		-> String which is to display                           */
/*            font Font info descriptor                                       */
/*            posX x-position to start (0 to LCD_WIDTH-msgSize)               */
/*				    posY y-position	to startine (0 to LCD_HEIGHT - font height)		  */
/*				    color font color choice                                     		*/
/*				    bg_color background color choice                             		*/
/* OUTPUT		: -                                                               */
/******************************************************************************/
/* COMMENTS : WRITE A MESSAGE ON THE SCREEN ON THE SELECTED POSITION          */
/******************************************************************************/
void LCD_DrawText(const uint8_t * msg,const FONT_INFO * font, ALIGN align,
        uint16_t posX, uint16_t posY, uint16_t color, uint16_t bg_color)
{
  uint8_t	x,y,i;
  int8_t	tempChar;
  uint16_t offset;
  uint8_t width=0;
  uint8_t fontByte,fontPixel;
  uint16_t posYEnd, posXEnd, yOffset;

  #define INTER_CHAR  2

  for(i=0;i<strlen(msg);i++)
  {
    tempChar = (msg[i]) - font->start_char;      // get char of table
    width += font->descriptor[tempChar].width+2;  // width of char
  }
  if(align == A_RIGHT)
  {
    posX = posX - width;
  }
  else if (align == A_CENTER)
  {
    posX = posX - width / 2;
  }

  do
  {
    tempChar = (*msg) - font->start_char;      // get char of table
    offset = font->descriptor[tempChar].offset;// offset of char in table
    width = font->descriptor[tempChar].width;  // width of char

    //--------------------------------------------------------------------------
    // set working area as the character width & height
    //--------------------------------------------------------------------------
    LCD_Cmd(COLUMN_ADDRESS);              // column address set
    LCD_Data(posX>>8);                    // start address
    LCD_Data(posX);                       // is X
    posXEnd = posX+width-1+INTER_CHAR;
    LCD_Data(posXEnd>>8);                 // end address
    LCD_Data(posXEnd);                    //
    //--------------------------------------------------------------------------
    LCD_Cmd(PAGE_ADDRESS);                // page address set
    LCD_Data(posY>>8);                    // start address
    LCD_Data(posY);                       // is Y
    posYEnd = posY + font->height-1;
    LCD_Data(posYEnd>>8);                 // end address
    LCD_Data(posYEnd);                    // 
    LCD_Cmd(MEMORY_WRITE);                // write data
    //--------------------------------------------------------------------------
    for(y=0;y<font->height;y++)            // for the height
    {
      yOffset = offset + (y * ((width+7)/8));
      for(x=0;x<width;x++)              // for the width
      {
        fontByte = font->bitmap[yOffset + (x/8)];
        fontPixel = fontByte & (0x80 >> (x%8)); // get pixel in char
        if(fontPixel != 0)
        {
          LCD_Data(color>>8);
          LCD_Data(color);
        }
        else
        {
          LCD_Data(bg_color>>8);
          LCD_Data(bg_color);
        }
      }
      for(x=0;x<INTER_CHAR;x++)
      {
        LCD_Data(bg_color>>8);
        LCD_Data(bg_color);
      }
    }
    posX = posX + 2 + width;              // next char position
    msg++;                                // next char in string
 }while(*msg != 0);                       // until end of string
}

/******************************************************************************/
/* FUNCTION : RGB2LCD                                                         */
/* INPUT		: * color table entry position                                    */
/* OUTPUT		: Color for RGB lcd color sheme                                   */
/******************************************************************************/
/* COMMENTS : Get LCD color from bitmap RGB 24 bits color table               */
/******************************************************************************/
uint16_t RGB2LCD(uint8_t * colorTableEntry)
{
    uint16_t red,green,blue;            // three colors
    uint16_t color;                     // LCD color 16 bits

    red = colorTableEntry[2] >> 3;      // get 5 bits red information
    green = colorTableEntry[1] >> 2;    // get 6 bits green information
    blue = colorTableEntry[0] >> 3;     // get 5 bits blue information
    color = (red << 11) | (green << 5) | (blue << 0); // concatene
    return color;
}

/******************************************************************************/
/* FUNCTION : LCD_Bitmap                                                      */
/* INPUT    : * bmpPtr -> bitmap array pointer                                */
/*            posX	 -> x position to start                                   */
/*            posY	 -> y position to start                                   */
/* OUTPUT       : return an error code                                        */
/*                0 - ok                                                      */
/*                1 - bad bmp format                                          */
/*                2 - image size error                                        */
/******************************************************************************/
/* COMMENTS     : Bitmap has to be 8 bits depth with RLE encoding             */
/******************************************************************************/
uint8_t LCD_Bitmap(const uint8_t * bmpPtr, uint16_t posX, uint16_t posY)
{
  uint32_t bfOffBits;
  uint16_t biWidth, biHeight;
  uint8_t * colorTablePtr;
  uint8_t endOfImage;
  uint16_t i,curXPos,curYPos;
  uint8_t  pixelCount;
  uint16_t color;
  uint16_t jumpColor;
  uint16_t jumpCount;
  uint16_t newY;
  BMP_STRUCT * strPtr;

  strPtr = (BMP_STRUCT *)bmpPtr;
  //----------------------------------------------------------------------------
  bfOffBits 	= strPtr->bfOffBits;    // get offset to start of image
  biWidth	= strPtr->biWidth;          // get width in pixel
  biHeight	= strPtr->biHeight;       // get height in pixel
  if((strPtr->biBitCount != 8) ||     // check 8 bits per pixel
     (strPtr->biCompression != BI_RLE_8))    // check compression RLE_8
  {
    return 1;                         // error code return
  }
  if(((posX + biWidth) > (LCD_WIDTH)) ||  // check width
     ((posY + biHeight) > (LCD_HEIGHT)))  // check height
  {
    return 2;                               // error code return
  }
  //----------------------------------------------------------------------------
  LCD_Cmd(CMD_MEM_ACC_CTRL);          // lcd memory access
//  LCD_Data(0xE8);                     // invert top-bottom order for bmp
  LCD_Data(CMD_MEM_ACC_DATA_BMP);       // invert top-bottom order for bmp

  colorTablePtr = (uint8_t *)bmpPtr + BI_COLOR_TBL;
  bmpPtr += bfOffBits;
  endOfImage = 0;
  jumpColor = RGB2LCD(colorTablePtr);
  //----------------------------------------------------------------------------
  LCD_Cmd(COLUMN_ADDRESS);            // column address set
  LCD_Data(posX>>8);                  // start address
  LCD_Data(posX);                     //
  posX = posX + biWidth-1;
  LCD_Data(posX>>8);                  // end address
  LCD_Data(posX);                     //
  //----------------------------------------------------------------------------
  LCD_Cmd(PAGE_ADDRESS);              // page address set
  newY = LCD_HEIGHT - posY - biHeight;
  LCD_Data(newY>>8);                  // start address
  LCD_Data(newY);                     // 
  newY = newY + biHeight-1;
  LCD_Data(newY>>8);                  // end address
  LCD_Data(newY);                     //
  //----------------------------------------------------------------------------
  LCD_Cmd(MEMORY_WRITE);              // write data to LCD
  curXPos = 0;
  curYPos = 0;
  do
  {
    if(bmpPtr[0] == 0)                // escape char in RLE data
    {
      switch (bmpPtr[1])              // check second parameter
      {
        case 0:                       // next line
          bmpPtr += 2;                // increment image pointer
          for(i=0;i<biWidth - curXPos;i++)
          {
            LCD_Data(jumpColor >> 8);
            LCD_Data(jumpColor);
          }
          curXPos = 0;
          curYPos++;
        break;
        case 1:                       // end of image
          for(i=0;i<biWidth - curXPos;i++)  // terminate line if any
          {
            LCD_Data(jumpColor >> 8);
            LCD_Data(jumpColor);
          }
          for(i=0;i<(biHeight - curYPos -1)*biWidth;i++) // and others lines
          {
            LCD_Data(jumpColor >> 8);
            LCD_Data(jumpColor);
          }
          endOfImage = 1;             // set flag
        break;
        case 2:                       // jump to (entry 0 for color is used)
          jumpCount = bmpPtr[2] + (bmpPtr[3]*biWidth);
          for(i=0;i<jumpCount;i++)
          {
            curXPos++;
            LCD_Data(jumpColor >> 8);
            LCD_Data(jumpColor);
          }
          curYPos = curYPos + (jumpCount / biWidth);
          curXPos = curXPos % biWidth;
          bmpPtr += 4;
        break;
        default:                      // not coded pixels
          pixelCount = bmpPtr[1];     // get number of following pixels
          for(i=0;i < pixelCount;i++)
          {
            color = RGB2LCD(colorTablePtr+(bmpPtr[2+i]*4));
            curXPos++;
            LCD_Data(color >> 8);
            LCD_Data(color);
          }
          if(pixelCount < 255)
          {
            pixelCount = (pixelCount+1) & 0xFE; // to upper even (RLE defs)
            bmpPtr+=pixelCount+2;       // increment image pointer
          }
          else
          {
              bmpPtr += 258;
          }
        break;
      }
    }
    else                              // coded pixels
    {
      pixelCount = bmpPtr[0];         // number of same pixels
      color = RGB2LCD(colorTablePtr+(bmpPtr[1]*4));
      for(i=0;i < pixelCount;i++)
      {
        curXPos++;
        LCD_Data(color >> 8);
        LCD_Data(color);
      }
      bmpPtr+=2;                      // increment image pointer
    }
  }while(endOfImage == 0);            // until end of image
  LCD_Cmd(CMD_MEM_ACC_CTRL);          // lcd memory access
//  LCD_Data(0xA8);                     // order is top-bottom again
  LCD_Data(CMD_MEM_ACC_DATA);         // order is top-bottom again
  return 0;                           // return no error
}




