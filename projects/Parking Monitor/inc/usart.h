/******************************************************************************
 * File           : USART driver header file
******************************************************************************/
#ifndef _USART_H_
#define _USART_H_

#include "stm32f0xx.h"
#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
  Function prototypes
******************************************************************************/
void terminal_init(void);

void terminal_putc(char c);
void terminal_putstr(char *str);

bool terminal_available(void);
char terminal_getc(void);
void terminal_getstr(char *str);

void terminal_clearscreen(void);

#endif // _USART_H_
