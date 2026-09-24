# Hardware Test Log

Do not mark a milestone PASS until it has been tested on the real STM32 board.

---

## Milestone Status

| ID | Feature | Build | Hardware | Commit | Notes |
|---|---|---|---|---|---|
M00 | Project setup and documentation | IN PROGRESS | N/A | - | |
M01 | Board bring-up and LEDs | PASS | Build 0 errors, 0 warnings. Hardware verified D10 -> D11 -> D12 -> D13 sequential operation. | - | |
M02 | Four push buttons | PASS | Build 0 errors, 0 warnings. Hardware verified D2/PA10, D3/PB3, D4/PB5, D5/PB4 using pull-up active-low inputs. Individual and simultaneous button presses passed.| - | |
M03 | 7-Segment | PASS | Build 0 errors, 0 warnings. Hardware verified 4-bit interface PC7/PA8/PB10/PA9. Display correctly cycles 0 -> 1 -> 2 -> 3 -> 4 -> 5 -> 6 -> 7 -> 8 -> 9 and repeats. | - | |
M04 | Timer / Timebase | PASS | Build 0 errors, 0 warnings. TIM2 hardware timebase verified on physical board. Non-blocking 7-segment diagnostic correctly cycles 0 -> 9 at approximately 1000 ms per digit using the millisecond timebase. | - | |
M05 | UART TX/RX Interrupt | PASS | Build 0 errors, 0 warnings. USART2 PA2/PA3 AF7 at 115200 8N1 verified on physical board. Interrupt-driven RX/TX confirmed by startup message and successful character echo.| - | |
M06 | LDR / ADC Interrupt | PASS | Build 0 errors, 0 warnings. PA1 / ADC1_IN1 verified on physical hardware. ADC sampling uses EOC interrupt with non-blocking periodic conversion. LDR readings changed with lighting conditions and USART2 remained operational during ADC sampling. | - | |
M07 | RFID + SPI | DEFERRED | RC522 implementation postponed until a conflict-free SPI pin mapping and physical wiring are confirmed. No RFID code implemented yet. | - | |
M08 | Deadman + EXTI | PASS | Build 0 errors, 0 warnings. D2 / PA10 EXTI verified on physical hardware. Deadman countdown 9 to 0 verified. D2 acknowledgement resets countdown to 9. Timeout turns ON D12 / PA6 and sends DEADMAN TIMEOUT exactly once. Non-blocking operation verified.| - | |
| M09 | Trip Timer + Distance | NOT STARTED | NOT STARTED | - | |
| M10 | Passenger Route | NOT STARTED | NOT STARTED | - | |
| M11 | Charging Route | NOT STARTED | NOT STARTED | - | |
| M12 | Obstacle Route | NOT STARTED | NOT STARTED | - | |
| M13 | Terminal + Result | NOT STARTED | NOT STARTED | - | |
| M14 | Emergency Integration | NOT STARTED | NOT STARTED | - | |
| M15 | Full Integration | NOT STARTED | NOT STARTED | - | |
| M16 | MISRA / Final Cleanup | NOT STARTED | NOT STARTED | - | |

---

## Test Entry Template

### Test: Mxx

Date:

Build:
- PASS / FAIL

Compiler warnings:
- 

Flash:
- PASS / FAIL

Hardware result:
- PASS / FAIL

Expected:
-

Observed:
-

Problems:
-

Fix:
-

Git commit:
-