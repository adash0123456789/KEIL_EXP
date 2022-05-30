/* Example program to detect a Pin attached to GPIO going low which represents button press	*/
/*
* Copyright (c) ARM Limited, 2006. All rights reserved..
*/

#include <LPC21xx.H>                 /* LPC21xx definitions                  */

#define PINMASK     0x00000001
#define BUTTON_PRESSED ((IOPIN0 & PINMASK)== 0) /* IF masked pin number is low THEN button pressed */

void wait (void)  {int d; for (d = 0; d < 1000000; d++);}   


int main (void) {
  IODIR0 = 0x00000000;		/* GPIO Port 0 is input (it's this by default after reset anyway) */
  IODIR1 = 0x00000001;		/* GPIO P0.1 defined as Output */
  while (1) {				/* endless loop                */
    if (BUTTON_PRESSED) {	/* IF button pressed THEN toggle an output pin for a visible time */
		IOSET1 = 1;
		wait();
		IOCLR1 = 1; 
  	}
  }
}


