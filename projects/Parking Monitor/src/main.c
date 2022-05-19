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
#define GATEWAY_ID 0x01 //Max 0x09
#define VAK_ID 0xDE//Max 0x3E7

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------------
bool button_pressed(void);
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
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
	terminal_putstr("User Button initialized\n");
	
	while(1){
		//Check status velostat (= user button)
		
		//Broadcast data
		terminal_putstr("Broadcasting parking space occupation...\n");
		bluetooth_set_broadcast_mode();
		delay(SECONDE);
		bluetooth_broadcast_occupation(button_pressed());
		
		delay(SECONDE * 5);
		
		//Listen for data
		terminal_putstr("Listening for reservation data...\n");
		bluetooth_set_listening_mode();
		delay(SECONDE);
		bluetooth_listen();
		
		delay(SECONDE * 5);
	}
}

bool button_pressed()
{
	if(STM_EVAL_PBGetState(BUTTON_USER) == Bit_SET)
	{
		return true;
	}
	
	return false;
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

