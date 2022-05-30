/*------------------------------------------------------------------------------
MAIN.C:  Interrupt Driven SIO Using printf for Philips LPC2100.

Copyright 2004-2006 KEIL Software, Inc.
------------------------------------------------------------------------------*/
#include <LPC21xx.H>
#include <stdio.h>
#include "sio.h"

int volatile intrp_count = 0;

/* External Interrupt 1 Service */
void eint1_srv (void) __irq {
  ++intrp_count;                              // increment interrupt count
  EXTINT      = 2;                            // Clear EINT1 interrupt flag
  VICVectAddr = 0;                            // Acknowledge Interrupt
}

/* Initialize EINT1 Interrupt Pin */
void init_eint1 (void) {
	EXTMODE      = 2;                           // Edge sensitive mode on EINT1
	EXTPOLAR     = 2;                           // raising edge sensitive
	PINSEL0      = 2 << 28;                     // Enable EINT1 on GPIO_0.14
  VICVectAddr0 = (unsigned long) eint1_srv;   // set interrupt vector in VIC 0
  VICVectCntl0 = 0x20 | 15;                   // use VIC 0 for EINT1 Interrupt
  VICIntEnable = 1 << 15;                     // Enable EINT1 Interrupt
}

int main (void) {
  unsigned int LED_out = 0;

  IODIR1 = 0xFF0000;

  init_eint1();                               // Enable EINT1

	while (1)  {
	  if ((intrp_count & 0xFF) != LED_out)  {		// intrp_count has changed
		  LED_out = (intrp_count & 0xFF);         // set LED out
			IOCLR1 = 0xFF0000;                      // clear LED's
			IOSET1 = (LED_out << 16);               // output intrp_count	on LED
	  }
	}
}

//const char message [] =
//  "This is a test to see if the interrupt driven serial I/O routines really work.";

//int main (void) {
//  /* General Purpose I/O Port 1 Setup ..............................*/
//  IODIR1 = 0x00FF0000;          /* P1.16..23 defined as Outputs     */
//  IOCLR1 = 0x00FF0000;          /* clear LEDs                       */

//  com_initialize ();            /* init interrupt driven serial I/O */

//  printf ("Interrupt-driven Serial I/O Example\r\n\r\n");

//  while (1) {
//    unsigned char c;

//    printf ("Press a key. ");
//    c = getchar ();
//    printf ("\r\n");
//    printf ("You pressed '%c'.\r\n\r\n", c);
//  }
//}	 

/*------------------------------------------------------------------------------
------------------------------------------------------------------------------*/
