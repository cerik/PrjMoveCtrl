/******************************************************************************
* File:    vectors.c
*
* Purpose: Configure interrupt vector table for Kinetis.
******************************************************************************/

#include "vectors.h"
#include "isr.h"
#include "common.h"

/******************************************************************************
* Vector Table
******************************************************************************/
typedef void (*vector_entry)(void);

#ifdef KEIL
#else
#pragma location = ".intvec"
const vector_entry  __vector_table[] = //@ ".intvec" =
#endif
{
   VECTOR_000,           // 0x0000_0000 0 -          ARM core        Initial Supervisor SP
   VECTOR_001,           // 0x0000_0004 1 -          ARM core        Initial Program Counter
   VECTOR_002,           // 0x0000_0008 2 -          ARM core        Non-maskable Interrupt (NMI)
   VECTOR_003,           // 0x0000_000C 3 -          ARM core        Hard Fault
   VECTOR_004,           // 0x0000_0010 4 -
   VECTOR_005,           // 0x0000_0014 5 -          ARM core         Bus Fault
   VECTOR_006,           // 0x0000_0018 6 -          ARM core         Usage Fault
   VECTOR_007,           // 0x0000_001C 7 -                           
   VECTOR_008,           // 0x0000_0020 8 -                           
   VECTOR_009,           // 0x0000_0024 9 -
   VECTOR_010,           // 0x0000_0028 10 -
   VECTOR_011,           // 0x0000_002C 11 -         ARM core         Supervisor call (SVCall)
   VECTOR_012,           // 0x0000_0030 12 -         ARM core         Debug Monitor
   VECTOR_013,           // 0x0000_0034 13 -                          
   VECTOR_014,           // 0x0000_0038 14 -         ARM core         Pendable request for system service (PendableSrvReq)
   VECTOR_015,           // 0x0000_003C 15 -         ARM core         System tick timer (SysTick)
   VECTOR_016,           // 0x0000_0040 16     0     DMA              DMA Channel 0 transfer complete
   VECTOR_017,           // 0x0000_0044 17     1     DMA              DMA Channel 1 transfer complete
   VECTOR_018,           // 0x0000_0048 18     2     DMA              DMA Channel 2 transfer complete
   VECTOR_019,           // 0x0000_004C 19     3     DMA              DMA Channel 3 transfer complete
   VECTOR_020,           // 0x0000_0050 20     4     DMA              DMA Channel 4 transfer complete
   VECTOR_021,           // 0x0000_0054 21     5     DMA              DMA Channel 5 transfer complete
   VECTOR_022,           // 0x0000_0058 22     6     DMA              DMA Channel 6 transfer complete
   VECTOR_023,           // 0x0000_005C 23     7     DMA              DMA Channel 7 transfer complete
   VECTOR_024,           // 0x0000_0060 24     8     DMA              DMA Channel 8 transfer complete
   VECTOR_025,           // 0x0000_0064 25     9     DMA              DMA Channel 9 transfer complete
   VECTOR_026,           // 0x0000_0068 26    10     DMA              DMA Channel 10 transfer complete
   VECTOR_027,           // 0x0000_006C 27    11     DMA              DMA Channel 11 transfer complete
   VECTOR_028,           // 0x0000_0070 28    12     DMA              DMA Channel 12 transfer complete
   VECTOR_029,           // 0x0000_0074 29    13     DMA              DMA Channel 13 transfer complete
   VECTOR_030,           // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete
   VECTOR_031,           // 0x0000_007C 31    15     DMA              DMA Channel 15 transfer complete
   VECTOR_032,           // 0x0000_0080 32    16     DMA              DMA Error Interrupt Channels 0-15
   VECTOR_033,           // 0x0000_0084 33    17     MCM              Normal interrupt
   VECTOR_034,           // 0x0000_0088 34    18     Flash memory     Command Complete
   VECTOR_035,           // 0x0000_008C 35    19     Flash memory     Read Collision
   VECTOR_036,           // 0x0000_0090 36    20     Mode Controller  Low Voltage Detect,Low Voltage Warning, Low Leakage Wakeup
   VECTOR_037,           // 0x0000_0094 37    21     LLWU
   VECTOR_038,           // 0x0000_0098 38    22     WDOG
   VECTOR_039,           // 0x0000_009C 39    23     RNGB
   VECTOR_040,           // 0x0000_00A0 40    24     I2C0
   VECTOR_041,           // 0x0000_00A4 41    25     I2C1
   VECTOR_042,           // 0x0000_00A8 42    26     SPI0            Single interrupt vector for all sources
   VECTOR_043,           // 0x0000_00AC 43    27     SPI1            Single interrupt vector for all sources
   VECTOR_044,           // 0x0000_00B0 44    28     SPI2            Single interrupt vector for all sources
   VECTOR_045,           // 0x0000_00B4 45    29     CAN0            OR'ed Message buffer (0-15)
   VECTOR_046,           // 0x0000_00B8 46    30     CAN0            Bus Off
   VECTOR_047,           // 0x0000_00BC 47    31     CAN0            Error
   VECTOR_048,           // 0x0000_00C0 48    32     CAN0            Transmit Warning
#ifndef BOOTLOADER
   VECTOR_049,           // 0x0000_00C4 49    33     CAN0            Receive Warning
   VECTOR_050,           // 0x0000_00C8 50    34     CAN0            Wake Up
   VECTOR_051,           // 0x0000_00CC 51    35     CAN0            Individual Matching Elements Update (IMEU)
   VECTOR_052,           // 0x0000_00D0 52    36     CAN0            Lost receive
   VECTOR_053,           // 0x0000_00D4 53    37     CAN1            OR'ed Message buffer (0-15)
   VECTOR_054,           // 0x0000_00D8 54    38     CAN1            Bus off
   VECTOR_055,           // 0x0000_00DC 55    39     CAN1            Error
   VECTOR_056,           // 0x0000_00E0 56    40     CAN1            Transmit Warning
   VECTOR_057,           // 0x0000_00E4 57    41     CAN1            Receive Warning
   VECTOR_058,           // 0x0000_00E8 58    42     CAN1            Wake Up
   VECTOR_059,           // 0x0000_00EC 59    43     CAN1            Individual Matching Elements Update (IMEU)
   VECTOR_060,           // 0x0000_00F0 60    44     CAN1            Lost receive 
   VECTOR_061,           // 0x0000_00F4 61    45     SCI0            Single interrupt vector for SCI status sou
   VECTOR_062,           // 0x0000_00F8 62    46     SCI0            Single interrupt vector for SCI error sour
   VECTOR_063,           // 0x0000_00FC 63    47     SCI1            Single interrupt vector for SCI status sou
   VECTOR_064,           // 0x0000_0100 64    48     SCI1            Single interrupt vector for SCI error sour
   VECTOR_065,           // 0x0000_0104 65    49     SCI2            Single interrupt vector for SCI status sou
   VECTOR_066,           // 0x0000_0108 66    50     SCI2            Single interrupt vector for SCI error sour
   VECTOR_067,           // 0x0000_010C 67    51     SCI3            Single interrupt vector for SCI status sou
   VECTOR_068,           // 0x0000_0110 68    52     SCI3            Single interrupt vector for SCI error sour
   VECTOR_069,           // 0x0000_0114 69    53     SCI4            Single interrupt vector for SCI status sou
   VECTOR_070,           // 0x0000_0118 70    54     SCI4            Single interrupt vector for SCI error sour
   VECTOR_071,           // 0x0000_011C 71    55     SCI5            Single interrupt vector for SCI status sou
   VECTOR_072,           // 0x0000_0120 72    56     SCI5            Single interrupt vector for SCI error sour
   VECTOR_073,           // 0x0000_0124 73    57     ADC0
   VECTOR_074,           // 0x0000_0128 74    58     ADC1
   VECTOR_075,           // 0x0000_012C 75    59     CMP0             High-speed comparator 
   VECTOR_076,           // 0x0000_0130 76    60     CMP1
   VECTOR_077,           // 0x0000_0134 77    61     CMP2
   VECTOR_078,           // 0x0000_0138 78    62     FTM0             Single interrupt vector for all sources
   VECTOR_079,           // 0x0000_013C 79    63     FTM1             Single interrupt vector for all sources
   VECTOR_080,           // 0x0000_0140 80    64     FTM2             Single interrupt vector for all sources
   VECTOR_081,           // 0x0000_0144 81    65     CMT
   VECTOR_082,           // 0x0000_0148 82    66     RTC Timer interrupt
   VECTOR_083,           // 0x0000_014C 83    67
   VECTOR_084,           // 0x0000_0150 84    68     PIT Channel 0
   VECTOR_085,           // 0x0000_0154 85    69     PIT Channel 1
   VECTOR_086,           // 0x0000_0158 86    70     PIT Channel 2
   VECTOR_087,           // 0x0000_015C 87    71     PIT Channel 3
   VECTOR_088,           // 0x0000_0160 88    72     PDB
   VECTOR_089,           // 0x0000_0164 89    73     USB OTG
   VECTOR_090,           // 0x0000_0168 90    74     USB Charger Detect
   VECTOR_091,           // 0x0000_016C 91    75     ENET             IEEE 1588 Timer interrupt           
   VECTOR_092,           // 0x0000_0170 92    76     ENET             Transmit interrupt
   VECTOR_093,           // 0x0000_0174 93    77     ENET             Receive interrupt
   VECTOR_094,           // 0x0000_0178 94    78     ENET             Error and miscellaneous interrupt
   VECTOR_095,           // 0x0000_017C 95    79     I2S
   VECTOR_096,           // 0x0000_0180 96    80     SDHC
   VECTOR_097,           // 0x0000_0184 97    81     DAC0
   VECTOR_098,           // 0x0000_0188 98    82     DAC1
   VECTOR_099,           // 0x0000_018C 99    83     TSI              Single interrupt vector for all sources
   VECTOR_100,           // 0x0000_0190 100   84     MCG
   VECTOR_101,           // 0x0000_0194 101   85     Low Power Timer
   VECTOR_102,           // 0x0000_0198 102   86     Segment LCD      Single interrupt vector for all sources
   VECTOR_103,           // 0x0000_019C 103   87     Port control module Pin Detect (Port A)
   VECTOR_104,           // 0x0000_01A0 104   88     Port control module Pin Detect (Port B)
   VECTOR_105,           // 0x0000_01A4 105   89     Port control module Pin Detect (Port C)
   VECTOR_106,           // 0x0000_01A8 106   90     Port control module Pin Detect (Port D)
   VECTOR_107,           // 0x0000_01AC 107   91     Port control module Pin Detect (Port E)
   VECTOR_108,           // 0x0000_01B0 108   92    
   VECTOR_109,           // 0x0000_01B4 109   93    
   VECTOR_110,           // 0x0000_01B8 110   94    
   VECTOR_111,           // 0x0000_01BC 111   95    
   VECTOR_112,           // 0x0000_01C0 112   96    
   VECTOR_113,           // 0x0000_01C4 113   97    
   VECTOR_114,           // 0x0000_01C8 114   98    
   VECTOR_115,           // 0x0000_01CC 115   99    
   VECTOR_116,           // 0x0000_01D0 116   100
   VECTOR_117,           // 0x0000_01D4 117   101
   VECTOR_118,           // 0x0000_01D8 118   102
   VECTOR_119,           // 0x0000_01DC 119   103
   VECTOR_120,
   VECTOR_121,
   VECTOR_122,
   VECTOR_123,
   VECTOR_124,
   VECTOR_125,
   VECTOR_126,
   VECTOR_127,
   VECTOR_128,
   VECTOR_129,
   VECTOR_130,
   VECTOR_131,
   VECTOR_132,
   VECTOR_133,
   VECTOR_134,
   VECTOR_135,
   VECTOR_136,
   VECTOR_137,
   VECTOR_138,
   VECTOR_139,
   VECTOR_140,
   VECTOR_141,
   VECTOR_142,
   VECTOR_143,
   VECTOR_144,
   VECTOR_145,
   VECTOR_146,
   VECTOR_147,
   VECTOR_148,
   VECTOR_149,
   VECTOR_150,
   VECTOR_151,
   VECTOR_152,
   VECTOR_153,
   VECTOR_154,
   VECTOR_155,
   VECTOR_156,
   VECTOR_157,
   VECTOR_158,
   VECTOR_159,
   VECTOR_160,
   VECTOR_161,
   VECTOR_162,
   VECTOR_163,
   VECTOR_164,
   VECTOR_165,
   VECTOR_166,
   VECTOR_167,
   VECTOR_168,
   VECTOR_169,
   VECTOR_170,
   VECTOR_171,
   VECTOR_172,
   VECTOR_173,
   VECTOR_174,
   VECTOR_175,
   VECTOR_176,
   VECTOR_177,
   VECTOR_178,
   VECTOR_179,
   VECTOR_180,
   VECTOR_181,
   VECTOR_182,
   VECTOR_183,
   VECTOR_184,
   VECTOR_185,
   VECTOR_186,
   VECTOR_187,
   VECTOR_188,
   VECTOR_189,
   VECTOR_190,
   VECTOR_191,
   VECTOR_192,
   VECTOR_193,
   VECTOR_194,
   VECTOR_195,
   VECTOR_196,
   VECTOR_197,
   VECTOR_198,
   VECTOR_199,
   VECTOR_200,
   VECTOR_201,
   VECTOR_202,
   VECTOR_203,
   VECTOR_204,
   VECTOR_205,
   VECTOR_206,
   VECTOR_207,
   VECTOR_208,
   VECTOR_209,
   VECTOR_210,
   VECTOR_211,
   VECTOR_212,
   VECTOR_213,
   VECTOR_214,
   VECTOR_215,
   VECTOR_216,
   VECTOR_217,
   VECTOR_218,
   VECTOR_219,
   VECTOR_220,
   VECTOR_221,
   VECTOR_222,
   VECTOR_223,
   VECTOR_224,
   VECTOR_225,
   VECTOR_226,
   VECTOR_227,
   VECTOR_228,
   VECTOR_229,
   VECTOR_230,
   VECTOR_231,
   VECTOR_232,
   VECTOR_233,
   VECTOR_234,
   VECTOR_235,
   VECTOR_236,
   VECTOR_237,
   VECTOR_238,
   VECTOR_239,
   VECTOR_240,
   VECTOR_241,
   VECTOR_242,
   VECTOR_243,
   VECTOR_244,
   VECTOR_245,
   VECTOR_246,
   VECTOR_247,
   VECTOR_248,
   VECTOR_249,
   VECTOR_250,
   VECTOR_251,
   VECTOR_252,
   VECTOR_253,
   VECTOR_254,
   VECTOR_255,
#ifndef BOOTLOADER_USER
   CONFIG_1,
   CONFIG_2,
   CONFIG_3,
   CONFIG_4,
#endif
#endif
};
// VECTOR_TABLE end
/******************************************************************************
* default_isr(void)
*
* Default ISR definition.
*
* In:  n/a
* Out: n/a
******************************************************************************/
void default_isr(void)
{
   #define VECTORNUM                     (*(volatile uint8_t*)(0xE000ED04))
   //printf("\n****default_isr entered on vector %d*****\r\n\n",VECTORNUM);
   return;
}

