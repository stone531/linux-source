/* Copyright (C) 1993, 1996 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Brendan Kehoe (brendan@zen.org).

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

#ifndef	_STATBUF_H
#define	_STATBUF_H	1

#include <gnu/types.h>

/* Versions of the `struct stat' data structure and
   the bits of the `xmknod' interface.  */
#define _STAT_VER	2
#define _MKNOD_VER	2

/* Structure describing file characteristics.  */
struct stat
  {
    unsigned long st_dev;	/* Device.  */
    long st_filler1[3];
    unsigned long st_ino;		/* File serial number.		*/
    unsigned long st_mode;	/* File mode.  */
    unsigned long st_nlink;	/* Link count.  */
    long st_uid;		/* User ID of the file's owner.	*/
    long st_gid;		/* Group ID of the file's group.*/
    unsigned long st_rdev;	/* Device number, if device.  */
    long st_filler2[2];

    long st_size;		/* Size of file, in bytes.  */
    /* SVR4 added this extra long to allow for expansion of off_t.  */
    long st_filler3;

    long st_atime;		/* Time of last access.  */
    unsigned long st_atime_usec;
    long st_mtime;		/* Time of last modification.  */
    unsigned long st_mtime_usec;
    long st_ctime;		/* Time of last status change.  */
    unsigned long st_ctime_usec;

    long st_blksize;		/* Optimal block size for I/O.  */
#define	_STATBUF_ST_BLKSIZE	/* Tell code we have this member.  */

    long st_blocks;		/* Number of 512-byte blocks allocated.  */
    char st_fstype[16];		/* The type of this filesystem.  */
    int st_aclcnt;
    unsigned long st_level;
    unsigned long st_flags;
    unsigned long st_cmwlevel;
    long st_filler4[4];
  };

/* Encoding of the file mode.  */

#define	__S_IFMT	0170000	/* These bits determine file type.  */

/* File types.  */
#define	__S_IFDIR	0040000	/* Directory.  */
#define	__S_IFCHR	0020000	/* Character device.  */
#define	__S_IFBLK	0060000	/* Block device.  */
#define	__S_IFREG	0100000	/* Regular file.  */
#define	__S_IFIFO	0010000	/* FIFO.  */

/* These don't actually exist on System V, but having them doesn't hurt.  */
#define	__S_IFLNK	0120000	/* Symbolic link.  */
#define	__S_IFSOCK	0140000	/* Socket.  */

/* Protection bits.  */

#define	__S_ISUID	04000	/* Set user ID on execution.  */
#define	__S_ISGID	02000	/* Set group ID on execution.  */
#define	__S_ISVTX	01000	/* Save swapped text after use (sticky).  */
#define	__S_IREAD	0400	/* Read by owner.  */
#define	__S_IWRITE	0200	/* Write by owner.  */
#define	__S_IEXEC	0100	/* Execute by owner.  */

#endif	/* statbuf.h */
