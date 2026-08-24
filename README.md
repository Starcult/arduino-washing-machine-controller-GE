# Arduino Washing Machine Controller

An Arduino-based washing machine control system developed to replace or modify the original control logic of a washing machine.

## Project Overview

This project uses an Arduino microcontroller as the main control unit for a washing machine.

The controller is responsible for managing the washing cycle and coordinating multiple inputs and outputs according to the programmed sequence.

The project was developed as a hands-on embedded control and automation project, involving:

* Microcontroller programming
* Digital input/output control
* Timing and sequencing
* Hardware interfacing
* Relay/output control
* Safety interlocks
* Troubleshooting and testing

## System Architecture

```text
                    ┌─────────────────┐
                    │     User Input  │
                    │ Buttons / Switch│
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │     Arduino     │
                    │ Main Controller │
                    └────────┬────────┘
                             │
             ┌───────────────┼───────────────┐
             │               │               │
             ▼               ▼               ▼
        Water Control    Motor Control   Drain Control
             │               │               │
             └───────────────┼───────────────┘
                             │
                             ▼
                    Washing Machine
```

## Features

* Automated washing-cycle control
* Sequential control of washing-machine functions
* Digital input monitoring
* Digital output control
* Timed operations
* Safety/interlock logic
* Microcontroller-based control architecture

## Hardware

The project uses an Arduino microcontroller together with the required interface circuitry for the washing machine.

Typical system components include:

| Component                 | Function                        |
| ------------------------- | ------------------------------- |
| Arduino                   | Main controller                 |
| Input switches/sensors    | User input and status detection |
| Relay/interface circuitry | Control of external loads       |
| Water valve               | Water supply control            |
| Drain pump                | Water drainage                  |
| Motor control             | Washing/spinning operation      |
| Door/interlock circuit    | Safety control                  |

> **Note:** The exact hardware configuration depends on the washing machine used in this project.

## Software

**Platform:** Arduino

**Programming Language:** C/C++

The controller program handles the washing sequence, monitors inputs, controls outputs, and manages timing between different stages of the cycle.

## Washing Cycle

The overall control sequence can be represented as:

```text
              ┌─────────┐
              │  IDLE   │
              └────┬────┘
                   │
                   ▼
              ┌─────────┐
              │  FILL   │
              └────┬────┘
                   │
                   ▼
              ┌─────────┐
              │  WASH   │
              └────┬────┘
                   │
                   ▼
              ┌─────────┐
              │  DRAIN  │
              └────┬────┘
                   │
                   ▼
              ┌─────────┐
              │  RINSE  │
              └────┬────┘
                   │
                   ▼
              ┌─────────┐
              │  SPIN   │
              └────┬────┘
                   │
                   ▼
              ┌─────────┐
              │COMPLETE │
              └─────────┘
```

The actual sequence and control conditions are defined by the firmware.

## Hardware Interface

The Arduino interfaces with the washing-machine hardware through appropriate electrical interface circuitry.

The controller should **not directly connect Arduino GPIO pins to mains-powered loads**.

Where high-voltage or high-current devices are involved, suitable isolation and switching hardware must be used.

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

## Engineering Challenges

### Control Sequencing

A washing machine requires multiple operations to occur in a controlled sequence.

The controller therefore needs to coordinate different outputs while monitoring the corresponding inputs and conditions.

### Timing

Different stages of the washing cycle require different operating times.

The firmware manages these timing requirements to ensure that each stage operates for the required duration.

### Safety

Safety conditions are an important part of the controller design.

For example, motor operation should only be permitted when the required safety conditions are satisfied.

### Hardware Interface

The Arduino operates at low-voltage logic levels while some washing-machine components operate at significantly higher voltages and currents.

Interface and isolation circuitry are therefore required between the microcontroller and external loads.

## Testing

Testing was performed by verifying individual functions before integrating them into the complete washing cycle.

Testing areas include:

* Input detection
* Output switching
* Timing
* Washing-cycle sequencing
* Safety/interlock conditions
* Motor operation
* Water filling
* Drain operation

## Lessons Learned

This project provided practical experience with:

* Embedded C/C++ programming
* Arduino microcontrollers
* Digital I/O
* Hardware interfacing
* Control logic
* Timing and sequencing
* Electrical troubleshooting
* System integration
* Real-world automation

## Project Status

**Status:** Completed / In Development

**Platform:** Arduino

**Project Type:** Embedded Control / Automation

## Safety Notice

This project involves electrical and mechanical equipment.

Washing machines may contain **dangerous mains voltages and high-current components**.

Do not reproduce the electrical connections without understanding the relevant electrical safety requirements. Use appropriate isolation, fusing, switching devices, enclosures, grounding, and other protective measures.

This project documentation is provided for educational and portfolio purposes.

## Author

**Marco**

Hands-on embedded electronics and automation project.
