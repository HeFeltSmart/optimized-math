/************************************************************************
 * OPTIMIZED_MATH SUBSYSTEM - fast_sqrt module
 * Copyright (c) 2026 Daniel J. DeLuca. All rights reserved.
 * 
 * This module is dual-licensed and available under choice of:
 * 1. OPEN-SOURCE TIER: GNU Affero General Public License v3 (AGPLv3)
 * 2. COMMERCIAL TIER: Proprietary Commercial License Agreement
 * 
 * For commercial licensing terms, fees, and procurement, contact:
 * dandeluca@att.net
 ************************************************************************/
#include "fast_sqrt.h"
#include <float.h>

#define MSB_OF_MANTISSA                     (uint64_t)0x1p52
#define HIGHEST_ACTUAL_BIT_OF_MANTISSA      (uint64_t)0x1p51
#define EXPONENT_BITS_TO_ADJUST_FOR         11
#define DOUBLE_EXPONENT_BIAS                0x3FF
#define ONE_AS_QUINT62                      ((uint64_t)0x1p62)

uint64_t fast_sqrt( int16_t exponent, uint64_t mantissa )
{
    // The exponent component is isolated and divided by two to establish 
    // the result exponent. Parity is checked to normalize the mantissa 
    // representation into the expected fixed-point register scale.
    int even = exponent & 1;
    uint64_t adjusted_mantissa = ( ( mantissa | MSB_OF_MANTISSA ) <<
                                   ( EXPONENT_BITS_TO_ADJUST_FOR - ( even? 1: 0 ) ) );
    exponent = ( exponent + DOUBLE_EXPONENT_BIAS ) >> 1;

    // Start by estimating what is the square of 1, which is known to be 1
    uint64_t estimate = ONE_AS_QUINT62;
    uint64_t estimate_squared = estimate;

    // Begin with the highest power of two smaller than the final result and
    // determine which bits should be set in the mantissa
    uint64_t final_mantissa = 0x00;
    uint64_t final_mantissa_bit = HIGHEST_ACTUAL_BIT_OF_MANTISSA;

    // Next what the one offset looks like as the exponent gets "adjusted"
    // In this way we are "multiplying" our estimate by 2 and the estimate squared
    // by 4
    estimate >>= 1;
    uint64_t adjusted_value_of_one = estimate >> 1;

    // Evaluate candidate iterations bit-by-bit using binomial expansion steps 
    // to detect overshoots against the target boundaries.
    for( uint16_t i = 0; i < ( DBL_MANT_DIG - 1 ); i++ )
    {
        // (n + 1)^2 = n^2 + 2n + 1
        uint64_t estimate_plus_one = estimate + adjusted_value_of_one;
        uint64_t estimate_plus_one_squared = estimate_squared + ( estimate << 1 ) +
                                             adjusted_value_of_one;

        // As long as the square of the estimate + 1 is not larger than x,
        // proceed to add that 1 to the estimate and set the bit in the final mantissa
        if( estimate_plus_one_squared <= adjusted_mantissa )
        {
            estimate = estimate_plus_one;
            estimate_squared = estimate_plus_one_squared;
            final_mantissa |= final_mantissa_bit;
        }

        // "Shift" the decimal point two bits with each iteration
        estimate >>= 1;
        adjusted_value_of_one >>= 2;
        final_mantissa_bit >>= 1;

        // If and when the exact square root is reached, this estimate is the solution
        if( estimate_squared == adjusted_mantissa )
        {
            break;
        }
    }    

    // Shift the divided exponent (stored in top) left by 52 places into its proper field
    uint64_t final_exponent = (uint64_t)exponent << ( DBL_MANT_DIG - 1 );

    // Return the 64 bit unsigned integer that holds the bit representation of the double
    // that represents the final result of the sqrt operation.
    return ( final_exponent | final_mantissa );

}
