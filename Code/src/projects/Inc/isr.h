#ifndef __ISR_H_
#define __ISR_H_

#ifdef KEIL
#define adc0_isrv                               ADC0_IRQHandler
#define USB_ISR                                 USB0_IRQHandler

#else

    extern void TSI_ISR(void);
    extern void PIT_Init(unsigned char cChannel);
    extern void PIT_Isr(void);

    #undef  VECTOR_038
    #define VECTOR_038 PIT_Isr

    #undef VECTOR_042
    #define VECTOR_042   TSI_ISR

#endif  //__ISR_H



/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr


// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/


#endif  //__ISR_H

/* End of "isr.h" */
