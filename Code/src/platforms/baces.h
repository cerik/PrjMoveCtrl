/*
 * File:        baces.h
 * Purpose:     BACES platform definitions
 *
 * Notes:
 */

#ifndef __BACES_H__
#define __BACES_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all Firebird daughter cards */
#define DEBUG_PRINT

#if (defined(ASB817))
  #define CPU_MK60N512VMD100

  /*
   * Input Clock Info
   */
  #define CLK0_FREQ_HZ        20000000
  #define CLK0_TYPE           CANNED_OSC
  #define MCG_OUT_FREQ        100000000
  /*
   * PLL Configuration Info
   */
//  #define NO_PLL_INIT  // define this to skip PLL initilaization and stay in default FEI mode
//  #define FLL_100     // define this to increase FLL clock to 100 MHZ
/* The expected PLL output frequency is:
 * PLL out = (((CLKIN/PRDIV) x VDIV) / 2)
 * where the CLKIN is CLK0_FREQ_HZ.
 * 
 * For more info on PLL initialization refer to the mcg driver files.
 */

  #define PLL0_PRDIV      5       // divider eference by 5 = 4MHz
  #define PLL0_VDIV       25      // multiply reference by 25 = 100 MHz

  /* Serial Port Info */
  #define TERM_PORT           UART1_BASE_PTR
  #define TERMINAL_BAUD       19200
  #undef  HW_FLOW_CONTROL
#else
  #error "No valid BACES platform defined"
#endif


#endif /* __BACES_H__ */
