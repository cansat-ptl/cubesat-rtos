# Cubesat RTOS

cansat-kernel-v2 remade from scratch

## Usage
1. Run ```make arch=<arch> mcu=<mcu>``` to compile the kernel. Supported MCUs are currently ```atmega128``` and ```atmega2560``` only. Set arch to ```avr```.
2. Link ```librtos.a``` with your application (```-lrtos``` flag).

API docs: https://thepetrovich.github.io/doxygen/
