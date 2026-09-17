/**
 ******************************************************************************
 * @file           : seven_segment_driver.c
 * @brief          : GPIO output driver for the 4-bit Training Shield 7-segment display.
 ******************************************************************************
 * @attention
 *
 * Temporary M03 diagnostic driver only.
 * This driver writes a 4-bit binary value to the shield interface.
 *
 ******************************************************************************
 */

#include "seven_segment_driver.h"

#include "stm32f411xe.h"

/* Private includes */

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define SEG7_PORTA_ENABLE_MASK          (0x01U << 0U)
#define SEG7_PORTB_ENABLE_MASK          (0x01U << 1U)
#define SEG7_PORTC_ENABLE_MASK          (0x01U << 2U)
#define SEG7_BIT0_PIN_MASK              (0x01U << 7U)
#define SEG7_BIT1_PIN_MASK              (0x01U << 8U)
#define SEG7_BIT2_PIN_MASK              (0x01U << 10U)
#define SEG7_BIT3_PIN_MASK              (0x01U << 9U)
#define SEG7_GPIO_MODE_OUTPUT           (0x01U)
#define SEG7_DIGIT_MIN                  (0U)
#define SEG7_DIGIT_MAX                  (9U)
#define SEG7_VALID_MASK                 (0x0FU)
#define SEG7_DEFAULT_VALUE              (0U)

/* Private macro */

/* Private constants */

/* Private variables */

/* External variables */

/* Private function prototypes */

/* Private user code */

/* Main function, if applicable */

/* Public functions */
void SEG7_Init(void)
{
    uint32_t moder_value;
    uint32_t odr_value;

    RCC->AHB1ENR |= SEG7_PORTA_ENABLE_MASK;
    RCC->AHB1ENR |= SEG7_PORTB_ENABLE_MASK;
    RCC->AHB1ENR |= SEG7_PORTC_ENABLE_MASK;

    moder_value = GPIOC->MODER;
    moder_value &= ~GPIO_MODER_MODER7_Msk;
    moder_value |= (SEG7_GPIO_MODE_OUTPUT << GPIO_MODER_MODER7_Pos);
    GPIOC->MODER = moder_value;

    moder_value = GPIOA->MODER;
    moder_value &= ~(GPIO_MODER_MODER8_Msk | GPIO_MODER_MODER9_Msk);
    moder_value |= (SEG7_GPIO_MODE_OUTPUT << GPIO_MODER_MODER8_Pos);
    moder_value |= (SEG7_GPIO_MODE_OUTPUT << GPIO_MODER_MODER9_Pos);
    GPIOA->MODER = moder_value;

    moder_value = GPIOB->MODER;
    moder_value &= ~GPIO_MODER_MODER10_Msk;
    moder_value |= (SEG7_GPIO_MODE_OUTPUT << GPIO_MODER_MODER10_Pos);
    GPIOB->MODER = moder_value;

    odr_value = GPIOC->ODR;
    odr_value &= ~SEG7_BIT0_PIN_MASK;
    GPIOC->ODR = odr_value;

    odr_value = GPIOA->ODR;
    odr_value &= ~(SEG7_BIT1_PIN_MASK | SEG7_BIT3_PIN_MASK);
    GPIOA->ODR = odr_value;

    odr_value = GPIOB->ODR;
    odr_value &= ~SEG7_BIT2_PIN_MASK;
    GPIOB->ODR = odr_value;
}

void SEG7_DisplayDigit(uint32_t digit_value)
{
    uint32_t display_value;

    if ((digit_value > SEG7_DIGIT_MAX) || (digit_value < SEG7_DIGIT_MIN))
    {
        display_value = SEG7_DEFAULT_VALUE;
    }
    else
    {
        display_value = digit_value & SEG7_VALID_MASK;
    }

    if ((display_value & 0x01U) != 0U)
    {
        GPIOC->ODR |= SEG7_BIT0_PIN_MASK;
    }
    else
    {
        GPIOC->ODR &= ~SEG7_BIT0_PIN_MASK;
    }

    if ((display_value & 0x02U) != 0U)
    {
        GPIOA->ODR |= SEG7_BIT1_PIN_MASK;
    }
    else
    {
        GPIOA->ODR &= ~SEG7_BIT1_PIN_MASK;
    }

    if ((display_value & 0x04U) != 0U)
    {
        GPIOB->ODR |= SEG7_BIT2_PIN_MASK;
    }
    else
    {
        GPIOB->ODR &= ~SEG7_BIT2_PIN_MASK;
    }

    if ((display_value & 0x08U) != 0U)
    {
        GPIOA->ODR |= SEG7_BIT3_PIN_MASK;
    }
    else
    {
        GPIOA->ODR &= ~SEG7_BIT3_PIN_MASK;
    }
}

/* Callback functions */

/* Private functions */
