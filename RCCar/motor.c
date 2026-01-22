#include "motor.h"

void Motor_Init(){
    //PA6 AF1: TIM3_CH1, PA7 AF1: TIM3_CH2
    RCC->APB1ENR |= 1U<<1;//TIM3 ENR
    RCC->AHBENR |= 1U<<17;//GPIOA ENR

    //PA0, PA1 Output
    GPIOA->MODER |= 1U;
    GPIOA->MODER &= ~(1U<<1);
    GPIOA->MODER |= 1U<<2;
    GPIOA->MODER &= ~(1U<<3);
    //PA6, PA7 AF1
    GPIOA->MODER &= ~(1U<<12);
    GPIOA->MODER |= 1U<<13;
    GPIOA->MODER &= ~(1U<<14);
    GPIOA->MODER |= 1U<<15;

    GPIOA->AFR[0] |= 0x0U;
    GPIOA->AFR[0] |= 1U<<24;
    GPIOA->AFR[0] |= 1U<<28;

    //1kHz
    TIM3->PSC = (CK_INT/1000000U)-1;
    TIM3->ARR = SG90_PERIOD*1000U;

    TIM3->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE);
    TIM3->CCR1 = 1500U;
    TIM3->CCMR1 |= (TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE);
    TIM3->CCR2 = 0U;

    TIM3->CCER |= (TIM_CCER_CC1E | TIM_CCER_CC2E);
    TIM3->CR1  |= TIM_CR1_CEN;
}

void Motor_Steer_Left(){
    TIM3->CCR1 = 2000U;
}
void Motor_Steer_Right(){
    TIM3->CCR1 = 1000U;
}
void Motor_Steer_Center(){
    TIM3->CCR1 = 1500U;
}

void Motor_Forward(){
    int dutyCycle = TIM3->CCR2;
    if (dutyCycle < SG90_PERIOD*900){
        TIM3->CCR2 = dutyCycle + 2000;
    }
    GPIOA->ODR |= 1U;
    GPIOA->ODR &= ~(1U<<1);
}
void Motor_Stop(){
    TIM3->CCR2 = 0;
    GPIOA->ODR &= ~(1U);
    GPIOA->ODR &= ~(1U<<1);
}
uint8_t TI3_Update(){
    if(TIM3->SR & 1U){
        TIM3->SR &= ~(1U);
        return 1U;
    } else {
        return 0U;
    }
}
