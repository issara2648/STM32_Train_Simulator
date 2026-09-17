/**
 ******************************************************************************
 * @file           : timebase_driver.c
 * @brief          : System timebase based on TIM2 update interrupt.
 ******************************************************************************
 * @attention
 *
 * M04 minimal project-owned system clock configuration.
 * HSI at 16 MHz is used as the M04 clock basis.
 *
 ******************************************************************************
 */

#include "timebase_driver.h"

#include "stm32f411xe.h"

/* Private includes */

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define TIMEBASE_HSI_STARTUP_TIMEOUT      (1000U)
#define TIMEBASE_TIM2_PRESCALER_VALUE     (0U)
#define TIMEBASE_TIM2_AUTO_RELOAD_VALUE   (15999U)
#define TIMEBASE_MS_PER_SECOND            (1000U)

/* Private macro */

/* Private constants */

/* Private variables */
volatile uint32_t g_timebase_milliseconds = 0U;

/* External variables */

/* Private function prototypes */

/* Private user code */

void SystemInit(void)
{
    uint32_t config_value;
    uint32_t timeout_counter;

    timeout_counter = 0U;

    RCC->CR |= RCC_CR_HSION;
    while (((RCC->CR & RCC_CR_HSIRDY) == 0U) && (timeout_counter < TIMEBASE_HSI_STARTUP_TIMEOUT))
    {
        timeout_counter++;
    }

    config_value = RCC->CFGR;
    config_value &= ~(RCC_CFGR_SW_Msk | RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk);
    config_value |= RCC_CFGR_SW_HSI;
    config_value |= RCC_CFGR_HPRE_DIV1;
    config_value |= RCC_CFGR_PPRE1_DIV1;
    RCC->CFGR = config_value;

    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI)
    {
    }
}

/* Main function, if applicable */

/* Public functions */
void TIMEBASE_Init(void)
{
    uint32_t config_value;
    uint32_t timeout_counter;

    timeout_counter = 0U;

    RCC->CR |= RCC_CR_HSION;
    while (((RCC->CR & RCC_CR_HSIRDY) == 0U) && (timeout_counter < TIMEBASE_HSI_STARTUP_TIMEOUT))
    {
        timeout_counter++;
    }

    config_value = RCC->CFGR;
    config_value &= ~(RCC_CFGR_SW_Msk | RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk);
    config_value |= RCC_CFGR_SW_HSI;
    config_value |= RCC_CFGR_HPRE_DIV1;
    config_value |= RCC_CFGR_PPRE1_DIV1;
    RCC->CFGR = config_value;

    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) != RCC_CFGR_SWS_HSI)
    {
    }

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    TIM2->CR1 = 0U;
    TIM2->CNT = 0U;
    TIM2->PSC = TIMEBASE_TIM2_PRESCALER_VALUE;
    TIM2->ARR = TIMEBASE_TIM2_AUTO_RELOAD_VALUE;
    TIM2->EGR = TIM_EGR_UG;
    TIM2->SR = 0U;
    TIM2->DIER = TIM_DIER_UIE;

    NVIC_EnableIRQ(TIM2_IRQn);

    TIM2->CR1 |= TIM_CR1_CEN;
}

uint32_t TIMEBASE_GetMilliseconds(void)
{
    return g_timebase_milliseconds;
}

/* Callback functions */

/* Private functions */
void TIM2_IRQHandler(void)
{
    if ((TIM2->SR & TIM_SR_UIF) != 0U)
    {
        TIM2->SR &= ~TIM_SR_UIF;
        g_timebase_milliseconds++;
    }
}
