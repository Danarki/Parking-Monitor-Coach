/******************************************************************************
 * File           : Helper funcions used throughout all targets
******************************************************************************/
#include "helper.h"

/**
  * @brief  This function sets PA9 (USART1_TX) and PA10 (USART1_RX) to
  *         Alternate Function 1. USART1 is initialized with it's default
  *         value, except for the baud rate: 115200 Bd.
  *         USART1 is enabled with 115200,8,n,1
  * @param  None
  * @retval None
  */
void USART_Setup(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  
  // --------------------------------------------------------------------------
  // Initialize USART1
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  
  // Setup Tx- and Rx pin
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 115200;
  USART_Init(USART1, &USART_InitStructure);

  USART_Cmd(USART1, ENABLE);
}

/**
  * @brief  This function sends a string of characters through USART1.
  *         If a LINEFEED character ('\n') is detected, the functions also
  *         sends a CONTROL ('\r') character.
  * @param  str: NULL ('\0') terminated string
  * @retval None
  */
void USART_Putstr(char *str)
{
  while(*str)
  {
    if(*str == '\n')
    {
      while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){;}
      USART_SendData(USART1, '\r');
    }
    
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){;}
    USART_SendData(USART1, *str++);
  }
}

/**
  * @brief  This function implements the following VT100 terminal commands
  *         * Clear screan
  *         * Cursor home
  * @param  None
  * @retval None
  */
void USART_Clearscreen(void)
{
  char cmd1[5] = {0x1B, '[', '2', 'J', '\0'}; // Clear screen
  char cmd2[4] = {0x1B, '[', 'f', '\0'}; // Cursor home
  
  USART_Putstr(cmd1);
  USART_Putstr(cmd2);
}

/**
  * @brief  This function converts an integer to an ASCII string of characters
  *         The result is always a four digit number, so leading zero's are
  *         inserted if neccesary. The string is terminated with a NULL ('\0')
  *         character.
  * @param  i: the number to convert. Negative numbers are also allowed.
  *         p: pointer to the destination buffer
  * @retval pointer to the start of the destination buffer
  */
char *USART_itoa(int16_t i, char *p)
{
  int16_t t1, t2;
  char h[10];

  t1 = t2 = 0;

  if (i < 0)
  {
    p[t2] = '-';
    t2++;
    i = -i;
  }

  if(i<1000){ p[t2++]='0'; }
  if(i< 100){ p[t2++]='0'; }
  if(i<  10){ p[t2++]='0'; }
  
  do
  {
    h[t1] = i % 10 + 48;
    t1++;
    i = i / 10;

  }while (i > 0);

  while (t1 > 0)
  {
    p[t2++] = h[--t1];
  }

  p[t2] = '\0';

  return(p);
}

/**
  * @brief  This function implements a delay.
  *         If the optimization level is set to -O3, this function takes 8
  *         cycles. To create a delay of 1 second, use the following function
  *         call: Delay(SystemCoreClock/8);
  * @param  d: number delay loops (1 loop takes 8/SystemCoreClock sec.)
  * @retval None
  */
#pragma push
#pragma O3
void Delay(const int d)
{
  volatile int i;

  for(i=d; i>0; i--){ ; }

  return;
}
#pragma pop
