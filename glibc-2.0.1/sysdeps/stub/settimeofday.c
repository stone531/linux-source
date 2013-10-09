/* Copyright (C) 1991, 1995, 1996 Free Software Foundation, Inc.
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

#include <errno.h>
#include <sys/time.h>

/* Set the current time of day and timezone information.
   This call is restricted to the super-user.  */
int
__settimeofday (tv, tz)
     const struct timeval *tv;
     const struct timezone *tz;
{
  __set_errno (ENOSYS);
  return -1;
}
stub_warning (settimeofday)

weak_alias (__settimeofday, settimeofday)