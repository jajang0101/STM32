#define PERIPH_BASE 0x40000000UL
#define GPIOA_OFFSET 0x00010800UL
#define GPIOC_OFFSET 0x00011000UL
#define GPIOA_BASE PERIPH_BASE + GPIOA_OFFSET
#define GPIOC_BASE PERIPH_BASE + GPIOC_OFFSET
#define RCC_OFFSET 0x00021000UL
#define RCC_BASE PERIPH_BASE + RCC_OFFSET
#define APB2EN_R_OFFSET 0x18UL
#define RCC_APB2EN_R *(volatile unsigned int *)(RCC_BASE + APB2EN_R_OFFSET)
#define A_MODE_R_OFFSET 0x00UL
#define C_MODE_R_OFFSET 0x04UL
#define GPIOA_MODE_R *(volatile unsigned int *)(GPIOA_BASE + A_MODE_R_OFFSET)
#define GPIOC_MODE_R *(volatile unsigned int *)(GPIOC_BASE + C_MODE_R_OFFSET)
#define OD_R_OFFSET 0x0CUL
#define GPIOA_OD_R *(volatile unsigned int *)(GPIOA_BASE + OD_R_OFFSET)
#define ID_R_OFFSET 0x08UL
#define GPIOC_ID_R *(volatile unsigned int *)(GPIOC_BASE + ID_R_OFFSET)
#define BSRR_OFFSET 0x10UL
#define GPIOA_BSRR *(volatile unsigned int*)(GPIOA_BASE + BSRR_OFFSET)