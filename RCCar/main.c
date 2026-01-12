#define STM32F030x8
#include "Inc\stm32f030x8.h"
#include "bluetooth.h"
#include "motor.h"
#include "led.h"

int main(void) {
    Bluetooth_Init(9600);
    Motor_Init();
    LED_Init();
    LED_Switch();
    while (1) {
    	if (Bluetooth_RXNE()){
    		unsigned char key = Bluetooth_ReadKey();
    		if (key == 'w'){
    			Motor_Forward();
    		} else if (key == 's'){
    			Motor_Stop();
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
    	//for (int i = 0; i < 1000; i++){}
    }
}
