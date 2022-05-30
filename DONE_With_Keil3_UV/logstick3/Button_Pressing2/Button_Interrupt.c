/* Example program to detect a Pin attached to GPIO going low which represents button press	*/
/*
* Copyright (c) ARM Limited, 2006. All rights reserved..
*/

#include <LPC21xx.H>                 /* LPC21xx definitions                  */

unsigned long volatile counter = 0;


/* Default Interrupt Function: could be called when pressed ISR is disabled */
void DefISR (void) __irq  { }

/* External Interrupt 0: Function: called when EXINT0 occurs */
void pressed (void) __irq  {
  counter++;
  VICVectAddr = 0;         // Acknowledge Interrupt
}


void wait (void)  {int d; for (d = 0; d < 1000000; d++);}   


int main (void) {
  IODIR0 = 0x00000000;		// GPIO Port 0 is input (it's this by default after reset anyway)
  IODIR1 = 0x00000001;		// GPIO P0.1 defined as Output 
  
  PINSEL0 = PINSEL0 | (3 << 2); 			// Tell the Pin Function Select Register that EINT0 is on pin 1
  VICVectAddr0 = (unsigned long)pressed;	// set interrupt vector in slot 0 to be the pressed irq function
  VICDefVectAddr = (unsigned long) DefISR;	// Default ISR for non vectored interrupts that can occur
  VICIntSelect &= ~0x4000;					// EINT0 contributes to IRQ
  VICVectCntl0 = 0x20 | 14;					// use it for EXINT0 and turn it on
  VICIntEnable = VICIntEnable | 0x4000;		// Enable this interrupt		

  while (1) {	// endless heartbeat loop 
	IOSET1 = 1;
	wait();
	IOCLR1 = 1;
	wait();
  }
}
