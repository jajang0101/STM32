#include "mpu6050.h"

void I2C1_Init(){
    //PB8 AF1: I2C1_SCL, PB9 AF1: I2C1_SDA
    RCC->AHBENR |= 1U<<18;//GPIOB ENR
    RCC->APB1ENR |= 1U<<21;//I2C1 ENR

    GPIOB->MODER &= ~(1U<<16);
    GPIOB->MODER |= 1U<<17;
    GPIOB->MODER &= ~(1U<<18);
    GPIOB->MODER |= 1U<<19;

    GPIOB->AFR[1] = 0x0U;
    GPIOB->AFR[1] |= 1U;
    GPIOB->AFR[1] |= 1U<<4;

    GPIOB->OTYPER |= 1U<<8;
    GPIOB->OTYPER |= 1U<<9;
    GPIOB->PUPDR |= 1U<<16;
    GPIOB->PUPDR |= 1U<<18;

    I2C1->CR1 &= ~I2C_CR1_PE;
    I2C1->TIMINGR = (0x1U<<28) | (0x4U<<20) | (0x2U<<16) | (0xFU<<8) | (0x13U);
    I2C1->CR1 |= I2C_CR1_PE;
}

void I2C1_Write(uint8_t devAddr, uint8_t regAddr, uint8_t data){
	while (I2C1->ISR & I2C_ISR_BUSY);
	I2C1->CR2 = devAddr | (2U << 16) | I2C_CR2_START | I2C_CR2_AUTOEND;

	while (!(I2C1->ISR & I2C_ISR_TXIS)) {
		if (I2C1->ISR & I2C_ISR_NACKF) { //Check for NACK (No Acknowledge)
			I2C1->ICR |= I2C_ICR_NACKCF;
			return; //Error occurred, stop trying to write
		}
	}
	I2C1->TXDR = regAddr;

	while (!(I2C1->ISR & I2C_ISR_TXIS)) {
		if (I2C1->ISR & I2C_ISR_NACKF) {
			I2C1->ICR |= I2C_ICR_NACKCF;
			return;
		}
	}
	I2C1->TXDR = data;

	while (!(I2C1->ISR & I2C_ISR_STOPF));
	I2C1->ICR |= I2C_ICR_STOPCF;
}

void I2C1_Read(uint8_t devAddr, uint8_t regAddr, uint8_t *buffer, uint8_t size) {
    while (I2C1->ISR & I2C_ISR_BUSY);

    I2C1->CR2 = (devAddr) | (1U << 16) | I2C_CR2_START;

    while (!(I2C1->ISR & I2C_ISR_TXIS));
    I2C1->TXDR = regAddr;
    while (!(I2C1->ISR & I2C_ISR_TC)); //Wait for Transfer Complete

    I2C1->CR2 = (devAddr) | (size << 16) | I2C_CR2_RD_WRN | I2C_CR2_START | I2C_CR2_AUTOEND;

    for (uint8_t i = 0; i < size; i++) {
        while (!(I2C1->ISR & I2C_ISR_RXNE));
        buffer[i] = I2C1->RXDR;
    }

    while (!(I2C1->ISR & I2C_ISR_STOPF));
    I2C1->ICR |= I2C_ICR_STOPCF;
}
uint8_t MPU6050_Init(){
	I2C1_Init();

	I2C1_Write(MPU6050_ADDR, 0x6B, 0x80); //PWR_MGMT_1
	for(volatile int i=0; i<500000; i++); 

	//PWR_MGMT_1
	I2C1_Write(MPU6050_ADDR, 0x6B, 0x00);
	I2C1_Write(MPU6050_ADDR, 0x19, 7U);  //SMPLRT_DIV
	I2C1_Write(MPU6050_ADDR, 0x1B, 0U);  //GYRO_CONFIG
	I2C1_Write(MPU6050_ADDR, 0x1C, 0U);  //ACCEL_CONFIG

	uint8_t addr = 0;
	I2C1_Read(MPU6050_ADDR, 0x75, &addr, 1);//WHO_AM_I
	return addr;
}
void MPU6050_GetData(float data_all[3]){
    uint8_t data_raw[14];
    I2C1_Read(MPU6050_ADDR, 0x3B, data_raw, 14);
    int16_t a_x = (int16_t)(data_raw[0] << 8 | data_raw[1]);//Accel X
    int16_t a_y = (int16_t)(data_raw[2] << 8 | data_raw[3]);//Accel Y
    int16_t g_z  = (int16_t)(data_raw[12] << 8 | data_raw[13]);//Gyro Z
    //int16_t a_z  = (int16_t)(data_raw[4] << 8 | data_raw[5]);//Accel Z

    data_all[0] = (float)a_x/16384.0;
    data_all[1] = (float)a_y/16384.0;
    data_all[2] = (float)g_z/131.0;
    //data_all[2] = (float)a_z/16384.0;
}
