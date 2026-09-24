# Hardware Mapping

## 1. Target Hardware

MCU:

STM32F411RE

Development Board:

NUCLEO-F411RE

Expansion Board:

Training Shield Rev 03.00

Programming style:

CMSIS / direct register access

IMPORTANT:

- Do not invent hardware pin assignments.
- Do not change confirmed assignments without permission.
- Check pin conflicts before enabling a new peripheral.
- Stable hardware-tested mappings must not be changed without a documented reason.

---

## 2. Pin Status Definitions

CONFIRMED:

The mapping is known from the Training Shield documentation and may be used for implementation.

CONFIRMED BY HARDWARE TEST:

The mapping and electrical behavior have been tested successfully on the physical project hardware.

PROPOSED:

The mapping is planned but must be verified before implementation.

UNASSIGNED:

No pin mapping has been approved yet.

---

## 3. Push Buttons

Status:

CONFIRMED BY HARDWARE TEST

| Shield | MCU Pin | Planned Train Simulator Function |
|---|---|---|
| D2 | PA10 | Deadman |
| D3 | PB3 | Emergency |
| D4 | PB5 | STOP |
| D5 | PB4 | Context Action |

The physical button meaning may change according to the current FSM state.

### Button Electrical Behavior

The four Training Shield push buttons were verified during M02 using GPIO input pull-up configuration.

Released button = logic 1.

Pressed button = logic 0.

Therefore, all four buttons are active-low inputs.

Multiple simultaneous button presses were also verified successfully.

### Normal Driving

| Button | Function |
|---|---|
| D2 | Deadman acknowledge |
| D3 | Emergency |
| D4 | STOP when a stop decision is active |
| D5 | No normal driving action |

### Passenger Service

D5 is used for the passenger door action.

The same button is used to open and close the door according to the current passenger-service phase.

### Charging Game

During CHARGING_GAME:

| Button | Function |
|---|---|
| D2 | Charge input 1 |
| D3 | Charge input 2 |
| D4 | Charge input 3 |
| D5 | Charge input 4 |

All four buttons are temporarily used by the charging mini-game.

### Result Screen

D5 is used as Play Again.

---

## 4. Onboard LEDs

Status:

CONFIRMED BY HARDWARE TEST

| Shield | MCU Pin | Normal Simulator Function |
|---|---|---|
| D10 | PB6 | Running indication |
| D11 | PA7 | Automatic headlight indication |
| D12 | PA6 | Emergency indication |
| D13 | PA5 | Stopped indication |

### LED Electrical Behavior

The four Training Shield LEDs were verified during M01 as active-high.

GPIO output logic 1 = LED ON.

GPIO output logic 0 = LED OFF.

### Normal Meaning

Running:

D10 / PB6 ON while the train is running.

Headlight:

D11 / PA7 represents the automatic headlight output.

Emergency:

D12 / PA6 ON while Emergency state is active.

Stopped:

D13 / PA5 ON while the train is normally stopped.

### Charging Game Reuse

During CHARGING_GAME, the four onboard LEDs may temporarily be reused as charging progress indicators.

Each successfully pressed charging button activates one corresponding LED.

After leaving CHARGING_GAME, the LEDs must return to their normal simulator meanings.

---

## 5. LDR / ADC

Status:

CONFIRMED BY HARDWARE TEST

Analog input pin:

PA1

ADC peripheral:

ADC1

ADC channel:

ADC1_IN1 / Channel 1

Purpose:

Ambient-light measurement for automatic headlight control.

### ADC Runtime Acquisition

ADC acquisition uses End-of-Conversion Interrupt.

ADC polling is not used for normal application operation.

The application periodically requests a conversion without waiting for completion.

The ADC interrupt reads the completed conversion result and stores the latest sample.

### M06 Hardware Verification

M06 was verified successfully on the physical hardware.

Verified behavior:

