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
#ifndef FAST_SQRT_H
#define FAST_SQRT_H

/* Force the compiler to load exact-width integer types for this module */
#include <stdint.h>
#include <libm.h>

uint64_t fast_sqrt( int16_t exponent, uint64_t mantissa );

#endif
