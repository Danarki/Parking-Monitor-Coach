#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "stdbool.h"
#include "led_functions.h"

void delay(const int d);

int main(){
	init_LED_GPIO();

	while(true){
		updateLEDS(1);
	}
	
	return 0;
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