/******************************************************************************/
/* Generic interrupt handler for the PTA4 GPIO interrupt connected to an 
 * abort switch. 
 * NOTE: For true abort operation this handler should be modified
 * to jump to the main process instead of executing a return.
 */
void abort_isr(void)
{
   //printf("\n****Abort button interrupt****\n\n");
   return;
}
/******************************************************************************/
/* Exception frame without floating-point storage 
 * hard fault handler in C,
 * with stack frame location as input parameter
 */
void hard_fault_handler_c(unsigned int * hardfault_args)
{
    /*  
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;
    
    //Exception stack frame
    stacked_r0 = ((unsigned long) hardfault_args[0]);
    stacked_r1 = ((unsigned long) hardfault_args[1]);
    stacked_r2 = ((unsigned long) hardfault_args[2]);
    stacked_r3 = ((unsigned long) hardfault_args[3]);
    
    stacked_r12 = ((unsigned long) hardfault_args[4]);
    stacked_lr = ((unsigned long) hardfault_args[5]);
    stacked_pc = ((unsigned long) hardfault_args[6]);
    stacked_psr = ((unsigned long) hardfault_args[7]);

    printf ("[Hard fault handler]\n");
    printf ("R0 = %x\n", stacked_r0);
    printf ("R1 = %x\n", stacked_r1);
    printf ("R2 = %x\n", stacked_r2);
    printf ("R3 = %x\n", stacked_r3);
    printf ("R12 = %x\n", stacked_r12);
    printf ("LR = %x\n", stacked_lr);
    printf ("PC = %x\n", stacked_pc);
    printf ("PSR = %x\n", stacked_psr);
    printf ("BFAR = %x\n", (*((volatile unsigned long *)(0xE000ED38))));
    printf ("CFSR = %x\n", (*((volatile unsigned long *)(0xE000ED28))));
    printf ("HFSR = %x\n", (*((volatile unsigned long *)(0xE000ED2C))));
    printf ("DFSR = %x\n", (*((volatile unsigned long *)(0xE000ED30))));
    printf ("AFSR = %x\n", (*((volatile unsigned long *)(0xE000ED3C))));
   */ 
    //for(;;)
    //;/*infinite loop*/
} 
void SRTC_ISR(void) 
{
  
  volatile uint32 temp;
  
  // printf("SRTC_ISR entered\r\n");
   
   temp = RTC_SR;
   
   if((RTC_SR & 0x01)== 0x01)
     {
      // printf("SRTC time invalid interrupt entered...\r\n");
   	   RTC_SR &= 0x07;  //clear TCE, or SRTC_TSR can  not be written 
   	   RTC_TSR = 0x00000005;  //clear TIF by writing to the seconds register
     }	
   else if((RTC_SR & 0x02) == 0x02)
   {
   	 //  printf("SRTC time overflow interrupt entered...\r\n");
   	   RTC_SR &= 0x07;  //clear TCE, or SRTC_STSR can  not be written
   	   RTC_TSR = 0x00000005;  //clear TOF
           RTC_SR = 0x10; //start time again to exit wait loop in application code.
   }	 	
   else if((RTC_SR & 0x04) == 0x04)
   {
   	  // printf("SRTC alarm interrupt entered...\r\n");
   	   RTC_TAR = 0x0;// Write 0 to disable
   	   //SPIOC_PTC_PDOR = 0x0001 ^ GPIOC_PTC_PDOR;
   }	
   else
   {
          // printf("No valid Flag was set!\n");
   }
   return;
}

/* End of "vectors.c" */
