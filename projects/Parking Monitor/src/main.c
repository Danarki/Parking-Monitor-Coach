/******************************************************************************
 * File           : Main program
 *****************************************************************************/
#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "usart.h"
#include "stdbool.h"
#include "led_functions.h"
#include "string.h"

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
	// Setup USART1 (PA9 & PA10) and USART2 (PA2 & PA3)
  USART_init();
	
	STM_EVAL_LEDInit(LED3);
	
	//Print introduction
  terminal_clearscreen();
  terminal_putstr("USART initialized\n");
	terminal_putstr("Waiting for input...\n");
		
	while(true)
	{
		if(bluetooth_available())
		{
			char c = bluetooth_getc();
			
			terminal_putstr("Input received: \'");
			terminal_putc(c);
			terminal_putstr("\'\n");
			
			if(c == 'j')
			{
				terminal_putstr("Turning LED on...");
				STM_EVAL_LEDOn(LED3);
			}
			else if(c == 'n')
			{
				terminal_putstr("Turning LED off...");
				STM_EVAL_LEDOff(LED3);
			}
			else
			{
				terminal_putstr("Unknown input!");
			}
			terminal_putstr("\n");
			
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

