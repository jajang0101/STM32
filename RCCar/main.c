#define STM32F030x8
#include "Inc\stm32f030x8.h"
#include <stdio.h>
#include "systick.h"
#include "uart.h"
#include "motor.h"
#include "led.h"
#include "mpu6050.h"

#define DELAY 50
#define WHEEL_SPEED_MAX 0.02

int main(void) {
    uart_init(115200);
    Motor_Init();
    LED_Init();
    SysTick_Init();
	uint8_t mpu6050_addr = MPU6050_Init();

    LED_Switch();
	printf("MPU6050 Address: %d\r\n", mpu6050_addr);


	unsigned char key;
	float sensor_data[3];
	float wheel_rotate = 0;
	float wheel_rotateRate = 0;
	uint32_t last_send_time = 0;
    while (1) {
    	if (uart_rxne()){
    		key = uart_readKey();
    		if (key == 'w'){
    			Motor_Forward();
				if (wheel_rotateRate <= WHEEL_SPEED_MAX){
					wheel_rotateRate += 0.008;
				}
				
    		} else if (key == 's'){
    			Motor_Stop();
				wheel_rotateRate = 0;
    		}
    		if (key == 'a'){
    			Motor_Steer_Left();
    		} else if (key == 'd'){
    			Motor_Steer_Right();
    		} else if (key == 'c') {
				Motor_Steer_Center();
			}else {
    			//Motor_Steer_Center();
    		}

    	}

		if (SysTick_getTick() - last_send_time >= 100){
			last_send_time = SysTick_getTick();
			wheel_rotate += wheel_rotateRate;

			//Accel_X Accel_Y Gyro_Z Wheel rotation count
			MPU6050_GetData(sensor_data);
			printf("%f %f %f %f\r\n", sensor_data[0], sensor_data[1], sensor_data[2], wheel_rotate);
			last_send_time = SysTick_getTick();


			//printf("AX: %f | AZ: %f\r\n", sensor_data[0], sensor_data[2]);
		}
		
    }
}
