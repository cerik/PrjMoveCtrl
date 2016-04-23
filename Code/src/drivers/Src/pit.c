/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file pit.c
*
* @author B37531
*
* @version 0.0.1
*
* @date Mar 5, 2012
*
* @brief application entry point which performs application specific tasks. 
*
*******************************************************************************
*
* provide TSI driver,complete electrode scan and processing,support stop mode
******************************************************************************/
#include "common.h"
#include "pit.h"
#include "sysinit.h"

/******************************************************************************
* Constants and macros
******************************************************************************/

#define MAX32 0xFFFFFFFF

/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/
static uint32 m_MsTickCount=0;

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: PIT_Init
*
* @brief 
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/
void InitPit(uint8 cChannel,uint32 MsPeriod,uint8 IntEn)
{
    uint32 val;
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT_MCR = PIT_MCR_FRZ_MASK; // MDIS = 0  enables timer
    if(MsPeriod >= MAX32)
    {
        val = MAX32;
    }
    else
    {
        val = (GetMcgClk()/1000)*MsPeriod/2;
    }
    PIT_LDVAL_REG(PIT_BASE_PTR,cChannel) = val;
    if(IntEn == TRUE)
    {
        PIT_TCTRL_REG(PIT_BASE_PTR,cChannel) = 
                            PIT_TCTRL_TEN_MASK | // enable PIT
                            PIT_TCTRL_TIE_MASK;  // enable interrupt
        enable_irq(INT_PIT-16);
    }
    else
    {
        PIT_TCTRL_REG(PIT_BASE_PTR,cChannel) = PIT_TCTRL_TEN_MASK; // enable PIT
    }
}

/*****************************************************************************//*!
+FUNCTION----------------------------------------------------------------
* @function name: PIT_Isr
*
* @brief 
*        
* @param  none 
*
* @return none
*
* @ Pass/ Fail criteria: none
*****************************************************************************/

void PIT_IRQHandler( void )
{
    if( PIT_TFLG0 )
    {
        PIT_TFLG0 = 0x01;
        m_MsTickCount++;
    }
    if( PIT_TFLG1 )
    {
        PIT_TFLG1 = 0x01;
    }
}

void WaitMs(int32 ms)
{
    int32 t0,t1;
    t0 = m_MsTickCount;
    while(ms>0)
    {
        t1 = m_MsTickCount;
        ms -= (t1<t0)?(t1+MAX32-t0):(t1-t0);
        t0 = t1;
    }
}

void WiatUs(int32 us)
{
    uint32 t0,tx,val;
    t0 = PIT_CVAL_REG(PIT_BASE_PTR,1);
    while(us > 0)
    {
        tx = PIT_CVAL_REG(PIT_BASE_PTR,1);
        if( t0 >= tx)
        {
            val = (t0 - tx)/(GetMcgClk()/2000000);
            us -= val;
            t0 = tx;
        }
        else
        {
            val = (MAX32 - tx+t0)/(GetMcgClk()/2000000);
            us -= val;
            t0 = tx;
        }
    }
}

void StartCounter(tagCounter *counter, int32 us)
{
    counter->t0 = PIT_CVAL_REG(PIT_BASE_PTR,1);
    counter->us = us/2;
}

int8 CounterArrived(tagCounter *counter)
{
    int32 tmp;
    if(counter->us > 0)
    {
        counter->tx = PIT_CVAL_REG(PIT_BASE_PTR,1);
        if(counter->t0 >= counter->tx)
        {
            tmp= (counter->t0 - counter->tx)/(GetMcgClk()/1000000);
        }
        else
        {
            tmp= (counter->t0 + MAX32 - counter->tx)/(GetMcgClk()/1000000);
        }
        counter->us -= tmp;
        counter->t0 = counter->tx;
    }
    return (counter->us > 0)?FALSE:TRUE;
}
