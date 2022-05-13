/******************************************************************************
 * File           : Binary number printer header file
******************************************************************************/
#ifndef _BINARY_NUMBERS_H_
#define _BINARY_NUMBERS_H_

#include "stm32f0xx.h"
#include "usart.h"
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
  Function prototypes
******************************************************************************/
//Print 8-bit number on the terminal in binary
void print_binary_short(uint8_t byte, uint8_t length);
//Print 16-bit number on the terminal in binary
void print_binary_long(uint16_t byte, uint8_t length);

#endif // _BINARY_NUMBERS_H_
