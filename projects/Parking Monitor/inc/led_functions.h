/******************************************************************************
 * File           : USART driver header file
******************************************************************************/
#ifndef _LED_FUNCTION_H_
#define _LED_FUNCTION_H_

#include "stdint.h"

/******************************************************************************
  Function prototypes
******************************************************************************/
void init_LED_GPIO(void);
void updateLEDS(uint8_t code);

#endif // _LED_FUNCTION_H_
