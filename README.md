# STM32 Train Simulator

Train Simulator project developed on STM32F411RE with Training Shield Rev 03.00.

## Project Objective

Design and implement a finite state machine (FSM) based train simulator.

The player acts as a train driver and must operate the train from the origin station to the terminal station while responding to stations, a charging point, an obstacle, the Deadman safety system, and emergency situations.

The target trip time is 3 minutes.

## Main Features

- Driver authentication using RFID
- UART control center
- Finite State Machine with more than 10 states
- Continuous trip timer
- Simulated distance calculation
- Deadman system with 7-segment countdown
- Automatic headlight using LDR
- Passenger station
- Charging station with 4-button mini-game
- Track obstacle event
- Emergency system
- Terminal station
- Trip result screen
- Replay system

## Main Hardware

- STM32F411RE
- Training Shield Rev 03.00
- 4 push buttons
- 4 onboard LEDs
- 7-segment display
- LDR
- RFID reader
- UART connection to PC

## Required Peripherals

- GPIO Input
- GPIO Output
- UART using Interrupt or DMA
- ADC using Interrupt or DMA
- External Interrupt (EXTI)
- Timer
- SPI for RFID

## Development Rules

- Non-blocking design
- No long blocking delays
- UART must not use polling
- ADC must not use polling
- Application and driver layers must be clearly separated
- Keep `main.c` minimal
- Follow the provided project template
- Follow the provided MISRA-C checklist
- Develop and test one milestone at a time

## Development Workflow

1. Define one milestone.
2. Generate or modify only the code required for that milestone.
3. Build the project.
4. Flash to the real STM32 board.
5. Test the hardware behavior.
6. Fix issues until the milestone passes.
7. Commit the working milestone to Git.
8. Continue to the next milestone.

Do not implement future milestones before the current milestone has passed hardware testing.