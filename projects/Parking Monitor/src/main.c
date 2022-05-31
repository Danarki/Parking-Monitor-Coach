/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "bluetooth.h"
#include "led_functions.h"
#include "button.h"

#define DIVIDER "- - - - - - - - - -\n"

#define SECONDE SystemCoreClock/8
#define FIVE_SECONDS SECONDE * 5

#define VAKNAAM "P1-003"
#define GATEWAY_ID 0x01 //Max 0x09
#define VAK_ID 0x03//Max 0x3E7

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
	bool is_parking_spot_reserved;
	
	// Setup USART1 for terminal (PA9 & PA10)
  terminal_init();
	terminal_clearscreen();
  terminal_putstr("Terminal initialized\n");
	
	//Setup USART2 for bluetooth (PA2 & PA3)
	bluetooth_init();
	terminal_putstr("Bluetooth initialized\n");
	
	//Initialize LEDs
	init_LED_GPIO();
	is_parking_spot_reserved = false;
	updateLEDS(is_parking_spot_reserved + 1);
	terminal_putstr("LEDs initialized\n");
	
	//Initialize User Button on STM32F0-Discovery
	button_init();
	terminal_putstr("User Button initialized\n");
	
	//Update a few settings of the bluetooth module
	bluetooth_set_name(VAKNAAM, GATEWAY_ID, VAK_ID);
	delay(SECONDE);
	terminal_putstr("Module name updated\n");
	
	bluetooth_set_connect_ability();
	delay(SECONDE);
	terminal_putstr("Connect ability set\n");
	
	terminal_putstr(DIVIDER);
	
	while(1){
		//Listen for data
		bluetooth_set_listening_mode();
		delay(SECONDE);
		terminal_putstr("Listening mode started\n");
		
		bluetooth_clear_slave_adress();
		delay(SECONDE);
		terminal_putstr("Slave adress cleared\n");
		
		terminal_putstr("Listening for data...\n");
		bluetooth_listen();
			
		terminal_putstr(DIVIDER);
		
		//Check if the button state has changed (meaning no broadcast has been received)
		if(get_button_flag())
		{
			terminal_putstr("Parking space occupation changed\n");
			terminal_putstr("Updating LEDs...\n");
			
			if(get_last_state_button())
			{
				updateLEDS(2);
			}	
			else
			{
				updateLEDS(is_parking_spot_reserved + 1);
			}
		}
		else
		{
			//Check if the received broadcast contains reservation data for this parking spot
			if(bluetooth_get_gatewayID() == GATEWAY_ID && bluetooth_get_vakID() == VAK_ID && bluetooth_get_direction() == DIRECTION_PARKING_SPOT)
			{		
				//If it does, update the LEDs
				terminal_putstr("Reservation update received\n");
				is_parking_spot_reserved = bluetooth_get_reservation();
				
				if(get_last_state_button())
				{	
					terminal_putstr("Can't update reservation\n");
					terminal_putstr("Parking spot is already occupied\n");
					//Do nothing, because the reservation doesn't matter
				}	
				else
				{
					terminal_putstr("Updating LEDs...\n");
					updateLEDS(is_parking_spot_reserved + 1);
				}
				
				//And also broadcast occupation data
				terminal_putstr("Broadcasting own parking space occupation...\n");
				bluetooth_set_broadcast_mode();
				delay(SECONDE);
				bluetooth_broadcast_occupation();
			
				//The broadcast lasts five seconds
				delay(FIVE_SECONDS);
			}
			//Otherwise ignore the received broadcast
			else
			{
				terminal_putstr("Data package ignored: wrong destination\n");
			}	
		}
			
		terminal_putstr(DIVIDER);
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

