/******************************************************************************
 * File           : Button driver header file
******************************************************************************/
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "stm32f0_discovery.h"
#include "stdbool.h"

/******************************************************************************
  Function prototypes
******************************************************************************/
void button_init(void);

bool has_state_button_changed(void);

uint8_t get_button_flag(void);
uint8_t get_last_state_button(void);

#endif // _BUTTON_H_
