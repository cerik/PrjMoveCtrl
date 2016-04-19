/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2011-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file pit.h
*
* @author B37531
*
* @version 0.0.1
*
* @date Aug 23, 2011
*
* @brief application entry point which performs application specific tasks. 
*
*******************************************************************************
*
*
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

/******************************************************************************
* Global variables
******************************************************************************/

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


#ifndef PIT_H_
#define PIT_H_

void ISR_PIT( void );
void InitPit(uint8 cChannel,uint32 MsPeriod,uint8 IntEn);
void WaitMs(int32 ms);
void WiatUs(int32 us);

typedef struct{
    int32 us;
    int32 t0,tx;
}tagCounter;

void StartCounter(tagCounter *counter, int32 us);
int8 CounterArrived(tagCounter *counter);

#endif /* PIT_H_ */
