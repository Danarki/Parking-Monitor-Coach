/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "bluetooth.h"
#include "led_functions.h"

#define SECONDE SystemCoreClock/8

#define VAKNAAM "P1-002"

//---Data that goes into the bytes---
#define TIME_TO_LIVE 0x00 //Max 0x0F
#define GATEWAY_ID 0x01 //Max 0x03
#define VAK_ID 0x02 //Max 0x1FF
#define RICHTING 0x00 //Max 0x01
#define SENSOR_DATA 0x00 //Max 0x01

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
void delay(const int d);

// ----------------------------------------------------------------------------
// Main
// ----------------------------------------------------------------------------
int main(){
	// Setup USART1 for terminal (PA9 & PA10)
  terminal_init();
	terminal_clearscreen();
  terminal_putstr("Terminal initialized\n");
	
	//Setup USART2 for bluetooth (PA2 & PA3)
	bluetooth_init();
	terminal_putstr("Bluetooth initialized\n");
	
	bluetooth_set_name(VAKNAAM);
	delay(SECONDE);
	terminal_putstr("Naam geupdated\n");
	
	bluetooth_set_connect_ability();
	delay(SECONDE);
	terminal_putstr("Connect ability = 1\n");
	
	while(1){
		//Broadcast data
		terminal_putstr("Broadcasting data...\n");
		bluetooth_set_broadcast_mode();
		delay(SECONDE);
		bluetooth_broadcast(TIME_TO_LIVE, GATEWAY_ID, VAK_ID, RICHTING, SENSOR_DATA);
		
		delay(SECONDE * 5);
		
		//Listen for data
		terminal_putstr("Listening for data...\n");
		bluetooth_set_listening_mode();
		delay(SECONDE);
		bluetooth_listen();
		
		delay(SECONDE * 5);
	}
}

#pragma push
#pragma O3
void delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
#pragma pop

