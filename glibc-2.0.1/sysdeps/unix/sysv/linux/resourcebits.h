/* Bit values & structures for resource limits.  Linux version.
   Copyright (C) 1994, 1996 Free Software Foundation, Inc.
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

#include <asm/resource.h>

/* Transmute defines to enumerations.  The macro re-definitions are
   necessary because some programs want to test for operating system
   features with #ifdef RUSAGE_SELF.  In ISO C the reflexive
   definition is a no-op.  */

/* Kinds of resource limit.  */
enum __rlimit_resource
{
  /* Per-process CPU limit, in seconds.  */
  _RLIMIT_CPU = RLIMIT_CPU,
#undef RLIMIT_CPU
  RLIMIT_CPU = _RLIMIT_CPU,
#define RLIMIT_CPU RLIMIT_CPU

  /* Largest file that can be created, in bytes.  */
  _RLIMIT_FSIZE = RLIMIT_FSIZE,
#undef RLIMIT_FSIZE
  RLIMIT_FSIZE = _RLIMIT_FSIZE,
#define	RLIMIT_FSIZE RLIMIT_FSIZE

  /* Maximum size of data segment, in bytes.  */
  _RLIMIT_DATA = RLIMIT_DATA,
#undef RLIMIT_DATA
  RLIMIT_DATA = _RLIMIT_DATA,
#define	RLIMIT_DATA RLIMIT_DATA

  /* Maximum size of stack segment, in bytes.  */
  _RLIMIT_STACK = RLIMIT_STACK,
#undef RLIMIT_STACK
  RLIMIT_STACK = _RLIMIT_STACK,
#define	RLIMIT_STACK RLIMIT_STACK

  /* Largest core file that can be created, in bytes.  */
  _RLIMIT_CORE = RLIMIT_CORE,
#undef RLIMIT_CORE
  RLIMIT_CORE = _RLIMIT_CORE,
#define	RLIMIT_CORE RLIMIT_CORE

  /* Largest resident set size, in bytes.
     This affects swapping; processes that are exceeding their
     resident set size will be more likely to have physical memory
     taken from them.  */
  _RLIMIT_RSS = RLIMIT_RSS,
#undef RLIMIT_RSS
  RLIMIT_RSS = _RLIMIT_RSS,
#define	RLIMIT_RSS RLIMIT_RSS

  /* Number of open files.  */
  _RLIMIT_NOFILE = RLIMIT_NOFILE,
#undef RLIMIT_NOFILE
  RLIMIT_NOFILE = _RLIMIT_NOFILE,
  RLIMIT_OFILE = RLIMIT_NOFILE, /* BSD name for same.  */
#define RLIMIT_NOFILE RLIMIT_NOFILE
#define RLIMIT_OFILE RLIMIT_OFILE

  /* Address space limit (?) */
  _RLIMIT_AS = RLIMIT_AS,
#undef RLIMIT_AS
  RLIMIT_AS = _RLIMIT_AS,
#define RLIMIT_AS RLIMIT_AS

  /* Number of processes.  */
  _RLIMIT_NPROC = RLIMIT_NPROC,
#undef RLIMIT_NPROC
  RLIMIT_NPROC = _RLIMIT_NPROC,
#define RLIMIT_NPROC RLIMIT_NPROC

  /* Locked-in-memory address space.  */
  _RLIMIT_MEMLOCK = RLIMIT_MEMLOCK,
#undef RLIMIT_MEMLOCK
  RLIMIT_MEMLOCK = _RLIMIT_MEMLOCK,
#define RLIMIT_MEMLOCK RLIMIT_MEMLOCK

  RLIMIT_NLIMITS = RLIM_NLIMITS,
#undef RLIM_NLIMITS
  RLIM_NLIMITS = RLIMIT_NLIMITS,
#define RLIMIT_NLIMITS RLIMIT_NLIMITS
#define RLIM_NLIMITS RLIM_NLIMITS

  /* Value to indicate that there is no limit.  */
  RLIM_INFINITY = (long int)(~0UL >> 1)
#define RLIM_INFINITY RLIM_INFINITY
};

struct rlimit
{
  /* The current (soft) limit.  */
  long int rlim_cur;
  /* The hard limit.  */
  long int rlim_max;
};

/* Whose usage statistics do you want?  */
enum __rusage_who
{
  /* The calling process.  */
  RUSAGE_SELF = 0,
#define RUSAGE_SELF RUSAGE_SELF

  /* All of its terminated child processes.  */
  RUSAGE_CHILDREN = -1,
#define RUSAGE_CHILDREN RUSAGE_CHILDREN

  /* Both.  */
  RUSAGE_BOTH = -2
#define RUSAGE_BOTH RUSAGE_BOTH
};

#include <sys/time.h>		/* For `struct timeval'.  */

/* Structure which says how much of each resource has been used.  */
struct rusage
{
  /* Total amount of user time used.  */
  struct timeval ru_utime;
  /* Total amount of system time used.  */
  struct timeval ru_stime;
  /* Maximum resident set size (in kilobytes).  */
  long int ru_maxrss;
  /* Amount of sharing of text segment memory
     with other processes (kilobyte-seconds).  */
  long int ru_ixrss;
  /* Amount of data segment memory used (kilobyte-seconds).  */
  long int ru_idrss;
  /* Amount of stack memory used (kilobyte-seconds).  */
  long int ru_isrss;
  /* Number of soft page faults (i.e. those serviced by reclaiming
     a page from the list of pages awaiting reallocation.  */
  long int ru_minflt;
  /* Number of hard page faults (i.e. those that required I/O).  */
  long int ru_majflt;
  /* Number of times a process was swapped out of physical memory.  */
  long int ru_nswap;
  /* Number of input operations via the file system.  Note: This
     and `ru_oublock' do not include operations with the cache.  */
  long int ru_inblock;
  /* Number of output operations via the file system.  */
  long int ru_oublock;
  /* Number of IPC messages sent.  */
  long int ru_msgsnd;
  /* Number of IPC messages received.  */
  long int ru_msgrcv;
  /* Number of signals delivered.  */
  long int ru_nsignals;
  /* Number of voluntary context switches, i.e. because the process
     gave up the process before it had to (usually to wait for some
     resource to be available).  */
  long int ru_nvcsw;
  /* Number of involuntary context switches, i.e. a higher priority process
     became runnable or the current process used up its time slice.  */
  long int ru_nivcsw;
};

/* Priority limits.  */
#define PRIO_MIN	-20	/* Minimum priority a process can have.  */
#define PRIO_MAX	20	/* Maximum priority a process can have.  */

/* The type of the WHICH argument to `getpriority' and `setpriority',
   indicating what flavor of entity the WHO argument specifies.  */
enum __priority_which
{
  PRIO_PROCESS = 0,		/* WHO is a process ID.  */
  PRIO_PGRP = 1,		/* WHO is a process group ID.  */
  PRIO_USER = 2			/* WHO is a user ID.  */
};
