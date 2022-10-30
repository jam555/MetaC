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

#include "headers.h"

#include "err/inner_err.h"



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		static msg_styleset errs;
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs;
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif



/* This file just contains some dummy code that shows how to use some stuff. */

retframe demo_func( stack_pair *stkp, void *v_ )
{
	uintptr_t a;
	
		/* Note, one or the other: *2() is assumed for "v" to be defined. */
		/*  The code for these is essentially identical each time, so you */
		/*  could honestly just have a wrapper macro that takes the function */
		/*  name only if you keep the variable names the same across all */
		/*  functions. */
	STACK_CHECK( stkp,  &errs, demo_func, stack_ENDRETFRAME );
	STACK_CHECK2( stkp, v_,  &errs, demo_func, stack_ENDRETFRAME );
		/* This is done to make it easier to use whatever v_ points at, so */
		/*  only relevant for some functions. */
	function_data *v = (function_data*)v_;
	
	int scratch_space, offset = 0;
		/* You would obviously only use ONE of these like this, or at least */
		/*  wouldn't have that pop with that peek without a test of "a", and */
		/*  wouldn't have the push without something inbetween to change the */
		/*  value of "a" somehow. */
	STACK_PEEK_UINT( &( stkp->data ), offset, a,  &errs, demo_func, scratch_space, stack_ENDRETFRAME );
	STACK_POP_UINT( &( stkp->data ), a,  &errs, demo_func, scratch_space, stack_ENDRETFRAME );
	STACK_PUSH_UINT( &( stkp->data ), a,  &errs, demo_func, scratch_space, stack_ENDRETFRAME );
	
		/* This is how you do a tailcall! */
	return( (retframe){ &next_function, (void*)0 } );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
