# This repository is a dumping ground for experimenting with bare metal programming on the Raspberry Pi 3b+ using the C language.

### The idea is to have a minimal environment for testing out ideas, in order to work through the various peripherals of the Pi while writing the same programs in assembly for a computer architecture course.

### It is hoped that accessing the peripherals in C will aid in understanding and debugging the assembly code.

### How to use this repo:

1. Clone a local copy on your machine.
2. Get your toolchain set up, see here: https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/downloads.html#gnu
3. Build the **kernel.img** file by using the **make** command (*see note below) from within the **build** directory.
4. The newly generated **kernel.img** will be appear in the root directory of this project.
5. Replace the kernel.img file on your Raspberry Pi SD card with the new one you just generated. (backup the old one if desired)
6. Plug the SD card back into the Pi and apply power to the Pi.
7. Use the **make clean** command from within the **build** directory to delete all generated binary files and kernel.img.

### *Note:
- The makefile allows you to choose a target demo application from the command line.
- Look through the /examples/ directory and select an example to demo.
- To build the example: **$ make demo TARGET=[name of the example application without the extension]**
- For example, to make the "simple_blink" demo: **$ make demo TARGET=simple_blink**
- If you don't select an example, it will choose the default example for you (which is simple_blink right now)

### These are the files that need to be on your SD card for it to boot:
- bootcode.bin
- start.elf
- kernel.img (this is the one that is generated from compiling your code)

### The two required files (besides kernel.img) can be found here: https://github.com/raspberrypi/firmware/tree/master/boot

### Some notes about the code structure:
- The Makefile and linker script are in the "build" folder.
- Object files are generated in the "bin" folder. This folder is automatically generated when the **make** command is invoked.
- All the .h header files are in the "include" folder. Read these to see how to use the various modules.
- All the .c and .s source files are in the "src" folder. Read these to see how it works. Keep a copy of the datasheet open while reading the .c files.
- Example demo applications are in the "examples" folder. See these for demo applications.
- Code files prefixed with "PSP" are part of the Processor Support Package. These files deal with registers and things close to the processor.
- Code files prefixed with "BSP" are part of the Board Support Package. These files support things like external encoders/displays/ADC/DAC stuff, etc. Stuff you attach to the Pi with wires.

### License
- This project is licensed under the MIT License. See LICENSE for details.
 