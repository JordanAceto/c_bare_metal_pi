/**
 * DESCRIPTION:
 *      Fixed Width Ints simply provides typedefs for standard fixed width
 *      integer types.
 * 
 * NOTES:
 *      Assumes the Raspberry Pi is operating in 32 bit mode.
 * 
 * REFERENCES:
 *      https://raspberry-projects.com/pi/programming-in-c/memory/variables
 */


#ifndef FIXED_WIDTH_INTS_H_INCLUDED
#define FIXED_WIDTH_INTS_H_INCLUDED

typedef signed char          int8_t; // -128 to 127
typedef unsigned char       uint8_t; // 0 to 255
typedef short int           int16_t; // -32768 to 32767
typedef unsigned short int uint16_t; // 0 to 65535
typedef int                 int32_t; // -2147483648 to 2147483647
typedef unsigned int       uint32_t; // 0 to 4294967295
typedef long long           int64_t; // −9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
typedef unsigned long long uint64_t; // 0 to 18,446,744,073,709,551,615

typedef volatile int8_t     vint8_t;
typedef volatile uint8_t   vuint8_t;
typedef volatile int16_t   vint16_t;
typedef volatile uint16_t vuint16_t;
typedef volatile int32_t   vint32_t;
typedef volatile uint32_t vuint32_t;
typedef volatile int64_t   vint64_t;
typedef volatile uint64_t vuint64_t;

#endif
