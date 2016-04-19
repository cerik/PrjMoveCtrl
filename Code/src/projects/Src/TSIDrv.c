
#include "TSIDrv.h"

static uint8 elec_array[16]={
               ELECTRODE0, ELECTRODE1, ELECTRODE2, ELECTRODE3,ELECTRODE4, ELECTRODE5,
		      ELECTRODE6,ELECTRODE7,ELECTRODE8,ELECTRODE9,ELECTRODE10,ELECTRODE11,
		      ELECTRODE12,ELECTRODE13,ELECTRODE14,ELECTRODE15};

static uint16 gu16Baseline[16];
static uint16 gu16Threshold[16]={
               THRESHOLD0, THRESHOLD1, THRESHOLD2, THRESHOLD3,THRESHOLD4, THRESHOLD5,
                          THRESHOLD6,THRESHOLD7,THRESHOLD8,THRESHOLD9,THRESHOLD10,THRESHOLD11,
                          THRESHOLD12,THRESHOLD13,THRESHOLD14,THRESHOLD15};
static uint16 gu16Delta[16];

static uint8 lgCvtEndFlag = TRUE;

static uint8 SliderPercentegePosition[2] = {NO_TOUCH,NO_TOUCH};
static uint8 SliderDistancePosition[2]   = {NO_TOUCH,NO_TOUCH};

static uint8 AbsolutePercentegePosition = NO_TOUCH;
static uint8 AbsoluteDistancePosition   = NO_TOUCH;

uint8 GetAbsPosPrecent(void)
{
    return AbsolutePercentegePosition;
}
static void TSI_SelfCalibration(void)
{
    uint8 cnt;
    uint8 trigger_backup;

    TSI0_GENCS |= TSI_GENCS_EOSF_MASK;      // Clear End of Scan Flag 
    TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;    // Disable TSI module

    if(TSI0_GENCS & TSI_GENCS_STM_MASK)     // Back-up TSI Trigger mode from Application 
        trigger_backup = TRUE;
    else
        trigger_backup = FALSE;

    TSI0_GENCS &= ~TSI_GENCS_STM_MASK;       // Use SW trigger
    TSI0_GENCS &= ~TSI_GENCS_TSIIEN_MASK;    // Enable TSI interrupts
    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;      // Enable TSI module
    for(cnt=0; cnt < TOTAL_ELECTRODE; cnt++) // Get Counts when Electrode not pressed
    {
        TSI0_DATA  = elec_array[cnt] << TSI_DATA_TSICH_SHIFT;
        TSI0_DATA |= TSI_DATA_SWTS_MASK;
        while(!(TSI0_GENCS & TSI_GENCS_EOSF_MASK));
        TSI0_GENCS |= TSI_GENCS_EOSF_MASK;
        gu16Baseline[cnt] = TSI0_DATA & TSI_DATA_TSICNT_MASK; 
    }
    TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;    // Disable TSI module
    TSI0_GENCS |= TSI_GENCS_TSIIEN_MASK;    // Enale TSI interrupt
    if(trigger_backup)                      // Restore trigger mode
        TSI0_GENCS |= TSI_GENCS_STM_MASK;
    else
        TSI0_GENCS &= ~TSI_GENCS_STM_MASK;
    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;     // Enable TSI module
    TSI0_DATA = ((elec_array[0]<<TSI_DATA_TSICH_SHIFT) );
    TSI0_DATA |= TSI_DATA_SWTS_MASK;
}

void TSI_Init (void)
{
    SIM_SCGC5 |= SIM_SCGC5_TSI_MASK; // Enable clock gating for TSI

    /* Enable proper GPIO as TSI channels */
    PORTA_PCR13 = PORT_PCR_MUX(0);   // PTB16 as TSI channel 9
    PORTB_PCR12 = PORT_PCR_MUX(0);   // PTB17 as TSI channel 8
    enable_irq(INT_TSI0-16);
    TSI0_GENCS |= (TSI_GENCS_ESOR_MASK
                   | TSI_GENCS_MODE(0)
                   | TSI_GENCS_REFCHRG(4)
                   | TSI_GENCS_DVOLT(0)
                   | TSI_GENCS_EXTCHRG(7)  
                   | TSI_GENCS_PS(4)
                   | TSI_GENCS_NSCN(11)
                   | TSI_GENCS_TSIIEN_MASK
                   | TSI_GENCS_STPE_MASK
                   //| TSI_GENCS_STM_MASK     //Trigger for the module 0=Sofware 
                   );

    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK; 
    TSI_SelfCalibration();
}

static void change_electrode(void)
{
    static uint8 ongoing_elec=0;
    uint16 u16TsiCount;
    int16  u16temp_delta;

    u16TsiCount   = (TSI0_DATA & TSI_DATA_TSICNT_MASK);          // Save Counts for current electrode
    u16temp_delta = u16TsiCount - gu16Baseline[ongoing_elec];  // Obtains Counts Delta from callibration reference

    if( u16temp_delta < 0)
        gu16Delta[ongoing_elec] = 0;
    else
        gu16Delta[ongoing_elec] = u16temp_delta;

    if(TOTAL_ELECTRODE > 1)  //Change Electrode to Scan
    {
        ongoing_elec = (ongoing_elec+1) % TOTAL_ELECTRODE;

        TSI0_DATA = ((elec_array[ongoing_elec]<<TSI_DATA_TSICH_SHIFT) );//set the channel will be measured.
        TSI0_DATA |= TSI_DATA_SWTS_MASK; //start a scan.
    } 
}

/*
 * TSI_Sliderread
 *  return  
 *       AbsolutePercentegePosition  0-100  or if not 
 *       AbsoluteDistancePosition
 *
*/
void TSI_SliderRead(void )
{
    if(lgCvtEndFlag)
    {    
        lgCvtEndFlag = FALSE;
        if((gu16Delta[0] > gu16Threshold[0])||(gu16Delta[1] > gu16Threshold[1]))
        {
            SliderPercentegePosition[0] = (gu16Delta[0]*100)/(gu16Delta[0]+gu16Delta[1]);
            SliderPercentegePosition[1] = (gu16Delta[1]*100)/(gu16Delta[0]+gu16Delta[1]);
            SliderDistancePosition[0] = (SliderPercentegePosition[0]* SLIDER_LENGTH)/100;
            SliderDistancePosition[1] = (SliderPercentegePosition[1]* SLIDER_LENGTH)/100;
            AbsolutePercentegePosition = ((100 - SliderPercentegePosition[0]) + SliderPercentegePosition[1])/2;
            AbsoluteDistancePosition = ((SLIDER_LENGTH - SliderDistancePosition[0]) + SliderDistancePosition[1])/2;
        }else
        {
            SliderPercentegePosition[0] = NO_TOUCH;  
            SliderPercentegePosition[1] = NO_TOUCH; 
            SliderDistancePosition[0] = NO_TOUCH; 
            SliderDistancePosition[1] = NO_TOUCH; 
            AbsolutePercentegePosition = NO_TOUCH; 
            AbsoluteDistancePosition = NO_TOUCH; 
        }
        //printf("Percentage = %3d %,Distance = %3d mm\r\n",AbsolutePercentegePosition,AbsoluteDistancePosition);   
    }
}

/* 
 * TSI Interrupt Handler.
 *
 */
void TSI0_IRQHandler(void)
{
    lgCvtEndFlag = TRUE;
    TSI0_GENCS |= TSI_GENCS_EOSF_MASK; // Clear End of Scan Flag
    change_electrode();
}

