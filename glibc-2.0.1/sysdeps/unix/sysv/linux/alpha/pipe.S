/* Copyright (C) 1993, 1995 Free Software Foundation, Inc.
   Contributed by David Mosberger (davidm@cs.arizona.edu).

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

/* __pipe is a special syscall since it returns two values.  */

#include <sysdep.h>

PSEUDO (__pipe, pipe, 0)
	stl	r0, 0(a0)
	stl	r1, 4(a0)
	mov	zero, v0
	ret
PSEUDO_END(__pipe)

weak_alias (__pipe, pipe)
