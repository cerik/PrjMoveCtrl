/*
 * File:        uart.c
 * Purpose:     Provide common uart routines for serial IO
 *
 * Notes:
 * 
 */
#include "common.h"
#include "uart.h"

/***************************************************************************
 * Begin UART0 functions
 **************************************************************************/
/********************************************************************/
/*
 * Initialize the uart for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the uarts are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable uart pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  uartch      uart channel to initialize
 *  sysclk      uart module Clock in kHz(used to calculate baud)
 *  baud        uart baud rate
 */
void uart0_init (UART0_MemMapPtr uartch, int sysclk, int baud)
{
    uint8 i;
    uint32 calculated_baud = 0;
    uint32 baud_diff = 0;
    uint32 osr_val = 0;
    uint32 sbr_val, uart0clk;
    uint32 baud_rate;
    uint32 reg_temp = 0;
    uint32 temp = 0;
    
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    
    // Disable UART0 before changing registers
    UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
  
    // Verify that a valid clock value has been passed to the function 
    if ((sysclk > 50000) || (sysclk < 32))
    {
        sysclk = 0;
        reg_temp = SIM_SOPT2;
        reg_temp &= ~SIM_SOPT2_UART0SRC_MASK;
        reg_temp |= SIM_SOPT2_UART0SRC(0);
        SIM_SOPT2 = reg_temp;
        // Enter inifinite loop because the 
        // the desired system clock value is 
        // invalid!!
        while(1);
    }
    
    // Verify that a valid value has been passed to TERM_PORT_NUM and update
    // uart0_clk_hz accordingly.  Write 0 to TERM_PORT_NUM if an invalid 
    // value has been passed.  
    if (TERM_PORT_NUM != 0)
    {
        reg_temp = SIM_SOPT2;
        reg_temp &= ~SIM_SOPT2_UART0SRC_MASK;
        reg_temp |= SIM_SOPT2_UART0SRC(0);
        SIM_SOPT2 = reg_temp;
        // Enter inifinite loop because the 
        // the desired terminal port number 
        // invalid!!
        while(1);
    }
    // Initialize baud rate
    baud_rate = baud;
    
    // Change units to Hz
    uart0clk = sysclk * 1000;
    // Calculate the first baud rate using the lowest OSR value possible.  
    i = 4;
    sbr_val = (uint32)(uart0clk/(baud_rate * i));
    calculated_baud = (uart0clk / (i * sbr_val));
        
    if (calculated_baud > baud_rate)
        baud_diff = calculated_baud - baud_rate;
    else
        baud_diff = baud_rate - calculated_baud;
    
    osr_val = i;
        
    // Select the best OSR value
    for (i = 5; i <= 32; i++)
    {
        sbr_val = (uint32)(uart0clk/(baud_rate * i));
        calculated_baud = (uart0clk / (i * sbr_val));
        
        if (calculated_baud > baud_rate)
            temp = calculated_baud - baud_rate;
        else
            temp = baud_rate - calculated_baud;
        
        if (temp <= baud_diff)
        {
            baud_diff = temp;
            osr_val = i; 
        }
    }
    
    if (baud_diff < ((baud_rate / 100) * 3))
    {
        // If the OSR is between 4x and 8x then both
        // edge sampling MUST be turned on.  
        if ((osr_val >3) && (osr_val < 9))
            UART0_C5|= UART0_C5_BOTHEDGE_MASK;
        
        // Setup OSR value 
        reg_temp = UART0_C4;
        reg_temp &= ~UART0_C4_OSR_MASK;
        reg_temp |= UART0_C4_OSR(osr_val-1);
    
        // Write reg_temp to C4 register
        UART0_C4 = reg_temp;
        
        reg_temp = (reg_temp & UART0_C4_OSR_MASK) + 1;
        sbr_val = (uint32)((uart0clk)/(baud_rate * (reg_temp)));
        
         /* Save off the current value of the uartx_BDH except for the SBR field */
        reg_temp = UART0_BDH & ~(UART0_BDH_SBR(0x1F));
   
        UART0_BDH = reg_temp |  UART0_BDH_SBR(((sbr_val & 0x1F00) >> 8));
        UART0_BDL = (uint8)(sbr_val & UART0_BDL_SBR_MASK);
        
        /* Enable receiver and transmitter */
        UART0_C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK );
    }
    else
    {
        // Unacceptable baud rate difference
        // More than 3% difference!!
        // Enter infinite loop!
        while(1);
    }
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified uart
 *
 * Parameters:
 *  channel      uart channel to read from
 *
 * Return Values:
 *  the received character
 */
char uart0_getchar (UART0_MemMapPtr channel)
{
    /* Wait until character has been received */
    while (!(UART0_S1_REG(channel) & UART0_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return UART0_D_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the uart Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      uart channel to send to
 *  ch           character to send
 */ 
void uart0_putchar (UART0_MemMapPtr channel, char ch)
{
    /* Wait until space is available in the FIFO */
    while(!(UART0_S1_REG(channel) & UART0_S1_TDRE_MASK));

    /* Send the character */
    UART0_D_REG(channel) = (uint8)ch;
    
 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      uart channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart0_getchar_present (UART0_MemMapPtr channel)
{
    return (UART0_S1_REG(channel) & UART0_S1_RDRF_MASK);
}
/********************************************************************/
