/******************************************************************************/
/*  This file is create using Keil uVision/ARM and the Keil CA ARM Compiler   */
/*  Copyright KEIL ELEKTRONIK GmbH 2004                                       */
/******************************************************************************/
/*                                                                            */
/*  EXTINT.C:  Demonstration of External Interrupt                            */
/*                                                                            */
/******************************************************************************/
                  
#include <LPC21xx.H>                        /* LPC21xx definitions */

int volatile intrp_count = 0;
int volatile timeval = 0;
int volatile SecondCnt = 0; 


/* Timer Counter 0 Interrupt executes each 10ms @ 60 MHz CPU Clock */
void tc0 (void) __irq{
  ++timeval;
  if ((timeval % 10000) == 0)  {
    timeval = 0;
    SecondCnt++;
  }
  T0IR        = 1;                            // Clear interrupt flag
  VICVectAddr = 0;                            // Acknowledge Interrupt
}

/* Setup the Timer Counter 0 Interrupt */
void init_timer (void) {
  T0MR0 = 1499;                               // 0.1mSec = 1.500-1 counts
  T0MCR = 3;                                  // Interrupt and Reset on MR0
  T0TCR = 1;                                  // Timer0 Enable
  VICVectAddr0 = (unsigned long)tc0;          // set interrupt vector in 0
  VICVectCntl0 = 0x20 | 4;                    // use it for Timer 0 Interrupt
  VICIntEnable = 0x00000010;                  // Enable Timer0 Interrupt
}

/* Create a long time delay */
void delay (void)  {
  int i;
  for (i = 0; i < 0x100000; i++)  {
    ;
  }
}

/* External Interrupt 1 Service */
extern void eint1_irq (void);                 // wrapper for interrupt nesting 

void eint1_srv (void) {                       // C interrupt function
  EXTINT      = 2;                            // Clear EINT1 interrupt flag
  delay ();                                   // wait a long time
  ++intrp_count;                              // increment interrupt count
//VICVectAddr = 0;                            // Acknowledge Interrupt
}

/* Initialize EINT1 Interrupt Pin */
void init_eint1 (void) {
	EXTMODE      = 2;                           // Edge sensitive mode on EINT1
	EXTPOLAR     = 2;                           // raising edge sensitive
	PINSEL0      = 2 << 28;                     // Enable EINT1 on GPIO_0.14
  VICVectAddr1 = (unsigned long) eint1_irq;   // set interrupt vector in VIC 0
  VICVectCntl1 = 0x21 | 15;                   // use VIC 1 for EINT1 Interrupt
  VICIntEnable = 1 << 15;                     // Enable EINT1 Interrupt
}

int main (void) {
  unsigned int LED_out = 0;

  IODIR1 = 0xFF0000;

  init_eint1();                               // Enable EINT1
  init_timer ();
   
	while (1)  {
	  if ((intrp_count & 0xFF) != LED_out)  {		// intrp_count has changed
		  LED_out = (intrp_count & 0x7F);         // set LED out
			IOCLR1 = 0x7F0000;                      // clear LED's
			IOSET1 = (LED_out << 16);               // output intrp_count	on LED
	  }
    if ((SecondCnt & 1) == 0) IOCLR1 = 0x800000;
    else                      IOSET1 = 0x800000;
	}
}




