//*****************************************************************************
//                                                                             
//*****************************************************************************
#ifndef __STMPE811_H__                                                         
#define __STMPE811_H__                                                         
#include <stdint.h>
#include "lcd_highlevel.h"
#include "lcd_lowlevel.h"
#include "application.h"
//*****************************************************************************
// TSC chip select pin definitions
//*****************************************************************************
#define nCS_TSC                 LATC6
#define DIR_nCS_TSC             TRISC6
#define DIR_INT_TSC             TRISB1
#define PORT_INT_TSC            PORTBbits.RB1
#define INTE_TSC                INT1IE
#define INTF_TSC                INT1IF
#define INTEDG_TSC              INTEDG1
//*****************************************************************************
// TSC measures resluts
// The measures on max and min values have to be here.
// These are used to convert from TSC coordiantes to LCD pixel coordinates
//*****************************************************************************
#if (DISPLAY_NHD == NHD_24_SF)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef VERTICAL_NORMAL
  #define MIN_Y_MEAS  280L
  #define MAX_Y_MEAS  3800L
  #define MIN_X_MEAS  3800L
  #define MAX_X_MEAS  480L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef VERTICAL_INVERTED
  #define MIN_Y_MEAS  3800L
  #define MAX_Y_MEAS  280L
  #define MIN_X_MEAS  380L
  #define MAX_X_MEAS  3800L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef HORIZONTAL_NORMAL
  #define MIN_Y_MEAS  3800L
  #define MAX_Y_MEAS  280L
  #define MIN_X_MEAS  3800L
  #define MAX_X_MEAS  380L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef HORIZONTAL_INVERTED
  #define MIN_Y_MEAS  300L
  #define MAX_Y_MEAS  3800L
  #define MIN_X_MEAS  380L
  #define MAX_X_MEAS  3800L
#endif

//------------------------------------------------------------------------------
#elif (DISPLAY_NHD == NHD_24_CF)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef VERTICAL_NORMAL
  #define MIN_Y_MEAS  280L
  #define MAX_Y_MEAS  3800L
  #define MIN_X_MEAS  3800L
  #define MAX_X_MEAS  480L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef VERTICAL_INVERTED
  #define MIN_Y_MEAS  3800L
  #define MAX_Y_MEAS  280L
  #define MIN_X_MEAS  380L
  #define MAX_X_MEAS  3800L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef HORIZONTAL_NORMAL
  #define MIN_Y_MEAS  3800L
  #define MAX_Y_MEAS  280L
  #define MIN_X_MEAS  3800L
  #define MAX_X_MEAS  380L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef HORIZONTAL_INVERTED
  #define MIN_Y_MEAS  300L
  #define MAX_Y_MEAS  3800L
  #define MIN_X_MEAS  380L
  #define MAX_X_MEAS  3800L
#endif

//------------------------------------------------------------------------------
#elif (DISPLAY_NHD == NHD_18)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef VERTICAL_NORMAL
  #define MIN_Y_MEAS  280L
  #define MAX_Y_MEAS  3800L
  #define MIN_X_MEAS  3800L
  #define MAX_X_MEAS  480L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef VERTICAL_INVERTED
  #define MIN_Y_MEAS  3800L
  #define MAX_Y_MEAS  280L
  #define MIN_X_MEAS  380L
  #define MAX_X_MEAS  3800L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef HORIZONTAL_NORMAL
  #define MIN_Y_MEAS  280L
  #define MAX_Y_MEAS  3800L
  #define MIN_X_MEAS  380L
  #define MAX_X_MEAS  3800L
#endif
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef HORIZONTAL_INVERTED
  #define MIN_Y_MEAS  3800L
  #define MAX_Y_MEAS  300L
  #define MIN_X_MEAS  3800L
  #define MAX_X_MEAS  280L
#endif
#endif


