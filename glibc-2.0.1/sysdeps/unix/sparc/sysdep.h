/* Copyright (C) 1993, 1994, 1995 Free Software Foundation, Inc.
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

#include <sysdeps/unix/sysdep.h>

#ifdef	ASSEMBLER

#ifdef	NO_UNDERSCORES
/* Since C identifiers are not normally prefixed with an underscore
   on this system, the asm identifier `syscall_error' intrudes on the
   C name space.  Make sure we use an innocuous name.  */
#define	syscall_error	C_SYMBOL_NAME(__syscall_error)
#endif

#define	ENTRY(name)							      \
  .global C_SYMBOL_NAME(name);						      \
  .align 2;								      \
  C_LABEL(name)

#define	PSEUDO(name, syscall_name, args)				      \
  .global syscall_error;						      \
  ENTRY (name)								      \
  mov SYS_ify(syscall_name), %g1;				   	      \
  ta 0;									      \
  bcc 1f;								      \
  sethi %hi(syscall_error), %g1;					      \
  jmp %g1 + %lo(syscall_error);	nop;					      \
1:

#define	ret		retl; nop
#define	r0		%o0
#define	r1		%o1
#define	MOVE(x,y)	mov x, y

#endif	/* ASSEMBLER */
