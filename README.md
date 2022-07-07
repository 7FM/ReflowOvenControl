# ReflowOvenControl
This repo is a combination of the following great projects:
- https://github.com/dasaki/nanoReflowController
- https://github.com/0xPIT/reflowOvenController
- https://github.com/engineertype/Controleo3.git

with the focus of using the UI schemes of the first two projects and using the profile management & control algorithms from [Controleo3](https://www.whizoo.com/).

The whole project targets an Arduino Nano and therefore almost all used Libraries where optimized for space and sometimes for performance too.

## Profiles
As mentiened before this projects uses the Controleo Profile Management but with a reduced Token set (see: src/read_profiles.cpp or include/read_profiles.hpp).
A good overview for all tokens(also non supported ones) with explanation can be found [here](https://www.whizoo.com/profiles).

## Hardware
- Arduino Nano
- 1.8'' Color TFT display (ST7735R) as from Adafruit: https://www.adafruit.com/product/358 (can be replaced with serial UI only)
- Rotary Encoder (can be replaced with serial UI only)
- SD Card reader
- Thermocouple reader (MAX6675)
- Buzzer (optional)
- Servo (optional)
- SSR or something to control the oven
- oven to be controlled
- cooling fan (optional)
- convection fan (optional)

For convenience a custom PCB was created using [KiCad](https://www.kicad.org/) but is not mandatory.
Please note that I am no professional (resistors & transistor were arbitrarily chosen by own availability) so any use of the PCB at your own risk!
I also designed some 3D Models using [FreeCAD](https://www.freecadweb.org/) for mounting the hardware.

## Building
Clone the repository with `git clone --recursive` as submodules are used.
If already cloned, the submodules can also be downloaded afterwards using: `git submodule update --init --recursive`.
For compiling simply use [PlatformIO](https://platformio.org/).

## Configuration
Features, Pins & more can be configured in `src/config.h`. Feel free to ask if the impact of certain options is unclear.

## Useful ressources
- https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf
- https://academy.cba.mit.edu/classes/embedded_programming/doc1022.pdf
- http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-0856-AVR-Instruction-Set-Manual.pdf
