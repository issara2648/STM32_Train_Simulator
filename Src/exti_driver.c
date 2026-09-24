/**
 ******************************************************************************
 * @file           : exti_driver.c
 * @brief          : EXTI configuration for the Deadman acknowledge input on PA10.
 ******************************************************************************
 */

#include "exti_driver.h"

#include "stm32f411xe.h"

/* Private includes */

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define EXTI_DEADMAN_GPIOA_ENABLE_MASK    (0x01U << 0U)
#define EXTI_DEADMAN_INPUT_MODE          (0x00U)
#define EXTI_DEADMAN_PULL_UP             (0x01U)
#define EXTI_DEADMAN_PIN_MASK            (0x01U << 10U)
#define EXTI_DEADMAN_LINE_MASK           (0x01U << 10U)
#define EXTI_DEADMAN_PRIORITY            (6U)

/* Private macro */

/* Private constants */

/* Private variables */
static volatile uint32_t g_exti_deadman_event = 0U;

/* External variables */

/* Private function prototypes */

/* Private user code */

/* Main function, if applicable */

/* Public functions */
void EXTI_DeadmanInit(void)
{
    uint32_t moder_value;
    uint32_t pupdr_value;

    RCC->AHB1ENR |= EXTI_DEADMAN_GPIOA_ENABLE_MASK;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    moder_value = GPIOA->MODER;
    moder_value &= ~GPIO_MODER_MODER10_Msk;
    moder_value |= (EXTI_DEADMAN_INPUT_MODE << GPIO_MODER_MODER10_Pos);
    GPIOA->MODER = moder_value;

    pupdr_value = GPIOA->PUPDR;
    pupdr_value &= ~GPIO_PUPDR_PUPD10_Msk;
    pupdr_value |= (EXTI_DEADMAN_PULL_UP << GPIO_PUPDR_PUPD10_Pos);
    GPIOA->PUPDR = pupdr_value;

    SYSCFG->EXTICR[2U] &= ~SYSCFG_EXTICR3_EXTI10_Msk;
    SYSCFG->EXTICR[2U] |= SYSCFG_EXTICR3_EXTI10_PA;

    EXTI->IMR |= EXTI_IMR_MR10;
    EXTI->RTSR &= ~EXTI_RTSR_TR10;
    EXTI->FTSR |= EXTI_FTSR_TR10;
    EXTI->PR = EXTI_PR_PR10;

    NVIC_SetPriority(EXTI15_10_IRQn, EXTI_DEADMAN_PRIORITY);
    NVIC_EnableIRQ(EXTI15_10_IRQn);

    g_exti_deadman_event = 0U;
}

uint32_t EXTI_DeadmanEventAvailable(void)
{
    uint32_t event_status;

    __disable_irq();
    event_status = g_exti_deadman_event;
    __enable_irq();

    return event_status;
}

void EXTI_ClearDeadmanEvent(void)
{
    __disable_irq();
    g_exti_deadman_event = 0U;
    __enable_irq();
}

/* Callback functions */

/* Private functions */
void EXTI15_10_IRQHandler(void)
{
    if ((EXTI->PR & EXTI_DEADMAN_LINE_MASK) != 0U)
    {
        EXTI->PR = EXTI_DEADMAN_LINE_MASK;
        g_exti_deadman_event = 1U;
    }
}
