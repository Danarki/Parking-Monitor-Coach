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
	// Setup USART1 (PA9 & PA10)
  USART_init();
	
	//Print introduction
  USART_putstr("USART initialized\n");
	USART_putstr("Waiting for message...\n");

	while(true){
		char AT_buffer[LENGTH_AT_COMMAND];
		
		USART_getstr(AT_buffer);
		USART_putstr("Message send from Bluetooth module: \"");
		USART_putstr(AT_buffer);
		USART_putstr("\"\n");
		
		memset(AT_buffer, 0, 255);
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

