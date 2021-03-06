# Copyright (C) 1994, 1995, 1996, 1997 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public License as
# published by the Free Software Foundation; either version 2 of the
# License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.

# You should have received a copy of the GNU Library General Public
# License along with the GNU C Library; see the file COPYING.LIB.  If not,
# write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.

#
#	Sub-makefile for resolv portion of the library.
#
subdir	:= resolv

headers	:= resolv.h netdb.h arpa/nameser.h sys/bitypes.h
distribute := ../conf/portability.h mapv4v6addr.h mapv4v6hostent.h \
	      Banner

routines := herror inet_addr inet_ntop inet_pton nsap_addr res_init

extra-libs := libresolv libnss_dns
extra-libs-others = $(extra-libs)
libresolv-routines := gethnamaddr res_comp res_debug	\
		      res_data res_mkquery res_query res_send		\
		      inet_net_ntop inet_net_pton inet_neta base64

subdir-dirs = nss_dns
vpath %.c nss_dns

libnss_dns-routines	:= dns-host dns-network
libnss_dns-inhibit-o	= $(filter-out .so,$(object-suffixes))

include ../Rules

CPPFLAGS += -Dgethostbyname=res_gethostbyname \
	    -Dgethostbyname2=res_gethostbyname2 \
	    -Dgethostbyaddr=res_gethostbyaddr \
	    -Dgetnetbyname=res_getnetbyname \
	    -Dgetnetbyaddr=res_getnetbyaddr

# The BIND code elicits some harmless warnings.
+cflags += -Wno-strict-prototypes -Wno-comment -Wno-write-strings

# Depend on libc.so so a DT_NEEDED is generated in the shared objects.
# This ensures they will load libc.so for needed symbols if loaded by
# a statically-linked program that hasn't already loaded it.
$(objpfx)libresolv.so: $(common-objpfx)libc.so

# The DNS NSS modules needs the resolver.
$(objpfx)libnss_dns.so: $(objpfx)libresolv.so
