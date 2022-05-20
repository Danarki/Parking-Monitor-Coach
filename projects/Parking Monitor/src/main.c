/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "bluetooth.h"
#include "led_functions.h"
#include "button.h"

#define SECONDE SystemCoreClock/8

#define VAKNAAM "P1-002"
#define GATEWAY_ID 0x01 //Max 0x09
#define VAK_ID 0xDE//Max 0x3E7

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
	
	bluetooth_set_name(VAKNAAM, GATEWAY_ID, VAK_ID);
	delay(SECONDE);
	terminal_putstr("Module name updated\n");
	
	bluetooth_set_connect_ability();
	delay(SECONDE);
	terminal_putstr("Connect ability set\n");
	
	//Initialize User Button on STM32F0-Discovery
	button_init();
	terminal_putstr("User Button initialized\n");
	
	while(1){
		//Listen for data
		terminal_putstr("Listening for data...\n");
		bluetooth_set_listening_mode();
		delay(SECONDE);
		bluetooth_listen();
		
		//Broadcast data
		bluetooth_set_broadcast_mode();
		delay(SECONDE);
		
		//Check if the button state has changed (meaning no broadcast has been received)
		if(get_button_flag())
		{
			terminal_putstr("Broadcasting own parking space occupation...\n");
			bluetooth_broadcast_occupation();
		}
		else
		{
			//Check if the received broadcast contains reservation data for this parking space
			terminal_putstr("Reservation status parking space updated...\n");
			
			//Otherwise send a received broadcast further into the mesh netwerk;
			terminal_putstr("Broadcasting received data...\n");
		}
		
		//The broadcast takes five seconds
		delay(SECONDE * 5);
	init_LED_GPIO();

	while(true){
		updateLEDS(2);
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

