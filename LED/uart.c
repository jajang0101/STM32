#include <stdint.h>
#include "uart.h"


void uart_init(void){
	RCC->APB2ENR |= 1U<<2;
	RCC->APB1ENR |= 1U<<17;//UART2 enable

	GPIOA->CRL &= ~(1U << 8);
	GPIOA->CRL |= (1U << 9);
	GPIOA->CRL &= ~(1U << 10);
	GPIOA->CRL |= (1U << 11);

    unsigned uartdiv = APB1_CLK/USART_BAUDRATE;
    unsigned mantissa = uartdiv / 16U;
    unsigned fraction = uartdiv % 16U;
    USART2->BRR = ((mantissa << 4) | fraction);
    //USART2->BRR = (APB1_CLK + (USART_BAUDRATE / 2u)) / USART_BAUDRATE;

    USART2->CR1 |= 1U<<3;
    USART2->CR1 |= 1U<<13;
}

int __io_putchar(int ch)
 {
    while(!(USART2->SR & (1U<<7))){}
    USART2->DR = (ch & 0xFF);
    return ch;
 }
