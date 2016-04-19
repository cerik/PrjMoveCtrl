/*
 * File:        firebird.h
 * Purpose:     Firebird daughter card definitions
 *
 * Notes:
 */

#ifndef __FIREBIRD_H__
#define __FIREBIRD_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all Firebird daughter cards */
#define DEBUG_PRINT

#define CPU_MKL25Z128LK4 //CPU_MKL05Z32LF4
//#define CPU_MKL05Z32LF4

  /*
   * Input Clock Info
   */
  #define CLK0_FREQ_HZ        8000000
  #define CLK0_TYPE           CRYSTAL

  /*
   * PLL Configuration Info
   */
//  #define NO_PLL_INIT  // define this to skip PLL initilaization and stay in default FEI mode

/* The expected PLL output frequency is:
 * PLL out = (((CLKIN/PRDIV) x VDIV) / 2)
 * where the CLKIN is CLK0_FREQ_HZ.
 * 
 * For more info on PLL initialization refer to the mcg driver files.
 */

  #define PLL0_PRDIV      4       // divider eference by 4 = 2MHz
  #define PLL0_VDIV       24      // multiply reference by 24 = 48MHz 

  /* Serial Port Info */

  /**************************************************************************
   * Note:
   *
   * If you want to use the LPSCI block, you must define the term port
   * as UART0 AND YOU MUST DEFINE USE_UART0!!!  
   *
   * The other UARTs will use the regular SCI block.    
   * 
   * If you are using the Firebird baseboard, you must define the 
   * TERM_PORT as UART2 and then PTD2 and PTD3 will be used and UART
   * traffic will be routed to UART0 on the baseboard (latest revision
   * of the baseboard).  
   *************************************************************************/
  #define TERM_PORT           UART0_BASE_PTR

/** USE_UART0 must be defined if you want to use UART0 **/
  #define USE_UART0

  #define TERMINAL_BAUD       115200
  #undef  HW_FLOW_CONTROL




#endif /* __FIREBIRD_H__ */
