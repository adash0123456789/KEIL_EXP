/* Timer Example from http://www.keil.com/support/docs/2888.htm	*/
/*
* Copyright (c) ARM Limited, 2006. All rights reserved..
*/

#include <LPC21xx.H>                 /* LPC21xx definitions                  */


unsigned long volatile counter;

/* Default Interrupt Function: may be called when timer ISR is disabled */
void DefISR (void) __irq  {
  ;
}

/* Timer 0 Interrupt Function: called when timer ISR is enabled */
void tc0 (void) __irq  {
  IOCLR1 = 0xFFFFFFFF;
  counter = 0;
  T0IR        = 1;                         // Clear interrupt flag
  VICVectAddr = 0;                         // Acknowledge Interrupt
}

/* Setup the Timer Counter 0 Interrupt */
void init_timer (void)  {
  T0MR0 = 149999;                          // 10mSec = 150000-1 counts
  T0MCR = 3;                               // Interrupt and Reset on MR0
  T0TCR = 1;                               // Timer0 Enable
  VICVectAddr0 = (unsigned long)tc0;       // set interrupt vector in 0
  VICVectCntl0 = 0x20 | 4;                 // use it for Timer 0 Interrupt
  VICIntEnable = 0x00000010;               // Enable Timer0 Interrupt
}


int main (void) {
  int v;

  IODIR1 = 0x00FF0000;
  IOCLR1 = 0x00FF0000;
  init_timer();
    VICDefVectAddr = (unsigned long) DefISR;   // ISR for un-assigned VIC interrupts

  while(1)  {
    VICIntEnClr |= 0x00000010;             // Disable Timer0 Interrupt
// put some instructions to ensure that the interrupt is blocked (pipeline effects)!
    v = (counter << 8) & 0xFF0000;         // the true I/O pins
    counter++;
    VICIntEnable |= 0x00000010;            // Enable Timer0 Interrupt

    IOSET1 = v;                            // turn on LED
    IOCLR1 = ~v;                           // turn off LED
  }
}

