# Arduino Washing Machine Controller

A custom Arduino-based washing machine controller developed to replace and automate the control logic of a washing machine.

The project implements a complete washing cycle using an Arduino, relay outputs, water-level sensors, an LCD interface, and push-button controls.

## Project Overview

The controller manages the complete washing process through a sequence of operating states:

```text
WASH
  ↓
SOAK
  ↓
DRAIN
  ↓
SPIN
  ↓
RINSE
  ↓
DRAIN
  ↓
SPIN
  ↓
COMPLETE
```

The controller continuously monitors water-level sensors and controls the washing-machine actuators according to the current cycle state.

The firmware was developed as a hands-on embedded control and automation project.

## Main Features

* Arduino-based washing machine control
* 16×2 LCD user interface
* Push-button power and start/pause control
* Water-level monitoring
* Automatic water filling
* Washing motor control
* Spin motor control
* Drain pump control
* Automatic cycle sequencing
* Wash / soak / rinse / spin timing
* Pause and resume functionality
* Water-fill timeout protection
* Cycle timer display
* Relay-based output control

## Hardware

### Controller

* Arduino
* 16×2 LCD Keypad Shield
* Relay outputs
* Water-level sensors
* Push buttons
* Status LEDs

The LCD interface uses the Arduino LCD Keypad Shield configuration from the DFRobot Arduino LCD KeyPad Shield example.

### Washing Machine Outputs

The controller operates the following outputs:

| Output     | Arduino Pin | Function          |
| ---------- | ----------: | ----------------- |
| Wash Motor |          A2 | Washing/agitation |
| Spin Motor |          A3 | Spin operation    |
| Drain Pump |          A0 | Drain water       |
| Fill Valve |          A1 | Fill water        |

The relay outputs use active-low logic:

```cpp
#define Relay_ON  LOW
#define Relay_OFF HIGH
```

### Inputs

| Input            | Arduino Pin | Function                     |
| ---------------- | ----------: | ---------------------------- |
| Low Water Level  |          A4 | Detect low water level       |
| High Water Level |          A5 | Detect full/high water level |
| Power Button     |          D2 | Power / cycle selection      |
| Start Button     |         D10 | Start / pause / resume       |

The water-level inputs and buttons use `INPUT_PULLUP`.

### LCD

The 16×2 LCD uses:

```text
RS = D8
EN = D9
D4 = D4
D5 = D5
D6 = D6
D7 = D7
```

The LCD is initialized as a 16-column × 2-row display.

## Washing Cycle

### 1. Water Filling

The controller checks the water-level sensors and activates the fill valve when the water level is low.

A maximum filling timeout is also implemented.

```text
Low Water Level
       ↓
 Fill Valve ON
       ↓
High Water Level
       ↓
 Fill Valve OFF
```

The fill timeout is set to 6 minutes.

### 2. Wash

Once the required water level is reached, the wash motor is activated.

The initial wash interval is configured for 8 minutes.

### 3. Soak

After the first wash period, the motor stops and the controller enters a soak period.

```text
Wash
 ↓
Motor OFF
 ↓
Soak
 ↓
Second Wash
```

The soak interval is configured for 2 minutes.

### 4. Drain

After washing, the controller stops the wash motor and activates the drain pump.

The controller also monitors the low-water sensor to determine when draining has sufficiently completed.

### 5. Spin

Once the water level is sufficiently low, the controller transitions to spin.

The spin sequence includes:

1. Stop wash motor
2. Activate drain pump
3. Start spin motor
4. Continue spinning for the configured interval

The temporary spin/drain transition uses a short delay before enabling the spin motor.

### 6. Rinse

The rinse cycle refills the machine, runs the wash motor, and then proceeds to the next drain/spin stage.

The rinse interval is configured for 8 minutes.

### 7. Final Spin

The final spin stage runs for the configured spin interval.

When the cycle is complete, the controller:

* Stops the motors
* Stops the timers
* Clears the LCD
* Displays `Done`
* Turns the machine off

## Cycle Timing

The firmware defines the following timing parameters:

| Operation          | Duration |
| ------------------ | -------: |
| Water Fill Timeout |    6 min |
| Wash               |    8 min |
| Soak               |    2 min |
| Rinse              |    8 min |
| Drain              |    2 min |
| Temporary Spin     |    2 min |
| Final Spin         |    8 min |

These values are defined as configurable constants in the firmware.

