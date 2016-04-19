/*
 * File:        fredom_test.c
 * Purpose:     Main process
 *
 */

#include "common.h"
#include "gpio.h"
#include "pit.h"
#include "global.h"
#include "TSIDrv.h"
#include "global.h"
#include "sysinit.h"
#include "mcg.h"


#define TPM_Cn_MODE         (TPM_CnSC_MSB_MASK | TPM_CnSC_ELSA_MASK)
#define TPM_MODULE          1000
#define TPM_INIT_VAL        25
#define SET_LED_GREEN(x)    TPM0_C2V = (x)
#define SET_LED_RED(x)      TPM0_C3V = (x)
#define SET_LED_BLUE(x)     TPM0_C1V = (x)

void accel_demo(void);
void accel_read(void);
void TPM_init(void);

/*****************************************************************************************************
* Definition of module wide VARIABLEs - NOT for use in other modules
*****************************************************************************************************/
signed short accel_x, accel_y, accel_z;
signed short resultx, resulty, resultz;

#define RED_MASK   0x01
#define GREEN_MASK 0x02
#define BLUE_MASK  0x04

void TPM_init(void)
{ 
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK;
    SIM_SCGC6|=( SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK);   
    SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1);

    PORTB_PCR8 = (0|PORT_PCR_MUX(2));   /* TPM0_CH3 enable on PTB18 */
    PORTB_PCR9 = (0|PORT_PCR_MUX(2));   /* TPM0_CH2 enable on PTB19 */
    PORTB_PCR10  = (0|PORT_PCR_MUX(2)); /* TPM0_CH1 enable on PTD1 */

    TPM0_MOD  = TPM_MODULE;    /* 0x0063 / 25MHz = 4uS PWM period */
    TPM0_C1SC = TPM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
    TPM0_C2SC = TPM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
    TPM0_C3SC = TPM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
    TPM0_C1V  = TPM_INIT_VAL;  /* 90% pulse width */
    TPM0_C2V  = TPM_INIT_VAL;  /* 90% pulse width */
    TPM0_C3V  = TPM_INIT_VAL;  /* 90% pulse width */
    TPM0_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
}

/********************************************************************/
int main (void)
{
    uint32 i=0;
    tagCounter counter;
    InitSystem();
    InitGpio();
    InitPit(0,1,1);
    InitPit(1,0xFFFFFFFF,0);
    //TSI_Init();
    //TPM_init();
    printf("\nRunning the fredom_test project.\n");
    StartCounter(&counter,1000000);
    while(1)
    {
        WiatUs(1000000);
        //if( CounterArrived(&counter) == TRUE)
        {
            StartCounter(&counter,1000000);
            printf("i:%d\n",i++);
        }
        //TSI_SliderRead();
        //if (GetAbsPosPrecent()>0 ) 
        //{ 
        //    SET_LED_BLUE(GetAbsPosPrecent()*10);
        //    SET_LED_RED(GetAbsPosPrecent()*10);
        //    SET_LED_GREEN(GetAbsPosPrecent()*10);
        //}
    } 
}
/********************************************************************/

