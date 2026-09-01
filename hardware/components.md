# Hardware Components

## Overview

This project uses an Arduino-based control system to operate a washing machine.

The Arduino monitors water-level sensors and user controls, and operates the washing machine's fill valve, wash motor, spin motor, and drain pump through relay outputs.

The system also includes a 16×2 LCD display and status LEDs for user feedback.

---

## Main Controller

### Arduino

**Function:** Main control unit

The Arduino runs the washing machine control firmware and manages:

* Washing cycle sequencing
* Water-level monitoring
* Fill valve control
* Wash motor control
* Spin motor control
* Drain pump control
* User buttons
* LCD display
* Status LEDs
* Cycle timing
* Pause / resume operation

---

## User Interface

### 16×2 LCD Keypad Shield

**Function:** User interface and machine status display

The LCD displays the current operating mode and cycle status.

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

The LCD is connected to the Arduino using:

| LCD Signal | Arduino Pin |
| ---------- | ----------: |
| RS         |          D8 |
| Enable     |          D9 |
| Data 4     |          D4 |
| Data 5     |          D5 |
| Data 6     |          D6 |
| Data 7     |          D7 |

The firmware initializes the display as a 16×2 LCD.

---

## Push Buttons

### Power Button

**Arduino Pin:** D2

The Power button is used to control the controller power state and select the washing cycle.

The input is configured using the Arduino's internal pull-up resistor.

```text
Arduino D2
    │
    └── Power Button
            │
           GND
```

### Start Button

**Arduino Pin:** D10

The Start button controls the washing cycle.

It is used to:

* Start the selected cycle
* Pause the cycle
* Resume the cycle

The input is configured using the Arduino's internal pull-up resistor.

---

## Water-Level Sensors

### Low Water-Level Sensor

**Arduino Pin:** A4

The low-level sensor is used to determine whether the water level has fallen sufficiently during the drain process.

It is also used as part of the water-filling control logic.

### High Water-Level Sensor

**Arduino Pin:** A5

The high-level sensor detects when the washing machine has reached the required water level.

The controller uses this signal to determine when filling is complete and when the washing/rinsing process can continue.

Both water-level inputs are configured using `INPUT_PULLUP`.

---

# Relay-Controlled Outputs

The Arduino controls the washing-machine loads through relay/interface outputs.

The firmware uses active-low relay logic:

```cpp
#define Relay_ON  LOW
#define Relay_OFF HIGH
```

Therefore:

| Arduino Output | Relay State | Function          |
| -------------- | ----------- | ----------------- |
| LOW            | ON          | Activate output   |
| HIGH           | OFF         | Deactivate output |

---

## Wash Motor

**Arduino Pin:** A2

The wash motor is used during the washing and rinsing stages.

The firmware controls the wash motor independently from the spin motor.

```text
Arduino A2
    │
    ▼
Relay / Interface
    │
    ▼
Wash Motor
```

---

## Spin Motor

**Arduino Pin:** A3

The spin motor is used during the spin stages.

The controller ensures that the wash and spin outputs are controlled separately.

```text
Arduino A3
    │
    ▼
Relay / Interface
    │
    ▼
Spin Motor
```

---

## Drain Pump

**Arduino Pin:** A0

The drain pump removes water from the washing machine.

It is activated during drain operations and remains active while the controller monitors the water-level sensor.

```text
Arduino A0
    │
    ▼
Relay / Interface
    │
    ▼
Drain Pump
```

---

## Water Fill Valve

**Arduino Pin:** A1

The fill valve controls the supply of water into the washing machine.

The controller activates the valve when the water level is low and stops filling when the required water level is reached.

A fill timeout is also implemented in the firmware to prevent the filling operation from continuing indefinitely.

---

# Status LEDs

## Power LED

**Arduino Pin:** D3

The Power LED provides visual indication of the controller's power state.

## Start LED

**Arduino Pin:** D11

The Start LED provides visual feedback related to the running state of the washing cycle.

