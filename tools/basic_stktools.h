/*
MetaC Preprocessor - A macro processor for preprocessing usage.
Copyright (C) 2022 Jared A. Maddox

This program is free software; you can redistribute it and/or modify it
under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the:
	Free Software Foundation, Inc.,
	59 Temple Place, Suite 330,
	Boston, MA 02111-1307 USA
*/
/* tools/basic_stktools.h */

#ifndef METAC_TOOLS_BASIC_STKTOOLS_H
 #define METAC_TOOLS_BASIC_STKTOOLS_H

	/* Copies an array of retframe instances onto the return */
	/*  stack, and immediately begins executing them in order, */
	/*  starting with the one at the start of the array. */
	/* Note that v must be a pointer to a pascal array of */
	/*  retframes: these are the retframes that will be pushed */
	/*  onto the stack, and ->body[ 0 ] will be the first one */
	/*  executed. */
	/* This function was directly inspired by (but not used */
	/*  in) require_preprocopener() from basic_toktools.c, */
	/*  where it would have been useful. Think of it as a */
	/*  variation on direct threaded code. */
retframe enqueue_returns( stackpair *stkp, void *v );

	/* ( uintptr_t*a -- uintptr_t*a uintptr_t*a ) */
retframe dup( stackpair *stkp, void *v );
	/* ( uintptr_t -- ) */
retframe drop( stackpair *stkp, void *v );

	/* ( uintptr_t*a uintptr_t*b -- uintptr_t*b uintptr_t*a ) */
retframe swap2nd( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t uintptr_t*b -- uintptr_t*b uintptr_t uintptr_t*a ) */
retframe swap3rd( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t uintptr_t uintptr_t*b -- uintptr_t*b uintptr_t uintptr_t uintptr_t*a ) */
retframe swap4th( stackpair *stkp, void *v );

	/* ( uintptr_t*a uintptr_t*b uintptr_t*c -- uintptr_t*b uintptr_t*c uintptr_t*a ) */
retframe raise3rd( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t*b uintptr_t*c -- uintptr_t*c uintptr_t*a uintptr_t*b ) */
retframe recede_to3rd( stackpair *stkp, void *v );

	/* ( uintptr_t retframe -- retframe uintptr_t ) */
retframe swap2nd_to_retframe( stackpair *stkp, void *v );
	/* ( retframe uintptr_t -- uintptr_t retframe ) */
retframe swap2nd_from_retframe( stackpair *stkp, void *v );
	/* ( uintptr_t*1 uintptr_t*2 retframe -- retframe uintptr_t*1 uintptr_t*2 ) */
retframe swap3rd_retframe( stackpair *stkp, void *v );

/* Note: using ANY of these with enqueue_returns() is VERY dicey. */
	/* ( uintptr_t -- ret: uintptr_t ) */
retframe swap2ret( stackpair *stkp, void *v );
	/* ( ret: uintptr_t -- uintptr_t ) */
retframe swap2data( stackpair *stkp, void *v );
	/* Validate that these two move retframes in ways compatible to the standard */
	/*  retframe macro, AND vm_pushretframe(). */
	/* ( retframe -- ret: retframe ) */
retframe swap_retframe2ret( stackpair *stkp, void *v );
	/* ( ret: retframe -- retframe ) */
retframe swap_retframe2data( stackpair *stkp, void *v );

retframe swapdata2shuffle( stackpair *stkp, void *v );
retframe swapshuffle2data( stackpair *stkp, void *v );


	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe and2( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe and3( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe and4( stackpair *stkp, void *v );

	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe ior2( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe ior3( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe ior4( stackpair *stkp, void *v );

	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe xor2( stackpair *stkp, void *v );

	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe vm_lesser( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe vm_equal( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe vm_greater( stackpair *stkp, void *v );

	/* ( uintptr_t -- uintptr_t ) */
retframe vm_not( stackpair *stkp, void *v );


retframe vm_push0( stackpair *stkp, void *v );
retframe vm_push1( stackpair *stkp, void *v );
retframe vm_push2( stackpair *stkp, void *v );
retframe vm_pushmax( stackpair *stkp, void *v );

	/* These require v_ to point to a uintptr_t, which will be used as the data */
	/*  in question. */
retframe vm_pushdata( stackpair *stkp, void *v_ );
retframe vm_popdata( stackpair *stkp, void *v_ );
	/* retframe*, not uintptr_t pointer. */
retframe vm_pushretframe( stackpair *stkp, void *v_ );

retframe vm_push_noop( stackpair *stkp, void *v_ );
retframe vm_push_placeholder( stackpair *stkp, void *v_ );


	/* These all require a pointer to a retframe as v. */
retframe just_run( stackpair *stkp, void *v );
	/* ( uintptr_t -- uintptr_t ) */
retframe run_if( stackpair *stkp, void *v );
retframe run_else( stackpair *stkp, void *v );
	/* As with run_if() and run_else, but these run on exact */
	/*  values only. */
retframe run_on0( stackpair *stkp, void *v );
retframe run_on1( stackpair *stkp, void *v );
retframe run_on2( stackpair *stkp, void *v );
retframe run_on3( stackpair *stkp, void *v );
	/* Pops a retframe from the data stack, and returns it. */
retframe vm_datacall( stackpair *stkp, void *v );
	/* Runs enqueue_returns() with v_ upon condition match. */
	/* ( uintptr_t -- uintptr_t ) */
retframe enqueue_if( stackpair *stkp, void *v_ );
	/* ( uintptr_t -- uintptr_t ) */
retframe enqueue_else( stackpair *stkp, void *v_ );
	/* As with run_on*(), but for retframe_parr{} pointers. */
retframe enqueue_on0( stackpair *stkp, void *v_ );
retframe enqueue_on1( stackpair *stkp, void *v_ );
retframe enqueue_on2( stackpair *stkp, void *v_ );
retframe enqueue_on3( stackpair *stkp, void *v_ );
	/* Runs vm_pushretframe() with v_ upon condition match. */
	/* ( uintptr_t -- uintptr_t ) */
retframe vm_pushretframe_if( stackpair *stkp, void *v_ );
	/* ( uintptr_t -- uintptr_t ) */
retframe vm_pushretframe_else( stackpair *stkp, void *v_ );



	/* ( char_parr* --  ) */
retframe dealloc_cparr( stackpair *stkp, void *v_ );


	/* The longjump() and divertthread() stuff, it used to be here. */
#include "longjump.h"

#endif
/* End tools/basic_stktools.h */
