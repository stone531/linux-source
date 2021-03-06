/* Copyright (C) 1991, 92, 93, 94, 95, 96 Free Software Foundation, Inc.
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

/*
 *	ISO C Standard: 4.1.3 Errors	<errno.h>
 */

#ifndef	_ERRNO_H

/* The includer defined __need_Emath if he wants only the definitions
   of EDOM and ERANGE, and not everything else.  */
#ifndef	__need_Emath
#define	_ERRNO_H	1
#include <features.h>
#endif

__BEGIN_DECLS

/* Get the error number constants from the system-specific file.
   This file will test __need_Emath and _ERRNO_H.  */
#include <errnos.h>
#undef	__need_Emath

#ifdef	_ERRNO_H

/* Declare the `errno' variable, unless it's defined as a macro by
   errnos.h.  This is the case in GNU, where it is a per-thread variable.
   This redeclaration using the macro still works, but it will be a
   function declaration without a prototype and may trigger a
   -Wstrict-prototypes warning.  */
#ifndef	errno
extern int errno;
#endif

#ifdef __USE_GNU

/* The Hurd <errnos.h> defines `error_t' as an enumerated type
   so that printing `error_t' values in the debugger shows the names.  */
#ifndef __error_t_defined
typedef int error_t;
#define __error_t_defined	1
#endif

/* The full and simple forms of the name with which the program was
   invoked.  These variables are set up automatically at startup based on
   the value of ARGV[0] (this works only if you use GNU ld).  */
extern char *program_invocation_name, *program_invocation_short_name;
#endif /* __USE_GNU */
#endif /* _ERRNO_H */

__END_DECLS

#endif /* errno.h	*/
