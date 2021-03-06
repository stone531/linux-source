/* Copyright (C) 1991, 92, 93, 94, 95, 96, 97 Free Software Foundation, Inc.
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
#define _ERRNO_H
#include <errnos.h>

.globl C_SYMBOL_NAME(errno)
.globl syscall_error

#undef syscall_error
#ifdef NO_UNDERSCORES
__syscall_error:
#else
syscall_error:
#endif
#if defined (EWOULDBLOCK_sys) && EWOULDBLOCK_sys != EAGAIN
	/* We translate the system's EWOULDBLOCK error into EAGAIN.
	   The GNU C library always defines EWOULDBLOCK==EAGAIN.
	   EWOULDBLOCK_sys is the original number.  */
	cmpl $EWOULDBLOCK_sys, %eax /* Is it the old EWOULDBLOCK?  */
	jne notb		/* Branch if not.  */
	movl $EAGAIN, %eax	/* Yes; translate it to EAGAIN.  */
notb:
#endif
#ifndef	PIC
	movl %eax, C_SYMBOL_NAME(errno)
#ifdef	_LIBC_REENTRANT
	pushl %eax
	call __errno_location
	popl %ecx
	movl %ecx, (%eax)
#endif
#else
	/* The caller has pushed %ebx and then set it up to
	   point to the GOT before calling us through the PLT.  */
	movl C_SYMBOL_NAME(errno@GOT)(%ebx), %ecx

#ifndef	_LIBC_REENTRANT
	/* Pop %ebx value saved before jumping here.  */
	popl %ebx
	movl %eax, (%ecx)
#else
	movl %eax, (%ecx)
	pushl %eax
	call C_SYMBOL_NAME(__errno_location@PLT)
	popl %ecx
	/* Pop %ebx value saved before jumping here.  */
	popl %ebx
	movl %ecx, (%eax)
#endif
#endif
	movl $-1, %eax
	ret

#undef	__syscall_error
END (__syscall_error)
