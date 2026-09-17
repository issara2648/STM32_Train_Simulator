# GitHub Copilot Instructions

## 1. Project

Project name: STM32 Train Simulator

Target MCU:
STM32F411RE

Development environment:
STM32CubeIDE project edited primarily in Visual Studio Code.

Programming language:
C

This project is developed incrementally and tested on real STM32 hardware after each milestone.

Before modifying source code, read:

- README.md
- docs/REQUIREMENTS.md
- docs/FSM.md
- docs/HARDWARE.md
- docs/TEST_LOG.md

These documents define the current project requirements.

Do not contradict these documents.

If the requested implementation conflicts with the documentation, report the conflict before modifying code.

---

## 2. Development Model

Implement ONLY the milestone explicitly requested in the current prompt.

Do not implement future milestones.

Do not add features that were not requested.

Do not redesign the project architecture unless explicitly requested.

Do not modify a previously hardware-tested feature unless the current task requires the modification.

Previously tested code should be treated as stable code.

When a change to stable code is necessary, explain why.

---

## 3. MCU Programming Style

This project uses CMSIS and direct STM32 peripheral register access.

Use register-level peripheral configuration where required by the existing project.

Examples include peripheral registers such as:

- RCC
- GPIO
- USART
- ADC
- TIM
- EXTI
- SPI
- NVIC

Do NOT introduce STM32 HAL APIs unless explicitly requested.

Do NOT convert the project into a HAL-based project.

Do NOT generate or require a CubeMX `.ioc` file unless explicitly requested.

Do NOT replace the existing project structure with a newly generated CubeMX project.

Use the CMSIS device definitions already provided by the project.

---

## 4. Required Source File Template

All manually created `.c` and `.h` modules must follow the supplied course templates.

For `.c` files, preserve the intended section organization where applicable:

- Includes
- Private includes
- Private typedef
- Private enum
- Private struct
- Private union
- Private define
- Private macro
- Private constants
- Private variables
- External variables
- Private function prototypes
- Private user code
- Main function, if applicable
- Public functions
- Callback functions
- Private functions

For `.h` files, preserve the intended exported section organization where applicable:

- Includes
- Exported includes
- Exported typedef
- Exported enum
- Exported struct
- Exported union
- Exported define
- Exported macro
- Exported constants
- Exported variables
- Exported function prototypes

Do not remove template sections merely because they are currently empty unless explicitly instructed.

Keep `main.c` minimal.

Application logic should be placed in appropriate modules instead of accumulating inside `main.c`.

---

## 5. Architecture

Maintain clear separation between:

- Application layer
- Driver / hardware layer

Application modules decide WHAT the simulator should do.

Driver modules perform HOW the MCU hardware performs an operation.

Application-level FSM code should not contain unnecessary peripheral register configuration.

Peripheral drivers should not contain train gameplay decisions.

Follow the principle:

One Role = One Function

Functions should have a clear and limited responsibility.

---

## 6. Non-Blocking Requirement

The simulator must use a non-blocking architecture.

Do not implement gameplay timing using long busy-wait loops.

Do not implement long-duration behavior using blocking delays.

Examples that must be non-blocking include:

- 30-second train travel
- 5-second station warning window
- 20-second passenger boarding
- 30-second obstacle waiting
- 10-second Deadman period
- charging operation
- waiting for UART commands
- emergency recovery

Use hardware timers, counters, timestamps, flags, interrupts, and FSM transitions as appropriate.

The application must remain responsive to required interrupts and events.

---

## 7. Total Trip Timer

The total trip timer starts when the first valid UART `g` command starts the journey.

After it starts, the total trip timer must NEVER pause during the trip.

It continues during:

- driving
- station stops
- passenger boarding
- charging
- obstacle waiting
- waiting for UART commands
- Deadman emergency
- physical emergency
- accident recovery

FSM state transitions must not reset the total trip timer.

Emergency recovery must not reset route progress unless explicitly required by the project requirements.

---

## 8. UART Requirement

UART must use Interrupt or DMA for required runtime communication.

Do not implement normal UART RX using polling.

UART is used as the PC Control Center.

Expected commands include:

- `g` for required GO/continue operations
- `e` for required Emergency acknowledgement/recovery

Invalid commands must not incorrectly advance the FSM.

---

## 9. ADC Requirement

ADC must use Interrupt or DMA.

Do not implement normal ADC acquisition using polling.

The LDR is used for automatic headlight control.

Automatic headlight behavior operates independently from normal FSM route progression.

---

## 10. EXTI Requirement

At least one physical input must use EXTI.

The Deadman button is a preferred EXTI input.

Additional EXTI inputs may be used if required by the milestone.