//*****************************************************************************
// STMPE811 registers definitions
//*****************************************************************************
#define STMPE811_ADDRESS        0x82     // chip SPI address
//------------------------------------------------------------------------------
#define STMPE811_CHIP_ID        0x00
#define STMPE811_ID_VER         0x02
#define STMPE811_SYS_CTRL1      0x03
#define STMPE811_SYS_CTRL2      0x04
#define STMPE811_SPI_CFG        0x08
#define STMPE811_INT_CTRL       0x09
#define STMPE811_INT_EN         0x0A
#define STMPE811_INT_STA        0x0B
#define STMPE811_GPIO_EN        0x0C
#define STMPE811_GPIO_INT_STA   0x0D
#define STMPE811_ADC_INT_EN     0x0E
#define STMPE811_ADC_INT_STA    0x0F
#define STMPE811_GPIO_SET_PIN   0x10
#define STMPE811_GPIO_CLR_PIN   0x11
#define STMPE811_MP_STA         0x12
#define STMPE811_GPIO_DIR       0x13
#define STMPE811_GPIO_ED        0x14
#define STMPE811_GPIO_RE        0x15
#define STMPE811_GPIO_FE        0x16
#define STMPE811_GPIO_AF        0x17
#define STMPE811_ADC_CTRL1      0x20
#define STMPE811_ADC_CTRL2      0x21
#define STMPE811_ADC_CAPT       0x22
#define STMPE811_ADC_DATA_CHO   0x30
#define STMPE811_ADC_DATA_CH1   0x32
#define STMPE811_ADC_DATA_CH2   0x34
#define STMPE811_ADC_DATA_CH3   0x36
#define STMPE811_ADC_DATA_CH4   0x38
#define STMPE811_ADC_DATA_CH5   0x3A
#define STMPE811_ADC_DATA_CH6   0x3C
#define STMPE811_ADC_DATA_CH7   0x3E
#define STMPE811_TSC_CTRL       0x40
#define STMPE811_TSC_CFG        0x41
#define STMPE811_WDW_TR_X       0x42
#define STMPE811_WDW_TR_Y       0x44
#define STMPE811_WDW_BL_X       0x46
#define STMPE811_WDW_BL_Y       0x48
#define STMPE811_FIFO_TH        0x4A
#define STMPE811_FIFO_STA       0x4B
#define STMPE811_FIFO_SIZE      0x4C
#define STMPE811_TSC_DATA_X     0x4D
#define STMPE811_TSC_DATA_Y     0x4F
#define STMPE811_TSC_DATA_Z     0x51
#define STMPE811_TSC_DATA_XYZ   0x52
#define STMPE811_TSC_FRACTION_Z 0x56
#define STMPE811_TSC_DATA       0x57
#define STMPE811_TSC_I_DRIVE    0x58
#define STMPE811_TSC_SHIELD     0x59
#define STMPE811_TEMP_CTRL      0x60
#define STMPE811_TEMP_DATA      0x61
#define STMPE811_TEMP_TH        0x62
//------------------------------------------------------------------------------
// bits definitions
//------------------------------------------------------------------------------
#define     STMPE811_SPI_MODE_0         0
#define     STMPE811_SPI_MODE_1         1
#define     STMPE811_SPI_MODE_2         2
#define     STMPE811_SPI_MODE_3         3
#define     STMPE811_AUTO_INCR          4
//------------------------------------------------------------------------------
#define STMPE811_SYS_CTRL1_SOFTRESET    0x02
#define STMPE811_SYS_CTRL1_HIBERNATE    0x01
//------------------------------------------------------------------------------
#define STMPE811_CTRL2_TS_OFF           0x08
#define STMPE811_CTRL2_GPIO_OFF         0x04
#define STMPE811_CTRL2_TSC_OFF          0x02
#define STMPE811_CTRL2_ADC_OFF          0x01
//------------------------------------------------------------------------------
#define STMPE811_INT_EN_GPIO            0x80
#define STMPE811_INT_EN_ADC             0x40
#define STMPE811_INT_EN_TEMP_SENS       0x20
#define STMPE811_INT_EN_FIFOEMPTY       0x10
#define STMPE811_INT_EN_FIFOFULL        0x08
#define STMPE811_INT_EN_FIFOOFLOW       0x04
#define STMPE811_INT_EN_TH              0x02
#define STMPE811_INT_EN_TOUCHDET        0x01
//------------------------------------------------------------------------------
#define STMPE811_ADC_CTRL1_SAMPLE_M     0x7
#define STMPE811_ADC_CTRL1_SAMPLE_S     4
#define STMPE811_ADC_CTRL1_MOD          0x8
#define STMPE811_ADC_CTRL1_SEL          0x2
//------------------------------------------------------------------------------
#define STMPE811_ADC_CTRL2_CLK1         0x0
#define STMPE811_ADC_CTRL2_CLK2         0x1
#define STMPE811_ADC_CTRL2_CLK13        0x2
#define STMPE811_ADC_CTRL2_CLK4         0x3
//------------------------------------------------------------------------------
#define STMPE811_TSC_CFG_AVE_1          0x00
#define STMPE811_TSC_CFG_AVE_2          0x40
#define STMPE811_TSC_CFG_AVE_4          0x80
#define STMPE811_TSC_CFG_AVE_8          0xC0
#define STMPE811_TSC_CFG_DELAY_M        0x7
#define STMPE811_TSC_CFG_DELAY_S        3
#define STMPE811_TSC_CFG_SETTING_M      0x7
#define STMPE811_TSC_CFG_SETTING_S      0
//------------------------------------------------------------------------------
#define STMPE811_FIFO_STA_OFLOW         0x80
#define STMPE811_FIFO_STA_FULL          0x40
#define STMPE811_FIFO_STA_EMPTY         0x20
#define STMPE811_FIFO_STA_TRIG          0x10
#define STMPE811_FIFO_STA_RESET         0x01
//------------------------------------------------------------------------------
#define STMPE811_FRACTION_Z_M           0x7
#define STMPE811_FRACTION_Z_S           0
//------------------------------------------------------------------------------
#define STMPE811_TSC_I_DRIVE_20         0x00
#define STMPE811_TSC_I_DRIVE_50         0x01
//------------------------------------------------------------------------------
#define STMPE811_TSC_CTRL_STA           0x80
#define STMPE811_TSC_CTRL_TRACK_M       0x7
#define STMPE811_TSC_CTRL_TRACK_S       4
#define STMPE811_TSC_CTRL_XYZ           0x00
#define STMPE811_TSC_CTRL_XY            0x02
#define STMPE811_TSC_CTRL_X             0x04
#define STMPE811_TSC_CTRL_Y             0x06
#define STMPE811_TSC_CTRL_Z             0x08
#define STMPE811_TSC_CTRL_EN            0x01
//------------------------------------------------------------------------------
#define STMPE811_INT_STA_GPIO           0x80
#define STMPE811_INT_STA_ADC            0x40
#define STMPE811_INT_STA_SENS           0x20
#define STMPE811_INT_STA_EMPTY          0x10
#define STMPE811_INT_STA_FULL           0x08
#define STMPE811_INT_STA_OFLOW          0x04
#define STMPE811_INT_STA_TH             0x02
#define STMPE811_INT_STA_DET            0x01
//------------------------------------------------------------------------------
#define STMPE811_INT_POLARITY           0x04
#define STMPE811_INT_TYPE               0x02
#define STMPE811_INT_GLOBAL             0x01


