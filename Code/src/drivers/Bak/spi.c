/*
 * File:        spi.c
 * Purpose:     Provide common adc routines
 *
 * Notes:       
 *              
 */

#include "common.h"
#include "spi.h"

   
uint8_t SPI_ReadWriteData(uint8_t *pRead,uint8_t *pWrite,uint32_t uiLength);
void SPI_InitMater( void );
void SPI_InitSlave( void );
void SPI_Irq( void );
uint8_t SPI_SlaveTest( void );



uint8_t g_ucSPI_SlaveReadBuffer[SPI_SLAVE_BUFFER_LENGTH];
uint16_t g_ucSPI_SlaveRxIndex;
uint16_t g_ucSPI_SlaveTxIndex;
uint8_t g_ucSPI_SlaveWriteBuffer[SPI_SLAVE_BUFFER_LENGTH];

/****************************************************************
                  initial the spi module to master mode
*****************************************************************/
void SPI_InitMater( void )
{
		// enable clock gate for spi module
	  SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;
	  
	  // enable PORT
	  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK;
  	
  	// disable SPI
  	SPI0_C1 &= ~SPI_C1_SPE_MASK;
  	
  	// configure I/O to SPI function
  	    PORTA_PCR5 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR5 |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //Use PTA5 as SPI0_SS_b
        PORTA_PCR6 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR6 |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //Use PTA6 as SPI0_MISO 
        PORTA_PCR7 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR7 |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //Use PTA7 as SPI0_MOSI
        PORTB_PCR0 &= ~PORT_PCR_MUX_MASK;
	PORTB_PCR0 = PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			    //Use PTB0 as SPI0_SCK
	
		SPI0_C1 |= SPI_C1_MSTR_MASK;
		SPI0_BR = 0x02;
		SPI0_C1 |= SPI_C1_SSOE_MASK;      
		SPI0_C2 |= SPI_C2_MODFEN_MASK;

		SPI0_C1 |= SPI_C1_CPHA_MASK;
		SPI0_C1 &= (~SPI_C1_CPHA_MASK);		
		SPI0_C1 |= SPI_C1_CPOL_MASK;
		SPI0_C1 &= (~SPI_C1_CPOL_MASK);		
		//SPI0_C1 |= SPI_C1_LSBFE_MASK;
		SPI0_C1 &= (~SPI_C1_LSBFE_MASK);
		
		SPI0_C1 |= SPI_C1_SPE_MASK;
}

/****************************************************************
                  initial the spi module to slave mode
*****************************************************************/
void SPI_InitSlave( void )
{
	// enable clock gate for spi module
	  SIM_SCGC4 |= SIM_SCGC4_SPI0_MASK;
	  
	  // enable PORT
	  SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK|SIM_SCGC5_PORTB_MASK;
  	
  	// disable SPI
  	SPI0_C1 &= ~SPI_C1_SPE_MASK;
  	
  	// configure I/O to SPI function
  	    PORTA_PCR5 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR5 |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //Use PTA5 as SPI0_SS_b
        PORTA_PCR6 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR6 |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //Use PTA6 as SPI0_MISO 
        PORTA_PCR7 &= ~PORT_PCR_MUX_MASK;
	PORTA_PCR7 |= PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			  //Use PTA7 as SPI0_MOSI
        PORTB_PCR0 &= ~PORT_PCR_MUX_MASK;
	PORTB_PCR0 = PORT_PCR_MUX(3)|PORT_PCR_DSE_MASK;			    //Use PTB0 as SPI0_SCK
	
		SPI0_C1 &= ~SPI_C1_MSTR_MASK;

		SPI0_C1 |= SPI_C1_SSOE_MASK;      
		SPI0_C2 |= SPI_C2_MODFEN_MASK;

		SPI0_C1 |= SPI_C1_CPHA_MASK;
		SPI0_C1 &= (~SPI_C1_CPHA_MASK);		
		SPI0_C1 |= SPI_C1_CPOL_MASK;
		SPI0_C1 &= (~SPI_C1_CPOL_MASK);		
		//SPI0_C1 |= SPI_C1_LSBFE_MASK;
		SPI0_C1 &= (~SPI_C1_LSBFE_MASK);
        
        // enable SPI_C1_RDRF interrupt in slave mode
        SPI0_C1 |= SPI_C1_SPIE_MASK|SPI_C1_SPTIE_MASK;
        
        enable_irq(10);
		
		SPI0_C1 |= SPI_C1_SPE_MASK;
        
}
          
/****************************************************************
             write data ot read data to SPI master module
*****************************************************************/
uint8_t SPI_ReadWriteData(uint8_t *pRead,uint8_t *pWrite,uint32_t uiLength)
{
	uint16_t i;
	for( i=0;i<uiLength;i++)
	{
        while(!(SPI0_S & SPI_S_SPTEF_MASK ) );
		SPI0_S |= SPI_S_SPTEF_MASK;
		SPI0_D = pWrite[i];
        while(!(SPI0_S & SPI_S_SPRF_MASK ) );
		pRead[i] = SPI0_D;
	}
    return 1;
}
/***********************************************************************************
    SPI interrupt service routine

************************************************************************************/

void SPI_Irq( void )
{
  if( SPI0_S & SPI_S_SPRF_MASK )
  {
    if( g_ucSPI_SlaveRxIndex >= SPI_SLAVE_BUFFER_LENGTH )
    {
      g_ucSPI_SlaveRxIndex = 0;
    }
    g_ucSPI_SlaveReadBuffer[g_ucSPI_SlaveRxIndex++] = SPI0_D;
  }
  
  if( SPI0_S & SPI_S_SPTEF_MASK )
  {
      if( g_ucSPI_SlaveTxIndex >= SPI_SLAVE_BUFFER_LENGTH )
      {
        g_ucSPI_SlaveTxIndex = 0;
      }
      SPI0_D = g_ucSPI_SlaveWriteBuffer[g_ucSPI_SlaveTxIndex++];
  }
}

uint8_t SPI_MasterTest( void )
{
  uint16_t i;
  uint8_t ucWriteData[16];
  static uint8_t ucReadData[16];
  SPI_InitMater( );
  for(i=0;i<16;i++)
  {
    ucWriteData[i] = i;
  }
  while(1)
  {
    SPI_ReadWriteData(ucReadData,ucWriteData,3);
    // delay some time 
    for(i=0;i<10000;i++);
  }
}

uint8_t SPI_SlaveTest( void )
{
      uint8_t i;
      
      for(i=0;i<16;i++)
      {
        g_ucSPI_SlaveWriteBuffer[i] = i|0x20;
      }
      g_ucSPI_SlaveRxIndex = 0;
      g_ucSPI_SlaveTxIndex = 0;
      
      SPI_InitSlave();
      
      return 1;
}