Interrupt Service Routines must remain short.

Do not place long application processing inside an ISR.

Prefer setting flags or capturing required event information for later processing.

---

## 11. Timer Requirement

Timing behavior must be derived from a hardware timer/timebase where appropriate.

Avoid creating unnecessary independent hardware timers when one suitable timebase plus software counters can satisfy the design.

Timing systems include:

- total trip time
- route progress
- Deadman
- passenger boarding
- obstacle waiting
- warning windows

Do not block the CPU while waiting for these timers.

---

## 12. RFID / SPI

RFID communication is planned to use SPI.

Do not invent SPI pin assignments.

Do not implement the RFID driver until the SPI peripheral and pins have been explicitly confirmed.

Use docs/HARDWARE.md as the hardware source of truth.

---

## 13. Hardware Pins

Never invent hardware pin mappings.

Before using a pin, check:

docs/HARDWARE.md

Do not silently change an existing pin assignment.

If a required peripheral conflicts with an existing assignment, report the conflict instead of choosing another pin without permission.

---

## 14. FSM Rules

The Train Simulator uses a finite state machine with meaningful simulator states.

Do not create artificial states only to increase the state count.

Do not create states whose only purpose is:

- checking one condition
- accelerating
- decelerating

Each state must represent meaningful simulator behavior or player interaction.

FSM processing must be non-blocking.

Emergency events must take priority over normal route behavior.

The Emergency state must retain enough context to determine the correct recovery destination.

---

## 15. MISRA / Course Checksheet

All manually written code must follow the supplied 22-rule course checksheet.

Important requirements include:

- Do not use `//` comments.
- Use `/* ... */` comments.
- Use readable spacing around operators.
- Use one assignment per line.
- Avoid hard-coded magic integer constants.
- Avoid ternary operators.
- Do not create unreachable code.
- Do not leave unused object definitions.
- Do not use octal constants.
- Apply `u` or `U` suffix where required for unsigned integer constants.
- Use function prototype form with named parameters.
- Initialize automatic objects before reading them.
- Use appropriate essential types.
- Make operator precedence explicit where necessary.
- Do not directly test floating-point values for equality or inequality.
- Do not use floating-point loop counters.
- Control expressions must be essentially Boolean.
- Iteration and selection bodies must use compound statements.
- Required `if / else if` constructs must terminate with `else`.
- Every switch clause must terminate with an unconditional `break`.
- Every switch statement must contain a `default` label.
- The `default` label must be the first or last switch label.

When uncertain, prefer simple and explicit C code over compact syntax.

---

## 16. Compiler Quality

The target for completed milestones is:

- 0 compiler errors
- 0 compiler warnings

Do not hide warnings by disabling compiler diagnostics.

Do not add casts or suppressions solely to silence a warning without understanding its cause.

If an existing warning is unrelated to the current milestone, report it separately.

---

## 17. Interrupt Rules

Interrupt handlers should perform the minimum necessary work.

Prefer:

1. detect interrupt source
2. clear required interrupt flag
3. store data or set an event flag
4. return

Perform normal application decisions outside the ISR where practical.

Shared variables accessed by both interrupt and normal execution contexts must be handled appropriately.

---

## 18. Code Modification Rules

Before editing:

1. inspect the existing implementation
2. identify the minimum files requiring modification
3. preserve unrelated working behavior

Do not rewrite an entire file when a smaller safe modification is sufficient.

Do not delete existing code merely because another implementation looks cleaner.

Do not rename public interfaces without a requirement.

---

## 19. Milestone Completion

After implementing each milestone, STOP.

Do not continue to the next milestone automatically.

Report:

1. Files created
2. Files modified
3. What was implemented
4. Important design decisions
5. Assumptions
6. Build procedure
7. Exact hardware test procedure
8. Expected hardware behavior
9. Any remaining warnings or limitations

Never claim:

- hardware PASS
- peripheral PASS
- milestone PASS

until the user reports successful testing on the real STM32 board.

---

## 20. Debugging

When the user reports a failed hardware test:

- analyze the reported evidence first
- modify only what is necessary
- do not redesign unrelated working modules
- explain the likely cause
- provide a new test procedure

A successful compile does not prove that hardware behavior is correct.

---

## 21. Source of Truth Priority

If information conflicts, use this priority:

1. Current explicit milestone prompt
2. docs/HARDWARE.md for confirmed hardware mapping
3. docs/REQUIREMENTS.md for simulator behavior
4. docs/FSM.md for state architecture
5. Supplied course C/H templates
6. Supplied 22-rule checksheet
7. Existing hardware-tested implementation

If a conflict cannot be resolved safely, ask before changing code.