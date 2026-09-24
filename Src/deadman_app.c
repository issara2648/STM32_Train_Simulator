/**
 ******************************************************************************
 * @file           : deadman_app.c
 * @brief          : Deadman countdown and timeout diagnostic for M08.
 ******************************************************************************
 */

#include "deadman_app.h"

#include "exti_driver.h"
#include "led_driver.h"
#include "seven_segment_driver.h"
#include "uart_driver.h"

/* Private includes */

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define DEADMAN_TIMEOUT_MS             (10000U)
#define DEADMAN_TICK_MS                (1000U)
#define DEADMAN_START_DIGIT            (9U)
#define DEADMAN_DEBOUNCE_MS            (250U)
#define DEADMAN_TIMEOUT_MESSAGE_SIZE   (18U)

/* Private macro */

/* Private constants */

/* Private variables */
static uint32_t g_deadman_start_ms = 0U;
static uint32_t g_deadman_last_ack_ms = 0U;
static uint32_t g_deadman_timed_out = 0U;
static uint32_t g_deadman_timeout_message_pending = 0U;
static uint32_t g_deadman_timeout_message_sent = 0U;

/* External variables */

/* Private function prototypes */
static void DEADMAN_TriggerTimeout(void);
static void DEADMAN_RequestTimeoutMessage(void);

/* Private user code */

/* Main function, if applicable */

/* Public functions */
void DEADMAN_Init(void)
{
    g_deadman_start_ms = 0U;
    g_deadman_last_ack_ms = 0U;
    g_deadman_timed_out = 0U;
    g_deadman_timeout_message_pending = 0U;
    g_deadman_timeout_message_sent = 0U;

    LED_SetState(LED_D12, 0U);
    SEG7_DisplayDigit(DEADMAN_START_DIGIT);
}

void DEADMAN_Start(uint32_t current_time_ms)
{
    g_deadman_start_ms = current_time_ms;
    g_deadman_last_ack_ms = current_time_ms;
    g_deadman_timed_out = 0U;
    g_deadman_timeout_message_pending = 0U;
    g_deadman_timeout_message_sent = 0U;

    SEG7_DisplayDigit(DEADMAN_START_DIGIT);
}

void DEADMAN_Update(uint32_t current_time_ms)
{
    uint32_t elapsed_ms;
    uint32_t display_value;
    uint32_t event_available;

    if (g_deadman_timed_out != 0U)
    {
        if (g_deadman_timeout_message_pending != 0U)
        {
            DEADMAN_RequestTimeoutMessage();
        }
        return;
    }

    event_available = EXTI_DeadmanEventAvailable();
    if (event_available != 0U)
    {
        EXTI_ClearDeadmanEvent();

        if ((current_time_ms - g_deadman_last_ack_ms) >= DEADMAN_DEBOUNCE_MS)
        {
            g_deadman_start_ms = current_time_ms;
            g_deadman_last_ack_ms = current_time_ms;
            g_deadman_timed_out = 0U;
            g_deadman_timeout_message_pending = 0U;
            g_deadman_timeout_message_sent = 0U;

            SEG7_DisplayDigit(DEADMAN_START_DIGIT);
            (void)UART_SendString("DEADMAN ACK\r\n");
        }
    }

    elapsed_ms = current_time_ms - g_deadman_start_ms;
    if (elapsed_ms >= DEADMAN_TIMEOUT_MS)
    {
        DEADMAN_TriggerTimeout();
        return;
    }

    display_value = DEADMAN_START_DIGIT - (elapsed_ms / DEADMAN_TICK_MS);
    if (display_value > DEADMAN_START_DIGIT)
    {
        display_value = 0U;
    }

    SEG7_DisplayDigit(display_value);
}

uint32_t DEADMAN_IsTimedOut(void)
{
    return g_deadman_timed_out;
}

/* Callback functions */

/* Private functions */
static void DEADMAN_RequestTimeoutMessage(void)
{
    if (g_deadman_timeout_message_sent != 0U)
    {
        g_deadman_timeout_message_pending = 0U;
        return;
    }

    if (UART_SendString("DEADMAN TIMEOUT\r\n") != 0U)
    {
        g_deadman_timeout_message_pending = 0U;
        g_deadman_timeout_message_sent = 1U;
    }
}

static void DEADMAN_TriggerTimeout(void)
{
    if (g_deadman_timeout_message_sent == 0U)
    {
        g_deadman_timeout_message_pending = 1U;
        LED_SetState(LED_D12, 1U);
        DEADMAN_RequestTimeoutMessage();
    }

    g_deadman_timed_out = 1U;
    SEG7_DisplayDigit(0U);
}
