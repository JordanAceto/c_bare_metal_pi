# This repository is a dumping ground for experimenting with bare metal programming on the Raspberry Pi 3b+ using the C language.

### The idea is to have a minimal environment for testing out ideas, in order to work through the various peripherals of the Pi while writing the same programs in assembly for a computer architecture course.

### It is hoped that accessing the peripherals in C will aid in understanding and debugging the assembly code.

### How to use this repo:

1. Clone a local copy on you machine.
2. Get your toolchain set up, see here: https://www.cl.cam.ac.uk/projects/raspberrypi/tutorials/os/downloads.html#gnu
3. Build the kernel.img file by using the **make** command.
4. Replace the kernel.img file on your Raspberry Pi SD card with the new one you just generated. (backup the old one if desired)
5. Plug the SD card back into the Pi and see what you messed up.
6. Get sad.
7. fix the problem.
8. Goto step 3
