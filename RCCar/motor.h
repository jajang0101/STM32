#include "Inc/stm32f030x8.h"
#define CK_INT 8000000U//TIM3 internal clock
#define SG90_PERIOD 20U//SG90 servo motor PWM period

void Motor_Init();

void Motor_Steer_Left();
void Motor_Steer_Right();
void Motor_Steer_Center();

void Motor_Forward();
void Motor_Stop();
