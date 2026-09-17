/**
 ******************************************************************************
 * @file           : button_driver.c
 * @brief          : GPIO input driver for the four Training Shield buttons.
 ******************************************************************************
 * @attention
 *
 * Temporary M02 diagnostic driver only.
 * Button configuration is a temporary hardware bring-up assumption.
 *
 ******************************************************************************
 */

#include "button_driver.h"

#include "stm32f411xe.h"

/* Private includes */

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define BUTTON_PORTA_ENABLE_MASK        (0x01U << 0U)
#define BUTTON_PORTB_ENABLE_MASK        (0x01U << 1U)
#define BUTTON_D2_PIN_MASK              (0x01U << 10U)
#define BUTTON_D3_PIN_MASK              (0x01U << 3U)
#define BUTTON_D4_PIN_MASK              (0x01U << 5U)
#define BUTTON_D5_PIN_MASK              (0x01U << 4U)
#define BUTTON_INPUT_MODE               (0x00U)
#define BUTTON_PULL_UP                  (0x01U)
#define BUTTON_D2_BIT                   (0U)
#define BUTTON_D3_BIT                   (1U)
#define BUTTON_D4_BIT                   (2U)
#define BUTTON_D5_BIT                   (3U)
#define BUTTON_PRESSED                  (1U)
#define BUTTON_RELEASED                 (0U)
/* Temporary M02 bring-up assumption: pull-up input, pressed = logic 0. */

/* Private macro */

/* Private constants */

/* Private variables */

/* External variables */

/* Private function prototypes */

/* Private user code */

/* Main function, if applicable */

/* Public functions */
void BUTTON_Init(void)
{
    uint32_t moder_value;
    uint32_t pupdr_value;

    RCC->AHB1ENR |= BUTTON_PORTA_ENABLE_MASK;
    RCC->AHB1ENR |= BUTTON_PORTB_ENABLE_MASK;

    moder_value = GPIOA->MODER;
    moder_value &= ~(GPIO_MODER_MODER10_Msk);
    moder_value |= (BUTTON_INPUT_MODE << GPIO_MODER_MODER10_Pos);
    GPIOA->MODER = moder_value;

    moder_value = GPIOB->MODER;
    moder_value &= ~(GPIO_MODER_MODER3_Msk | GPIO_MODER_MODER4_Msk | GPIO_MODER_MODER5_Msk);
    moder_value |= (BUTTON_INPUT_MODE << GPIO_MODER_MODER3_Pos);
    moder_value |= (BUTTON_INPUT_MODE << GPIO_MODER_MODER4_Pos);
    moder_value |= (BUTTON_INPUT_MODE << GPIO_MODER_MODER5_Pos);
    GPIOB->MODER = moder_value;

    pupdr_value = GPIOA->PUPDR;
    pupdr_value &= ~(GPIO_PUPDR_PUPD10_Msk);
    pupdr_value |= (BUTTON_PULL_UP << GPIO_PUPDR_PUPD10_Pos);
    GPIOA->PUPDR = pupdr_value;

    pupdr_value = GPIOB->PUPDR;
    pupdr_value &= ~(GPIO_PUPDR_PUPD3_Msk | GPIO_PUPDR_PUPD4_Msk | GPIO_PUPDR_PUPD5_Msk);
    pupdr_value |= (BUTTON_PULL_UP << GPIO_PUPDR_PUPD3_Pos);
    pupdr_value |= (BUTTON_PULL_UP << GPIO_PUPDR_PUPD4_Pos);
    pupdr_value |= (BUTTON_PULL_UP << GPIO_PUPDR_PUPD5_Pos);
    GPIOB->PUPDR = pupdr_value;
}

uint32_t BUTTON_ReadAll(void)
{
    uint32_t button_state;
    uint32_t raw_state;

    button_state = 0U;
    raw_state = 0U;

    raw_state = GPIOA->IDR;
    if ((raw_state & BUTTON_D2_PIN_MASK) == 0U)
    {
        button_state |= (BUTTON_PRESSED << BUTTON_D2_BIT);
    }

    raw_state = GPIOB->IDR;
    if ((raw_state & BUTTON_D3_PIN_MASK) == 0U)
    {
        button_state |= (BUTTON_PRESSED << BUTTON_D3_BIT);
    }
    if ((raw_state & BUTTON_D4_PIN_MASK) == 0U)
    {
        button_state |= (BUTTON_PRESSED << BUTTON_D4_BIT);
    }
    if ((raw_state & BUTTON_D5_PIN_MASK) == 0U)
    {
        button_state |= (BUTTON_PRESSED << BUTTON_D5_BIT);
    }

    return button_state;
}

uint8_t BUTTON_IsPressed(Button_Number_t button_number)
{
    uint32_t button_state;
    uint8_t pressed_status;

    button_state = BUTTON_ReadAll();
    pressed_status = BUTTON_RELEASED;

    switch (button_number)
    {
        case BUTTON_D2:
            if ((button_state & (BUTTON_PRESSED << BUTTON_D2_BIT)) != 0U)
            {
                pressed_status = BUTTON_PRESSED;
            }
            break;

        case BUTTON_D3:
            if ((button_state & (BUTTON_PRESSED << BUTTON_D3_BIT)) != 0U)
            {
                pressed_status = BUTTON_PRESSED;
            }
            break;

        case BUTTON_D4:
            if ((button_state & (BUTTON_PRESSED << BUTTON_D4_BIT)) != 0U)
            {
                pressed_status = BUTTON_PRESSED;
            }
            break;

        case BUTTON_D5:
            if ((button_state & (BUTTON_PRESSED << BUTTON_D5_BIT)) != 0U)
            {
                pressed_status = BUTTON_PRESSED;
            }
            break;

        default:
            pressed_status = BUTTON_RELEASED;
            break;
    }

    return pressed_status;
}

/* Callback functions */

/* Private functions */
