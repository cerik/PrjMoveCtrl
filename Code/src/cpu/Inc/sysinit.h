/*
 * File:        sysinit.h
 * Purpose:     Kinetis Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 */

/********************************************************************/

// function prototypes
void InitSystem(void);
void enable_abort_button(void);
void clk_out_init(void);
void InitUart0(void);
void flash_identify (void);
void cpu_identify (void);

uint32 GetMcgClk(void);
uint32 GetCoreClk(void);
uint32 GetPeriphClk(void);

/********************************************************************/
