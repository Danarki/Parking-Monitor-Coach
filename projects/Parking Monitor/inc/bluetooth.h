/******************************************************************************
 * File           : Bluetooth driver header file
******************************************************************************/
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stm32f0xx.h"
#include "usart.h"
#include "stdint.h"
#include "stdbool.h"
#include "string.h"

//Amount of bytes in a broadcast
#define MIN_LENGTH_BROADCAST_DATA 5
#define MAX_LENGTH_BROADCAST_DATA 12

//IDs of the bytes that are broadcast
#define BYTE_ID_1 0x00 << 6; //Max 0x03
#define BYTE_ID_2 0x01 << 6; //Max 0x03
#define BYTE_ID_3 0x02 << 6; //Max 0x03
#define BYTE_ID_4 0x03 << 6; //Max 0x03

//Used to signal the end of a broadcast
#define EOT 0x04 //End of Transmission

//AT commands for the Bluetooth modules
#define AT_AVDA "AT+AVDA="

/******************************************************************************
  Function prototypes
******************************************************************************/
void bluetooth_init(void);

void bluetooth_broadcast(uint8_t time_to_live, uint8_t gateway_ID, uint16_t vak_ID, uint8_t richting, uint8_t sensor_data);
void bluetooth_listen(void);

#endif // _BLUETOOTH_H_