- PA1 / ADC1_IN1 acquisition operates correctly.
- ADC values remain within the 12-bit ADC range.
- ADC readings change when the light level at the LDR changes.
- ADC conversion is non-blocking.
- ADC completion is handled by ADC_IRQHandler.
- USART2 remains operational while ADC sampling is active.
- Build completed with 0 errors and 0 warnings.

### Light-Response Polarity

Status:

NOT YET RECORDED

The exact relationship between ambient light and ADC value has not yet been recorded in this document.

Before implementing final automatic-headlight threshold logic, record:

- whether DARKER conditions increase or decrease the ADC value
- whether BRIGHTER conditions increase or decrease the ADC value
- an appropriate threshold based on physical testing

Do not assume the polarity in application logic until it has been physically confirmed.

---

## 6. 7-Segment Display

Status:

CONFIRMED BY HARDWARE TEST

The Training Shield 7-segment interface receives a 4-bit binary value.

| Binary Bit | MCU Pin |
|---|---|
| bit 0 | PC7 |
| bit 1 | PA8 |
| bit 2 | PB10 |
| bit 3 | PA9 |

The mapping was verified during M03 hardware bring-up.

The display successfully showed digits 0 through 9.

Primary Train Simulator purpose:

Deadman countdown.

Required displayed sequence:

9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0

The Deadman countdown is displayed on the 7-segment only.

Do not display the normal Deadman countdown on the PC UART terminal.

---

## 7. EXTI

Purpose:

Deadman acknowledgement input.

Status:

CONFIRMED BY HARDWARE TEST

Primary EXTI input:

D2 / PA10 / Deadman button

EXTI line:

EXTI10

Trigger:

Falling edge

Reason:

The D2 button is active-low.

Released = logic 1.

Pressed = logic 0.

Therefore, a physical button press produces a falling edge.

### M08 Implementation

PA10 is configured as EXTI line 10 through SYSCFG.

The Deadman acknowledgement is detected using an external interrupt.

The application does not poll PA10 for Deadman acknowledgement.

The EXTI interrupt handler performs only minimal interrupt work:

- detect the EXTI10 pending event
- clear the EXTI pending flag
- set a Deadman acknowledgement event flag
- return from the interrupt

Deadman countdown calculations, 7-segment updates, UART messages, and timeout behavior are handled outside the ISR.

### Deadman Timing

The Deadman period is 10 seconds.

The existing TIM2 millisecond timebase is used for Deadman timing.

No additional hardware timer is used.

The normal 7-segment countdown is:

9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0

Pressing D2 before timeout acknowledges the Deadman and resets the countdown to 9.

A non-blocking debounce mechanism is used for the physical button input.

### Timeout Behavior

During the M08 diagnostic:

If the Deadman countdown expires:

- Deadman enters timeout condition.
- D12 / PA6 Emergency LED turns ON.
- The PC UART terminal receives:

DEADMAN TIMEOUT

The timeout UART message is transmitted exactly once.

If USART2 is busy when the timeout occurs, the application keeps the diagnostic message pending and retries later without blocking.

The timeout message is never transmitted from the EXTI ISR.

The M08 diagnostic intentionally does not implement the `e` recovery command.

Emergency recovery and final Emergency FSM integration will be implemented in a later milestone.

### M08 Hardware Verification

M08 was verified successfully on the physical hardware.

Verified behavior:

- 7-segment counted from 9 to 0.
- D2 / PA10 generated Deadman acknowledgement events.
- Pressing D2 reset the countdown to 9.
- Multiple acknowledgements were verified.
- UART displayed DEADMAN ACK during acknowledgement testing.
- Allowing the countdown to expire activated D12 / PA6.
- UART displayed DEADMAN TIMEOUT exactly once.
- Firmware remained non-blocking during normal Deadman operation.
- Existing LDR ADC interrupt operation remained functional before Deadman timeout.
- Build completed with 0 errors and 0 warnings.

PA10 / EXTI10 is now a stable hardware-tested project configuration.

Do not change this EXTI mapping without a documented reason.

## 8. UART

Purpose:

PC Control Center.

Status:

CONFIRMED BY HARDWARE TEST

Peripheral:

