/******************************************************************************
 * File           : Bluetooth driver header file
******************************************************************************/
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stm32f0xx.h"
#include "stdint.h"
#include "stdbool.h"

#define MAX_LENGTH_BROADCAST_DATA 12

//AT commands for the Bluetooth modules
#define AT_AVDA "AT+AVDA="

/******************************************************************************
  Function prototypes
******************************************************************************/
void bluetooth_init(void);

void bluetooth_broadcast(char *data);

#endif // _BLUETOOTH_H_
