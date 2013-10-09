/* Copyright (C) 1996 Free Software Foundation, Inc.
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ether.h>
#include <netinet/if_ether.h>


int
ether_line (const char *line, struct ether_addr *addr, char *hostname)
{
  size_t cnt;
  char *cp;

  for (cnt = 0; cnt < 6; ++cnt)
    {
      unsigned int number;
      char ch;

      ch = tolower (*line++);
      if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
	return -1;
      number = isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);

      ch = tolower (*line);
      if ((cnt < 5 && ch != ':') || (cnt == 5 && ch != '\0' && !isspace (ch)))
	{
	  ++line;
	  if ((ch < '0' || ch > '9') && (ch < 'a' || ch > 'f'))
	    return -1;
	  number <<= 4;
	  number = isdigit (ch) ? (ch - '0') : (ch - 'a' + 10);

	  ch = *line;
	  if (cnt < 5 && ch != ':')
	    return -1;
	}

      /* Store result.  */
      addr->ether_addr_octet[cnt] = (unsigned char) number;

      /* Skip ':'.  */
      if (ch != '\0')
	++line;
    }

  /* Remove trailing white space.  */
  cp = strchr (line, '#');
  if (cp == NULL)
    cp = strchr (line, '\0');
  while (cp > line && isspace (cp[-1]))
    --cp;
  *cp = '\0';

  if (*line == '\0')
    /* No hostname.  */
    return -1;

  /* XXX This can cause trouble because the hostname might be too long
     but we have no possibility to check it here.  */
  strcpy (hostname, line);

  return 0;
}
