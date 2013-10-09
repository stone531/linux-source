/* Copyright (C) 1993, 1994, 1995, 1996 Free Software Foundation, Inc.
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

#include <ansidecl.h>
#include <errno.h>
#include <stddef.h>
#include <dirent.h>
#include <unistd.h>
#include "dirstream.h"

/* Seek to position POS in DIRP.  */
void
DEFUN(seekdir, (dirp, pos), DIR *dirp AND __off_t pos)
{
  __libc_lock_lock (dirp->__lock);
  /* Change our entry index pointer to POS and discard any data already
     read.  The next `readdir' call will notice the empty block and read
     anew from the location in DIRP->__entry_ptr and reset the other state
     variables.  */
  dirp->__entry_ptr = pos;
  dirp->__size = 0;
  __libc_lock_unlock (dirp->__lock);
}
