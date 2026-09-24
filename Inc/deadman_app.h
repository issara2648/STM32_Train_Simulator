/**
 ******************************************************************************
 * @file           : deadman_app.h
 * @brief          : Deadman application logic for the M08 diagnostic countdown.
 ******************************************************************************
 */

#ifndef DEADMAN_APP_H_
#define DEADMAN_APP_H_

#include <stdint.h>

/* Exported typedef */

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void DEADMAN_Init(void);
void DEADMAN_Start(uint32_t current_time_ms);
void DEADMAN_Update(uint32_t current_time_ms);
uint32_t DEADMAN_IsTimedOut(void);

#endif
