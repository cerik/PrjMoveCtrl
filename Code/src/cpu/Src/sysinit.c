/*
 * File:        sysinit.c
 * Purpose:     Kinetis Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 */
#include "common.h"
#include "sysinit.h"
#include "uart.h"
#include "rtc.h"
#include "rcm.h"

/********************************************************************/
/* Actual system clock frequency */
static uint32 mcg_clk_hz;
static uint32 core_clk_hz;
static uint32 periph_clk_hz;
static uint32 uart0_clk_hz;

uint32 GetMcgClk(void)
{
    return mcg_clk_hz;
}

uint32 GetCoreClk(void)
{
    return core_clk_hz;
}
uint32 GetPeriphClk(void)
{
    return periph_clk_hz;
}

void InitUart0(void)
{
     /* Enable the pins for the selected SCI */
#ifdef UART_PIN_PTB_1_2_
    /* Enable the SCI1_TXD function on PTC4 */
    PORTB_PCR1 = PORT_PCR_MUX(0x2); // LPSCI is alt3 function for this pin

    /* Enable the SCI1_RXD function on PTC3 */
    PORTB_PCR2 = PORT_PCR_MUX(0x2); // LPSCI is alt3 function for this pin 
#else
    /* Enable the SCI1_TXD function on PTC4 */
    PORTB_PCR3 = PORT_PCR_MUX(0x3); // LPSCI is alt3 function for this pin
    
    /* Enable the SCI1_RXD function on PTC3 */
    PORTB_PCR4 = PORT_PCR_MUX(0x3); // LPSCI is alt3 function for this pin
#endif
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); // select the FLLFLLCLK as UART0 clock source
    uart0_init (TERM_PORT,uart0_clk_hz/1000,TERMINAL_BAUD);
}

/********************************************************************/
void enable_abort_button(void)
{
    /* Configure the PTA4 pin for its GPIO function */
    PORTA_PCR4 = PORT_PCR_MUX(0x1); // GPIO is alt1 function for this pin
    
    /* Configure the PTA4 pin for rising edge interrupts */
    PORTA_PCR4 |= PORT_PCR_IRQC(0x9); 
    
    /* Enable the associated IRQ in the NVIC */
    enable_irq(30);      
}
/********************************************************************/

/********************************************************************/
void clk_out_init(void)
{
    /* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
    PORTA_PCR15 = ( PORT_PCR_MUX(0x3));
    /* Select the CLKOUT in the SMI_SOPT2 mux */
    SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(2);
}

/********************************************************************/
/*!
 * \brief   Kinetis Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 * - Kinetis family
 * - package
 * - die revision
 * - P-flash size
 * - Ram size
 */
