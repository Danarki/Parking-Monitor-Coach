/******************************************************************************
 * File           : USART driver implementation file
 *****************************************************************************/
#include "usart.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------


void terminal_init(void)
{
  // GPIOA Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  
  // PA9, PA10, PA2 and PA3 Alternate function mode
  GPIOA->MODER |= (GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1);
  
  // Set alternate functions AF1 for PA9 and PA10
  GPIOA->AFR[1] |= 0x00000110;
  
  // USART1 clock enable
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 
	
  // 115200 Bd @ 48 MHz
  // USARTDIV = 48 MHz / 115200 = 416 = 0x01A0
  // BRR[15:4] = USARTDIV[15:4]
  // When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
  USART1->BRR = 0x01A0;

  // USART enable
  // Receiver enable
  // Transmitter enable
  USART1->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;

  // Default value
  USART1->CR2 = 0;
  USART1->CR3 = 0; 
}

void terminal_putc(char c)
{
  // Wait for Transmit data register empty
  while((USART1->ISR & USART_ISR_TXE) == 0) ;

  // Transmit data by writing to TDR, clears TXE flag  
  USART1->TDR = c;
}

void terminal_putstr(char *str)
{
  while(*str)
  {
    if(*str == '\n')
    {
      terminal_putc('\r');
    }
    
    terminal_putc(*str++);
  }
}


bool terminal_available()
{
	if(USART1->ISR & USART_ISR_RXNE)
	{
		return true;
	}
	
	return false;
}

char terminal_getc(void)
{
  char c;

  // Was there an Overrun error?
  if((USART1->ISR & USART_ISR_ORE) != 0)
  {
    // Yes, clear it 
    USART1->ICR |= USART_ICR_ORECF;
  }

  // Wait for data in the Receive Data Register
  while((USART1->ISR & USART_ISR_RXNE) == 0) ;

  // Read data from RDR, clears the RXNE flag
  c = (char)USART1->RDR;

  return(c);
}

void terminal_getstr(char *str)
{
  while(1)
  {
		char c = terminal_getc();
		
		//When enter is pressed (carrige return)
    if(c == '\r')
    {
			break;
    }
		
		*str = c;
		str++;
	}
}

// Implements the following VT100 terminal commands
// - Clear screan
// - Cursor home
void terminal_clearscreen(void)
{
  char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
  char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home
  
  terminal_putstr(cmd1);
  terminal_putstr(cmd2);
}
