#ifndef FAST_SQRT_H
#define FAST_SQRT_H

/* Force the compiler to load exact-width integer types for this module */
#include <stdint.h>
#include <libm.h>

uint64_t fast_sqrt( int16_t exponent, uint64_t mantissa );

#endif