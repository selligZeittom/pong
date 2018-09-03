/*
 * File:   main.c
 * Author: yan.michello
 *
 * Created on 31. août 2018, 12:27
 */


#include <xc.h>
#include "spi.h"
#include "tsc.h"
#include "lcd_highlevel.h"
#include "lcd_lowlevel.h"

void enable_high_speed() {
    PLLEN = 1; // activate PLL x4
    OSCCON = 0b01110000; // for 64MHz cpu clock (default is 8MHz)
    // Caution -> the PLL needs up to 2 [ms] to start !
    // Wait a moment if needed with: __delay_ms(2);
}

void createPlayButton(void){
    btn_t rightButton;
    btn_t leftButton;
    
    uint8_t l = 0x4C;
    
    LCD_ButtonCreate(0,0,50,50,BLACK,WHITE,&l,0,0,0,0,&leftButton,0);
    LCD_ButtonDraw(&leftButton);
 
}

void initPhysicButton(void){
    INT0IE = 1; // enable external interrupt
    INTEDG0 = 0; // interrupt on falling edge
    GIE = 1;
    PEIE = 1;
    
}

void initTimer2(void){
    PR2 = 255;
    TMR2 = 0;
    TMR2IF = 0;
    GIE = 1;
    PEIE = 1;
    TMR2IE = 1;
    TMR2ON = 1;
}

void initTimer0_XF(void)
{
    T08BIT = 0; //timer0 as 16 bits
    PSA = 0; //prescaler nEnalble
    //T0PS = 1; //prescaler value = 4
    
    GIE = 1;
    PEIE = 1;
    TMR0IE = 1;
    TMR0IF = 1;
}

void main(void) {
    IRCF0 = 1;
    IRCF1 = 1;
    IRCF2 = 1;
    enable_high_speed();
    SPIInit();
    TscInit();
    LCD_Init();
    TRISD0 = 0;
    LATD0 = 1;
    TRISC2 = 0;
    LATC2 = 1;
  //  LCD_Fill(WHITE);
    
    //test
    TRISD4 = 0;
    LATD4 = 1;
    
    initTimer2();
    initPhysicButton();
    createPlayButton();
    while(1){
        
      
        
    }
 
    return;
}
