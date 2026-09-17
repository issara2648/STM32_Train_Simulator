# Train Simulator Requirements

## 1. General

The system simulates a train journey from an origin station to a terminal station.

Target trip time:

3 minutes (180 seconds)

The final result compares the actual total trip time against the 180-second target.

The result may be:

- EARLY
- ON TIME
- LATE

Missing the terminal station must also be recorded in the final result.

---

## 2. Starting the Simulator

The initial state waits for the driver to tap an RFID card.

After successful driver authentication, the control center asks the user to enter:

g

through UART.

If any incorrect character is entered, the system must reject it and continue waiting for `g`.

The total trip timer starts immediately when the first valid `g` is received.

After starting, the total trip timer NEVER pauses.

The timer continues during:

- normal driving
- station stops
- passenger boarding
- charging
- waiting for commands
- obstacle waiting
- Deadman emergency
- physical emergency
- accident recovery

---

## 3. Route

The route contains four travel sections.

1. Origin -> Passenger Station
2. Passenger Station -> Charging Station
3. Charging Station -> Obstacle
4. Obstacle -> Terminal Station

Each normal travel section takes 30 seconds of route progress.

Route progress must not reset after an emergency.

---

## 4. Distance Display

The PC terminal must display simulated distance instead of displaying travel section time.

Internally, time may be used to calculate route progress.

The displayed distance is derived from route progress.

The terminal must not display values such as:

`Segment: 15 / 30 sec`

The user should see simulated distance instead.

---

## 5. Station Warning

Five seconds before reaching each event point, the PC terminal must display a warning.

This warning occurs during the final 5 seconds of the 30-second travel section.

The 5-second warning does NOT add additional travel time.

Events requiring a stop decision:

- Passenger Station
- Charging Station
- Obstacle
- Terminal Station

The STOP button is used to choose to stop.

---

## 6. Passenger Station

Five seconds before reaching the Passenger Station, the PC terminal asks whether the player wants to stop.

If STOP is not pressed, the train passes the station and continues.

If STOP is pressed:

- running indication turns off
- stopped indication turns on
- player must open the door
- passenger boarding requires 20 seconds

The total trip timer continues during the entire stop.

The door cannot be closed until at least 20 seconds have passed after opening.

After 20 seconds, the player may close the door.

After the door is closed, the control center waits for UART command `g`.

Incorrect UART characters must be rejected.

A valid `g` resumes the journey.

---

## 7. Charging Station

Five seconds before reaching the Charging Station, the PC terminal asks whether the player wants to stop.

If STOP is not pressed, the train continues without charging.

If STOP is pressed, the charging mini-game begins.

The charging mini-game uses all four physical buttons.

Each button activates one corresponding LED.

The LED remains active after its button has been successfully pressed.

Charging is complete when all four LEDs are active.

No required button order is used.

After charging is complete, the PC terminal displays that charging is complete and waits for UART command `g`.

Incorrect UART characters must be rejected.

A valid `g` resumes the journey.

The total trip timer continues throughout charging and while waiting for `g`.

---

## 8. Obstacle

Five seconds before reaching the obstacle, the PC terminal displays a warning and asks the player to stop.

If STOP is pressed:

- train stops
- stopped indication is active
- the player must wait 30 seconds

After 30 seconds, the PC terminal allows the player to enter `g` to continue.

The total trip timer continues during the 30-second wait and while waiting for `g`.

If the player does not stop before reaching the obstacle:

- an accident occurs
- the system enters Emergency state
- emergency indication activates

The player must enter `e` through UART to recover.

After recovery, route progress continues from the appropriate point and is not reset.

---

## 9. Terminal Station

Five seconds before reaching the Terminal Station, the PC terminal asks the player to stop.

If STOP is pressed:

- train stops at the terminal
- trip completion is recorded
- the system enters the result screen

If STOP is not pressed:

- the train passes the terminal
- emergency braking activates
- the result records that the terminal was missed

The player must enter `e`.

After `e`, the system goes to the result screen rather than resuming normal driving.

---

## 10. Deadman System

The Deadman system operates while the train is driving.

The driver must acknowledge the Deadman system within every 10-second period.

The Deadman countdown is displayed only on the 7-segment display.

The PC terminal must not display the normal Deadman countdown.

The 7-segment display counts:

9, 8, 7, 6, 5, 4, 3, 2, 1, 0

Pressing the Deadman button resets the countdown.

If the driver fails to acknowledge the Deadman system:

- the system enters Emergency state
- emergency indication activates
- the train is considered stopped

The PC terminal may display that the emergency was caused by Deadman timeout.

The player must enter `e` to recover.

After recovery:

- total trip time is not reset
- route progress is not reset
- the current travel section is not restarted
- Deadman timing begins a new acknowledgement period

---

## 11. Physical Emergency Button

A physical Emergency button is available during normal driving.

Pressing it enters Emergency state.

Emergency indication activates.

The player must enter:

e

through UART to recover.

Total trip time continues while Emergency is active.

Route progress must not be reset.

---

## 12. Automatic Headlight

An LDR is used to measure ambient light through ADC.

When ambient light is low:

- headlight turns ON

When ambient light is sufficiently bright:

- headlight turns OFF

The automatic headlight operates independently of normal FSM progression.

ADC must use Interrupt or DMA.

---

## 13. PC Terminal

The normal driving display should contain useful information such as:

- Total Trip Time
- Simulated Distance
- Train Status
- Headlight Status
- Next Event

Deadman countdown must not be displayed on the PC terminal.

The terminal also displays event-specific instructions and warnings.

---

## 14. Result Screen

The result screen displays:

- total trip time
- target time (3:00)
- EARLY / ON TIME / LATE
- whether the terminal was stopped at correctly
- whether an accident occurred, if implemented in trip statistics

After the result has been displayed, a physical button is used to start a new game.

The new game returns to the initial RFID state.