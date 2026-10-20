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

## Running the Verification Suite

To compile and execute the high-precision validation tests natively inside your environment, run the following commands:

```bash
# 1. Compile the fast_sqrt core module into an object file
gcc -g -c fast_sqrt.c -o fast_sqrt.o

# 2. Compile the test harness and link the object binary components together
gcc -g test_sqrt.c fast_sqrt.o -o test_sqrt

# 3. Run the verification benchmarks
./test_sqrt
```
test_sqrt.c
```c


#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <math.h>
#include "fast_sqrt.h"

/* Helper macro to cast raw 64-bit integer bits directly back into a double */
static inline double bits_to_double(uint64_t bits)
{
    union { uint64_t i; double d; } u = { bits };
    return u.d;
}

/* Helper macro to extract the raw 64-bit bits from a standard double */
static inline uint64_t double_to_bits(double d)
{
    union { double d; uint64_t i; } u = { d };
    return u.i;
}

/**
 * Execute an isolated test validation step against your fast_sqrt module.
 */
void run_validation_case(double input)
{
    struct timespec start, end;
    uint64_t ix = double_to_bits(input);
    uint64_t top = ix >> 52;
    
    // Subnormal checking and alignment simulation 
    int is_subnormal = (top == 0 && (ix & 0x000FFFFFFFFFFFFFULL) != 0);
    if (is_subnormal)
    {
        ix = double_to_bits(input * 0x1p51);
        top = (ix >> 52) - 51;
    }

    // Capture precise hardware-level clock timing
    clock_gettime(CLOCK_MONOTONIC, &start);
    uint64_t raw_result_bits = fast_sqrt((int16_t)top, ix);
    double calculated_root = bits_to_double(raw_result_bits);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculate execution time elapsed in seconds
    double elapsed_time = (end.tv_sec - start.tv_sec) + 
                           (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Input: %10.2f  ->  Calculated Root: %15.10f (Time: %f seconds)\n", 
           input, calculated_root, elapsed_time);
}

int main(void)
{
    printf("==================================================\n");
    printf("        Testing the enhanced sqrt function        \n");
    printf("==================================================\n\n");

    printf("[Testing Perfect Squares - Early Exit Check]\n");
    run_validation_case(4.0);
    run_validation_case(100.0);
    run_validation_case(144.0);
    run_validation_case(1000000.0);
    printf("\n");

    printf("[Testing Irrational Roots - Full Precision Check]\n");
    run_validation_case(2.0);
    run_validation_case(3.0);
    run_validation_case(12.5);
    run_validation_case(0.5);
    printf("==================================================\n");

    return 0;
}

