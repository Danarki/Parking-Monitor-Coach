/******************************************************************************
 * File           : Bluetooth driver header file
******************************************************************************/
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stm32f0xx.h"
#include "usart.h"
#include "stdint.h"
#include "stdbool.h"

#include "binary_numbers.h"

//Amount of bytes in a broadcast
#define MIN_LENGTH_BROADCAST_DATA 5
#define MAX_LENGTH_BROADCAST_DATA 12

//IDs of the bytes that are broadcast
#define BYTE_ID_1 0x00 << 6; //Max 0x03
#define BYTE_ID_2 0x01 << 6; //Max 0x03
#define BYTE_ID_3 0x02 << 6; //Max 0x03
#define BYTE_ID_4 0x03 << 6; //Max 0x03

//Used to signal the start and end of a broadcast
#define STX 0x02 //Start of Text
#define ETX 0x03 //End of Text
#define EOT 0x04 //End of Transmission

//Bitmasks to get data from bytes
#define TIME_TO_LIVE_MASK 0x0F << 2
#define GATEWAY_ID_MASK 0x03
#define VAK_ID_D1_MASK 0x3F
#define VAK_ID_D2_MASK 0x07 << 3
#define RICHTING_MASK 0x01 << 2
#define SENSOR_DATA_MASK 0x01 << 1

//AT commands for the Bluetooth modules
#define AT_NAME "AT+NAME="
#define AT_CONT "AT+CONT=1"
#define AT_ROLE "AT+ROLE="
#define AT_AVDA "AT+AVDA="

/******************************************************************************
  Function prototypes
******************************************************************************/
void bluetooth_init(void);

void bluetooth_set_name(char *vaknaam);
void bluetooth_set_connect_ability(void);

void bluetooth_set_broadcast_mode(void);
void bluetooth_set_listening_mode(void);

void bluetooth_broadcast(uint8_t time_to_live, uint8_t gateway_ID, uint16_t vak_ID, uint8_t richting, uint8_t sensor_data);
void bluetooth_listen(void);
void bluetooth_info_data(void);

#endif // _BLUETOOTH_H_
