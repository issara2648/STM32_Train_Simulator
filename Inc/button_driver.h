/**
 ******************************************************************************
 * @file           : button_driver.h
 * @author         : STM32 Train Simulator
 * @brief          : Training Shield push button driver for M02 diagnostic.
 ******************************************************************************
 * @attention
 *
 * Temporary M02 diagnostic driver only.
 * Button polarity and pull configuration are temporary bring-up values.
 *
 ******************************************************************************
 */

#ifndef BUTTON_DRIVER_H_
#define BUTTON_DRIVER_H_

#include <stdint.h>

/* Exported typedef */
typedef enum Button_Number_e
{
    BUTTON_D2 = 0U,
    BUTTON_D3 = 1U,
    BUTTON_D4 = 2U,
    BUTTON_D5 = 3U
} Button_Number_t;

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void BUTTON_Init(void);
uint32_t BUTTON_ReadAll(void);
uint8_t BUTTON_IsPressed(Button_Number_t button_number);

#endif
