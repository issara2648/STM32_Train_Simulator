/**
 ******************************************************************************
 * @file           : ldr_driver.c
 * @brief          : ADC1 driver for LDR on PA1 using interrupt-driven conversion
 ******************************************************************************
 */

#include "ldr_driver.h"

#include "stm32f411xe.h"

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define LDR_ADC_CHANNEL                 (1U)
#define LDR_SAMPLE_BUFFER_EMPTY         (0U)
#define LDR_SAMPLE_BUFFER_FULL          (1U)

/* Private macro */

/* Private variables */
static volatile uint32_t ldr_latest_sample = 0U;
static volatile uint32_t ldr_sample_ready = 0U;

/* Private function prototypes */

/* Public functions */
void LDR_Init(void)
{
    uint32_t moder_value;

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure PA1 as analog mode (MODER = 0b11) */
    moder_value = GPIOA->MODER;
    moder_value &= ~GPIO_MODER_MODER1_Msk;
    moder_value |= (0x03U << GPIO_MODER_MODER1_Pos);
    GPIOA->MODER = moder_value;

    /* Enable ADC1 clock */
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /* Ensure ADC is off before configuration */
    ADC1->CR2 &= ~ADC_CR2_ADON;

    /* Select channel LDR_ADC_CHANNEL as first in regular sequence */
    ADC1->SQR3 &= ~ADC_SQR3_SQ1_Msk;
    ADC1->SQR3 |= (LDR_ADC_CHANNEL << ADC_SQR3_SQ1_Pos);

    /* Configure sample time for channel1 in SMPR2 (use a small sample time) */
    ADC1->SMPR2 &= ~ADC_SMPR2_SMP1_Msk;
    ADC1->SMPR2 |= (0x00U << ADC_SMPR2_SMP1_Pos);

    /* Enable end-of-conversion interrupt */
    ADC1->CR1 |= ADC_CR1_EOCIE;

    /* Enable NVIC for ADC
     * Priority chosen to allow ADC ISR to run but not preempt critical timing.
     */
    NVIC_SetPriority(ADC_IRQn, 6U);
    NVIC_EnableIRQ(ADC_IRQn);

    /* Enable ADC */
    ADC1->CR2 |= ADC_CR2_ADON;

    /* Initialize sample state */
    ldr_latest_sample = 0U;
    ldr_sample_ready = LDR_SAMPLE_BUFFER_EMPTY;
}

void LDR_StartConversion(void)
{
    /* Start a single conversion by software trigger (SWSTART) */
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint32_t LDR_GetRawValue(void)
{
    uint32_t result = 0U;

    if (ldr_sample_ready == LDR_SAMPLE_BUFFER_EMPTY)
    {
        return 0U;
    }

    __disable_irq();
    result = ldr_latest_sample;
    ldr_sample_ready = LDR_SAMPLE_BUFFER_EMPTY;
    __enable_irq();

    return result;
}

uint32_t LDR_IsSampleAvailable(void)
{
    return ldr_sample_ready;
}

/* ISR for ADC conversion complete */
void ADC_IRQHandler(void)
{
    uint32_t dr_value;

    /* Check EOC flag in SR - read DR to clear */
    if ((ADC1->SR & ADC_SR_EOC) != 0U)
    {
        dr_value = (uint32_t)ADC1->DR;
        ldr_latest_sample = dr_value & 0x0FFFU;
        ldr_sample_ready = LDR_SAMPLE_BUFFER_FULL;
        /* Clear EOC by reading DR (already done) */
    }
}

/* Private functions */
