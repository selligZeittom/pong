#include <pic18.h>
#include <stdint.h>

#define NHD_24_CF   0
#define NHD_24_SF   1
#define NHD_18      2

#define _XTAL_FREQ   64000000L

/******************************************************************************/
/* Display selection                                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* choose one of the three choices                                            */
/******************************************************************************/
//#define  DISPLAY_NHD NHD_24_CF   // 2.4 " display (controller ST7789S)
#define  DISPLAY_NHD NHD_24_SF    // 2.4 " display (controller ILI9163)
//#define  DISPLAY_NHD   18   // 1.8 " display (controller ILI9341)
/******************************************************************************/
/* Display direction choice                                                   */
/* todo - > vertical modes have not been tested !!!                           */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/* choose one of the four choices                                             */
/* the normal direction means the connector is to the left (or to the top)    */
/******************************************************************************/
//#define HORIZONTAL_NORMAL
//#define HORIZONTAL_INVERTED
//#define VERTICAL_NORMAL
#define VERTICAL_INVERTED

#ifdef HORIZONTAL_NORMAL
#if (DISPLAY_NHD == NHD_24_SF)
#define   LCD_WIDTH  320
#define   LCD_HEIGHT 240
#define CMD_MEM_ACC_DATA     0b01101000
#define CMD_MEM_ACC_DATA_BMP 0b00101000
#endif
#if (DISPLAY_NHD == NHD_24_CF)
#define   LCD_WIDTH  320
#define   LCD_HEIGHT 240
#define CMD_MEM_ACC_DATA     0b01100000
#define CMD_MEM_ACC_DATA_BMP 0b00100000
#endif
#if (DISPLAY_NHD == NHD_18)
#define   LCD_WIDTH  160
#define   LCD_HEIGHT 128
#define CMD_MEM_ACC_DATA     0b01101000
#define CMD_MEM_ACC_DATA_BMP 0b00101000
#endif
#endif
#ifdef HORIZONTAL_INVERTED
#if (DISPLAY_NHD == NHD_24_SF)
#define   LCD_WIDTH  320
#define   LCD_HEIGHT 240
#define CMD_MEM_ACC_DATA     0b10101000
#define CMD_MEM_ACC_DATA_BMP 0b11101000
#endif
#if (DISPLAY_NHD == NHD_24_CF)
#define   LCD_WIDTH  320
#define   LCD_HEIGHT 240
#define CMD_MEM_ACC_DATA     0b10100000
#define CMD_MEM_ACC_DATA_BMP 0b11100000
#endif
#if (DISPLAY_NHD == NHD_18)
#define   LCD_WIDTH  160
#define   LCD_HEIGHT 128
#define CMD_MEM_ACC_DATA     0b10101000
#define CMD_MEM_ACC_DATA_BMP 0b11101000
#endif
#endif
#ifdef VERTICAL_NORMAL
#if (DISPLAY_NHD == NHD_24_SF)
#define   LCD_WIDTH  240
#define   LCD_HEIGHT 320
#define CMD_MEM_ACC_DATA     0b11001000
#define CMD_MEM_ACC_DATA_BMP 0b01001000
#endif
#if (DISPLAY_NHD == NHD_24_CF)
#define   LCD_WIDTH  240
#define   LCD_HEIGHT 320
#define CMD_MEM_ACC_DATA     0b11000000
#define CMD_MEM_ACC_DATA_BMP 0b01000000
#endif
#if (DISPLAY_NHD == NHD_18)
#define   LCD_WIDTH  128
#define   LCD_HEIGHT 160
#define CMD_MEM_ACC_DATA     0b11001000
#define CMD_MEM_ACC_DATA_BMP 0b01001000
#endif
#endif
#ifdef VERTICAL_INVERTED
#if (DISPLAY_NHD == NHD_24_SF)
#define   LCD_WIDTH  240
#define   LCD_HEIGHT 320
#define CMD_MEM_ACC_DATA     0b00001000
#define CMD_MEM_ACC_DATA_BMP 0b10001000
#endif
#if (DISPLAY_NHD == NHD_24_CF)
#define   LCD_WIDTH  240
#define   LCD_HEIGHT 320
#define CMD_MEM_ACC_DATA     0b00000000
#define CMD_MEM_ACC_DATA_BMP 0b10000000
#endif
#if (DISPLAY_NHD == NHD_18)
#define   LCD_WIDTH  128
#define   LCD_HEIGHT 160
#define CMD_MEM_ACC_DATA     0b00001000
#define CMD_MEM_ACC_DATA_BMP 0b10001000
#endif
#endif
/******************************************************************************/
/* constantes	for ILI9341 commands access                                     */
/******************************************************************************/
#if (DISPLAY_NHD == NHD_24_SF)

