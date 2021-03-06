/*
 * Written by J.T. Conklin <jtc@netbsd.org>.
 * Changes for long double by Ulrich Drepper <drepper@cygnus.com>
 * Public domain.
 */

#include <machine/asm.h>

RCSID("$NetBSD: $")

ENTRY(__scalbnl)
	fildl	16(%esp)
	fldt	4(%esp)
	fscale
	ret
END (__scalbnl)
weak_alias (__scalbnl, scalbnl)
