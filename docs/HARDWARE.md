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
- UART and RFID mappings must be verified before implementation.

---

## 2. Pin Status Definitions

CONFIRMED:
The mapping is known from the Training Shield documentation and may be used for implementation.

PROPOSED:
The mapping is planned but must be verified before implementation.

UNASSIGNED:
No pin mapping has been approved yet.

---

## 3. Push Buttons

Status: CONFIRMED

| Shield | MCU Pin | Planned Train Simulator Function |
|---|---|---|
| D2 | PA10 | Deadman |
| D3 | PB3 | Emergency |
| D4 | PB5 | STOP |
| D5 | PB4 | Context Action |

The physical button meaning may change according to the current FSM state.

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

Status: CONFIRMED

| Shield | MCU Pin | Normal Simulator Function |
|---|---|---|
| D10 | PB6 | Running indication |
| D11 | PA7 | Automatic headlight indication |
| D12 | PA6 | Emergency indication |
| D13 | PA5 | Stopped indication |

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

## 5. LDR

Status: CONFIRMED

Analog input:

PA1

Purpose:

Ambient-light measurement for automatic headlight control.

Requirement:

ADC runtime acquisition must use Interrupt or DMA.

ADC polling must not be used for normal application operation.

The exact ADC peripheral/channel configuration must be verified before the ADC milestone is implemented.

---

## 6. 7-Segment Display

Status: CONFIRMED

The Training Shield 7-segment interface receives a 4-bit binary value.

| Binary Bit | MCU Pin |
|---|---|
| bit 0 | PC7 |
| bit 1 | PA8 |
| bit 2 | PB10 |
| bit 3 | PA9 |

Primary Train Simulator purpose:

Deadman countdown.

Required displayed sequence:

9 -> 8 -> 7 -> 6 -> 5 -> 4 -> 3 -> 2 -> 1 -> 0

The Deadman countdown is displayed on the 7-segment only.

Do not display the normal Deadman countdown on the PC UART terminal.

---

## 7. EXTI

Planned primary EXTI input:

D2 / PA10 / Deadman button

Status:

PIN CONFIRMED
EXTI CONFIGURATION NOT YET IMPLEMENTED

The Deadman button is the preferred input for satisfying the External Interrupt requirement.

D3 / PB3 Emergency may also use EXTI if appropriate.

Final interrupt configuration will be decided during the button/EXTI milestone.

---

## 8. UART

Purpose:

PC Control Center.

Required runtime behavior:

UART RX must use Interrupt or DMA.

Polling must not be used for normal UART RX operation.

Planned commands include:

g = GO / continue
e = Emergency acknowledge / recovery

### Current Mapping

Proposed peripheral:

USART2

Proposed pins:

PA2 = USART2 TX
PA3 = USART2 RX

Status:

PROPOSED - NOT YET LOCKED

Do not implement UART register configuration until this mapping has been verified against the STM32F411RE / NUCLEO-F411RE hardware configuration.

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

Before implementing RFID:

1. Select an available SPI peripheral.
2. Check the STM32F411RE alternate-function mapping.
3. Check all Training Shield pin usage.
4. Check UART pin usage.
5. Check 7-segment pin usage.
6. Check button pin usage.
7. Check LED pin usage.
8. Check LDR pin usage.
9. Resolve all conflicts.
10. Record the approved mapping in this document.

Copilot must not choose RFID pins automatically.

---

## 10. Timer

Purpose:

Provide a non-blocking system timebase for simulator timing.

Timing requirements include:

- 10-second Deadman period
- 30-second route section
- final 5-second warning window
- 20-second passenger boarding
- 30-second obstacle wait
- continuous total trip time

Status:

TIMER PERIPHERAL NOT YET SELECTED

Prefer one suitable hardware timebase plus software timing counters/timestamps instead of consuming one hardware timer for every gameplay timer.

Do not use blocking delays for gameplay timing.

---

## 11. Current Hardware Lock Summary

| Hardware | Status |
|---|---|
| D2 PA10 | CONFIRMED |
| D3 PB3 | CONFIRMED |
| D4 PB5 | CONFIRMED |
| D5 PB4 | CONFIRMED |
| D10 PB6 | CONFIRMED |
| D11 PA7 | CONFIRMED |
| D12 PA6 | CONFIRMED |
| D13 PA5 | CONFIRMED |
| LDR PA1 | CONFIRMED |
| 7-Segment PC7 | CONFIRMED |
| 7-Segment PA8 | CONFIRMED |
| 7-Segment PB10 | CONFIRMED |
| 7-Segment PA9 | CONFIRMED |
| USART2 PA2/PA3 | PROPOSED |
| RFID SPI | UNASSIGNED |
| Timer peripheral | UNASSIGNED |

---

## 12. Conflict Rule

Before implementing any peripheral that uses alternate-function pins:

STOP and verify the pin mapping first.

Do not resolve a pin conflict silently.

If a conflict is discovered, report:

- peripheral involved
- conflicting pin
- existing function using that pin
- possible alternatives

Wait for approval before changing the hardware mapping.