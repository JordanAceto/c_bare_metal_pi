# This repository is a dumping ground for experimenting with bare metal programming on the Raspberry Pi 3b+ using the C language.

### The idea is to have a minimal environment for testing out ideas, in order to work through the various peripherals of the Pi while writing the same programs in assembly for a computer architecture course.

### It is hoped that accessing the peripherals in C will aid in understanding and debugging the assembly code.

### As peripheral modules are added, I'll add simple demos to the Hardware_Demos.h file. Then, in main.c, uncomment exactly one of the peripheral demo functions before compiling.

### How to use this repo:

1. Clone a local copy on your machine.
2. Get your toolchain set up, see here: https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/downloads.html#gnu
3. Build the kernel.img file by using the **make** command.
4. Replace the kernel.img file on your Raspberry Pi SD card with the new one you just generated. (backup the old one if desired)
5. Plug the SD card back into the Pi and see what you messed up.
6. Get sad.
7. Rage when you realize you had a (!) where you should have had a (~), fix it.
8. Goto step 3

### These are the files that need to be on your SD card for it to boot:
- bootcode.bin
- fixup.dat
- start.elf
- kernel.img (this is the one that is generated from compiling your code)

### The three required files (besides kernel.img) can be found here: https://github.com/raspberrypi/firmware/tree/master/boot

### Some notes about the code structure:
- main.c is a sketchpad for whatever peripherals I'm working on.
- Code files prefixed with "PSP" are part of the Processor Support Package. These files deal with registers and things close to the processor.
- Code files prefixed with "BSB" are part of the Board Support Package. These files support things like communication protocols, ADC/DAC stuff, etc.
- At a minimum, most PSP/BSP .h files will define register addresses and set up pointers to those registers for reading/writing to registers directly.
