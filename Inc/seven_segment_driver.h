/**
 ******************************************************************************
 * @file           : seven_segment_driver.h
 * @author         : STM32 Train Simulator
 * @brief          : Training Shield 7-segment output driver.
 ******************************************************************************
 * @attention
 *
 * Temporary M03 diagnostic driver only.
 * The four-bit interface drives the display with a binary value.
 *
 ******************************************************************************
 */

#ifndef SEVEN_SEGMENT_DRIVER_H_
#define SEVEN_SEGMENT_DRIVER_H_

#include <stdint.h>

/* Exported typedef */

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void SEG7_Init(void);
void SEG7_DisplayDigit(uint32_t digit_value);

#endif
