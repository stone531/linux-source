/* brk system call for Linux/SPARC.
   Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Miguel de Icaza (miguel@nuclecu.unam.mx)

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

#include <errno.h>
#include <unistd.h>
#include <sysdep.h>

/* This must be initialized data because commons can't have aliases.  */
void *__curbrk = 0;

/* Old braindamage in GCC's crtstuff.c requires this symbol in an attempt
   to work around different old braindamage in the old Linux ELF dynamic
   linker.  */
weak_alias (__curbrk, ___brk_addr)

int
__brk (void *addr)
{
  void *newbrk, *scratch;

  asm ("mov %1, %%g1\n\t"
       "mov %2, %%o0\n\t"
       "t 0x10\n\t"
       "mov %%o0, %0\n\t"
       : "=r" (newbrk)
       : "0" (SYS_brk), "r" (addr)
       : "g1", "o0");

  __curbrk = newbrk;

  if (newbrk < addr)
    {
      __set_errno (ENOMEM);
      return -1;
    }

  return 0;
}
weak_alias (__brk, brk)
