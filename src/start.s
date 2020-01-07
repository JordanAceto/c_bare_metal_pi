/**
 * DESCRIPTION:
 *      start.s simply provides an assembly start routine prior to branching to the 
 *      main c function.
 * 
 * NOTES:
 *      None.
 * 
 * REFERENCES:
 *      None
 */

.section ".text.boot"

.global _start

_start:
mov     sp,     #0x8000
bl      main

empty_loop:
b empty_loop
