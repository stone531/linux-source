/* Perform a `longjmp' on a Mach thread_state.  Alpha version.
Copyright (C) 1991, 1994 Free Software Foundation, Inc.
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

#include <hurd/signal.h>
#include <setjmp.h>
#include <mach/thread_status.h>


/* Set up STATE to do the equivalent of `longjmp (ENV, VAL);'.  */

void
_hurd_longjmp_thread_state (void *state, jmp_buf env, int val)
{
  struct alpha_thread_state *ts = state;

  ts->r9 = env[0].__jmpbuf[0].__9;
  ts->r11 = env[0].__jmpbuf[0].__11;
  ts->r12 = env[0].__jmpbuf[0].__12;
  ts->r13 = env[0].__jmpbuf[0].__13;
  ts->r14 = env[0].__jmpbuf[0].__14;
  ts->r15 = (long int) env[0].__jmpbuf[0].__fp;
  ts->r30 = (long int) env[0].__jmpbuf[0].__sp;
  ts->pc = (long int) env[0].__jmpbuf[0].__pc;
  ts->r0 = val ?: 1;
}
