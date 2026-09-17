/**
 ******************************************************************************
 * @file           : uart_driver.c
 * @brief          : USART2 interrupt-driven UART driver implementation
 ******************************************************************************
 */

#include "uart_driver.h"

#include "stm32f411xe.h"
#include <stddef.h>

/* Private typedef */

/* Private enum */

/* Private struct */

/* Private union */

/* Private define */
#define UART_TX_BUFFER_SIZE            (128U)
#define UART_RX_BUFFER_SIZE            (64U)

/* Clock / baud constants */
#define UART_PCLK1_HZ                  (16000000U)
#define UART_BAUD                      (115200U)

/* Private macro */

/* Private constants */

/* Private variables */
static volatile uint8_t tx_buffer[UART_TX_BUFFER_SIZE];
static volatile uint32_t tx_head = 0U;
static volatile uint32_t tx_tail = 0U;
static volatile uint32_t tx_count = 0U;
static volatile uint8_t tx_busy = 0U;

static volatile uint8_t rx_buffer[UART_RX_BUFFER_SIZE];
static volatile uint32_t rx_head = 0U;
static volatile uint32_t rx_tail = 0U;
static volatile uint32_t rx_count = 0U;

/* Private function prototypes */

/* Private user code */

void UART_Init(void)
{
    uint32_t moder_value;
    uint32_t afrl_value;
    uint32_t brr_value;

    /* Enable GPIOA clock */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure PA2 and PA3 as alternate function AF7 (USART2) */
    moder_value = GPIOA->MODER;
    moder_value &= ~(GPIO_MODER_MODER2_Msk | GPIO_MODER_MODER3_Msk);
    moder_value |= (0x02U << GPIO_MODER_MODER2_Pos);
    moder_value |= (0x02U << GPIO_MODER_MODER3_Pos);
    GPIOA->MODER = moder_value;

    /* AFRL: PA0..PA7; set AF7 for PA2 and PA3 */
    afrl_value = GPIOA->AFR[0U];
    afrl_value &= ~((0x0FU << (4U * 2U)) | (0x0FU << (4U * 3U)));
    afrl_value |= (7U << (4U * 2U));
    afrl_value |= (7U << (4U * 3U));
    GPIOA->AFR[0U] = afrl_value;

    /* Enable USART2 peripheral clock on APB1 */
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Calculate BRR as integer-rounded PCLK1 / BAUD for OVER8 = 0
     * Use BRR = (PCLK1 + BAUD/2) / BAUD to produce a rounded integer value.
     * For PCLK1 = 16 000 000 and BAUD = 115200 this yields 139 (0x008B).
     */
    brr_value = (UART_PCLK1_HZ + (UART_BAUD / 2U)) / UART_BAUD;
    USART2->BRR = brr_value;

    /* Enable transmitter and receiver */
    USART2->CR1 |= (USART_CR1_TE_Msk | USART_CR1_RE_Msk);

    /* Enable RXNE interrupt */
    USART2->CR1 |= USART_CR1_RXNEIE_Msk;

    /* Enable NVIC interrupt for USART2 */
    NVIC_SetPriority(USART2_IRQn, 5U);
    NVIC_EnableIRQ(USART2_IRQn);

    /* Enable USART2 */
    USART2->CR1 |= USART_CR1_UE_Msk;

    /* Initialize buffer state */
    tx_head = 0U;
    tx_tail = 0U;
    tx_count = 0U;
    tx_busy = 0U;

    rx_head = 0U;
    rx_tail = 0U;
    rx_count = 0U;
}

uint32_t UART_IsRxAvailable(void)
{
    return rx_count;
}

char UART_ReadChar(void)
{
    char result = (char)0;

    if (rx_count == 0U)
    {
        return result;
    }

    /* Protect against concurrent ISR access */
    __disable_irq();

    result = (char)rx_buffer[rx_tail];
    rx_tail++;
    if (rx_tail >= UART_RX_BUFFER_SIZE)
    {
        rx_tail = 0U;
    }
    rx_count--;

    __enable_irq();

    return result;
}

uint32_t UART_SendString(const char *text)
{
    uint32_t len = 0U;

    if (text == NULL)
    {
        return 0U;
    }

    /* If a transmission is already active, reject the new request */
    __disable_irq();
    if (tx_busy != 0U)
    {
        __enable_irq();
        return 0U;
    }

    /* Copy text into driver TX buffer up to capacity */
    while ((text[len] != '\0') && (len < UART_TX_BUFFER_SIZE))
    {
        tx_buffer[len] = (uint8_t)text[len];
        len++;
    }

    if (len == 0U)
    {
        __enable_irq();
        return 0U;
    }

    tx_head = len;
    tx_tail = 0U;
    tx_count = len;
    tx_busy = 1U;

    /* Enable TXE interrupt to start transmission. ISR will send bytes. */
    USART2->CR1 |= USART_CR1_TXEIE_Msk;

    __enable_irq();

    return 1U;
}

/* Interrupt handler */
void USART2_IRQHandler(void)
{
    uint32_t sr = USART2->SR;

    /* RXNE: read received byte and store in RX buffer */
    if ((sr & USART_SR_RXNE_Msk) != 0U)
    {
        uint8_t data = (uint8_t)(USART2->DR & 0xFFU);
        uint32_t next = rx_head + 1U;

        if (next >= UART_RX_BUFFER_SIZE)
        {
            next = 0U;
        }

        if (rx_count < UART_RX_BUFFER_SIZE)
        {
            rx_buffer[rx_head] = data;
            rx_head = next;
            rx_count++;
        }
        else
        {
            /* Buffer full: drop byte */
        }
    }

    /* TXE: transmit next byte if available */
    if ((sr & USART_SR_TXE_Msk) != 0U)
    {
        if (tx_count > 0U)
        {
            USART2->DR = tx_buffer[tx_tail];
            tx_tail++;
            if (tx_tail >= UART_TX_BUFFER_SIZE)
            {
                tx_tail = 0U;
            }
            tx_count--;
        }
        else
        {
            /* No more data: disable TXE interrupt and clear busy flag */
            USART2->CR1 &= ~USART_CR1_TXEIE_Msk;
            tx_busy = 0U;
        }
    }
}

/* Private functions */
