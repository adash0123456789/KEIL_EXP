/*----------------------------------------------------------------------------
 * Name:    Serial.c
 * Purpose: STM32 serial lowlevel routines
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


/* USART initialisation is done in STM32_Init.c */

/*------------------------------------------------------------------------------
  Write character to Serial Port
  used by printf function to output data. See file Retarget.c
 *------------------------------------------------------------------------------*/
int sendchar (int ch)  {

  if (ch == '\n')  {
    while (!(USART1->SR & USART_FLAG_TXE));
    USART1->DR = (0x0D & 0x1FF);                  // output CR
  }
  while (!(USART1->SR & USART_FLAG_TXE));
  return (USART1->DR = (ch & 0x1FF));
}


/*------------------------------------------------------------------------------
  Read character from Serial Port
 *------------------------------------------------------------------------------*/
int getkey (void) {

  if (USART1->SR & USART_FLAG_RXNE) {
    return (USART1->DR & 0x1FF);
  } else {
    return 0;
  }
}
