/*----------------------------------------------------------------------------
 * Name:    T1Int.c
 * Purpose: Timer1 Interrupt Service Routine
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

#include <stm32f10x_lib.h>                        // STM32F10x Library Definitions
#include "DTMF.h"
#include <math.h>

extern unsigned short int  ADC_ConvertedValue;

/* Setup the Timer 1 Interrupt is done in STM32_Init*/

/*------------------------------------------------------------------------------
  Timer Counter 1 Interrupt executes in 8Khz frequency
 *------------------------------------------------------------------------------*/
void TIM1_UP_IRQHandler (void) {
  int val;

  if ((TIM1->SR & 0x0001) != 0) {                 // check interrupt source

    val = ADC_ConvertedValue;

    dail1.AInput[dail1.AIindex & (DTMFsz-1)] = (val);
    dail1.AIindex++;

    TIM1->SR &= ~(1<<0);                          // clear UIF flag
 }
}

