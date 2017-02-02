# Libraries

## Description

Software Libaries for the Uno32


## Rules 
Ideally each folder below this one is its own MPLABX project containing one module.  Each module should have its own markdown file
describing the module function at a more detailed level. Every project should have its own test harness that is turned on using a #ifdef structure.


## Index

* [AD](AD.x\README.md): Provides access to the A/D converter. Gives ability to add/remove pins as well as monitoring battery voltage 
* [BOARD](BOARD.x\README.md): Low level hardware defines along with hardware initialization, sets up the stack.
* [ES_Framework](ES_Framework.X\README.md): Events and Services Framework, TattleTale, allows for simple services, FSM and HSM structures 
* [IO_Ports](IO_Ports.x\README.md): Helper library to map Uno32 Pins to logical pins on the Uno32 Stack
* [LED](LED.X\README.md): Helper library to map the LED bank on the Uno32 Stack and provide access functions
* [PWM](PWM.x\README.md): Gives access to all PWM pins allowing to change both duty cycle and frequency
* [RC_Servo](RC_Servo.X\README.md): Driver library for connecting RC Servos to 10 pins on the Uno32 Stack.
* [Roach_LIB](Roach_LIB.x\README.md): Wrapper library to give higher level functions for controlling the Roach 
* [SERIAL](SERIAL.x\README.md): Serial access through use of a circular buffer. Links into printf along with primitives to determine if buffer is full.
* [Stepper](Stepper.X\README.md): Simple stepper motor driver code, implements full step and use of the DRV8814 driver board
* [Timers](Timers.X\README.md): Timer Module that implements software timers based on a single hardware timer