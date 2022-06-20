/******************************************************************************
 * File           : Binary number printer implementation file
 *****************************************************************************/
#include "binary_numbers.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------


void print_binary_short(uint8_t byte, uint8_t length)
{
	uint8_t bitmask = 0x01 << (length - 1);
	uint8_t bitCounter = 0;
	
	terminal_putstr("0b");
	
	while(bitCounter < length)
	{
		if(byte & (bitmask >> bitCounter))
		{
			terminal_putc('1');
		}	
		else
		{
			terminal_putc('0');
		}
		
		bitCounter++;
	}	
}

void print_binary_long(uint16_t byte, uint8_t length)
{
	uint16_t bitmask = 0x01 << (length - 1);
	uint16_t bitCounter = 0;
	
	terminal_putstr("0b");
	
	while(bitCounter < length)
	{
		if(byte & (bitmask >> bitCounter))
		{
			terminal_putc('1');
		}	
		else
		{
			terminal_putc('0');
		}
		
		bitCounter++;
	}	
}
