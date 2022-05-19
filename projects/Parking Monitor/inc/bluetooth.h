/******************************************************************************
 * File           : Bluetooth driver header file
******************************************************************************/
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stm32f0xx.h"
#include "usart.h"
#include "stdint.h"
#include "stdbool.h"

//Amount of bytes in a broadcast
#define MAX_LENGTH_BROADCAST_DATA 12

//Used to signal the start and end of a broadcast
#define STX 0x02 //Start of Text
#define ETX 0x03 //End of Text
#define EOT 0x04 //End of Transmission

//(Start of) AT commands for the Bluetooth modules
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
void bluetooth_broadcast(uint8_t time_to_live, uint8_t gateway_ID, uint16_t vak_ID, uint8_t richting, uint8_t sensor_data);

void bluetooth_set_listening_mode(void);
void bluetooth_listen(void); //<---Hier moeten nog comments bij

#endif // _BLUETOOTH_H_
