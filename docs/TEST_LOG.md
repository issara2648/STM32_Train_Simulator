# Hardware Test Log

Do not mark a milestone PASS until it has been tested on the real STM32 board.

---

## Milestone Status

| ID | Feature | Build | Hardware | Commit | Notes |
|---|---|---|---|---|---|
M00 | Project setup and documentation | IN PROGRESS | N/A | - | |
M01 | Board bring-up and LEDs | PASS | Build 0 errors, 0 warnings. Hardware verified D10 -> D11 -> D12 -> D13 sequential operation. | - | |
M02 | Four push buttons | PASS | Build 0 errors, 0 warnings. Hardware verified D2/PA10, D3/PB3, D4/PB5, D5/PB4 using pull-up active-low inputs. Individual and simultaneous button presses passed.| - | |
| M03 | 7-Segment | NOT STARTED | NOT STARTED | - | |
| M04 | Timer/timebase | NOT STARTED | NOT STARTED | - | |
| M05 | UART TX/RX Interrupt | NOT STARTED | NOT STARTED | - | |
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