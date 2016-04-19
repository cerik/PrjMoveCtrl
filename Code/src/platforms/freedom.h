/*
 * File:        tower.h
 * Purpose:     Kinetis tower CPU card definitions
 *
 * Notes:
 */

#ifndef __TOWER_H__
#define __TOWER_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all Tower cards */
#define DEBUG_PRINT

#define CPU_MKL05Z32LF4

/*
* Input Clock Info
*/
#define CLK0_FREQ_HZ        32768
#define CLK0_TYPE           CRYSTAL

/*
 * PLL Configuration Info
 */
//define this to skip PLL initilaization and stay in default FEI mode
//#define NO_PLL_INIT  

/* The expected PLL output frequency is:
 * PLL out = (((CLKIN/PRDIV) x VDIV) / 2)
 * where the CLKIN is CLK0_FREQ_HZ.
 * 
 * For more info on PLL initialization refer to the mcg driver files.
 */

#define PLL0_PRDIV          1       // divider eference by 2 = 4 MHz
#define PLL0_VDIV           24      // multiply reference by 24 = 96 MHz

/* Serial Port Info */
/* Uses UART0 for both OSJTAG and TWR-SER Tower card */
#define TERM_PORT           UART0_BASE_PTR
#define TERM_PORT_NUM       0

/** USE_UART0 must be defined if you want to use UART0 **/
#define UART_PIN_PTB_1_2_

#if (defined(LOW_POWER_MODE))
  #define TERMINAL_BAUD       19200
#else 
  #define TERMINAL_BAUD       115200
#endif
#undef  HW_FLOW_CONTROL


#define true                1
#define false               0

#endif /* __TOWER_H__ */
