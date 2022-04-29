/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "bluetooth.h"
#include "led_functions.h"

#define SECONDE SystemCoreClock/8

//These bytes of data are going to be broadcast
#define BYTE_1 0xAA //0b10101010
#define BYTE_2 0xCC //0b11001100
#define BYTE_3 0x77 //0b01110111
#define BYTE_4 0x92 //0b10010010

//Signals the end of a broadcast
#define EOT 0x04 //End of Transmission

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
	//Data to broadcast
	char data[MIN_LENGTH_BROADCAST_DATA] = {BYTE_1, BYTE_2, BYTE_3, BYTE_4,	EOT};
	
	// Setup USART1 for terminal (PA9 & PA10)
  terminal_init();
	terminal_clearscreen();
  terminal_putstr("Terminal initialized\n");
	
	//Setup USART2 for bluetooth (PA2 & PA3)
	bluetooth_init();
	terminal_putstr("Bluetooth initialized\n");
	
	//Broadcast the data
	terminal_putstr("Broadcasting data...");
	bluetooth_broadcast(data);
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

