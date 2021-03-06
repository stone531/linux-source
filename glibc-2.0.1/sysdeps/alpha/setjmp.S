/* Copyright (C) 1992, 1994, 1996 Free Software Foundation, Inc.
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
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <sysdep.h>

/* The function __sigsetjmp_aux saves all the registers, but it can't
   reliably access the stack or frame pointers, so we pass them in as
   extra arguments.  */
ENTRY (__sigsetjmp)
	ldgp	$29, 0($27)
#ifdef PROF
	.set noat
	lda	AT, _mcount
	jsr	AT, (AT), _mcount
	.set at
#endif
	.prologue 1

	bis	$30, $30, $18		/* Pass SP as 3rd arg.  */
	bis	$15, $15, $19		/* Pass FP as 4th arg.  */
	jmp	$31, __sigsetjmp_aux	/* Call __sigsetjmp_aux.  */

	END(__sigsetjmp)
