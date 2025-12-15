## Peripheral Pin Select (PPS) – Rotary Encoder Interface

**Tools:** PIC32MX, MPLAB X, Embedded C  
**Concepts:** Peripheral Pin Select (PPS), external interrupts, GPIO, rotary encoders

### Overview
This project demonstrates the use of Peripheral Pin Select (PPS) on a PIC32
microcontroller to interface with a rotary encoder. PPS was used to map the
encoder output signals to external interrupt pins, allowing the system to
respond immediately to changes in position.

### Key Features
- Configuration of PPS to assign external interrupt functionality to GPIO pins
- Interrupt-driven rotary encoder input (no polling)
- Detection of rotational movement using encoder signal transitions
- Real-time response to user input

### What I Learned
- How to configure and use PPS on the PIC32
- Why PPS provides flexibility in pin assignments
- Using interrupts to handle fast, asynchronous inputs efficiently
