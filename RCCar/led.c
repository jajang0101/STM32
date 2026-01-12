#include "led.h"
void LED_Init(){
	RCC->AHBENR |= 1U<<19;
	GPIOC->MODER |= 1U<<26;
	GPIOC->MODER &= ~(1U<<27);
	GPIOC->ODR &= ~(1U<<13);
}
void LED_Switch(){
	GPIOC->ODR ^= (1U<<13);
}
