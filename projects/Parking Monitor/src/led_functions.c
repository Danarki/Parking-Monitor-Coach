#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

#define LEDPINS (1 << 10) | (1 << 11)

void init_LED_GPIO(){
	GPIO_InitTypeDef GPIO_InitStructure;  
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Pin   = (GPIO_Pin_10 | GPIO_Pin_11); 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void updateLEDS(uint8_t code){
	// turn off all LEDS on PC10-PC12
	GPIOC->ODR &= ~(LEDPINS);
	
	// turn on current LED location on PC
	GPIOC->ODR |= (1 << (9 + code));
}
