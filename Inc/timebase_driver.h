/**
 ******************************************************************************
 * @file           : timebase_driver.h
 * @brief          : Software timebase using TIM2 update interrupt.
 ******************************************************************************
 * @attention
 *
 * M04 timebase driver.
 * Uses a 1 ms interrupt tick from TIM2.
 *
 ******************************************************************************
 */

#ifndef TIMEBASE_DRIVER_H_
#define TIMEBASE_DRIVER_H_

#include <stdint.h>

/* Exported typedef */

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void TIMEBASE_Init(void);
uint32_t TIMEBASE_GetMilliseconds(void);

#endif
