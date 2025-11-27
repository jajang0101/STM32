#include "include\stm32f103xb.h"
#define TIMER_CLK 36000U//in kHz

void tim2_init(unsigned arr);
int tim2_update();
unsigned* tim2_start(unsigned* ms, unsigned* finish);
