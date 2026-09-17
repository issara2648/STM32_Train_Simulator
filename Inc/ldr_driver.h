/**
 ******************************************************************************
 * @file           : ldr_driver.h
 * @brief          : LDR ADC driver (ADC1, PA1) using interrupt-driven sampling
 ******************************************************************************
 */

#ifndef LDR_DRIVER_H_
#define LDR_DRIVER_H_

#include <stdint.h>

/* Exported typedef */

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void LDR_Init(void);
void LDR_StartConversion(void);
uint32_t LDR_GetRawValue(void);
uint32_t LDR_IsSampleAvailable(void);

#endif