## Control Architecture

The firmware uses a state-based control structure.

Major operating modes include:

```text
ModeOff
ModeFilling
ModeWash
ModeRinse
ModeSpin
ModeDrain
ModeSoak
ModeNext
ModeFinish
ModeFilled
ModeChkWaterLow
```

Within each major cycle, `NextStep` is used to determine the current stage of the operation.

This allows the controller to transition between:

```text
Filling
   ↓
Washing
   ↓
Soaking
   ↓
Draining
   ↓
Water Level Check
   ↓
Spinning
   ↓
Next Cycle
```

## Pause / Resume

The Start button is used to control the running state of the washing cycle.

The firmware supports:

```text
RUNNING
   ↓
PAUSE
   ↓
RESUME
```

The controller uses separate timers for the running cycle and total cycle time.

When paused, the timers are paused and the outputs are switched to the appropriate safe/off state.

## User Interface

The LCD displays the selected cycle and current operating state.

Examples include:

```text
WASH 45mins
Washing..

RINSE 18mins
Rinse..

Spin 10mins
Spin..

Filling...
Drain..
Soak..
Finshing...
```

The LCD also displays the elapsed cycle time in `MM:SS` format.

## Software Architecture

The main control functions are separated into individual functions:

```text
setup()
   │
   └── Initialize hardware

loop()
   │
   ├── detectPowerBtn()
   ├── detectStartBtn()
   ├── updateLCD()
   │
   └── Cycle Controller
          │
          ├── Wash()
          ├── Rinse()
          └── Spin()
                 │
                 └── WaterFills()

OFFMode()
   │
   └── Safely disable controlled outputs
```

The main loop selects the appropriate cycle function based on `ModeSelected`.

## Timing and State Management

The project uses the `Timer` library together with Arduino timing functions.

Two timers are maintained:

```cpp
Timer RunningTimer;
Timer TotalTimer;
```

`RunningTimer` is used to track the active operation while `TotalTimer` tracks the overall cycle time.

This allows the controller to support pause/resume behavior without simply restarting the cycle.

## Engineering Challenges

### Coordinating Multiple Actuators

The washing machine contains several independently controlled components:

* Fill valve
* Wash motor
* Spin motor
* Drain pump

The firmware must ensure that these components operate in the correct sequence.

### Water-Level Control

Two water-level inputs are used:

* Low-level detection
* High-level detection

These signals determine when filling should begin, when filling should stop, and when draining has progressed sufficiently.

### Cycle State Management

The washing process is not a single timed operation.

Different stages have different conditions and timing requirements. The firmware therefore maintains a current mode and next-step state to control transitions.

### Pause / Resume

The controller needs to stop the machine safely while preserving the current cycle timing.

The timer state is therefore used to distinguish between:

```text
STOPPED
RUNNING
PAUSED
```

### Hardware Interface

The Arduino operates as the low-voltage control system while the washing-machine components require separate switching/interface circuitry.

Relay outputs are used to interface the Arduino with the controlled devices.

## Project Structure

```text
arduino-washing-machine-controller/
│
├── README.md
│
├── src/
│   └── washing_machine_controller.ino
│
├── hardware/
│   ├── wiring-diagram.png
│   ├── pinout.md
│   └── components.md
│
├── documentation/
│   ├── system-overview.md
│   ├── operation.md
│   └── troubleshooting.md
│
├── photos/
│   ├── controller.jpg
│   ├── wiring.jpg
│   └── installation.jpg
│
└── CHANGELOG.md
```

## Project Status

**Status:** Completed

**Platform:** Arduino

**Language:** C/C++

**Project Type:** Embedded Control / Automation

## Skills Demonstrated

This project demonstrates practical experience in:

* Embedded C/C++
* Arduino programming
* Digital I/O
* Relay control
* Sensor interfacing
* Water-level detection
* State-based control
* Timing systems
* LCD user interfaces
* Button handling
* Embedded troubleshooting
* Automation
* Hardware/software integration

## Safety

This project interfaces with a washing machine and may involve mains voltage, high-current loads, rotating machinery, water, and electrical hazards.

The Arduino should **never be connected directly to mains-powered loads**.

Appropriate electrical isolation, relay/interface circuitry, fusing, grounding, enclosure, and other applicable safety measures must be used.

This repository documents the control software and engineering project for educational and portfolio purposes.

## Author

**Marco**

Embedded Electronics & Automation Project
