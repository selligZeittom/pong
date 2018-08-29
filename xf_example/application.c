#include <stdio.h>
#include "application.h"

extern void setLed(unsigned char thisLed, bool status);

void App_init(App* me)
{
    me->sm1 = ST_SM1INIT;
    me->sm2 = ST_SM2INIT;

    me->oldSm1 = me->sm1;
    me->oldSm2 = me->sm2;
    me->timerID = NULLTIMER;

    XF_pushEvent(evInit, false);
}

void App_SM1(App* me, Event ev)
{
    /*

      This is the State machine 1

                     O ST_SM1INIT
                     |
                     | /ontransition: printf("START")
                     |
                     v
         +-----------------------+
         |  ST_SM1OFF            |<---+
         |  LED1 on LED2 off     |    |
         +-----------------------+    |
                     | tm(1000        | tm(2000)
                     v                |
         +-----------------------+    |
         |  ST_SM1ON             |----+
         |  LED1 off LED2 on     |
         +-----------------------+

    */

    //this is the sm1 control switch
    me->oldSm1 = me->sm1;
    switch (me->sm1)
    {
        case ST_SM1INIT:
            if (ev == evInit)
            {
                me->sm1 = ST_SM1OFF;
            }
            break;
        case ST_SM1OFF:
            if (ev == evTM1000)
            {
                me->sm1 = ST_SM1ON;
            }
            break;
        case ST_SM1ON:
            if (ev == evTM2000)
            {
                me->sm1 = ST_SM1OFF;
            }
            break;
    }

    //this is the sm1 action  switch
    switch (me->sm1)
    {
        case ST_SM1OFF:
            if (me->oldSm1 == ST_SM1INIT)
            {
                printf("SM1 started :-)\n\n");
            }
            if (me->oldSm1 != ST_SM1OFF)
            {
                me->timerID = XF_scheduleTimer(TM1000, evTM1000, false);
                printf("SM1 OFF\n" );
                setLed(0,true);
                setLed(1,false);
            }
            break;
        case ST_SM1ON:
            if (me->oldSm1 == ST_SM1OFF)
            {
                me->timerID = XF_scheduleTimer(TM2000, evTM2000, false);
                printf("SM1 ON\n");
                setLed(0,false);
                setLed(1,true);
            }
            break;
    }
}

void App_SM2(App* me, Event ev)
{
    /*
      This is the State machine 2

                 O ST_SM2INIT
                 |
                 | /ontransition: printf("START")
                 |
                 v
         +--------------+
         |  ST_SM2WAIT  |<-----------------------------------------+
         |              |<--------------------------------------+  |
         +--------------+             evDefault                 |  |
                 | evBDown                                      |  |
                 v                                              |  |
         +--------------+          +-----------------------+    |  |
         |  ST_SM2BDOWN |  evBUp   | ST_SM2SHORT           |    |  |
         |              |--------->| /entry: emit evBShort |----+  |
         +--------------+          +-----------------------+       |
                 | tm(long)                                        |
                 v                                                 |
         +-------------------------+                               |
         | ST_SM2BLONG             |            evDefault          |
         | /entry: emit evBLong    |-------------------------------+
         +-------------------------+
    */

    me->oldSm2 = me->sm2;

    //this is the sm2 control switch
    switch (me->sm2)
    {
        case ST_SM2INIT:
            if (ev == evInit)
            {
                me->sm2 = ST_SM2WAIT;
            }
            break;
        case ST_SM2WAIT:
            if (ev == evBDown)
            {
                me->sm2 = ST_SM2BDOWN;
            }
            break;
        case ST_SM2BDOWN:
            if (ev == evTMLONG)
            {
                me->sm2 = ST_SM2LONG;
            }
            if (ev == evBUp)
            {
                me->sm2 = ST_SM2SHORT;
            }
            break;
        case ST_SM2LONG:
            if (ev == evDefault)
            {
                me->sm2 = ST_SM2WAIT;
            }
            break;
        case ST_SM2SHORT:
            if (ev == evDefault)
            {
                me->sm2 = ST_SM2WAIT;
            }
            break;
    }

    //this is the sm2 action switch

    switch (me->sm2)
    {
        case ST_SM2WAIT:
            if (me->oldSm2 == ST_SM2INIT)
            {
            }
            if (me->oldSm2 == ST_SM2LONG)
            {
            }
            if (me->oldSm2 == ST_SM2SHORT)
            {
            }
            break;
        case ST_SM2BDOWN:
            if (me->oldSm2 == ST_SM2WAIT)
            {
                setLed(2,false);
                setLed(3,false);
                me->timerID = XF_scheduleTimer(TMLONG, evTMLONG, false);
            }
            break;
        case ST_SM2LONG:
            if (me->oldSm2 == ST_SM2BDOWN)
            {
                XF_pushEvent(evBLong,false);
                XF_pushEvent(evDefault,false);
                setLed(3,true);
                printf("SM2 LONGPRESS\n");
            }
            break;
        case ST_SM2SHORT:
            if (me->oldSm2 == ST_SM2BDOWN)
            {
                XF_pushEvent(evBShort,false);
                XF_unscheduleTimer(me->timerID, false);
                XF_pushEvent(evDefault,false);
                setLed(2,true);
                printf("SM2 SHORTPRESS\n");
            }
            break;
    }

}
