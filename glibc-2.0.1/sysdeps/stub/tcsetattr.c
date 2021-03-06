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
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <errno.h>
#include <stddef.h>
#include <termios.h>

static int bad_speed __P ((speed_t speed));

/* Set the state of FD to *TERMIOS_P.  */
int
tcsetattr (fd, optional_actions, termios_p)
     int fd;
     int optional_actions;
     const struct termios *termios_p;
{
  if (fd < 0)
    {
      __set_errno (EBADF);
      return -1;
    }
  if (termios_p == NULL)
    {
      __set_errno (EINVAL);
      return -1;
    }
  switch (optional_actions)
    {
    case TCSANOW:
    case TCSADRAIN:
    case TCSAFLUSH:
      break;
    default:
      __set_errno (EINVAL);
      return -1;
    }

  if (bad_speed(termios_p->__ospeed) ||
      bad_speed(termios_p->__ispeed == 0 ?
		termios_p->__ospeed : termios_p->__ispeed))
    {
      __set_errno (EINVAL);
      return -1;
    }

  __set_errno (ENOSYS);
  return -1;
}


/* Strychnine checking.  */
static int
bad_speed (speed)
     speed_t speed;
{
  switch (speed)
    {
    case B0:
    case B50:
    case B75:
    case B110:
    case B134:
    case B150:
    case B200:
    case B300:
    case B600:
    case B1200:
    case B1800:
    case B2400:
    case B4800:
    case B9600:
    case B19200:
    case B38400:
      return 0;
    default:
      return 1;
    }
}


stub_warning (tcsetattr)
