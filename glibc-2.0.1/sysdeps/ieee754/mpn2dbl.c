/* Copyright (C) 1995, 1996 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#include "gmp.h"
#include "gmp-impl.h"
#include "ieee754.h"
#include <float.h>

/* Convert a multi-precision integer of the needed number of bits (53 for
   double) and an integral power of two to a `double' in IEEE754 double-
   precision format.  */

double
__mpn_construct_double (mp_srcptr frac_ptr, int expt, int negative)
{
  union ieee754_double u;

  u.ieee.negative = negative;
  u.ieee.exponent = expt + IEEE754_DOUBLE_BIAS;
#if BITS_PER_MP_LIMB == 32
  u.ieee.mantissa1 = frac_ptr[0];
  u.ieee.mantissa0 = frac_ptr[1] & ((1 << (DBL_MANT_DIG - 32)) - 1);
#elif BITS_PER_MP_LIMB == 64
  u.ieee.mantissa1 = frac_ptr[0] & ((1L << 32) - 1);
  u.ieee.mantissa0 = (frac_ptr[0] >> 32) & ((1 << (DBL_MANT_DIG - 32)) - 1);
#else
  #error "mp_limb size " BITS_PER_MP_LIMB "not accounted for"
#endif

  return u.d;
}