#define     CMD_SOFTRESET       0x01
#define     CMD_DISPOFF         0x28
#define     CMD_DISPON          0x29
#define     CMD_SLPOUT          0x11
#define     CMD_PWR_CTRL_A      0xCB
#define     CMD_PWR_CTRL_B      0xCF
#define     CMD_DRV_TIM_CTRL_A  0xE8
#define     CMD_DRV_TIM_CTRL_B  0xEA
#define     CMD_PWR_ON_SEQ_CTRL 0xED
#define     CMD_PUMP_RATIO_CTRL 0xF7
#define     CMD_INTERFACE_CTRL  0xF6
#define     CMD_PWR_CTRL_1      0xC0
#define     CMD_PWR_CTRL_2      0xC1
#define     CMD_VCOM_CTRL_1     0xC5
#define     CMD_VCOM_CTRL_2     0xC7
#define     CMD_MEM_ACC_CTRL    0x36
#define     CMD_FRAME_RATE_CTRL 0xB1
#define     CMD_DISP_FUNC_CTRL  0xB6
#define     CMD_PIXEL_FORMAT    0x3A
#define     GAMMA_CONTROL       0xF2
#define     GAMMA_CURVE         0x26
#define     COLUMN_ADDRESS      0x2A
#define     PAGE_ADDRESS        0x2B
#define     MEMORY_WRITE        0x2C
#define     MEMORY_READ         0x2E

/******************************************************************************/
/* constantes	for ST7789S commands access                                   */
/******************************************************************************/
#elif (DISPLAY_NHD == NHD_24_CF)

#define     CMD_SOFTRESET       0x01
#define     CMD_DISPOFF         0x28
#define     CMD_DISPON          0x29
#define     CMD_SLPOUT          0x11
#define     CMD_PWR_CTRL_A      0xCB
#define     CMD_PWR_CTRL_B      0xCF
#define     CMD_DRV_TIM_CTRL_A  0xE8
#define     CMD_DRV_TIM_CTRL_B  0xEA
#define     CMD_PWR_ON_SEQ_CTRL 0xED
#define     CMD_PUMP_RATIO_CTRL 0xF7
#define     CMD_INTERFACE_CTRL  0xF6
#define     CMD_PWR_CTRL_1      0xD0
#define     CMD_PWR_CTRL_2      0xC1
#define     CMD_VCOM_CTRL_1     0xC5
#define     CMD_VCOM_CTRL_2     0xC7
#define     CMD_MEM_ACC_CTRL    0x36
#define     CMD_FRAME_RATE_CTRL 0xC6
#define     CMD_DISP_FUNC_CTRL  0xB6
#define     CMD_PIXEL_FORMAT    0x3A
#define     GAMMA_CONTROL       0xF2
#define     GAMMA_CURVE         0x26
#define     COLUMN_ADDRESS      0x2A
#define     PAGE_ADDRESS        0x2B
#define     MEMORY_WRITE        0x2C
#define     MEMORY_READ         0x2E
#define     CMD_PORCH_SETTING   0xB2
#define     CMD_GATE_CTRL       0xB7
#define     CMD_VCOM_SETTING    0xBB
#define     CMD_LCM_CTRL        0xC0
#define     CMD_VDVVRH_EN       0xC2
#define     CMD_VRHS_SET        0XC3
#define     CMD_VDVS_SET        0xC4
#define     CMD_POSVOL_GAM_CTRL 0xE0
#define     CMD_NEGVOL_GAM_CTRL 0xE1

