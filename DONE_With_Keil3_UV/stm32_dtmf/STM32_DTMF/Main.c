/*----------------------------------------------------------------------------
 * Name:    Main.c
 * Purpose: Main module DTMF Tester
 * Version: V1.00
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2008 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------
 * History:
 *          V1.00 Initial Version
 *----------------------------------------------------------------------------*/
                  
#include <stdio.h>                                // standard I/O .h-file
#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "STM32_Init.h"                           // STM32 Initialization
#include "LCD.h"                                  // LCD function prototypes
#include "DTMF.h"


#define LED_On(led)     GPIOB->ODR |=  (led << 8)
#define LED_Off(led)    GPIOB->ODR &= ~(led << 8)


extern int getkey(void);

extern unsigned long timeval;


/*  DTMF Digit encoding */
static char DTMFchar[16] = {
  '1', '2', '3', 'A', 
  '4', '5', '6', 'B', 
  '7', '8', '9', 'C', 
  '*', '0', '#', 'D', 
};

unsigned short int  ADC_ConvertedValue;           // holds the converted Analog value

/*------------------------------------------------------------------------------
  Initialises the Analog/Digital converter
  PA1 (ADC Channel1) is used as analog input
  use DMA Channel1 for ADC1 (see DMA request mapping)
 *------------------------------------------------------------------------------*/
void adc_Init (void) {

//  GPIOA->CRL &= ~0x0000000F;                    // set PIN1 as analog input (see stm32_Init.c)

  RCC->AHBENR |= (1<<0);                          // enable periperal clock for DMA

  DMA1_Channel1->CMAR  = (u32)&ADC_ConvertedValue;// set channel1 memory address
  DMA1_Channel1->CPAR  = (u32)&(ADC1->DR);        // set channel1 peripheral address
  DMA1_Channel1->CNDTR = 1;                       // transmit 1 word
  DMA1_Channel1->CCR   = 0x00002520;              // configure DMA channel
  DMA1_Channel1->CCR  |= (1 << 0);                // DMA Channel 1 enable


  RCC->APB2ENR |= (1<<9);                         // enable periperal clock for ADC1

  ADC1->SQR1 &= ~0x00F00000;                      // only one conversion

  ADC1->SMPR2 &= 0x00000038;                      // clear bits 3..5 (channel1)
  ADC1->SMPR2 |= 0x00000028;                      // set sample time (55,5 cycles)

  ADC1->SQR3  &= 0x0000001F;                      // clear bits 0..4
  ADC1->SQR3  |= 0x00000001;                      // set rank

  ADC1->CR1   =  0x00000100;                      // use independant mode, SCAN mode
  ADC1->CR2   =  0x000E0103;                      // use data align right,continuous conversion
                                                  // EXTSEL = SWSTART 
                                                  // enable ADC, DMA mode, no external Trigger
  ADC1->CR2  |=  0x00500000;					  // start SW conversion
}

/*----------------------------------------------------------------------------
  display didit
 *---------------------------------------------------------------------------*/
void digit_display (char c) {
  static char disp_buf[] = "Digit:          ";    // display string

  disp_buf[7] = c;                                // display digit to LCD disp
  set_cursor (0, 1);
  lcd_print  ((char *)disp_buf);                  // print string to LCD

}
/*------------------------------------------------------------------------------
  Main loop
 *------------------------------------------------------------------------------*/
int main (void) {

  adc_Init();                                     // initialise A/D converter
  stm32_Init ();                                  // STM32 setup

  lcd_init  ();
  lcd_clear ();
  lcd_print ("  DTMF Checker  ");
  set_cursor(0, 1);
  lcd_print (" MCBSTM32 DEMO  ");
  
  printf ("\nDTMF Checker\n\n");

  while (1) {                                     // Loop forever
    if (dail1.AIindex >= dail1.AIcheck)  {
      DTMF_Detect (&dail1);
    } 
    if (dail1.early) LED_On(1);                   // Turn on  LED
	else             LED_Off(1);                  // Turn off LED
    
    if (dail1.new)  {
      printf ("\n%c", DTMFchar[dail1.digit & 0x0F]);
      digit_display  (DTMFchar[dail1.digit & 0x0F]);
      dail1.new = 0;                              // digit taken
    }
  }
}

