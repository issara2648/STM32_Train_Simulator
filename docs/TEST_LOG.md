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
| M06 | LDR + ADC | NOT STARTED | NOT STARTED | - | |
| M07 | RFID + SPI | NOT STARTED | NOT STARTED | - | |
| M08 | Deadman System | NOT STARTED | NOT STARTED | - | |
| M09 | Trip Timer + Distance | NOT STARTED | NOT STARTED | - | |
| M10 | Passenger Route | NOT STARTED | NOT STARTED | - | |
| M11 | Charging Route | NOT STARTED | NOT STARTED | - | |
| M12 | Obstacle Route | NOT STARTED | NOT STARTED | - | |
| M13 | Terminal + Result | NOT STARTED | NOT STARTED | - | |
| M14 | Emergency Integration | NOT STARTED | NOT STARTED | - | |
| M15 | Full Integration | NOT STARTED | NOT STARTED | - | |
| M16 | MISRA / Final Cleanup | NOT STARTED | NOT STARTED | - | |
| M06 | LDR + ADC | IMPLEMENTED | AWAITING HARDWARE TEST | - | ADC1 PA1 interrupt-driven sampling implemented; reporting via UART every 1000 ms. |

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