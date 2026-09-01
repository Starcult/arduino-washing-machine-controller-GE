# Hardware Components

## Hardware List

| # | Component | Quantity | Function |
|---|---|---:|---|
| 1 | Arduino | 1 | Main controller |
| 2 | 16×2 LCD Keypad Shield | 1 | User interface and status display |
| 3 | Relay Board | 1 | Interface between Arduino and washing-machine loads | 4 small relay control wash, sping, drain, fill
| 4 | Water Level Sensor – Low | 1 | Detect low water level | Washer Parts
| 5 | Water Level Sensor – High | 1 | Detect high water level | Washer Parts
| 6 | Power Button | 1 | Power / cycle selection |
| 7 | Start Button | 1 | Start / pause / resume |
| 8 | Power LED | 1 | Power status indication |
| 9 | Start LED | 1 | Running status indication |
| 10 | Water Fill Valve | 1 | Controls water inlet | *washer parts
| 11 | Wash Motor | 1 | Washing/agitation | * Washer parts
| 12 | Spin Motor | 1 | Spin cycle | *Washer parts
| 13 | Drain Pump | 1 | Removes water from machine | * Washer parts
| 14 | Wiring / Connectors | — | Electrical connections |
| 15 | Motor Contactor | 2 | Motor for Wash and Spin |
> Component model numbers and electrical ratings should be added when they are confirmed from the physical hardware.

---

## Main Controller

### Arduino

**Quantity:** 1

**Function:** Main control unit

The Arduino runs the washing-machine control firmware and coordinates all inputs and outputs.

Main responsibilities:

- Washing-cycle control
- Water-level monitoring
- Fill valve control
- Wash motor control
- Spin motor control
- Drain pump control
- Button input
- LCD interface
- Status LEDs
- Cycle timing
- Pause / resume

---

## User Interface

### 16×2 LCD Keypad Shield

**Quantity:** 1

**Function:** User interface and machine status display

The LCD displays the selected cycle, current operating state, and cycle timer.

| LCD Signal | Arduino Pin |
|---|---:|
| RS | D8 |
| Enable | D9 |
| D4 | D4 |
| D5 | D5 |
| D6 | D6 |
| D7 | D7 |

### Power Button

**Quantity:** 1  
**Arduino Pin:** D2

Used for controller power and washing-cycle selection.

### Start Button

**Quantity:** 1  
**Arduino Pin:** D10

Used to:

- Start the selected cycle
- Pause the cycle
- Resume the cycle

---

## Sensors

### Low Water-Level Sensor

**Quantity:** 1  
**Arduino Pin:** A4

Used to detect when the water level has fallen sufficiently during draining.

### High Water-Level Sensor

**Quantity:** 1  
**Arduino Pin:** A5

Used to detect when the required water level has been reached.

Both water-level inputs use the Arduino internal pull-up resistors.

---

## Relay Interface

### Relay Board

**Quantity:** 1

The relay board provides the switching interface between the Arduino and the washing-machine loads.

The firmware uses active-low relay logic:

```cpp
#define Relay_ON  LOW
#define Relay_OFF HIGH