USART2

Pins:

PA2 = USART2 TX / AF7

PA3 = USART2 RX / AF7

Peripheral clock:

APB1 = 16 MHz

UART configuration:

115200 baud

8 data bits

No parity

1 stop bit

No flow control

### Runtime Requirement

UART RX and TX use interrupt-driven operation.

Polling must not be used for normal UART communication.

Planned simulator commands include:

g = GO / continue

e = Emergency acknowledge / recovery

The final game-command interpretation belongs to the application/FSM layer and is not implemented inside the UART driver.

### M05 Hardware Verification

M05 was verified successfully using the physical board and PC serial terminal.

Verified behavior:

- USART2 TX operates on PA2 / AF7.
- USART2 RX operates on PA3 / AF7.
- 115200 8N1 communication operates correctly.
- Startup UART transmission was received by the PC.
- Characters sent from the PC were received successfully.
- Interrupt-driven character echo was verified.
- Test characters included lowercase and uppercase input.
- Build completed with 0 errors and 0 warnings.

USART2 PA2/PA3 is now a locked project mapping.

Do not move USART2 to another pin mapping without checking all existing hardware assignments.

---

## 9. RFID

Purpose:

Driver authentication before the journey starts.

Planned module:

RC522 or compatible RFID reader

Interface:

SPI

Status:

UNASSIGNED

SPI peripheral:

NOT YET SELECTED

SCK:

NOT YET SELECTED

MISO:

NOT YET SELECTED

MOSI:

NOT YET SELECTED

NSS / CS:

NOT YET SELECTED

RST:

NOT YET SELECTED

IRQ:

NOT REQUIRED UNLESS LATER DESIGN REQUIRES IT

The RC522 has not yet been connected to the physical board.

Before implementing RFID:

1. Select an available SPI peripheral.
2. Check the STM32F411RE alternate-function mapping.
3. Check all Training Shield pin usage.
4. Check USART2 PA2/PA3 usage.
5. Check 7-segment pin usage.
6. Check button pin usage.
7. Check LED pin usage.
8. Check LDR PA1 usage.
9. Check timer/peripheral conflicts.
10. Resolve all conflicts.
11. Record the approved mapping in this document.
12. Connect the physical RC522 only after the mapping has been approved.

Copilot must not choose RFID pins automatically.

Do not implement the RC522 driver until the SPI peripheral and complete wiring have been locked.

---

## 10. Timer / System Timebase

Purpose:

Provide a non-blocking system timebase for simulator timing.

Status:

CONFIRMED BY HARDWARE TEST

Peripheral:

TIM2

Clock source configuration:

HSI = 16 MHz

SYSCLK = 16 MHz

AHB = 16 MHz

APB1 = 16 MHz

TIM2 configuration:

PSC = 0

ARR = 15999

TIM2 update interrupt period:

1 millisecond

### Runtime Design

TIM2 provides a reusable millisecond system timebase.

Application timing must use timestamp subtraction, counters, or other non-blocking timing logic.

Do not create blocking gameplay delays.

Prefer the existing TIM2 timebase plus software timing state instead of consuming a separate hardware timer for every gameplay timer.

### Planned Timing Uses

The simulator timing requirements include:

- 10-second Deadman period
- 30-second route section
- final 5-second warning window
- 20-second passenger boarding
- 30-second obstacle wait
- continuous total trip time

The total trip timer starts after the first valid GO command and must never pause until the trip result is reached.

### M04 Hardware Verification

M04 was verified successfully on the physical hardware.

The 7-segment diagnostic displayed digits 0 through 9 with approximately one second per digit using the TIM2 millisecond timebase.

The diagnostic used non-blocking timestamp-based timing.

Build completed with 0 errors and 0 warnings.

TIM2 is now a stable project peripheral and must not be reconfigured without a documented reason.

---

## 11. Current Hardware Lock Summary

