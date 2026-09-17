/**
 ******************************************************************************
 * @file           : uart_driver.h
 * @brief          : USART2 interrupt-driven UART driver API
 ******************************************************************************
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <stdint.h>

/* Exported typedef */

/* Exported define */

/* Exported macro */

/* Exported constants */

/* Exported variables */

/* Exported function prototypes */
void UART_Init(void);
uint32_t UART_IsRxAvailable(void);
char UART_ReadChar(void);
uint32_t UART_SendString(const char *text);

#endif
