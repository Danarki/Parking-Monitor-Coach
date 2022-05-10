/******************************************************************************
 * File           : Bluetooth driver implementation file
 *****************************************************************************/
#include "bluetooth.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
uint8_t receivedBytes[4];

// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
void bluetooth_putc(char c);
void bluetooth_putstr(char *str);
char bluetooth_getc(void);
void bluetooth_getstr(char *str);

void print_binary_short(uint8_t byte, uint8_t length);
void print_binary_long(uint16_t byte, uint8_t length);

//bool bluetooth_available(void);

void bluetooth_init()
{
	// GPIOA Periph clock enable
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
  
  // PA2 and PA3 Alternate function mode
  GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);
  
	// Set alternate functions AF1 for PA2 and PA3
  GPIOA->AFR[0] |= 0x00001100;
  
	//USART2 clock enable
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
  // 9600 Bd @ 48 MHz
  // USARTDIV = 48 MHz / 9600 = 5000 = 0x1388
  // BRR[15:4] = USARTDIV[15:4]
  // When OVER8 = 0, BRR [3:0] = USARTDIV [3:0]
	USART2->BRR = 0x1388;

  // USART enable
  // Receiver enable
  // Transmitter enable
	USART2->CR1 = USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;

  // Default value
	USART2->CR2 = 0;
  USART2->CR3 = 0; 
}

void bluetooth_broadcast(uint8_t time_to_live, uint8_t gateway_ID, uint16_t vak_ID, uint8_t richting, uint8_t sensor_data)
{
	//These bytes of data are going to be broadcast
	uint8_t byte_one;
	uint8_t byte_two;
	uint8_t byte_three;
	uint8_t byte_four;
	
	//Building the first byte
	byte_one = BYTE_ID_1;
	byte_one |= time_to_live << 2;
	byte_one |= gateway_ID;
	
	//Building the second byte
	byte_two = BYTE_ID_2;
	byte_two |= (uint8_t)(vak_ID >> 3);
	
	//Building the third byte
	byte_three = BYTE_ID_3;
	byte_three |= ((uint8_t)(vak_ID << 5)) >> 2;
	byte_three |= richting << 2;
	byte_three |= sensor_data << 1;
	
	//Building the fourth byte
	byte_four = BYTE_ID_4;
	//Empty byte to prevent data corruption
	
	//Start broadcasting the bytes
	bluetooth_putstr(AT_AVDA);
	
	bluetooth_putc(byte_one);
	bluetooth_putc(byte_two);
	bluetooth_putc(byte_three);
	bluetooth_putc(byte_four);
	
	//End broadcast
	bluetooth_putc(EOT);
}

void bluetooth_listen()
{
	uint8_t byteCounter = 0;
	
	while(1){
		char newByte = bluetooth_getc();
		
		if(newByte == EOT)
		{
			bluetooth_info_data();
			
			byteCounter = 0;
		}
		else
		{
			receivedBytes[byteCounter] = newByte;
			byteCounter++;
		}
	}
}
void bluetooth_info_data()
{
	uint16_t vakID = 0;
	
	terminal_putstr("Broadcast received!\n");
	terminal_putstr("-------------------\n");

	/*terminal_putstr("Byte 1: ");
	print_binary(receivedBytes[0]);
	terminal_putstr("\n");
	
	terminal_putstr("Byte 2: ");
	print_binary(receivedBytes[1]);
	terminal_putstr("\n");
	
	terminal_putstr("Byte 3: ");
	print_binary(receivedBytes[2]);
	terminal_putstr("\n");
	
	terminal_putstr("Byte 4: ");
	print_binary(receivedBytes[3]);
	terminal_putstr("\n");*/
	
	terminal_putstr("Time to live: ");
	print_binary_short((receivedBytes[0] & TIME_TO_LIVE_MASK) >> 2, 4);
	terminal_putstr("\n");

	terminal_putstr("Gateway ID: ");
	print_binary_short(receivedBytes[0] & GATEWAY_ID_MASK, 2);
	terminal_putstr("\n");

	vakID = ((receivedBytes[1] & VAK_ID_D1_MASK) << 3) | ((receivedBytes[2] & VAK_ID_D2_MASK) >> 3);
	terminal_putstr("Vak ID: ");
	print_binary_long(vakID, 12);
	terminal_putstr("\n");

	terminal_putstr("Richting: ");
	print_binary_short((receivedBytes[2] & RICHTING_MASK) >> 2, 1);
	terminal_putstr("\n");

	terminal_putstr("Sensordata: ");
	print_binary_short((receivedBytes[2] & SENSOR_DATA_MASK) >> 1, 1);
	terminal_putstr("\n");
	
	terminal_putstr("-------------------\n");
}

void bluetooth_putc(char c)
{
  // Wait for Transmit data register empty
  while((USART2->ISR & USART_ISR_TXE) == 0) ;

  // Transmit data by writing to TDR, clears TXE flag  
  USART2->TDR = c;
}

void bluetooth_putstr(char *str)
{
  while(*str)
  {
    if(*str == '\n')
    {
      bluetooth_putc('\r');
    }
    
    bluetooth_putc(*str++);
  }
}

char bluetooth_getc(void)
{
  char c;

  // Was there an Overrun error?
  if((USART2->ISR & USART_ISR_ORE) != 0)
  {
    // Yes, clear it 
    USART2->ICR |= USART_ICR_ORECF;
  }

  // Wait for data in the Receive Data Register
  while((USART2->ISR & USART_ISR_RXNE) == 0) ;

  // Read data from RDR, clears the RXNE flag
  c = (char)USART2->RDR;

  return(c);
}

void bluetooth_getstr(char *str)
{
  while(1)
  {
		char c = bluetooth_getc();
		
		//When the end of transmission is reached
    if(c == EOT)
    {
			break;
    }
		
		*str = c;
		str++;
	}
}

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

/*bool bluetooth_available()
{
	if(USART2->ISR & USART_ISR_RXNE)
	{
		return true;
	}
	
	return false;
}*/
