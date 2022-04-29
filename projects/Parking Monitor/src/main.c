/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "bluetooth.h"
#include "led_functions.h"

#define SECONDE SystemCoreClock/8

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
	
	//Broadcast data
	terminal_putstr("Broadcasting data...");
	bluetooth_broadcast();
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

