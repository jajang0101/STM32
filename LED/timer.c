#include "timer.h"
void tim2_init(unsigned arr){
	RCC->APB1ENR |= 1U;
	TIM2->PSC = TIMER_CLK/10U + 1U;
	TIM2->ARR = arr - 1U;
	TIM2->CNT = 0;

	TIM2->CR1 |= 1U;
}

int tim2_update(void){
	if (TIM2->SR & 1U) {
		TIM2->SR &= ~(1U);
		return 1;
	} else {
		return 0;
	}
}

