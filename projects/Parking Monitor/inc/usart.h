/******************************************************************************
 * File           : USART driver header file
******************************************************************************/
#ifndef _USART_H_
#define _USART_H_

#include "stdint.h"
#include "stdbool.h"

/******************************************************************************
  Function prototypes
******************************************************************************/
void USART_init(void);

void terminal_putc(char c);
char terminal_getc(void);
void terminal_putstr(char *str);
void terminal_getstr(char *str);
bool terminal_available(void);
void terminal_clearscreen(void);

void bluetooth_putc(char c);
char bluetooth_getc(void);
void bluetooth_putstr(char *str);
void bluetooth_getstr(char *str);
bool bluetooth_available(void);

#endif // _USART_H_
