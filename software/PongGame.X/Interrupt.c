#include <xc.h>

#include "xf.h"


void interrupt high_isr(void) {

    // End of frame
    if ((TMR2IE == 1)&&(TMR2IF == 1)) {
        LATD4 = !LATD4;
        PR2= 255;
        
        TMR2IF = 0; //clear interrupt flag  
    }
    else if(INT0IF == 1){
        LATD0 = !LATD0;
        INT0IF = 0;
    }
    
    if (TMR0IE == 1 && TMR0IF == 1)
    {
        //clear flag
        TMR0IF = 0;
        
        XF_decrementAndQueueTimers();
    }
}

