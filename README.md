# Optimized Math Subsystem

A high-performance, bit-exact, branchless fixed-point mathematics engine.

## Module: fast_sqrt

The `fast_sqrt` module implements a highly optimized, bit-by-bit fixed-point square root extraction loop utilizing a virtual radix shift to prevent bit decay and register overflow.

### Repository File Architecture
* `fast_sqrt.h` - Module interface and datatype configurations.
* `fast_sqrt.c` - Core extraction loop implementation.

### Integration Instructions
To link this module into an external C library or standalone target:
1. Include `fast_sqrt.c` and `fast_sqrt.h` in your project toolchain.
2. Call the module interface from your library's standard mathematical wrapper layer:

```c
#include <stdint.h>

/* Link to the standalone optimized module binary */
uint64_t fast_sqrt(int16_t exponent, uint64_t mantissa);

double sqrt(double x)
{
    // ... insert standard library boundary and subnormal checks ...

    // Invoke the decoupled core loop and cast the raw return bits instantly
    return asdouble(fast_sqrt((int16_t)top, ix));
}
```
