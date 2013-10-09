/* Definitions of constants and data structure for POSIX 1003.1b-1993
   scheduling interface.
   Copyright (C) 1996, 1997 Free Software Foundation, Inc.
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

#ifndef	_SCHEDBITS_H

#define	_SCHEDBITS_H	1
#include <features.h>

/* Scheduling algorithms.  */
#define SCHED_OTHER	0
#define SCHED_FIFO	1
#define SCHED_RR	2

/* Data structure to describe a process' schedulability.  */
struct sched_param
  {
    int sched_priority;
  };

/* Cloning flags.  */
#define	CSIGNAL	      0x000000ff  /* Signal mask to be sent at exit.  */
#define	CLONE_VM      0x00000100  /* Set if VM shared between processes.  */
#define	CLONE_FS      0x00000200  /* Set if fs info shared between processes.*/
#define CLONE_FILES   0x00000400  /* Set if open files shared between processes*/
#define CLONE_SIGHAND 0x00000800  /* Set if signal handlers shared.  */
#define CLONE_PID     0x00001000  /* Set if pid shared.  */


/* Clone current process.  */
extern int __clone __P ((int (*__fn) (void *__arg), void *__child_stack,
			 int __flags, void *__arg));
#ifdef __USE_MISC
extern int clone __P ((int (*__fn) (void *__arg), void *__child_stack,
		       int __flags, void *__arg));
#endif


#endif /* schedbits.h */
