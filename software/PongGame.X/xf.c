/******************************************************************************/
/* FILENAME	: xf.h                                                            */
/*----------------------------------------------------------------------------*/
/* GOAL		  : Offers the femto XF functions (for PIC CPU)                     */
/*----------------------------------------------------------------------------*/
/* AUTHOR   : Medard Rieder / Pascal Sartoretti                               */
/*----------------------------------------------------------------------------*/
/* DATE:    : original (Medard Rieder 08.2011)                                */
/*            corrections & simplified (Pascal Sartoretti 06.2016)            */
/******************************************************************************/
#include <xc.h>                   // XC8 General Include File
#include <stdbool.h>              // boolean types
#include "xf.h"

XF theXF;                         // really the XF

static void ENTERCRITICAL(bool inISR);
static void LEAVECRITICAL(bool inISR);
/******************************************************************************/
/* FUNCTION     : Init the XF structure                                       */
/* INPUT        : -                                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : Have to be called once                                      */
/******************************************************************************/
void XF_init()
{
    int i;
    for (i=0; i<MAXEVENT; i++)
    {
        theXF.eventQueue[i] = NULLEVENT;
    }

    for (i=0; i<MAXTIMER; i++)
    {
        theXF.timerList[i].tm = NULLTIMER;
        theXF.timerList[i].ev = NULLEVENT;
    }
    theXF.in = 0;
    theXF.out = 0;
}

/******************************************************************************/
/* FUNCTION     : Push an event on the events queue                           */
/* INPUT        : ev - the event number (not 0)                               */
/*                inISR - (true if called in an ISR, else false)              */
/* OUTPUT       : return false if the queue was full, else true               */
/* COMMENTS     : -                                                           */
/******************************************************************************/
bool XF_pushEvent(Event ev, bool inISR)
{
  uint8_t temp;
    ENTERCRITICAL(inISR);
    
    temp = (theXF.in+1) % (uint8_t)sizeof(theXF.eventQueue);
    if(temp == theXF.out)
    {      
      LEAVECRITICAL(inISR);
      return false;
    }
    theXF.eventQueue[theXF.in] = ev;
    theXF.in = temp;
    LEAVECRITICAL(inISR);
    return true;
}

/******************************************************************************/
/* FUNCTION     : Pop an event on the events queue                            */
/* INPUT        : inISR - (true if called in an ISR, else false)              */
/* OUTPUT       : return the next waiting event if any, else 0                */
/* COMMENTS     : -                                                           */
/******************************************************************************/
Event XF_popEvent(bool inISR)
{
    Event ev;
    ev = NULLEVENT;
    ENTERCRITICAL(inISR);
    if(theXF.in == theXF.out)
    {
      LEAVECRITICAL(inISR);
      return ev;
    }
    ev = theXF.eventQueue[theXF.out];
    theXF.out = (theXF.out + 1)%(uint8_t)sizeof(theXF.eventQueue);
    LEAVECRITICAL(inISR);
    return ev;    
}

/******************************************************************************/
/* FUNCTION     : Post a timer in timers queue                                */
/* INPUT        : tm - time before event arrives                              */
/*                ev - event to post                                          */
/*                inISR - (true if called in an ISR, else false)              */
/* OUTPUT       : return the timer Id used                                    */
/* COMMENTS     : -                                                           */
/******************************************************************************/
TimerID XF_scheduleTimer(Time tm, Event ev, bool inISR)
{
    uint8_t i;

    ENTERCRITICAL(inISR);
    for (i=0; i<MAXTIMER; i++)
    {
        if (theXF.timerList[i].ev == NULLEVENT)
        {
            theXF.timerList[i].tm = tm;
            theXF.timerList[i].ev = ev;
            break;
        }
    }

    //here you could react
    //if timerlist is full

    LEAVECRITICAL(inISR);
    return i;
}

/******************************************************************************/
/* FUNCTION     : Remove a timer in timers queue                              */
/* INPUT        : id - the timer id to remove                                 */
/*                inISR - (true if called in an ISR, else false)              */
/* OUTPUT       : -                                                           */
/* COMMENTS     : -                                                           */
/******************************************************************************/
void XF_unscheduleTimer(TimerID id, bool inISR)
{
    ENTERCRITICAL(inISR);
    theXF.timerList[id].tm = NULLTIMER;
    theXF.timerList[id].ev = NULLEVENT;
    LEAVECRITICAL(inISR);
}

/******************************************************************************/
/* FUNCTION     : Decrement timers to post events if time elapsed             */
/* INPUT        : -                                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : This function has to be called from the timer ISR           */
/******************************************************************************/
void XF_decrementAndQueueTimers()
{
    int i;
    for (i=0; i<MAXTIMER; i++)
    {
        if (theXF.timerList[i].ev != NULLEVENT)
        {
            theXF.timerList[i].tm-=1;
            if (theXF.timerList[i].tm ==0)
            {
                if(XF_pushEvent(theXF.timerList[i].ev, true) == true)
                {  
                  XF_unscheduleTimer(i, true);
                }
                else
                {
                  theXF.timerList[i].tm=1;
                }
            }
        }
    }

    //here you could use done to react
    //if timerID was not found (done == 0)
}

/******************************************************************************/
/* FUNCTION     : Lock interrupts if not in ISR                               */
/* INPUT        : -                                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : -                                                           */
/******************************************************************************/
static void ENTERCRITICAL(bool inISR)
{
    if (inISR == false)
    {
      GIE = 0;
    }
}

/******************************************************************************/
/* FUNCTION     : Unlock interrupts if not in ISR                             */
/* INPUT        : -                                                           */
/* OUTPUT       : -                                                           */
/* COMMENTS     : -                                                           */
/******************************************************************************/
static void LEAVECRITICAL(bool inISR)
{
    if (inISR == false)
    {
      GIE = 1;
    }
}