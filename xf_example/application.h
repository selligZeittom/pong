#ifndef APPDEF
#define APPDEF

#include "defines.h"
#include "xf.h"

#ifdef __cplusplus
extern "C"
{
#endif


typedef enum SM1
{
    ST_SM1INIT,
    ST_SM1ON,
    ST_SM1OFF
} SM1;

typedef enum SM2
{
    ST_SM2INIT,
    ST_SM2WAIT,
    ST_SM2BDOWN,
    ST_SM2SHORT,
    ST_SM2LONG
} SM2;


typedef struct App
{
    SM1 sm1, oldSm1;
    SM2 sm2, oldSm2;
    TimerID timerID;
} App;


void App_init(App* me);
void App_SM1(App* me, Event ev);
void App_SM2(App* me, Event ev);

#ifdef __cplusplus
}
#endif


#endif
