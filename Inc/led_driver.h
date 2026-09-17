/**
 ******************************************************************************
 * @file           : led_driver.h
 * @author         : STM32 Train Simulator
 * @brief          : LED driver for the four Training Shield onboard LEDs.
 ******************************************************************************
 * @attention
 *
 * Temporary M01 diagnostic driver only.
 *
 ******************************************************************************
 */

#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#include <stdint.h>

/* Exported typedef */

typedef enum LED_Number_e
{
    LED_D10 = 0U,
    LED_D11 = 1U,
    LED_D12 = 2U,
    LED_D13 = 3U
} LED_Number_t;

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void LED_Init(void);
void LED_ClearAll(void);
void LED_SetOnly(LED_Number_t led_number);
void LED_SetState(LED_Number_t led_number, uint8_t led_state);
void LED_RunDiagnosticSequence(void);

#endif
