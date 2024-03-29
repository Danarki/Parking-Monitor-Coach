/******************************************************************************
 * File           : Bluetooth driver header file
******************************************************************************/
#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_

#include "stm32f0xx.h"
#include "usart.h"
#include "math.h"
#include "stdint.h"
#include "stdbool.h"

#include "button.h"

#define MAX_LENGTH_BROADCAST_DATA 12 //Amount of bytes in a broadcast
#define MAX_TIME_TO_LIVE 0x09 //Maximum time to live of a data package

//Used for determining the type of data being broadcasted
#define DIRECTION_GATEWAY 0x00
#define DIRECTION_PARKING_SPOT 0x01

//Used to signal the start and end of a broadcast
#define STX 0x02 //Start of Text
#define ETX 0x03 //End of Text
#define EOT 0x04 //End of Transmission

//(Start of) AT commands for the Bluetooth modules
#define AT_NAME "AT+NAME="
#define AT_CONT "AT+CONT=1"
#define AT_ROLE "AT+ROLE="
#define AT_AVDA "AT+AVDA="
#define AT_CLEAR "AT+CLEAR"

/******************************************************************************
  Function prototypes
******************************************************************************/
void bluetooth_init(void);

void bluetooth_set_name(char *vaknaam, uint8_t gateway_ID, uint16_t vak_ID);
void bluetooth_set_connect_ability(void);

void bluetooth_set_broadcast_mode(void);
void bluetooth_broadcast_occupation(void);
void bluetooth_broadcast(uint8_t time_to_live, uint8_t new_gateway_ID, uint16_t new_vak_ID, uint8_t richting, uint8_t data);

void bluetooth_set_listening_mode(void);
void bluetooth_clear_slave_adress(void);
void bluetooth_listen(void);
uint8_t bluetooth_get_time_to_live(void);
uint8_t bluetooth_get_gatewayID(void); 
uint16_t bluetooth_get_vakID(void); 
uint8_t bluetooth_get_reservation(void); 
uint8_t bluetooth_get_direction(void); 
uint8_t bluetooth_get_data(void); 

void bluetooth_broadcast_received_bytes(void);

#endif // _BLUETOOTH_H_
