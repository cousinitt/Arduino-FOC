---
layout: default
title: Library Source
nav_order: 7
parent: Using the Code
description: "Arduino Simple Field Oriented Control (FOC) library ."
permalink: /source_code
has_children: True
has_toc: false
---

# Arduino <span class="simple">Simple<span class="foc">FOC</span>library</span> source code
The arduino library code is organized into the standard [Arduino library structure](https://github.com/arduino/Arduino/wiki/Library-Manager-FAQ). 
The library contains main BLDC motor class `BLDCMotor` and  two sensor classes `Encoder` and `MagneticSensor` implementing the `Sensor`. Furthermore it has `FOCutils` file with all the necessary utility functions and `defaults.h` with all the default configuration variables.

## Arduino library source structure
```sh
| src
| ├─ SimpleFOC.h               # SimpleFOC library include file
| |
| ├─ BLDCMotor.cpp/h           # BLDCMotor class implementing all the FOC operations
│ │ 
│ ├─ Sensor.h                  # Abstract Sensor class that all the sensors implement
│ ├─ Encoder.cpp/h             # Encoder class implementing the Quadrature encoder operations
│ ├─ MagneticSensor.cpp/h      # class implementing SPI angle read and interface for AS5047/8 type sensors
| |
| ├─ FOCutils.cpp/h            # Utility functions 
| |
  └─ defaults.h                # Default configuration values 
```

<blockquote class="info">For more info visit <a href="http://source.simplefoc.com/" target="_blank"> full source code documentation <i class="fa fa-external-link fa-sm"></i></a></blockquote>

### `BLDCMotor.cpp/h`
BLDCMotor class implementation
- BLDC driver hardware configuration: `init()`, `pwmA,pwmB,pwmC,enable`
- FOC algorithm functions and configuration: `loopFOC()`, `initFOC`,`foc_modulation`
- PI controllers for the velocity and angle control: `PI_s`, `controllerPI()`,`velocityPI()`,`positionP`
- Monitoring functions: `useMonitoring()`,`monitor()`,`monitor_port`
- Communication interface with user in form of commands: `command()`  

<blockquote class="info"><a href="foc_implementation"><i class="fa fa-copy"></i> FOC implementation details</a> - Documentation of the procedures and detailed explanations of the code implementing FOC algorithm 
</blockquote>
<blockquote class="info">
     <a href="motion_control_implementation"><i class="fa fa-copy"></i> Motion control implementation details</a> - Documentation of the motion control algorithms and code implementation choices
</blockquote>


### `Sensor.h`
Abstract sensor class that every sensor needs to implement in order to be connectable to the motor (`BLDCMotor` class). 
If you want to implement your own version of the sensor, jut extend this class and implement the virtual functions and you will be able to run the FOC algorithm with it.
You will be abele to link motor and the sensor by doing `motor.linkSensor(your sensor)`
```cpp
class Sensor{
public:
    // get current angle (rad) 
    virtual float getAngle();
    // get current angular velocity (rad/s)
    virtual float getVelocity();
    // set current angle as zero angle 
    // return the angle [rad] difference
    virtual float initRelativeZero();
    // set absolute zero angle as zero angle
    // return the angle [rad] difference
    virtual float initAbsoluteZero();

    // returns 0 if it has no absolute 0 measurement
    // 0 - incremental encoder without index
    // 1 - encoder with index & magnetic sensors
    virtual int hasAbsoluteZero();
    // returns 0 if it does need search for absolute zero
    // 0 - magnetic sensor (& encoder with index which is found)
    // 1 - encoder with index (with index not found yet)
    virtual int needsAbsoluteZeroSearch();
}
```

### `Encoder.cpp/h`
Quadrature Encoder class implementation  
- Extends `Sensor` class
- Encoder interrupt functions and configuration: `enableInterrupt()`, `handleA()`, `handleB()`, `handleIndex()`...
- Calculates motor angle and velocity ( using the [Mixed Time Frequency Method](https://github.com/askuric/Arduino-Mixed-Time-Frequency-Method)). 
- Support any type of optical and magnetic encoder. 

### `MagneticSensor.cpp/h`
Absolute Magnetic sensor(encoder) class implementation
- Extends `Sensor` class
- SPI communication
- Calculates motor angle and velocity
- Supports magnetic position sensors such as AS5048, AS5047 and similar. 
  

### `FOCutils.cpp/h`
This is implementation of all the necessary hardware specific and FOC utility functions.
```cpp
// High PWM frequency setting function
// - hardware specific
void setPwmFrequency(int pin);

// Function implementing delay() function in milliseconds 
// - blocking function
// - hardware specific
void _delay(unsigned long ms);

//Function implementing timestamp getting function in microseconds
// hardware specific
unsigned long _micros();

// Function approximating the sine calculation by using fixed size array
// - execution time ~40us (Arduino UNO)
float _sin(float a);
// Function approximating cosine calculation by using fixed size array
// - execution time ~50us (Arduino UNO)
float _cos(float a);
```


### `defaults.h`
Header file containing all the default configuration variables
```cpp
// default configuration values
// change this file to optimal values for your application

#define DEF_POWER_SUPPLY 12.0 // default power supply voltage
// velocity PI controller params
#define DEF_PI_VEL_P 0.5 // default PI controller P value
#define DEF_PI_VEL_I 10 //  default PI controller I value
#define DEF_PI_VEL_U_RAMP 1000 // default PI controller voltage ramp value
// angle P params
#define DEF_P_ANGLE_P 20 // default P controller P value
#define DEF_P_ANGLE_VEL_LIM 20 // angle velocity limit default
// index search 
#define DEF_INDEX_SEARCH_TARGET_VELOCITY 1 // default index search velocity
// align voltage
#define DEF_VOLTAGE_SENSOR_ALIGN 6.0 // default voltage for sensor and motor zero alignment
// low pass filter velocity
#define DEF_VEL_FILTER_Tf 0.005 // default velocity filter time constant
```

### `SimpleFOC.h`
The main library include file, its contents is:
```cpp
#include "FOCutils.h"
#include "Sensor.h"
#include "Encoder.h"
#include "MagneticSensor.h"
#include "BLDCMotor.h"
```


## Digging deeper

For more info about the FOC procedures and detailed explanations of the code implementing FOC algorithm please visit: <a href="foc_implementation"> FOC implementation details <i class="fa fa-external-link fa-sm"></i></a>

For documentation of the motion control algorithms and code implementation choices, visit <a href="motion_control_implementation"> Motion control implementation details <i class="fa fa-external-link fa-sm"></i></a>

The library comes with a lot of motor control examples for different microcontrollers. See more on [library examples <i class="fa fa-external-link"></i>](library_examples)

To dig deeper in the source code please visit <a href="http://source.simplefoc.com/" target="_blank"> doxygen generated code documentation <i class="fa fa-external-link fa-sm"></i></a>

<div class="image_icon width80" >
    <a href="http://source.simplefoc.com/" target="_blank">
        <img src="extras/Images/source_docs.jpg" >
        <i class="fa fa-external-link-square fa-2x"></i>
    </a>
</div>
