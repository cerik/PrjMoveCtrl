/*
 * File:        adc.c
 * Purpose:     Provide common adc routines
 *
 * Notes:       
 *              
 */

#include "common.h"
#include "adc.h"


uint8 ADC_Cal(ADC_MemMapPtr adcmap);
void InitADC( void );
unsigned char TriggerADC(unsigned char Channel);
unsigned int ReadADCPoll(void);
/****************************************************************
                  initial the ADC module
*****************************************************************/
void InitADC( void )
{
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;  // enable ADC0 clock

  	ADC0_CFG1 = ADC_CFG1_ADIV(2)|          // sselect divide is 2
  				ADC_CFG1_MODE(1)|
                ADC_CFG1_ADLSMP_MASK|
  				ADC_CFG1_ADICLK(1);        // select bus/2 as clock source
  	
}
        
          
/****************************************************************
                      software trigger channel x
*****************************************************************/
unsigned char TriggerADC(unsigned char Channel)
{
  
  if(Channel == 0x1B)                                          // Bandgap is selected as input
      PMC_REGSC |= PMC_REGSC_BGBE_MASK;                        // Bandgap buffer enabled
   ADC0_SC1A = Channel;                   // update the channel value
   return 1;
}

/****************************************************************
                      ADC Read
*****************************************************************/
unsigned int ReadADCPoll(void)
{
    while((ADC0_SC1A & ADC_SC1_COCO_MASK) == 0);
    //ADC0_SC1A &= ~ ADC0_SC1A_COCO_MASK;
    return ADC0_RA;
}

void ADC_ISR(void)
{
  printf("\n ADC sample data: %x.\n", ADC0_RA);
}

/******************************************************************************
Function 1. Name	AUTO CAL ROUTINE   

Parameters		ADC module pointer points to adc0.. adc3 register map 
                         base address.
Returns			Zero indicates success.
Notes         		Calibrates the ADC16. Required to meet specifications 
                        after reset and before a conversion is initiated.
******************************************************************************/
uint8 ADC_Cal(ADC_MemMapPtr adcmap)
{

  unsigned short cal_var;
  
  SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;  // enable ADC0 clock
  
  ADC_SC2_REG(adcmap) &=  ~ADC_SC2_ADTRG_MASK ; // Enable Software Conversion Trigger for Calibration Process   
  ADC_SC3_REG(adcmap) &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK ); // set single conversion, clear avgs bitfield for next writing
//For best calibration results
  ADC_SC3_REG(adcmap) |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) );  // Turn averaging ON and set at max value ( 32 )
  
  ADC_SC3_REG(adcmap) |= ADC_SC3_CAL_MASK ;      // Start CAL
  
//ADC1_SC1A
  while ( (ADC_SC1_REG(adcmap,0) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK ); // Wait calibration end

 //COCO=1 calibration complete
  if ((ADC_SC3_REG(adcmap)& ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK )
  {  
   return(1);    // Check for Calibration fail error and return 
  }
  // Calculate plus-side calibration
  cal_var = 0x00;
  
  cal_var =  ADC_CLP0_REG(adcmap); 
  cal_var += ADC_CLP1_REG(adcmap);
  cal_var += ADC_CLP2_REG(adcmap);
  cal_var += ADC_CLP3_REG(adcmap);
  cal_var += ADC_CLP4_REG(adcmap);
  cal_var += ADC_CLPS_REG(adcmap);

  cal_var = cal_var/2;
  cal_var |= 0x8000; // Set MSB

  ADC_PG_REG(adcmap) = ADC_PG_PG(cal_var);
 
/*
  // Calculate minus-side calibration
  cal_var = 0x00;

  cal_var =  ADC_CLM0_REG(adcmap); 
  cal_var += ADC_CLM1_REG(adcmap);
  cal_var += ADC_CLM2_REG(adcmap);
  cal_var += ADC_CLM3_REG(adcmap);
  cal_var += ADC_CLM4_REG(adcmap);
  cal_var += ADC_CLMS_REG(adcmap);

  cal_var = cal_var/2;

  cal_var |= 0x8000; // Set MSB

  ADC_MG_REG(adcmap) = ADC_MG_MG(cal_var); 
*/  
  
  ADC_SC3_REG(adcmap) &= ~ADC_SC3_CAL_MASK ; /* Clear CAL bit */

  return(0);
}

void ADC_SwitchVref( unsigned char VrefSource )
{
  ADC0_SC2 &= ~ADC_SC2_REFSEL_MASK;
  ADC0_SC2 |= ADC_SC2_REFSEL(VrefSource);
}