| Hardware | Status |
|---|---|
| D2 / PA10 | CONFIRMED / HARDWARE VERIFIED |
| D3 / PB3 | CONFIRMED / HARDWARE VERIFIED |
| D4 / PB5 | CONFIRMED / HARDWARE VERIFIED |
| D5 / PB4 | CONFIRMED / HARDWARE VERIFIED |
| D10 / PB6 | CONFIRMED / HARDWARE VERIFIED |
| D11 / PA7 | CONFIRMED / HARDWARE VERIFIED |
| D12 / PA6 | CONFIRMED / HARDWARE VERIFIED |
| D13 / PA5 | CONFIRMED / HARDWARE VERIFIED |
| LDR / PA1 / ADC1_IN1 | CONFIRMED / HARDWARE VERIFIED |
| 7-Segment bit 0 / PC7 | CONFIRMED / HARDWARE VERIFIED |
| 7-Segment bit 1 / PA8 | CONFIRMED / HARDWARE VERIFIED |
| 7-Segment bit 2 / PB10 | CONFIRMED / HARDWARE VERIFIED |
| 7-Segment bit 3 / PA9 | CONFIRMED / HARDWARE VERIFIED |
| USART2 TX / PA2 / AF7 | CONFIRMED / HARDWARE VERIFIED |
| USART2 RX / PA3 / AF7 | CONFIRMED / HARDWARE VERIFIED |
| TIM2 millisecond timebase | CONFIRMED / HARDWARE VERIFIED |
| Deadman / PA10 / EXTI10 | CONFIRMED / HARDWARE VERIFIED |
| RFID SPI | UNASSIGNED |

---

## 12. Peripheral Usage Summary

| Peripheral | Current Purpose | Status |
|---|---|---|
| GPIOA | LEDs, buttons, 7-segment, UART, LDR | IN USE |
| GPIOB | LEDs, buttons, 7-segment | IN USE |
| GPIOC | 7-segment | IN USE |
| TIM2 | 1 ms system timebase | HARDWARE VERIFIED |
| USART2 | PC Control Center communication | HARDWARE VERIFIED |
| ADC1 | LDR acquisition | HARDWARE VERIFIED |
| EXTI10 | D2 / PA10 Deadman acknowledgement | HARDWARE VERIFIED |
| SPI | RC522 RFID | UNASSIGNED |

Before selecting RFID SPI pins, this table and the complete pin lock summary must be checked for conflicts.

---

## 13. Stable Hardware Milestones

The following hardware milestones have passed physical testing:

M01:

Four Training Shield LEDs.

M02:

Four Training Shield push buttons.

M03:

4-bit 7-segment interface.

M04:

TIM2 millisecond timebase.

M05:

USART2 interrupt-driven TX/RX.

M06:
M08:

D2 / PA10 Deadman acknowledgement using EXTI10, non-blocking 10-second countdown, acknowledgement reset, and timeout indication.

PA1 LDR using ADC1 End-of-Conversion Interrupt.

These implementations are considered stable project baselines.

Do not modify stable drivers during later milestones unless:

- a verified defect is found
- a required integration change cannot be achieved otherwise
- the change has been reviewed for regression risk

After modifying any stable driver, its relevant hardware behavior must be regression-tested.

---

## 14. Conflict Rule

Before implementing any peripheral that uses alternate-function pins:

STOP and verify the pin mapping first.

Do not resolve a pin conflict silently.

If a conflict is discovered, report:

- peripheral involved
- conflicting pin
- existing function using that pin
- possible alternatives

Wait for approval before changing an existing confirmed hardware mapping.

This rule is especially important for the RFID / SPI milestone because the SPI peripheral and pins have not yet been assigned.

---

## 15. Current Next Hardware Decision

Next milestone:

M07 RFID + SPI

Current state:

RC522 is not yet connected.

Before M07 implementation:

- select the SPI peripheral
- select SCK
- select MOSI
- select MISO
- select NSS / CS
- select RST
- verify STM32F411RE alternate functions
- verify Training Shield accessibility
- verify no conflicts with all M01-M06 hardware
- record the final mapping in this document

Do not ask Copilot to invent or automatically select the RC522 wiring.