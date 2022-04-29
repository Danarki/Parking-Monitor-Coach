/******************************************************************************
 * File           : Bluetooth driver header file
******************************************************************************/
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stm32f0xx.h"
#include "stdint.h"
#include "stdbool.h"

//Amount of bytes in a broadcast
#define MIN_LENGTH_BROADCAST_DATA 5
#define MAX_LENGTH_BROADCAST_DATA 12

//---Data that goes into the bytes---
//Contents of the first byte
#define BYTE_ID_1 0x00 << 6; //Max 0x03
#define TIME_TO_LIVE 0x00 << 2; //Max 0x0F
#define GATEWAY_ID 0x01; //Max 0x03

//Contents of the second byte
#define BYTE_ID_2 0x01 << 6; //Max 0x03
#define VAK_ID_D1 0x00; //Max 3F

//Contents of the third byte
#define BYTE_ID_3 0x02 << 6; //Max 0x03
#define VAK_ID_D2 0x01 << 3; //Max 0x07
#define RICHTING 0x00 << 2; //Max 0x01
#define SENSOR_DATA 0x01 << 1; //Max 0x01

//Contents of the fourth byte
#define BYTE_ID_4 0x03 << 6; //Max 0x03
//Empty byte to prevent data corruption
//---****---

//Used to signal the end of a broadcast
#define EOT 0x04 //End of Transmission

//AT commands for the Bluetooth modules
#define AT_AVDA "AT+AVDA="

/******************************************************************************
  Function prototypes
******************************************************************************/
void bluetooth_init(void);

void bluetooth_broadcast(void);

#endif // _BLUETOOTH_H_
