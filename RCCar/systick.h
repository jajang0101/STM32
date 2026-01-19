

volatile uint32_t ms_ticks = 0;

void SysTick_Handler(void) {
    ms_ticks++;
}

uint32_t SysTick_getTick(void) {
    return ms_ticks;
}

void SysTick_Init(void) {
    //We want an interrupt every 1ms -> 8000000 / 1000 = 8000
    SysTick->LOAD = (8000000U / 1000U) - 1U; 
    
    // Reset current value
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk   | SysTick_CTRL_ENABLE_Msk;
}