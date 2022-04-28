/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "stdbool.h"
#include "led_functions.h"
#include "string.h"

#define LENGTH_AT_COMMAND 20
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
	char AT_buffer[LENGTH_AT_COMMAND];
	
	// Setup USART1 (PA9 & PA10) and USART2 (PA2 & PA3)
  USART_init();
	
	//Print introduction
  terminal_clearscreen();
  terminal_putstr("USART initialized\n");
	terminal_putstr("Waiting for AT command...");

	while(1)
	{
		if(bluetooth_available())
		{
			terminal_putc(bluetooth_getc());
		}
		else if(terminal_available())
		{
			terminal_getstr(AT_buffer);
	
			bluetooth_putstr(AT_buffer);
	
			terminal_putstr("\nCommand send to Bluetooth module: \"");
			terminal_putstr(AT_buffer);
			terminal_putstr("\"\n");
			
			memset(AT_buffer,0,20);
		}
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

