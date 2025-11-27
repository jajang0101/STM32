#include <stdint.h>
#include <stdio.h>
#include "include\stm32f103xb.h"

#include "uart.h"
#include "timer.h"

#if !defined(__SOFT_FP__) && defined(__ARM_FP)
  #warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize the FPU before use."
#endif



unsigned readBit(unsigned line, unsigned position){
	unsigned mask = 1U<<position;
	return ((mask & line) >> position);
}

int main(void)
{
	RCC->APB2ENR |= 1U<<2;
	RCC->APB2ENR |= 1U<<4;

	//led init
	GPIOA->CRL &= ~(1U<<20);
	GPIOA->CRL |= 1U<<21;
	GPIOA->CRL &= ~(1U<<22);
	GPIOA->CRL &= ~(1U<<23);
	//button init
	GPIOC->CRH &= ~(1U<<20);
	GPIOC->CRH &= ~(1U<<21);
	GPIOC->CRH |= 1U<<22;
	GPIOC->CRH &= ~(1U<23);

	uart_init();
    /* Loop forever */
	unsigned button_pressed = 0;
	unsigned led_off_trigger = 1;
	unsigned ms = 0;
	for(;;){
		//printf("12345");
		if (!readBit(GPIOC->IDR, 13)){
					button_pressed = 1;
				} else {
					if (button_pressed){
						if (!led_off_trigger){
							led_off_trigger = 1;
							GPIOA->BSRR |= 1U<<21;
							printf("%d", ms);
						} else {
							led_off_trigger = 0;
							ms = 0;
							//unsigned *p1 = &ms;
							//unsigned *p2 = &led_off_trigger;
							tim2_start(&ms, &led_off_trigger);
							GPIOA->BSRR |= 1U<<5;
						}

					}
					button_pressed = 0;
				}
	}
}
