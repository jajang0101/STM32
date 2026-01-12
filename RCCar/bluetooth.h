#define STM32F030x8
#include "Inc/stm32f030x8.h"
#define FCK 8000000U
#define BAUDRATE 9600U

void Bluetooth_Init(unsigned baudrate);
int Bluetooth_RXNE();
uint8_t Bluetooth_ReadKey();
