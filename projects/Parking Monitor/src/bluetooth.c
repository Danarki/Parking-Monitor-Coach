/******************************************************************************
 * File           : Bluetooth driver implementation file
 *****************************************************************************/
#include "bluetooth.h"

// ----------------------------------------------------------------------------
// Global variables
// ----------------------------------------------------------------------------
char receivedBytes[MAX_LENGTH_BROADCAST_DATA]; //Bytes received through a broadcast

uint8_t gateway_ID;
uint16_t vak_ID;

// ----------------------------------------------------------------------------
// Local function prototypes
// ----------------------------------------------------------------------------
void bluetooth_putc(char c);
void bluetooth_putstr(char *str);
bool bluetooth_available(void);
char bluetooth_getc(void);

void bluetooth_print_info(char *bluetooth_data);

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

void bluetooth_set_name(char *vaknaam, uint8_t local_gateway_ID, uint16_t local_vak_ID)
{
	gateway_ID = local_gateway_ID;
	vak_ID = local_vak_ID;
	
	bluetooth_putstr(AT_NAME);
	bluetooth_putstr(vaknaam);
}

void bluetooth_set_connect_ability()
{
	bluetooth_putstr(AT_CONT);
}

void bluetooth_set_broadcast_mode()
{
	bluetooth_putstr(AT_ROLE);
	bluetooth_putc('S');	
}

void bluetooth_broadcast_occupation()
{
	bluetooth_broadcast(0, gateway_ID, vak_ID, DIRECTION_GATEWAY, get_last_state_button());
}

void bluetooth_broadcast(uint8_t time_to_live, uint8_t new_gateway_ID, uint16_t new_vak_ID, uint8_t richting, uint8_t data)
{
	//Bytes broadcasted to other devices 
	char broadcastBytes[MAX_LENGTH_BROADCAST_DATA]; 
	
	//Prepare the vak ID for broadcasting
	uint16_t vakIDHonderdtal = new_vak_ID - (new_vak_ID % 100);
  uint8_t vakIDTiental = (new_vak_ID - vakIDHonderdtal) - ((new_vak_ID % 100) % 10);
  uint8_t vakIDEental = (new_vak_ID % 100) % 10;
	
	//Gather the required data
	broadcastBytes[0] = STX;
	broadcastBytes[1] = 'T';
	broadcastBytes[2] = time_to_live + '0';
	broadcastBytes[3] = 'V';
	broadcastBytes[4] = new_gateway_ID + '0';
	broadcastBytes[5] = (vakIDHonderdtal / 100) + '0';
	broadcastBytes[6] = (vakIDTiental / 10) + '0';
	broadcastBytes[7] = vakIDEental + '0';
	broadcastBytes[8] = 'D';
	broadcastBytes[9] = richting + '0';
	broadcastBytes[10] = data + '0';
	broadcastBytes[11] = ETX;
	
	//Start broadcasting the bytes
	bluetooth_putstr(AT_AVDA);
	bluetooth_putstr(broadcastBytes);
	
	bluetooth_print_info(broadcastBytes);
}

void bluetooth_set_listening_mode()
{
	bluetooth_putstr(AT_ROLE);
	bluetooth_putc('M');
}

void bluetooth_clear_slave_adress()
{
	bluetooth_putstr(AT_CLEAR);
}

void bluetooth_listen()
{
	char new_byte = ' ';
	bool started_transmission = false;
	uint8_t byte_counter = 0;
	
	do
	{
		if(bluetooth_available())
		{
			new_byte = bluetooth_getc();
			
			if(new_byte == STX)
			{
				started_transmission = true;
			}
			
			if(started_transmission && (byte_counter == 0 || (byte_counter > 0 && new_byte != STX)))
			{
				receivedBytes[byte_counter] = new_byte;
				byte_counter++;
			}
		}
		else if(!started_transmission && has_state_button_changed())
		{
			break;
		}
		
	} while(new_byte != ETX);
	
	if(started_transmission)
	{
		bluetooth_print_info(receivedBytes);
	}
}

uint8_t bluetooth_get_time_to_live()
{
	return receivedBytes[2] - '0';
}

uint8_t bluetooth_get_gatewayID() 
{ 
	 return receivedBytes[4] - '0';
} 
 
uint16_t bluetooth_get_vakID() 
{ 
	uint16_t received_vakID = 0;
	uint8_t i;
	
	for(i = 0; i < 3; i++)
	{
		received_vakID += (receivedBytes[i + 5] - '0') * pow(10, (2 - i));
	}
	
	return received_vakID;
} 
uint8_t bluetooth_get_reservation() 
{ 
	//Only data that goes to a parking spot can be a reservation 
	if(receivedBytes[9] - '0' == DIRECTION_PARKING_SPOT && receivedBytes[10] - '0' == 1) 
	{ 
		return 1; 
	} 
	 
	return 0; 
} 

uint8_t bluetooth_get_direction()
{
	return receivedBytes[9] - '0';
}	

uint8_t bluetooth_get_data()
{
	return receivedBytes[10] - '0';
}

void bluetooth_broadcast_received_bytes()
{
	uint8_t received_time_to_live = bluetooth_get_time_to_live();
	
	bluetooth_broadcast(received_time_to_live + 1, bluetooth_get_gatewayID(), bluetooth_get_vakID(), bluetooth_get_direction(), bluetooth_get_data());
}

void bluetooth_print_info(char *bluetooth_data)
{
	uint8_t i;
	
	for(i = 0; i < MAX_LENGTH_BROADCAST_DATA; i++)
	{
		if(bluetooth_data[i] == STX)
		{
			terminal_putc('+');
		}
		else if(bluetooth_data[i] == ETX)
		{
			terminal_putc('-');
		}
		else
		{
			terminal_putc(bluetooth_data[i]);
		}
	}
	
	terminal_putstr("\n");
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

bool bluetooth_available(void)
{
	if(USART2->ISR & USART_ISR_RXNE)
	{
		return true;
	}
	
	return false;
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



