.section ".text.boot"

.global _start

_start:
mov     sp,     #0x8000
bl      main

empty_loop:
b empty_loop
