/******************************************************************************/
/* FILENAME	: lcd_highlevel.h                                                 */
/*----------------------------------------------------------------------------*/
/* GOAL		: Offers the high level function for the LCD control                */
/******************************************************************************/
#include <stdint.h>

typedef struct
{
    uint8_t width;
    uint16_t offset;
}FONT_CHAR_INFO;

typedef struct
{
    uint8_t height;
    uint8_t start_char;
    uint8_t end_char;
    const FONT_CHAR_INFO * descriptor;
    const uint8_t * bitmap;

}FONT_INFO;

#ifndef LCD_HIGHLEVEL_H
#define LCD_HIGHLEVEL_H
//------------------------------------------------------------------------------
// colors definitions for LCD usage
//
// Bits are             0bRRRRRGGGGGGBBBBB
// Bits are really      0bGGGGGGBBBBBRRRRR
//------------------------------------------------------------------------------
#define     WHITE       0b1111111111111111
#define     ORANGE      0b1111100100011110
#define     BLACK       0b0000000000000000
#define     DARK_GRAY   0b0010000100000100
#define     GRAY        0b0110001100001100
#define     LIGHT_GRAY  0b1010010100010100
#define     RED         0b1111100000000000
#define     GREEN       0b0000011111100000
#define     DARK_GREEN  0b0000001111100000
#define     BLUE        0b0000000000011111
#define     LIGHTBLUE   0b0111101111111111
#define     CYAN        0b0000011111111111
#define     FUCHSIA     0b1111100000011111
#define     YELLOW      0b1111111111100000
#define     MEDIRIO_C   0b0000001111011001



typedef enum 
{
    A_LEFT,
    A_RIGHT,
    A_CENTER
}ALIGN;

/******************************************************************************/
/* bmp-file information (File Header, Image Header)                           */
/******************************************************************************/
#define   BI_COLOR_TBL        54    // offset to color table (fixed)
#define   BI_RLE_8            1     // RLE 8 bits encoding mode
#define   BI_RLE_4            2     // RLE 4 bits encoding mode

typedef struct
{
	uint16_t bfType;                  // define BMP type of image
	uint32_t bfSize;                  // define size of file (bytes)
	uint16_t bfRvd1;                  // unused - 0
	uint16_t bfRvd2;                  // unused - 0
	uint32_t bfOffBits;               // offset to start of data pixel data
	uint32_t biSize;                  // size of header
	uint32_t biWidth;                 // image width in pixels
	uint32_t biHeight;                // image height in pixels
	uint16_t biPlanes;                // must be 1
	uint16_t biBitCount;              // bits per pixel
	uint32_t biCompression;           // 0 = uncompressed, 1=RLE
	uint32_t biSizeImage;             // image size (for compressed only)
	uint32_t biXPelsPerMeter;         // pref resol. in pixel/meter
	uint32_t biYPelsPerMeter;         // pref resol. in pixel/meter
	uint32_t biClrUsed;               // num. color map entries
	uint32_t biClrImportant;          // num. of signif. colors
}BMP_STRUCT;

/******************************************************************************/
/* Button descriptor                                                          */
/******************************************************************************/
typedef struct
{
  uint16_t posX;
  uint16_t posY;
  uint16_t width;
  uint16_t height;
  uint16_t txtColor;
  uint16_t bgColor;
  const uint8_t  *text;
  FONT_INFO * font;
  uint8_t  pressed;
  void    (*fpPress)(uint8_t);
  void    (*fpReleased)(uint8_t);
  void    (*fpOut)(uint8_t);
  uint8_t index;
}btn_t;

/******************************************************************************/
/* Slider descriptor                                                          */
/******************************************************************************/
typedef struct
{
  uint16_t posX;
  uint16_t posY;
  uint16_t width;
  uint16_t height;
  uint16_t sldColor;
  uint16_t bgColor;
  uint16_t borderColor;
  uint16_t cursorWidth;
  uint8_t  pressed;
  uint16_t value;
  uint16_t oldValue;
  uint16_t steps;
  void    (*fpPress)();
}sld_t;

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
/*                The bmp has to be 6 pixel little than the button because of */
/*                shadow effet onn button press (top and left)                */
/******************************************************************************/
void LCD_ButtonCreate(uint16_t posX, uint16_t posY,
                      uint16_t width, uint16_t height,
                      uint16_t txtColor, uint16_t bgColor,
                      const uint8_t * textOrBmp, FONT_INFO * font,
                      void (*fpPress)(uint8_t), void (* fpReleased)(uint8_t), void (* fpOut)(uint8_t),
                      btn_t * button,
                      uint8_t index);