void cpu_identify (void)
{
    uint16 temp;
    /* Determine the Kinetis family */
    switch((SIM_SDID & SIM_SDID_FAMID(0x7))>>SIM_SDID_FAMID_SHIFT) 
    {  
    case 0x0:
        printf("\nK0");
        break;
    case 0x1:
        printf("\nK1");
        break;
    case 0x2:
        printf("\nK2");
        break;
    case 0x3:
        printf("\nK3");
        break;
    case 0x4:
        printf("\nK4");
        break;
    default:
        printf("\nUnrecognized Kinetis family device.\n");  
        break;      
    }

    /* Determine Sub-Family ID */
    switch((SIM_SDID & SIM_SDID_SUBFAMID(0x7))>>SIM_SDID_SUBFAMID_SHIFT) 
    {  
    case 0x4:
        printf("4");
        break;
    case 0x5:
        printf("5");
        break;
    default:
        printf("\nUnrecognized Kinetis sub-family device.\n");  
        break;      
    }

    /* Determine the package size */
    switch((SIM_SDID & SIM_SDID_PINID(0xF))>>SIM_SDID_PINID_SHIFT) 
    {
    case 0x6:
        printf("80pin       ");
        break;
    case 0x7:
        printf("81pin       ");
        break;
    case 0x8:
        printf("100pin      ");
        break;
    case 0x9:
        printf("121pin      ");
        break;
    case 0xA:
        printf("144pin      ");
        break;
    default:
        printf("\nUnrecognized Kinetis package code.      ");  
        break;      
    }

    /* Determine Attribute ID */
    /*
    switch((SIM_SDID & SIM_SDID_ATTRID(0x7))>>SIM_SDID_ATTRID_SHIFT) 
    {  
    case 0x1:
    printf(" Low Power Line with Cortex M0+\n\n");
    break;
    default:
    printf("\nUnrecognized Kinetis family attribute.\n");  
    break;      
    }
    */
    /* Determine the System SRAM Size */
    switch((SIM_SDID & SIM_SDID_SRAMSIZE(0x7))>>SIM_SDID_SRAMSIZE_SHIFT) 
    {  
    case 0x0:
        printf("SRAM Size: 0.5 KB\n");
        break;
    case 0x1:
        printf("SRAM Size:  1 KB\n");
        break;
    case 0x2:
        printf("SRAM Size:  2 KB\n");
        break;
    case 0x3:
        printf("SRAM Size:  4 KB\n");
        break;
    case 0x4:
        printf("SRAM Size:  8 KB\n");
        break;
    case 0x5:
        printf("SRAM Size:  16 KB\n");
        break;
    case 0x6:
        printf("SRAM Size:  32 KB\n");
        break;
    case 0x7:
        printf("SRAM Size:  64 KB\n");
        break;
    default:
        printf("\nUnrecognized SRAM Size.\n");  
        break;      
    }                

    /* Determine the revision ID */
    temp = ((SIM_SDID_REVID(0xF))>>SIM_SDID_REVID_SHIFT);
    printf("Silicon rev %d\n ", temp);

    /* Determine the flash revision */
    flash_identify();    

    /* Determine the P-flash size */
    switch((SIM_FCFG1 & SIM_FCFG1_PFSIZE(0xF))>>SIM_FCFG1_PFSIZE_SHIFT)
    {
    case 0x0:
        printf("Flash size:  8 KB program flash, 0.25 KB protection region\n");
        break;
    case 0x1:
        printf("Flash size:  16 KB program flash, 0.5 KB protection region\n");
        break;
    case 0x3:
        printf("Flash size:  32 KB program flash, 1 KB protection region\n");
        break;
    case 0x5:
        printf("Flash size:  64 KB program flash, 2 KB protection region\n");
        break;
    case 0x7:
        printf("Flash size:  128 KB program flash, 4 KB protection region\n");
        break;
    case 0x9:
        printf("Flash size:  256 KB program flash, 4 KB protection region\n");
        break;
    case 0xF:
        printf("Flash size:  128 KB program flash, 4 KB protection region\n");
        break;
    default:
        printf("ERR!! Undefined flash size\n");  
        break;              
    }
}
/********************************************************************/
/*!
 * \brief   flash Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific flash parameters and flash version ID for 
 * the current device. These parameters are obtained using a special
 * flash command call "read resource." The first four bytes returned
 * are the flash parameter revision, and the second four bytes are
 * the flash version ID.
 */
void flash_identify (void)
{
    /* Get the flash parameter version */
    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFA_FCCOB0 = 0x03;
    FTFA_FCCOB1 = 0x00;
    FTFA_FCCOB2 = 0x00;
    FTFA_FCCOB3 = 0x00;
    FTFA_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while(!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));
    
    //printf("Flash parameter version %d.%d.%d.%d\n",FTFA_FCCOB4,FTFA_FCCOB5,FTFA_FCCOB6,FTFA_FCCOB7);

    /* Get the flash version ID */   
    /* Write the flash FCCOB registers with the values for a read resource command */
    FTFA_FCCOB0 = 0x03;
    FTFA_FCCOB1 = 0x00;
    FTFA_FCCOB2 = 0x00;
    FTFA_FCCOB3 = 0x04;
    FTFA_FCCOB8 = 0x01;

    /* All required FCCOBx registers are written, so launch the command */
    FTFA_FSTAT = FTFA_FSTAT_CCIF_MASK;

    /* Wait for the command to complete */
    while(!(FTFA_FSTAT & FTFA_FSTAT_CCIF_MASK));

    //printf("Flash version ID %d.%d.%d.%d\n",FTFA_FCCOB4,FTFA_FCCOB5,FTFA_FCCOB6,FTFA_FCCOB7);  
}

