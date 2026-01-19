#define STM32F030x8
#include "Inc/stm32f030x8.h"
#define FCK 8000000U
#define BAUDRATE 9600U

void uart_init(unsigned baudrate);
int uart_rxne();
uint8_t uart_readKey();