/******************************************************************************/
/* FUNCTION     : Draw the button                                             */
/* INPUT        : button - The buttons descriptor                             */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Draw the button, needs to be called once                    */
/******************************************************************************/
void LCD_ButtonDraw(btn_t * button);

/******************************************************************************/
/* FUNCTION     : Redraw the button status (pressed or released)              */
/* INPUT        : button - The buttons descriptor                             */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Redraw button borders for pressed or released state         */
/******************************************************************************/
void LCD_ButtonUpdate(btn_t * button);

/******************************************************************************/
/* FUNCTION     : Create a slider                                             */
/* INPUT        : posX, posY, width, height: slider coordinates               */
/*                sldColor, bgColor, borderColor: the slider colors           */
/*                cursorWidth : width of the cursor (if 0, progress bar)      */
/*                steps : the number of steps in the slider                   */
/*                fpPress : pointer to function action to do                  */
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
                      sld_t * slider);

/******************************************************************************/
/* FUNCTION     : Redraw the slider (value has changed)                       */
/* INPUT        : button - The buttons descriptor                             */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Redraw button borders for pressed or released state         */
/******************************************************************************/
void LCD_SliderUpdate(sld_t * slider);

/******************************************************************************/
/* FUNCTION     : Draw the slider                                             */
/* INPUT        : slider - The slider descriptor                              */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Draw the slider, needs to be called once                    */
/******************************************************************************/
void LCD_SliderDraw(sld_t * slider);

/******************************************************************************/
/* FUNCTION     : Check if the coordinates are in the slider                  */
/* INPUT        : slider - The slider descriptor                              */
/*                posX, posY                                                  */
/* OUTPUT       : true or false                                               */
/* COMMENTS     : Typically used for touchscreen test                         */
/******************************************************************************/
uint8_t LCD_InSlider(sld_t * slider, uint16_t posX, uint16_t posY);

/******************************************************************************/
/* FUNCTION     : Check if the coordinates are in the button                  */
/* INPUT        : button - The buttons descriptor                             */
/*                posX, posY                                                  */
/* OUTPUT       : true or false                                               */
/* COMMENTS     : Typically used for touchscreen test                         */
/******************************************************************************/
uint8_t LCD_InButton(btn_t * button, uint16_t posX, uint16_t posY);

/******************************************************************************/
/* FUNCTION     : lcd_init                                                    */
/* INPUT		: -                                                               */
/* OUTPUT		: -                                                               */
/******************************************************************************/
/* COMMENTS     : INITALISE THE LCD-MODULE                                    */
/******************************************************************************/
void LCD_Init(void);

/******************************************************************************/
/* FUNCTION     : LCD_Fill                                                    */
/* INPUT		: color The color to fill                                         */
/* OUTPUT		: -                                                               */
/******************************************************************************/
/* COMMENTS     : Fill the screen with selected color      					   			  */
/******************************************************************************/
void LCD_Fill(uint16_t color);

/******************************************************************************/
/* FUNCTION     : LCD_SetPixel(PosX,PosY)                                     */
/* INPUT        : posX (0-319), posY (0-239)                                  */
/* OUTPUT       : -                                                           */
/* COMMENTS     : THIS FUNCTION WRITE A PIXEL ON THE DISPLAY                  */
/******************************************************************************/
void LCD_SetPixel(uint16_t posX,uint16_t posY, uint16_t color);

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
		uint16_t posY2,uint8_t fill,uint16_t color);

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
        uint16_t posX, uint16_t posY, uint16_t color, uint16_t bg_color);

/******************************************************************************/
/* FUNCTION : RGB2LCD                                                         */
/* INPUT		: * color table entry position                                    */
/* OUTPUT		: Color for RGB lcd color sheme                                   */
/******************************************************************************/
/* COMMENTS : Get LCD color from bitmap RGB 24 bits color table               */
/******************************************************************************/
uint16_t RGB2LCD(uint8_t * colorTableEntry);

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
uint8_t LCD_Bitmap(const uint8_t * bmpPtr, uint16_t posX, uint16_t posY);
#endif