/********************************************************************/
/*!
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then 
 * branches to the main process.
 */
void InitSystem (void)
{
/* Disable the watchdog timer */
    SIM_COPC = 0x00;

    /* Copy any vector or data sections that need to be in RAM */
#ifndef CMSIS    // If conforming to CMSIS, we do not need to perform this code
#ifndef BOOTLOADER
    /* Copy any vector or data sections that need to be in RAM */
    common_startup();
#endif
#endif

    /* Enable all of the port clocks. These have to be enabled to configure
     * pin muxing options, so most code will need all of these on anyway.
     */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK);

    // releases hold with ACKISO:  Only has an effect if recovering from VLLS1, VLLS2, or VLLS3
    // if ACKISO is set you must clear ackiso before calling pll_init 
    //    or pll init hangs waiting for OSC to initialize
    // if osc enabled in low power modes - enable it first before ack
    // if I/O needs to be maintained without glitches enable outputs and modules first before ack.
    if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK) PMC_REGSC |= PMC_REGSC_ACKISO_MASK;
#ifdef BOOTLOADER
    SIM_CLKDIV1 = ( 0 | SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(1) );
    mcg_clk_hz =  fee_32OSC();
    uart0_clk_khz = (mcg_clk_hz / 1000); // the UART0 clock frequency will equal the FLL frequency
#else

    #if defined(NO_PLL_INIT)
    mcg_clk_hz = 21000000; //FEI mode
    SIM_SOPT2 &= !SIM_SOPT2_PLLFLLSEL_MASK; // clear PLLFLLSEL to select the FLL for this clock source
    uart0_clk_khz = (mcg_clk_hz / 1000); // the UART0 clock frequency will equal the FLL frequency
    #else 
    /* Ramp up the system clock */
    /* Set the system dividers */
    SIM_CLKDIV1 = ( 0 | SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(1) );
    // only for demo, reset rtc,user can remove it
    rtc_reset();
    mcg_clk_hz =  fei_fee(CLK0_FREQ_HZ,HIGH_GAIN,CLK0_TYPE);
    if (mcg_clk_hz < 0x100) while(1);
    uart0_clk_hz = mcg_clk_hz; // UART0 clock frequency will equal half the PLL frequency
    #endif

    /*
     * Use the value obtained from the pll_init function to define variables
     * for the core clock in kHz and also the peripheral clock. These
     * variables can be used by other functions that need awareness of the
     * system frequency.
     */
    core_clk_hz = mcg_clk_hz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);
    periph_clk_hz = core_clk_hz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> 16)+ 1);

    /* Enable pin interrupt for the abort button - PTA4 */
    /* This pin could also be used as the NMI interrupt, but since the NMI
     * is level sensitive each button press will cause multiple interrupts.
     * Using the GPIO interrupt instead means we can configure for an edge
     * sensitive interrupt instead = one interrupt per button press.
     */
    //enable_abort_button();
    //clk_out_init();
#endif
    InitUart0();
    
#ifndef BRIDGE_BOARD  
#ifndef BOOTLOADER
    printf("\r\n");
    /* Determine the last cause(s) of reset */
    outSRS();   
    
    /* Determine specific Kinetis device and revision */
    cpu_identify();
#endif
#endif 
}
