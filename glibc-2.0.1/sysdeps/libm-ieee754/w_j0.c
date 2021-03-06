/* @(#)w_j0.c 5.1 93/09/24 */
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
static char rcsid[] = "$NetBSD: w_j0.c,v 1.6 1995/05/10 20:49:11 jtc Exp $";
#endif

/*
 * wrapper j0(double x), y0(double x)
 */

#include "math.h"
#include "math_private.h"

#ifdef __STDC__
	double __j0(double x)		/* wrapper j0 */
#else
	double __j0(x)			/* wrapper j0 */
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_j0(x);
#else
	double z = __ieee754_j0(x);
	if(_LIB_VERSION == _IEEE_ || __isnan(x)) return z;
	if(fabs(x)>X_TLOSS) {
	        return __kernel_standard(x,x,34); /* j0(|x|>X_TLOSS) */
	} else
	    return z;
#endif
}
weak_alias (__j0, j0)
#ifdef NO_LONG_DOUBLE
strong_alias (__j0, __j0l)
weak_alias (__j0, j0l)
#endif


#ifdef __STDC__
	double __y0(double x)		/* wrapper y0 */
#else
	double __y0(x)			/* wrapper y0 */
	double x;
#endif
{
#ifdef _IEEE_LIBM
	return __ieee754_y0(x);
#else
	double z;
	z = __ieee754_y0(x);
	if(_LIB_VERSION == _IEEE_ || __isnan(x) ) return z;
        if(x <= 0.0){
                if(x==0.0)
                    /* d= -one/(x-x); */
                    return __kernel_standard(x,x,8);
                else
                    /* d = zero/(x-x); */
                    return __kernel_standard(x,x,9);
        }
	if(x>X_TLOSS) {
	        return __kernel_standard(x,x,35); /* y0(x>X_TLOSS) */
	} else
	    return z;
#endif
}
weak_alias (__y0, y0)
#ifdef NO_LONG_DOUBLE
strong_alias (__y0, __y0l)
weak_alias (__y0, y0l)
#endif
