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
- start.elf
- kernel.img (this is the one that is generated from compiling your code)

### The two required files (besides kernel.img) can be found here: https://github.com/raspberrypi/firmware/tree/master/boot

### Some notes about the code structure:
- All the .h header files are in the "api" folder. Read these to see how to use the various modules.
- All the .c and .s source files are in the "src" folder. Read these to see how it works.
- Hardware_Demos.h contains a simple demo program for each of the modules as I add them.
- main.c is a place to run one of the demos to verify the module on the real board.
- Code files prefixed with "PSP" are part of the Processor Support Package. These files deal with registers and things close to the processor.
- Code files prefixed with "BSP" are part of the Board Support Package. These files support things like external encoders/displays/ADC/DAC stuff, etc. Stuff you attach to the Pi with wires.
