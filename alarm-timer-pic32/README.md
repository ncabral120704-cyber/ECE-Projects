## Piezo Buzzer Alarm Timer (PIC32)

**Tools:** PIC32MX, MPLAB X, C, SNAP Debugger  
**Concepts:** Timers, external interrupts, GPIO, seven-segment displays

### Overview
This project implements a one-minute alarm timer using a PIC32 microcontroller.
When a button is pressed, a 60-second countdown is displayed on two seven-segment
displays. When the timer reaches zero, a piezo buzzer sounds for 10 seconds or
until the button is pressed again.

### Key Features
- External interrupt for immediate button response
- Timer-based countdown logic
- Two-digit seven-segment display output
- Automatic alarm shutoff after 10 seconds

### What I Learned
- Using hardware timers for precise timing
- Handling external interrupts reliably
- Designing simple real-time embedded systems

