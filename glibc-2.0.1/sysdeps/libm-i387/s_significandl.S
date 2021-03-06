/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Changes for long double by Ulrich Drepper <drepper@cygnus.com>
 * Public domain.
 */

#include <machine/asm.h>

RCSID("$NetBSD: $")

ENTRY(__significandl)
	fldt	4(%esp)
	fxtract
	fstp	%st(0)
	ret
END (__significandl)
weak_alias (__significandl, significandl)
