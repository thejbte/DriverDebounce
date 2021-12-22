/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "debounce.h"
#include <stdlib.h>
/*
 =================================================================================================
              	  	  	  	 	##### Initialization functions #####
 =================================================================================================
    */
/* if mode is DEBOUNCE_COUNTERTICK then , debounceUpdate must be in function Systick else  if mode is DEBOUNCE_WAIT, debounceUpdate set in anywhere */
void debounceInit(debounceData_t *ptrData, uint32_t debounceTick, debounceMode_t mode, delayFcn_t delay, debounceState_t pull_x)
{
    ptrData->delay = debounceTick <= 0 ? DEBOUNCE_DELAY : debounceTick; /*DELAY_DEBOUNCE;*/
    ptrData->previousState_ = (pull_x == DEBOUNCE_PULL_UP) ? DEBOUNCE_HIGH : DEBOUNCE_LOW;
    ptrData->previousState_ = (pull_x == DEBOUNCE_PULL_DOWN) ? DEBOUNCE_LOW : DEBOUNCE_HIGH;
    ptrData->flagFalling = 0;
    ptrData->flagRising = 0;
    ptrData->mode = mode;
    ptrData->delayFcn = delay;
}

void debounceUpdate(debounceData_t *ptrData, uint8_t pinState)
{
    int flagReadyTimeout = 0;
    if ((ptrData->previousState_ && !pinState) || (!ptrData->previousState_ && pinState))
    {
        if(DEBOUNCE_COUNTERTICK == ptrData->mode  ){

            if (ptrData->timeCounter++ >= (ptrData->delay - 1))
                flagReadyTimeout = 1;
        }else if(DEBOUNCE_WAIT == ptrData->mode  ){
            if( NULL != ptrData->delayFcn )
                ptrData->delayFcn(ptrData->delay); /*blocking*/
            flagReadyTimeout = 1;
        }

        if(1 == flagReadyTimeout){
            if (ptrData->previousState_ && !pinState)
            {
                //ptrData->status = DEBOUNCE_FALLING;
                ptrData->flagFalling = 1;
                //ptrData->flagRising = 0;
            }
            if (!ptrData->previousState_ && pinState)
            {
                //ptrData->status = DEBOUNCE_RISING;
                ptrData->flagRising = 1;
                // ptrData->flagFalling = 0;
            }
            ptrData->previousState_ = pinState;
            ptrData->timeCounter = 0;
            flagReadyTimeout = 0;
        }
        ptrData->status = pinState; //update status
    }
}

uint8_t debounceIsFallingEdgeCf(debounceData_t *ptrData)
{
    uint8_t edge = 0;
    if (ptrData->flagFalling)
    {
        ptrData->flagFalling = 0;
        edge = 1;
    }
    return edge;
}

uint8_t debounceIsFallingEdge(debounceData_t *ptrData)
{
    uint8_t edge = 0;
    if (ptrData->flagFalling)
    {
        edge = 1;
    }
    return edge;
}

uint8_t debounceIsRisingEdgeCf(debounceData_t *ptrData)
{
    uint8_t edge = 0;
    if (ptrData->flagRising)
    {
        ptrData->flagRising = 0;
        edge = 1;
    }
    return edge;
}

uint8_t debounceIsRisingEdge(debounceData_t *ptrData)
{
    uint8_t edge = 0;
    if (ptrData->flagRising)
    {
        edge = 1;
    }
    return edge;
}

uint8_t debounceGetStatus(debounceData_t *ptrData)
{
    return ptrData->status;
}

void debounceSetDelay(debounceData_t *ptrData, uint32_t debounceTick)
{
    ptrData->delay = debounceTick;
}

void debounceSetPull(debounceData_t *ptrData, debounceState_t pull_x)
{
    if (pull_x == DEBOUNCE_PULL_UP)
    {
        ptrData->previousState_ = DEBOUNCE_HIGH;
    }
    else if (pull_x == DEBOUNCE_PULL_DOWN)
    {
        ptrData->previousState_ = DEBOUNCE_LOW;
    }
}

void debounceClearFlag(debounceData_t *ptrData, debounceState_t flag)
{
    if (flag == DEBOUNCE_FALLING)
    {
        ptrData->flagFalling = 0;
    }
    else if (flag == DEBOUNCE_RISING)
    {
        ptrData->flagRising = 0;
    }
}