
#ifndef _TSI_DRV_H_
#define _TSI_DRV_H_

#include "common.h"
#ifndef KEIL
#include "vectors.h"
#endif

#define NO_TOUCH               0

#define SLIDER_LENGTH          40 //LENGTH in mm

#define TOTAL_TSICHANNEL       2

#define TSICH0         0
#define TSICH1         1
#define TSICH2         2
#define TSICH3         3
#define TSICH4         4
#define TSICH5         5
#define TSICH6         6
#define TSICH7         7
#define TSICH8         8
#define TSICH9         9
#define TSICH10        10
#define TSICH11        11
#define TSICH12        12
#define TSICH13        13
#define TSICH14        14
#define TSICH15        15

#define THRESHOLD0    100
#define THRESHOLD1    100
#define THRESHOLD2    100
#define THRESHOLD3    100
#define THRESHOLD4    100
#define THRESHOLD5    100
#define THRESHOLD6    100
#define THRESHOLD7    100
#define THRESHOLD8    100
#define THRESHOLD9    100
#define THRESHOLD10   100
#define THRESHOLD11   100
#define THRESHOLD12   100
#define THRESHOLD13   100
#define THRESHOLD14   100
#define THRESHOLD15   100

/*
 * Module related data defination.
 */
typedef enum _tagUserAction{
    UA_NONE=0,
    UA_RELEASE,
    UA_SPRESS,          //Single press
    UA_DPRESS,          //Double press
    UA_LEFTSLIP,        //Left slipping
    UA_RIGHTSLIP,       //Right slipping
    UA_CLOCKWISE,       //clockwise
    UA_ANTICLOCKWISE    //anti-clockwise
}USER_ACTION_T;


typedef struct _tagTsiCtrlBlk{
    uint16 ChDat[TOTAL_TSICHANNEL];
    //uint8  NewActionFlag;    //Read clear.to flag whether these have new action.
    USER_ACTION_T NewAction; //Set to UA_NONE after read.
    USER_ACTION_T LastAction;//Record the last user action.
}TSI_CTRLBLK_T;

/*
 * Exported function.
 */
void  TSI_Init(void);
void  TSI_SliderRead(void);
uint8 GetAbsPosPrecent(void);

void  TSI0_IRQHandler(void);
#endif

