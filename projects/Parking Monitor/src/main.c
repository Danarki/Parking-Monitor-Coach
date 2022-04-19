#include "stm32f0xx.h"
#include "stm32f0_discovery.h"

int main(){
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

