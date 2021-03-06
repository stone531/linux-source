/* Copyright (C) 1991, 1996 Free Software Foundation, Inc.
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
#include <stdio.h>

#undef	fsetpos


/* Set the file position of STREAM to *POS.  */
int
fsetpos (stream, pos)
     FILE *stream;
     const fpos_t *pos;
{
  if (pos == NULL)
    {
      __set_errno (EINVAL);
      return EOF;
    }

  return fseek (stream, *pos, SEEK_SET);
}
