#include "uart.h"

void uart_init(unsigned baudrate){
    //PB6 AF0: USART1_TX, PB7 AF0: USART1_RX
    RCC->APB2ENR |= 1U<<14;//USART1 ENR
    RCC->AHBENR |= 1U<<18;//GPIOB ENR

    GPIOB->MODER &= ~(1U<<12);
    GPIOB->MODER |= 1U<<13;
    GPIOB->MODER &= ~(1U<<14);
    GPIOB->MODER |= 1U<<15;

    GPIOB->AFR[0] |= 0x0U;

    USART1->BRR = FCK/baudrate;
    USART1->CR1 = 0x0U;
    USART1->CR1 |= 1U|(1U<<2)|(1U<<3);
}

int uart_rxne(){
	if (USART1->ISR & USART_ISR_ORE) {
	        USART1->ICR |= USART_ICR_ORECF; // Clear the ORE flag
	}
    return (USART1->ISR & USART_ISR_RXNE);
}
uint8_t uart_readKey(){
    return ((uint8_t)(USART1->RDR)) ;
}

int __io_putchar(int ch)
 {
    while(!(USART1->ISR & USART_ISR_TXE)){}
    USART1->TDR = (ch & 0xFF);
    return ch;
 }