/******************************************************************************/
/* constantes	for ILI9163 commands access                                   */
/******************************************************************************/
#elif (DISPLAY_NHD == NHD_18)
#define     CMD_SOFTRESET       0x01
#define     GAMMA_CURVE         0x26
#define     GAMMA_CONTROL       0xF2
#define     CMD_FRAME_RATE_CTRL 0xB1
#define     CMD_PWR_CTRL_1      0xC0
#define     CMD_PWR_CTRL_2      0xC1
#define     CMD_VCOM_CTRL_1     0xC5
#define     CMD_VCOM_CTRL_2     0xC7
#define     COLUMN_ADDRESS      0x2A
#define     PAGE_ADDRESS        0x2B
#define     CMD_MEM_ACC_CTRL    0x36
#define     CMD_PIXEL_FORMAT    0x3A
#define     CMD_DISPON          0x29
#define     MEMORY_WRITE        0x2C


#define     CMD_DISPOFF         0x28
#define     CMD_SLPOUT          0x11
#define     CMD_PWR_CTRL_A      0xCB
#define     CMD_PWR_CTRL_B      0xCF
#define     CMD_DRV_TIM_CTRL_A  0xE8
#define     CMD_DRV_TIM_CTRL_B  0xEA
#define     CMD_PWR_ON_SEQ_CTRL 0xED
#define     CMD_PUMP_RATIO_CTRL 0xF7
#define     CMD_INTERFACE_CTRL  0xF6
#define     MEMORY_READ         0x2E

#endif

//------------------------------------------------------------------------------
// LCD connected pins definitions
//------------------------------------------------------------------------------
#define LCD_nRD           LATB3
#define LCD_nWR           LATB4
#define LCD_DnC           LATC1
#define LCD_nRES          LATC0
#define LCD_nCS           LATB5

#define DIR_LCD_nRD       TRISB3
#define DIR_LCD_nWR       TRISB4
#define DIR_LCD_DnC       TRISC1
#define DIR_LCD_nRES      TRISC0
#define DIR_LCD_nCS       TRISB5

#define LCD_DATA_BUS      PORTA
#define DIR_LCD_DATA_BUS  TRISA

#define LCD_BACKLIGHT_DIR   TRISC2
#define LCD_BACKLIGHT       LATC2

//------------------------------------------------------------------------------
// Macro definition for very fast access
// Add a chip select if needed
//------------------------------------------------------------------------------
#define LCD_Data(data) LCD_DATA_BUS = data;  LCD_nWR = 0; LCD_nWR = 1;
#define LCD_Cmd(data) LCD_DATA_BUS = data; LCD_DnC = 0; LCD_nWR = 0; LCD_nWR = 1; LCD_DnC = 1;

/******************************************************************************/
/* FUNCTION : LCD_PowerOff                                                    */
/* INPUT		: -                                                               */
/* OUTPUT		:                                                                 */
/******************************************************************************/
/* COMMENTS : Turns off LCD and all I/O pins ...                              */
/*            Do not forget to power off the LCD !!!                          */
/******************************************************************************/
void LCD_PowerOff(void);

/******************************************************************************/
/* FUNCTION : LCD_Lowlevel_Init                                               */
/* INPUT		: -                                                               */
/* OUTPUT		:                                                                 */
/******************************************************************************/
/* COMMENTS : Initialize ports, reset and so ...                              */
/******************************************************************************/
void LCD_Lowlevel_Init(void);

/******************************************************************************/
/* FUNCTION : lcd_read                                                        */
/* INPUT		: posX the x position of pixel to read                            */
/*            posY the y position of pixel to read                            */
/* OUTPUT		: the color read                                                  */
/******************************************************************************/
/* COMMENTS : Reads a pixel from the selected lcd memory position             */
/******************************************************************************/
uint16_t LCD_Read(uint16_t posX, uint16_t posY);