/******************************************************************************/
// TSC structure definition
/******************************************************************************/
typedef struct
{
    uint8_t pressed;          // is currently pressed
    uint8_t released;         // has been released
    uint16_t posX;            // current pos X
    uint16_t posY;            // current pos Y
}tsc_t;

extern tsc_t tsc;             // global current tsc status

/******************************************************************************/
/* FUNCTION : tscInit                                                         */
/* INPUT		: -                                                   */
/* OUTPUT		: -                                                   */
/******************************************************************************/
/* COMMENTS     : Initialise the TSC controller                               */
/******************************************************************************/
void TscInit(void);

/******************************************************************************/
/* FUNCTION : tscPowerOff                                                     */
/* INPUT		: -                                                   */
/* OUTPUT		: -                                                   */
/******************************************************************************/
/* COMMENTS : Turn off the TSC controller                                     */
/*            Do not forget to power off the TSC chip                         */
/*            Control the interrupt too, if used                              */
/******************************************************************************/
void tscPowerOff(void);

/******************************************************************************/
/* FUNCTION     : TscWrite                                                    */
/* INPUT        : ucReg is internal register address                          */
/*                ucNum is the number of bytes to write (max 4)               */
/*                ulVal is th data to write (max 32 bits)                     */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Write a data on the touchscreen controller                  */
/******************************************************************************/
void TscWrite(unsigned char ucReg, unsigned char ucNum,
                          unsigned long ulVal);

/******************************************************************************/
/* FUNCTION     : TscRead                                                     */
/* INPUT        : ucReg is internal register address                          */
/*                ucNum is the number of bytes to read                        */
/*                pucBuffer is th pointer to data will be placed              */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Reads data from the touchscreen controller                  */
/******************************************************************************/
void TscRead(unsigned char ucReg, unsigned char ucNum,
                         unsigned char* pucBuffer);                            

/******************************************************************************/
/* FUNCTION     : TscUpdate                                                   */
/* INPUT        : -                                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : After an interrupt, update the current tsc values           */
/******************************************************************************/
void TscUpdate(void);

/******************************************************************************/
/* FUNCTION     : TSC_ButtonExecute                                           */
/* INPUT        : button descriptor                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Update the button style and execute functions if any        */
/******************************************************************************/
void TSC_ButtonExecute(btn_t * button, uint8_t index);

/******************************************************************************/
/* FUNCTION     : TSC_SliderExecute                                           */
/* INPUT        : slider descriptor                                           */
/* OUTPUT       : return the current slider value                             */
/* COMMENTS     : Update the slider and execute functions if any              */
/******************************************************************************/
void TSC_SliderExecute(sld_t * slider);

#endif //__STMPE811_H__