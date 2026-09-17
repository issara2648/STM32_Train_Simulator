/**
 ******************************************************************************
 * @file           : led_driver.c
 * @brief          : LED GPIO driver for the four Training Shield LEDs.
 ******************************************************************************
 * @attention
 *
 * Temporary M01 diagnostic driver only.
 * No simulator logic is included.
 *
 ******************************************************************************
 */

#include "led_driver.h"

#include "stm32f411xe.h"

/* Private includes */

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define LED_PORTA_ENABLE_MASK          (0x01U << 0U)
#define LED_PORTB_ENABLE_MASK          (0x01U << 1U)
#define LED_PIN5_OUTPUT_MASK           (0x01U << 5U)
#define LED_PIN6_OUTPUT_MASK           (0x01U << 6U)
#define LED_PIN7_OUTPUT_MASK           (0x01U << 7U)
#define LED_4_LED_COUNT                (4U)
#define LED_GPIO_MODE_OUTPUT           (0x01U)

/* Private macro */

/* Private constants */

/* Private variables */

/* External variables */

/* Private function prototypes */

/* Private user code */

/* Main function, if applicable */

/* Public functions */
void LED_Init(void)
{
    uint32_t moder_value;
    uint32_t odr_value;

    RCC->AHB1ENR |= LED_PORTA_ENABLE_MASK;
    RCC->AHB1ENR |= LED_PORTB_ENABLE_MASK;

    moder_value = GPIOA->MODER;
    moder_value &= ~(GPIO_MODER_MODER5_Msk | GPIO_MODER_MODER6_Msk | GPIO_MODER_MODER7_Msk);
    moder_value |= (LED_GPIO_MODE_OUTPUT << GPIO_MODER_MODER5_Pos);
    moder_value |= (LED_GPIO_MODE_OUTPUT << GPIO_MODER_MODER6_Pos);
    moder_value |= (LED_GPIO_MODE_OUTPUT << GPIO_MODER_MODER7_Pos);
    GPIOA->MODER = moder_value;

    moder_value = GPIOB->MODER;
    moder_value &= ~GPIO_MODER_MODER6_Msk;
    moder_value |= (LED_GPIO_MODE_OUTPUT << GPIO_MODER_MODER6_Pos);
    GPIOB->MODER = moder_value;

    odr_value = GPIOA->ODR;
    odr_value &= ~(LED_PIN5_OUTPUT_MASK | LED_PIN6_OUTPUT_MASK | LED_PIN7_OUTPUT_MASK);
    GPIOA->ODR = odr_value;

    odr_value = GPIOB->ODR;
    odr_value &= ~LED_PIN6_OUTPUT_MASK;
    GPIOB->ODR = odr_value;
}

void LED_ClearAll(void)
{
    GPIOA->ODR &= ~(LED_PIN5_OUTPUT_MASK | LED_PIN6_OUTPUT_MASK | LED_PIN7_OUTPUT_MASK);
    GPIOB->ODR &= ~LED_PIN6_OUTPUT_MASK;
}

void LED_SetOnly(LED_Number_t led_number)
{
    LED_ClearAll();

    switch (led_number)
    {
        case LED_D10:
            GPIOB->ODR |= LED_PIN6_OUTPUT_MASK;
            break;

        case LED_D11:
            GPIOA->ODR |= LED_PIN7_OUTPUT_MASK;
            break;

        case LED_D12:
            GPIOA->ODR |= LED_PIN6_OUTPUT_MASK;
            break;

        case LED_D13:
            GPIOA->ODR |= LED_PIN5_OUTPUT_MASK;
            break;

        default:
            break;
    }
}

void LED_SetState(LED_Number_t led_number, uint8_t led_state)
{
    switch (led_number)
    {
        case LED_D10:
            if (led_state != 0U)
            {
                GPIOB->ODR |= LED_PIN6_OUTPUT_MASK;
            }
            else
            {
                GPIOB->ODR &= ~LED_PIN6_OUTPUT_MASK;
            }
            break;

        case LED_D11:
            if (led_state != 0U)
            {
                GPIOA->ODR |= LED_PIN7_OUTPUT_MASK;
            }
            else
            {
                GPIOA->ODR &= ~LED_PIN7_OUTPUT_MASK;
            }
            break;

        case LED_D12:
            if (led_state != 0U)
            {
                GPIOA->ODR |= LED_PIN6_OUTPUT_MASK;
            }
            else
            {
                GPIOA->ODR &= ~LED_PIN6_OUTPUT_MASK;
            }
            break;

        case LED_D13:
            if (led_state != 0U)
            {
                GPIOA->ODR |= LED_PIN5_OUTPUT_MASK;
            }
            else
            {
                GPIOA->ODR &= ~LED_PIN5_OUTPUT_MASK;
            }
            break;

        default:
            break;
    }
}

void LED_RunDiagnosticSequence(void)
{
    uint32_t led_index;

    for (led_index = 0U; led_index < LED_4_LED_COUNT; led_index++)
    {
        LED_SetOnly((LED_Number_t)led_index);

        /* Temporary M01 diagnostic delay. This is not application timing. */
        for (volatile uint32_t delay = 0U; delay < 100000U; delay++)
        {
        }
    }
}

/* Callback functions */

/* Private functions */
