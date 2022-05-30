/*------------------------------------------------------------------------------
AD8Input.C:  MSC 1200 A/D Conversion for 8 single ended channels.
             Uses 0-5V on inputs AIN0 to AIN7 and AINCOM = 2.5V.

Copyright 2004 Keil Software, Inc.
------------------------------------------------------------------------------*/

#include <REG1210.H>
#include <stdio.h>


#define XTAL          11059200      // XTAL frequency

// defines for UART BAUDRATE
#define BAUDRATE          9600      // 9600bps communication baudrate
#define T2RELOAD   (65536-(XTAL/32/BAUDRATE))

// defines for A/D Converter setup
#define A_CLK      ((XTAL/1000000)-1)       // about 1MHz Analog Clock
#define ANA_CLK    (XTAL/(A_CLK+1))         // precise Analog Clock
#define DECIMATION (ANA_CLK/64/10)          // 10 Hz Decimation
#define CONV_FREQ  (ANA_CLK/64/DECIMATION)  // Conversion Frequency

// defines for conversion to Volts
#define fV    (0xFFFFFF00UL/5)      // Factor for Volts caluculation
#define fmV   ((fV << 2) / 1000)    // Factor for mVolts
#define fuV   ((fmV << 10) / 1000)  // Factor for uVolts

// Helper structure to read in ADC values
union  {
  unsigned char  c[4];              // bytes
  unsigned long  l;                 // unsigned long
} res;


void main(void)  {
  unsigned char k, chan, V;
  unsigned int  mV, uV;

  CKCON = 0;                        /* 0 MOVX cycle stretch					*/
  PDCON = 0x14;                     /* turn on ADC-Vref, SPI and Systimers	*/

/* Setup Serial Interface		*/
  SCON0  = 0x50;                    /* SCON: mode 1, 8-bit UART, enable rcvr            */
  TMOD |= 0x20;                     /* TMOD: timer 1, mode 2, 8-bit reload              */
  TH1   = 0xF4;                     /* TH1:  reload value for 2400 baud @ 11.0592 MHz   */
  TR1   = 1;                        /* TR1:  timer 1 run                                */
  TI_0    = 1;                      /* TI:   set TI to send first char of UART          */

  P3DDRL &= 0xF0;                   /* set port pins of UART to input/strong drive output */
  P3DDRL |= 0x07;                   /* set port pins of UART to input/strong drive output */

  printf("\nMSC1210 Analog/Dignal Converter Test\n\n");

/* Setup ADC					*/
  ADMUX  = 0x08;                    /* (AIN+ = AIN0), (AIN- = AINCOM) Voltage from DAC	*/
  ACLK   = A_CLK;                   /* set ACLK factor for about 1MHz					*/
  ADCON0 = 0x30;                    /* Vref On, Vref Hi, Buff off, BOD off, PGA			*/

  ADCON2 = DECIMATION & 0xFF;       /* LSB of decimation                                */
  ADCON3 =(DECIMATION>>8) & 0x07;   /* MSB of decimation                                */
  ADCON1 = 0x01;                    /* bipolar, auto, self calibration (offset, gain)   */
                                                                                        
  while (1)  {                      /* Repeat Conversions Forever                       */
    printf("\nSingle-Ended 0V to 5V inputs, AINCOM=2.5V\n\n");
    printf("Chan.  Dec.  Rate   Hex Value   Voltage\n");

                                                                                        
    for (chan=0; chan<8; chan++) {  /* Channels Loop                                    */
      ADMUX = (chan<<4) | 8;        /* AINP = chan, AINN = AINCOM                       */

/* Read Input Voltage           */
      for (k=0; k<5; k++) {         /* ignore 4 conversions for filter to settle after calibration */
        while (!(AIE & 0x20));      /* Wait for data ready                                   */
        res.c[0] = ADRESH ^ 0x80;   /* Invert the Most significant bit (only positiv inputs) */
        res.c[1] = ADRESM;
        res.c[2] = ADRESL;
        res.c[3] = 0;
      }

      printf ("%3bd %7d %3dHz   ", chan, (int) DECIMATION, (int) CONV_FREQ);
      printf ("0x%06lX    ", (res.l >> 8));

/* Convert Result to Volts      */
      V = res.l / fV;               /* Volts part        */
      res.l %= fV;
      res.l <<= 2;

      mV = res.l / fmV;             /* Millivolts part   */
      res.l %= fmV;
      res.l <<= 10;
      
      uV = res.l / fuV;             /* Microvolts part   */
      printf ("%bd.%03d%03d\n", V, mV, uV);
    }
  }
}



