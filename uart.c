#include <stdint.h>
#include "uart.h"

#define USART_DIV(_PCLK_, _BAUD_)      (((_PCLK_)*25U)/(4U*(_BAUD_)))

#define USART_DIVMANT(_PCLK_, _BAUD_)  (USART_DIV((_PCLK_), (_BAUD_))/100U)

#define USART_DIVFRAQ(_PCLK_, _BAUD_)  (((USART_DIV((_PCLK_), (_BAUD_)) - (USART_DIVMANT((_PCLK_), (_BAUD_)) * 100U)) * 16U + 50U) / 100U)
            
#define USART_BRR(_PCLK_, _BAUD_)      (((USART_DIVMANT((_PCLK_), (_BAUD_)) << 4U) + \
                                        ((USART_DIVFRAQ((_PCLK_), (_BAUD_)) & 0xF0U) << 1U)) + \
                                         (USART_DIVFRAQ((_PCLK_), (_BAUD_)) & 0x0FU))

void uart_init(void){
	RCC->APB2ENR |= 1U<<2;
	RCC->APB1ENR |= 1U<<17;//UART2 enable

	GPIOA->CRL &= ~(0xFUL << 8);
	GPIOA->CRL |= (1U << 9);
	GPIOA->CRL |= (1U << 11);

    /*unsigned uartdiv = APB1_CLK/USART_BAUDRATE;
    unsigned mantissa = uartdiv / 16;
    unsigned fraction = uartdiv % 16;
    USART2_BRR_R = (mantissa << 4) | fraction;*/
	/*uint32_t divier = (APB1_CLK + (USART_BAUDRATE/2U))/USART_BAUDRATE;
	USART2->BRR = divier;*/
    USART2->BRR = USART_BRR(36000000, 115200);

    USART2->CR1 |= 1U<<3;
    USART2->CR1 |= 1U<<13;
}

int __io_putchar(int ch)
 {
    while(!(USART2->SR & (1U<<7))){}
    USART2->DR = (ch & 0xFF);
    return ch;
 }
