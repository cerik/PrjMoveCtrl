
#include "TSIDrv.h"

/*
 *  Channel distribution:
 *            CH8      CH9
 *
 *                CH10
 *
 *            CH11     CH12
 */
 
//define the channel convert sequence by array index.
static uint8  lgCvtSequence[16]={
                TSICH9, TSICH8, TSICH0, TSICH1, TSICH2, TSICH3, TSICH4, TSICH5,
                TSICH6, TSICH7, TSICH10,TSICH11,TSICH12,TSICH13,TSICH14,TSICH15};
//define the threshold for each channel.
static uint16 lgu16Threshold[16]={
                THRESHOLD0, THRESHOLD1, THRESHOLD2, THRESHOLD3,THRESHOLD4, THRESHOLD5,
                THRESHOLD6, THRESHOLD7, THRESHOLD8, THRESHOLD9,THRESHOLD10,THRESHOLD11,
                THRESHOLD12,THRESHOLD13,THRESHOLD14,THRESHOLD15};
static uint16 lgu16DeltaVal[16];
static uint16 lgu16Baseline[16];//stored the min value without any input.

//flag to identification wheather convert is finished.
static uint8  lgCvtEndFlag = TRUE;

/*************************************
 * For calculate the distance between two side of that slide.
 */
static uint8 SliderPercentegePosition[TOTAL_TSICHANNEL] = {NO_TOUCH,NO_TOUCH};
static uint8 SliderDistancePosition[TOTAL_TSICHANNEL]   = {NO_TOUCH,NO_TOUCH};
static uint8 AbsolutePercentegePosition  = NO_TOUCH;
static uint8 AbsoluteDistancePosition    = NO_TOUCH;
uint8 GetAbsPosPrecent(void)
{
    return AbsolutePercentegePosition;
}
uint8 GetAbsDistance(void)
{
    return AbsoluteDistancePosition;
}

/*******************************************************************
 * Self calibration funtion.
 *  Stored the default scaned value into the array lgu16Baseline[].
 *  Later, the converted value will be subed by this default value 
 *  for each channel to get a real input value.
 */
static void TSI_SelfCalibration(void)
{
    uint8 cnt;
    uint8 trigger_backup;

    TSI0_GENCS |= TSI_GENCS_EOSF_MASK;      // Clear End of Scan Flag 
    TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;    // Disable TSI module

    // Back-up TSI Trigger mode from Application 
    trigger_backup = (TSI0_GENCS & TSI_GENCS_STM_MASK)?TRUE:FALSE;

    TSI0_GENCS &= ~TSI_GENCS_STM_MASK;       // Use SW trigger
    TSI0_GENCS &= ~TSI_GENCS_TSIIEN_MASK;    // Disable TSI interrupts
    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;      // Enable TSI module
    
    for(cnt=0; cnt < TOTAL_TSICHANNEL; cnt++) // Get Counts when Electrode not pressed
    {
        //Set convert channel
        TSI0_DATA  = lgCvtSequence[cnt] << TSI_DATA_TSICH_SHIFT;
        TSI0_DATA |= TSI_DATA_SWTS_MASK;
        //Wait the convert until finished.
        while(!(TSI0_GENCS & TSI_GENCS_EOSF_MASK));
        TSI0_GENCS |= TSI_GENCS_EOSF_MASK;//Clear the flag of end-off-scan.
        lgu16Baseline[cnt] = TSI0_DATA & TSI_DATA_TSICNT_MASK; //store the converted data.
    }
    TSI0_GENCS &= ~TSI_GENCS_TSIEN_MASK;    // Disable TSI module
    TSI0_GENCS |= TSI_GENCS_TSIIEN_MASK;    // Enale TSI interrupt
    if(trigger_backup)                      // Restore trigger mode
        TSI0_GENCS |= TSI_GENCS_STM_MASK;
    else
        TSI0_GENCS &= ~TSI_GENCS_STM_MASK;
    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK;     // Enable TSI module
    TSI0_DATA   = ((lgCvtSequence[0]<<TSI_DATA_TSICH_SHIFT) );//conver from the fist one of the defined.
    TSI0_DATA  |= TSI_DATA_SWTS_MASK;
}

/*******************************************************************
 * TSI module initialization.
 */
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

    TSI0_GENCS |= TSI_GENCS_TSIEN_MASK; //Enable TSI module.
    TSI_SelfCalibration();
}

/*******************************************************************
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
        if((lgu16DeltaVal[0] > lgu16Threshold[0])||(lgu16DeltaVal[1] > lgu16Threshold[1]))
        {
            SliderPercentegePosition[0] = (lgu16DeltaVal[0]*100)/(lgu16DeltaVal[0]+lgu16DeltaVal[1]);
            SliderPercentegePosition[1] = (lgu16DeltaVal[1]*100)/(lgu16DeltaVal[0]+lgu16DeltaVal[1]);
            SliderDistancePosition[0]   = (SliderPercentegePosition[0]* SLIDER_LENGTH)/100;
            SliderDistancePosition[1]   = (SliderPercentegePosition[1]* SLIDER_LENGTH)/100;
            AbsolutePercentegePosition  = ((100 - SliderPercentegePosition[0]) + SliderPercentegePosition[1])/2;
            AbsoluteDistancePosition    = ((SLIDER_LENGTH - SliderDistancePosition[0]) + SliderDistancePosition[1])/2;
        }else
        {
            SliderPercentegePosition[0] = NO_TOUCH;
            SliderPercentegePosition[1] = NO_TOUCH;
            SliderDistancePosition[0]   = NO_TOUCH;
            SliderDistancePosition[1]   = NO_TOUCH;
            AbsolutePercentegePosition  = NO_TOUCH;
            AbsoluteDistancePosition    = NO_TOUCH;
        }
        //printf("Percentage = %3d %%,Distance = %3d mm\r\n",AbsolutePercentegePosition,AbsoluteDistancePosition);
        printf("CH[0,1]: %d , %d\r\n",lgu16DeltaVal[0],lgu16DeltaVal[1]);
    }
}

/*******************************************************************
 * Get the user action.
 * User action is identified within 1s.
 *
 */
USER_ACTION_T GetUserAction(void)
{
    return UA_NONE;
}

/*******************************************************************
 * TSI Interrupt Handler.
 *
 */
void TSI0_IRQHandler(void)
{
    static uint8 tsiChIndex=0;
    uint16 u16TsiChValue;
    int16  i16TempDelta;

    TSI0_GENCS |= TSI_GENCS_EOSF_MASK; // Clear End of Scan Flag
    lgCvtEndFlag = TRUE;
    
    u16TsiChValue = (TSI0_DATA & TSI_DATA_TSICNT_MASK);        // Save Counts for current scaned value.
    i16TempDelta  = u16TsiChValue - lgu16Baseline[tsiChIndex]; // Obtains Counts Delta from callibration reference

    if( i16TempDelta < 0)
        lgu16DeltaVal[tsiChIndex] = 0;
    else
        lgu16DeltaVal[tsiChIndex] = i16TempDelta;

    if(TOTAL_TSICHANNEL > 1)  //Change Electrode to Scan
    {
        tsiChIndex = (tsiChIndex+1) % TOTAL_TSICHANNEL;
        
        //Set the next channel which will be scaned.
        TSI0_DATA  = ((lgCvtSequence[tsiChIndex]<<TSI_DATA_TSICH_SHIFT) );
        TSI0_DATA |= TSI_DATA_SWTS_MASK; //start a scan.
    } 
}