The firmware also uses blinking behaviour to provide additional status indication.

---

# Pin Assignment

The current firmware uses the following pin assignment:

| Arduino Pin | Component         | Direction | Function               |
| ----------- | ----------------- | --------- | ---------------------- |
| A0          | Drain Pump Relay  | Output    | Drain control          |
| A1          | Fill Valve Relay  | Output    | Water filling          |
| A2          | Wash Motor Relay  | Output    | Wash/agitation         |
| A3          | Spin Motor Relay  | Output    | Spin                   |
| A4          | Low Water Sensor  | Input     | Low-level detection    |
| A5          | High Water Sensor | Input     | High-level detection   |
| D2          | Power Button      | Input     | Power / mode selection |
| D3          | Power LED         | Output    | Power indication       |
| D4          | LCD Data 4        | Output    | LCD                    |
| D5          | LCD Data 5        | Output    | LCD                    |
| D6          | LCD Data 6        | Output    | LCD                    |
| D7          | LCD Data 7        | Output    | LCD                    |
| D8          | LCD RS            | Output    | LCD                    |
| D9          | LCD Enable        | Output    | LCD                    |
| D10         | Start Button      | Input     | Start / pause / resume |
| D11         | Start LED         | Output    | Run status             |

---

# System Block Diagram

```text
                         ┌─────────────────────┐
                         │       Arduino       │
                         │   Main Controller   │
                         └──────────┬──────────┘
                                    │
             ┌──────────────────────┼──────────────────────┐
             │                      │                      │
             ▼                      ▼                      ▼
      User Interface          Water Sensors          Relay Outputs
             │                      │                      │
       ┌─────┴─────┐          ┌─────┴─────┐       ┌──────┼───────┐
       │           │          │           │       │      │       │
       ▼           ▼          ▼           ▼       ▼      ▼       ▼
      LCD        Buttons     LOW        HIGH    Valve  Motors    Pump
                              Level      Level
```

---

# Washing Machine Components

The firmware identifies the following washing-machine functions:

* Water inlet / fill valve
* Wash motor
* Spin motor
* Drain pump
* Low water-level detection
* High water-level detection

These components are coordinated by the Arduino firmware to perform the complete washing cycle.

---

# Control Sequence

The controller operates the washing machine through several states:

```text
                 ┌─────────┐
                 │  FILL   │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  WASH   │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  SOAK   │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  WASH   │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  DRAIN  │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  SPIN   │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  RINSE  │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  DRAIN  │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │  SPIN   │
                 └────┬────┘
                      ↓
                 ┌─────────┐
                 │ COMPLETE│
                 └─────────┘
```

---

# Electrical Interface

The Arduino outputs are intended to control external loads through relay or suitable interface circuitry.

The Arduino GPIO pins must not be used to directly drive mains-powered washing-machine components.

The relay/interface stage provides the required electrical isolation and switching capability between the low-voltage controller and the washing-machine hardware.

---

# Hardware Documentation

The following documentation can be added to this directory as the hardware is further documented:

```text
hardware/
├── components.md
├── pinout.md
└── wiring-diagram.png
```

Photographs of the completed controller and installation are stored separately:

```text
photos/
├── control-panel.jpg
├── controller-wiring.jpg
├── control-board.jpg
└── relay-component.jpg
```

---

# Safety

This project interfaces with electrical, mechanical, and water-containing equipment.

A washing machine may contain hazardous mains voltage and high-current circuits.

**Never connect Arduino GPIO pins directly to mains-powered loads.**

Appropriate relay/interface circuitry, electrical isolation, fusing, grounding, enclosure, and other applicable safety measures should be used.

This documentation is intended to describe the engineering and software architecture of the project and is not a substitute for applicable electrical safety requirements.

---

# Notes

This document describes the hardware interface represented by the current Arduino firmware.

Specific manufacturer part numbers, relay ratings, motor ratings, valve specifications, and detailed wiring information should be added when the physical components are positively identified.
