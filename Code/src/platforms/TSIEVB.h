/*
 * File:        TSIEVB.h
 * Purpose:     Kinetis TSIEVB CPU card definitions
 *
 * Notes:
 */

#ifndef __TSIEVB_H__
#define __TSIEVB_H__

#include "mcg.h"

/********************************************************************/

#define DEBUG_PRINT

#if (defined(TSIEVB_K60N512))
  #define CPU_MK60N512VMD100

  /*
   * Input Clock Info
   */
  #define CLK0_FREQ_HZ        4000000
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

  #define PLL0_PRDIV      1       // divider eference by 1 = 4MHz
  #define PLL0_VDIV       24      // multiply reference by 24 = 96MHz

  /* Serial Port Info */
  #define TERM_PORT           UART4_BASE_PTR
  #define TERMINAL_BAUD       115200
  #undef  HW_FLOW_CONTROL
#else
  #error "No valid tower CPU card defined"
#endif


#endif /* __TSIEVB_H__ */
