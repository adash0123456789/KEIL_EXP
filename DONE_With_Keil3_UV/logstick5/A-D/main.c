/* Example program  to read and analog input between 			*/
/* 0 and 3 Volts and represent the result in 10 bits 			*/
/*
* Copyright (c) ARM Limited, 2006. All rights reserved..
*/
#include <LPC214x.h>

#define CHANNEL 2 						// Actual channel number is CHANNEL - 1
#define DONE_MASK 0x80000000
#define TEN_BIT_MASK 0x3FF

static unsigned short ADC_Read()
{
	unsigned int i;

	AD0CR |= 0x01200000;				// bit 21 = 1 power it on
										// bits 26:24 = 001 Start conversion now!
	do
	{
		i = AD0GDR;						// Repeatedly read the Global Data Register of AD0
	} while ((i & DONE_MASK) == 0);		// Until result is cooked (Done bit set)
	AD0CR ^= 0x00200000;				// bit 21 = 0 turn it off
	return (i >> 6) & TEN_BIT_MASK;		// bits 15:6 are the 10 bit result so shift down and mask clean
}

volatile unsigned int result=0xDEADBEEF; // Made global volatile to easily watch results in debugger

// Assumes Startup sets LPC2148 to run at CCLK = 60Mhz
// Default condition at Reset is that VPB bus clock is 1/4 of processor clock. So PCLK is 15MhZ
int main(void)
{
	AD0CR  = 0x000300| CHANNEL;			// 0000001100000000 | channel#
										// bits 7:0 are channel# but can't be 1 or 5 
										// bits 15:8 = 011 (3) So divide PCLK by (3+1)
										// bit 16 = 0 So Burst mode is off and conversion is by software
										// bits 19:17 = 0 so use 11 clocks for 10 bits of accuracy
										// bit 20 must be 0
										// bit 21 = 0 powered off
	while(1) {
		result = ADC_Read();
	}
}
