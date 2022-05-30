#include <AT91SAM7S64.H>                 /* AT91SAM7S definitions                 */
#include <stdio.h>                       /* standard I/O .h-file                  */

#define VREF  3                          /* number of records                     */
#define AT91C_ADC_PRESCAL_SHIFT 8        /* ADC Prescaler bit offset              */

AT91S_ADC * pADC = AT91C_BASE_ADC;       /* Global Pointer to ADC                 */

extern void init_serial (void);          /* Initialize Serial Interface           */

/* Main Program */
int main (void)  {                      /* main entry for program                */

unsigned int adval;

/* Enable peripheral clocks */
  *AT91C_PMC_PCER = (1<<AT91C_ID_ADC) |  /* enable ADC,                           */
	 			    (1<<AT91C_ID_US0);   /* enable USART0                         */
	  
  init_serial();                         /* Initialize Serial Interface           */

/* Setup the A/D converter */
  pADC->ADC_CR = AT91C_ADC_SWRST;        /* reset ADC                             */
  pADC->ADC_CHER = AT91C_ADC_CH0 | 
				   AT91C_ADC_CH1;        /* enable ADC channels 0..3              */
  pADC->ADC_MR = AT91C_ADC_LOWRES_10_BIT |
                (9<<AT91C_ADC_PRESCAL_SHIFT); /* set 10-bit res., prescale = 9    */

  while (1)  {                                     /* loop forever                */

    pADC->ADC_CR = AT91C_ADC_START;                /* start ADC0 conversion       */
    while ((pADC->ADC_SR & AT91C_ADC_EOC0) == 0);  /* wait for ADC0 conversion    */

/* Read result of ADC conversion   */
      adval = (unsigned int) pADC->ADC_CDR0 & AT91C_ADC_DATA;

/* Output 0-1023 AD0 value without using floating-point */
      printf ("AD0 Output Value %4u = %01u.%04u Volts\r",
             (unsigned) adval,
             (unsigned) (adval * VREF) >> 10,                          /* Output Integer Portion */
             (unsigned) ((adval * VREF * 10000UL) >> 10UL) % 10000);   /* Output Decimal Portion */
  }
}
