/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "stdbool.h"
#include "led_functions.h"

#define LENGTH_AT_COMMAND 20
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
	
	// Setup USART1 (PA9 & PA10)
  USART_init();
	
	//Print introduction
  USART_clearscreen();
  USART_putstr("USART initialized\n");
	USART_putstr("Waiting for AT command...\n");

	while(true){
		USART_getstr(AT_buffer);
		USART_putstr("Command send to Bluetooth module: \"");
		USART_putstr(AT_buffer);
		USART_putstr("\"\n");
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

