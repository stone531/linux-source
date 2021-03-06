/* Copyright (C) 1992, 1994, 1995, 1996 Free Software Foundation, Inc.
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
#include <hurd.h>
#include <hurd/fd.h>
#include <sys/socket.h>
#include <hurd/socket.h>
#include <sys/un.h>
#include <hurd/ifsock.h>

/* Open a connection on socket FD to peer at ADDR (which LEN bytes long).
   For connectionless socket types, just set the default address to send to
   and the only address from which to accept transmissions.
   Return 0 on success, -1 for errors.  */
int
DEFUN(__connect, (fd, addr, len),
      int fd AND const struct sockaddr_un *addr AND size_t len)
{
  error_t err;
  addr_port_t aport;

  if (addr->sun_family == AF_LOCAL)
    {
      /* For the local domain, we must look up the name as a file and talk
	 to it with the ifsock protocol.  */
      file_t file = __file_name_lookup (addr->sun_path, 0, 0);
      if (file == MACH_PORT_NULL)
	return -1;
      err = __ifsock_getsockaddr (file, &aport);
      __mach_port_deallocate (__mach_task_self (), file);
      if (err == MIG_BAD_ID || err == EOPNOTSUPP)
	/* The file did not grok the ifsock protocol.  */
	err = ENOTSOCK;
      if (err)
	return __hurd_fail (err);
    }
  else
    err = EIEIO;

  err = HURD_DPORT_USE (fd,
			({
			  if (err)
			    err = __socket_create_address (port,
							   addr->sun_family,
							   (char *) addr, len,
							   &aport);
			  if (! err)
			    {
			      err = __socket_connect (port, aport);
			      __mach_port_deallocate (__mach_task_self (),
						      aport);
			    }
			  err;
			}));

  return err ? __hurd_dfail (fd, err) : 0;
}
weak_alias (__connect, connect)
