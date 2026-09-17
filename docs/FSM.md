# Train Simulator Finite State Machine

This document defines the high-level FSM.

The FSM must remain non-blocking.

## States

| ID | State | Main Behavior |
|---|---|---|
| S0 | WAIT_DRIVER_CARD | Wait for RFID driver authentication |
| S1 | WAIT_START_COMMAND | Wait for UART `g` and start trip timer |
| S2 | DRIVE_TO_PASSENGER | Travel toward Passenger Station |
| S3 | PASSENGER_DECISION | Final 5-second stop decision |
| S4 | PASSENGER_SERVICE | Door control and 20-second boarding |
| S5 | WAIT_PASSENGER_DEPARTURE | Wait for UART `g` after doors close |
| S6 | DRIVE_TO_CHARGING | Travel toward Charging Station |
| S7 | CHARGING_DECISION | Final 5-second stop decision |
| S8 | CHARGING_GAME | Four-button charging mini-game |
| S9 | WAIT_CHARGING_DEPARTURE | Wait for UART `g` after charging |
| S10 | DRIVE_TO_OBSTACLE | Travel toward obstacle |
| S11 | OBSTACLE_DECISION | Final 5-second stop decision |
| S12 | OBSTACLE_STOP | Wait 30 seconds and then wait for `g` |
| S13 | DRIVE_TO_TERMINAL | Travel toward Terminal Station |
| S14 | TERMINAL_DECISION | Final 5-second terminal stop decision |
| S15 | EMERGENCY | Handle Deadman, physical emergency, accident, or missed terminal |
| S16 | TRIP_RESULT | Display trip result and wait for replay |

## Normal Route

S0 WAIT_DRIVER_CARD
-> S1 WAIT_START_COMMAND
-> S2 DRIVE_TO_PASSENGER
-> S3 PASSENGER_DECISION

Passenger stop:
S3 -> S4 -> S5 -> S6

Passenger skip:
S3 -> S6

Then:

S6 DRIVE_TO_CHARGING
-> S7 CHARGING_DECISION

Charging stop:
S7 -> S8 -> S9 -> S10

Charging skip:
S7 -> S10

Then:

S10 DRIVE_TO_OBSTACLE
-> S11 OBSTACLE_DECISION

Obstacle stop:
S11 -> S12 -> S13

Obstacle not stopped:
S11 -> S15 EMERGENCY
-> after `e`
-> S13

Then:

S13 DRIVE_TO_TERMINAL
-> S14 TERMINAL_DECISION

Terminal stopped:
S14 -> S16 TRIP_RESULT

Terminal missed:
S14 -> S15 EMERGENCY
-> after `e`
-> S16 TRIP_RESULT

Replay:

S16 -> S0

## Emergency Behavior

Emergency can be entered because of:

- Deadman timeout
- physical Emergency button
- collision with obstacle
- missed terminal

The system must store the reason for the emergency.

For recoverable emergencies during normal route travel, `e` returns the simulator to the appropriate route behavior without resetting route progress.

For a missed terminal, `e` goes to TRIP_RESULT.

## Important Timing Rule

The total trip timer is independent from individual FSM states.

Once started by the first valid `g`, it continues continuously until the trip result is captured.

FSM transitions must never reset the total trip timer.