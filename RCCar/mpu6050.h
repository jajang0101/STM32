#define STM32F030x8
#include "Inc/stm32f030x8.h"
#define HCLK 8000000U
#define MPU6050_ADDR (0x68<<1U)

uint8_t MPU6050_Init();
void I2C1_Write(uint8_t devAddr, uint8_t regAddr, uint8_t data);
void I2C1_Read(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint8_t size);
void MPU6050_GetData(float data_all[3]);
