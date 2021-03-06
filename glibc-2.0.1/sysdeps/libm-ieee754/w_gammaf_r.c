/* w_gammaf_r.c -- float version of w_gamma_r.c.
 * Conversion to float by Ian Lance Taylor, Cygnus Support, ian@cygnus.com.
 */

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: w_gammaf_r.c,v 1.4 1995/11/20 22:06:50 jtc Exp $";
#endif

/* 
 * wrapper float gammaf_r(float x, int *signgamp)
 */

#include "math.h"
#include "math_private.h"


#ifdef __STDC__
	float __gammaf_r(float x, int *signgamp) /* wrapper lgammaf_r */
#else
	float __gammaf_r(x,signgamp)              /* wrapper lgammaf_r */
        float x; int *signgamp;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_lgammaf_r(x,signgamp);
#else
        float y;
        y = __ieee754_lgammaf_r(x,signgamp);
        if(_LIB_VERSION == _IEEE_) return y;
        if(!__finitef(y)&&__finitef(x)) {
            if(__floorf(x)==x&&x<=(float)0.0)
	        /* gammaf pole */
                return (float)__kernel_standard((double)x,(double)x,141);
            else
	        /* gamma overflow */
                return (float)__kernel_standard((double)x,(double)x,140);
        } else
            return y;
#endif
}             
weak_alias (__gammaf_r, gammaf_r)
