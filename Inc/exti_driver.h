/**
 ******************************************************************************
 * @file           : exti_driver.h
 * @brief          : EXTI driver configuration for the Deadman acknowledge input.
 ******************************************************************************
 */

#ifndef EXTI_DRIVER_H_
#define EXTI_DRIVER_H_

#include <stdint.h>

/* Exported typedef */

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void EXTI_DeadmanInit(void);
uint32_t EXTI_DeadmanEventAvailable(void);
void EXTI_ClearDeadmanEvent(void);

#endif
