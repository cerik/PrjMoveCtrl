/*
 * File:		spi.h
 * Purpose:     
 *
 * Notes:
 */

#ifndef __SPI_H__
#define __SPI_H__

/********************************************************************/
#define SPI_SLAVE_BUFFER_LENGTH     16
   
   
   
/*******************************************************************/   

uint8_t SPI_ReadWriteData(uint8_t *pRead,uint8_t *pWrite,uint32_t uiLength);
void SPI_InitMater( void );
void SPI_InitSlave( void );
uint8_t SPI_MasterTest( void );
uint8_t SPI_SlaveTest( void );
void SPI_Irq( void );
/********************************************************************/

#endif /* __SPI_H__ */
