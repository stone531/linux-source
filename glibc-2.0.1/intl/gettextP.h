/* gettextP.h -- header describing internals of gettext library
Copyright (C) 1995, 1996 Free Software Foundation, Inc.
Written by Ulrich Drepper <drepper@gnu.ai.mit.edu>, 1995.

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
not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#ifndef _GETTEXTP_H
#define _GETTEXTP_H

#include "loadinfo.h"

/* @@ end of prolog @@ */

#ifndef PARAMS
# if __STDC__
#  define PARAMS(args) args
# else
#  define PARAMS(args) ()
# endif
#endif

#ifndef W
# define W(flag, data) ((flag) ? SWAP (data) : (data))
#endif


static nls_uint32 SWAP PARAMS ((nls_uint32 i));

static inline nls_uint32
SWAP (i)
     nls_uint32 i;
{
  return (i << 24) | ((i & 0xff00) << 8) | ((i >> 8) & 0xff00) | (i >> 24);
}


struct loaded_domain
{
  const char *data;
  int must_swap;
  nls_uint32 nstrings;
  struct string_desc *orig_tab;
  struct string_desc *trans_tab;
  nls_uint32 hash_size;
  nls_uint32 *hash_tab;
};

struct binding
{
  struct binding *next;
  char *domainname;
  char *dirname;
};

struct loaded_l10nfile *_nl_find_domain PARAMS ((const char *__dirname,
						 char *__locale,
						 const char *__domainname));
void _nl_load_domain PARAMS ((struct loaded_l10nfile *__domain));

/* @@ begin of epilog @@ */

#endif /* gettextP.h  */
