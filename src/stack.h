#ifndef METAC_STACK_H
 #define METAC_STACK_H
	
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
	
	
	
	typedef struct stackframe stackframe;
	struct stackframe
	{
		char_pascalarray *stack;
		size_t used;
		
		int lang;
		
		gentyped_parr *ops;
		genname_parr *directives, *linemacros, *omnimacros;
	};
	
	typedef struct stackpair stackpair;
	struct stackpair
	{
		stackframe ret, data;
		int run;
		
	};
	extern stackpair std_stacks;
	
	
	typedef struct retframe retframe;
	typedef retframe (*framefunc)( stackpair*, void* );
	struct retframe
	{
		framefunc handler;
		void *data;
	};
	
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( retframe_, retframe );
	typedef retframe_pascalarray retframe_parr;
	#define BUILD_RETFRAMEPARR( set ) \
		(retframe_pascalarray){ \
			sizeof( (retframe[]){ set } ) / sizeof( retframe ) - 1, \
			(retframe[]){ set } \
		}
	
	
		/* Marks a stack as ready to run. */
	int enable_loop( stackpair* );
		/* Executes a stack context. The retframe provided will be */
		/*  the first function run, and will be altered as execution */
		/*  proceeds to indicate the next function to run. At the end */
		/*  of an execution run, it SHOULD already be nulled, but make */
		/*  sure to check, as this may change in the future. */
	int run_loop( retframe*,  stackpair* );
		/* Marks the stack to end execution of run_loop. This is what */
		/*  sets the run_loop() retframe to null. */
	retframe end_run( stackpair*, void* );
	
		/* Seriously, all it does is return. For when you want */
		/*  a placeholder, but don't want to end execution. */
	retframe noop( stackpair *stkp, void *v );
		/* For when you want a placeholder that SHOULD end execution. */
	retframe vm_placeholder( stackpair *stkp, void *v );
	
	const retframe noop_retframe, vm_placeholder_retframe;
	
	
	
	int init_stack( stackframe *stk );
	int resize_stack( stackframe *stk,  int deltaChars );
			/* *res will be set equal to stk->used. */
			/* Otherwise, (  --  ) */
		int tellmark_stack( stackframe *stk,  size_t *res );
			/* stk->used will be set equal to targ. */
			/* Otherwise, (  --  ) */
		int rewind_stack( stackframe *stk,  size_t targ );
	int clear_stack( stackframe *stk );
	
	
		/* ( -- 8bit-val ) */
	int push_char( stackframe *stk,  char val );
		/* ( 8bit-val -- ) */
	int pop_char( stackframe *stk,  char *dest );
		/* ( 8bit-val -- 8bit-val ) */
	int peek_char( stackframe *stk,  size_t off,  char *dest );
	
	
		/* ( -- int-val ) */
	int push_int( stackframe *stk,  int val );
		/* ( int-val -- ) */
	int pop_int( stackframe *stk,  int *dest );
		/* ( int-val -- int-val ) */
	int peek_int( stackframe *stk,  size_t off,  int *dest );
	
	int push_uintptr( stackframe *stk,  uintptr_t val );
	int pop_uintptr( stackframe *stk,  uintptr_t *dest );
	int peek_uintptr( stackframe *stk,  size_t off,  uintptr_t *dest );
	
	int push_retframe( stackframe *stk,  retframe rf );
	int pop_retframe( stackframe *stk,  retframe *rf );
	int peek_retframe( stackframe *stk,  size_t off,  retframe *rf );
	
	int push_tokenhead( stackframe *stk,  token_head val );
	int pop_tokenhead( stackframe *stk,  token_head *dest );
	int peek_tokenhead( stackframe *stk,  size_t off,  token_head *dest );
	
	
	int push_block( stackframe *stk,  const char *src, size_t len );
	int pop_block( stackframe *stk,  char *dest, size_t len );
	
	int push_token( stackframe *stk,  token_head *src );
	int pop_token( stackframe *stk,  token_head **dest );
	
	
	
	#define stack_ENDRETFRAME( ... ) return( (retframe){ &end_run, (void*)0 } )
	
		/* Old version: STACK_CHECK( stack,  errfunc, errnum,  ... ) */
	#define STACK_CHECK( stack,  stylesetptr, caller, endfunc ) \
		if( !( stack ) ) { \
			STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( stack ) ); \
			( endfunc )(); }
		/* Old version: STACK_CHECK2( stack, v,  errfunc, errnum,  ... ) */
	#define STACK_CHECK2( stack, v,  stylesetptr, caller, endfunc ) \
		if( !( stack ) || !( v ) ) { \
			STDMSG_BADNULL2_WRAPPER( ( stylesetptr ), ( caller ), &( stack ), &( v ) ); \
			( endfunc )(); }
	
	
		/* Old version: STACK_PEEK_UINT( stk, offset, var,  errfunc, err1, err2,  ... ) */
	#define STACK_PEEK_UINT( stk, offset, dest,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = peek_uintptr( ( stk ),  ( offset ),  &( dest ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "peek_uintptr", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( dest ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( dest ) ); \
				( endfunc )(); } }
		/* Old version: STACK_POP_UINT( stk, var,  errfunc, err1, err2,  ... ) */
	#define STACK_POP_UINT( stk, dest,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = pop_uintptr( ( stk ),  &( dest ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "pop_uintptr", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( dest ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( dest ) ); \
				( endfunc )(); } }
		/* Old version: STACK_PUSH_UINT( stk, val,  errfunc, err,  ... ) */
	#define STACK_PUSH_UINT( stk, val,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = push_uintptr( ( stk ),  ( val ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "push_uintptr", ( caller ), ( scratch ) ); \
				( endfunc )(); } }
	
	#define STACK_PEEK_RETFRAME( stk, offset, dest,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = peek_retframe( ( stk ),  ( offset ),  &( dest ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "peek_retframe", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( dest ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( dest ) ); \
				( endfunc )(); } }
	#define STACK_POP_RETFRAME( stk, dest,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = pop_retframe( ( stk ),  &( dest ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "pop_retframe", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( dest ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( dest ) ); \
				( endfunc )(); } }
	#define STACK_PUSH_RETFRAME( stk, val,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = push_retframe( ( stk ),  ( val ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "push_retframe", ( caller ), ( scratch ) ); \
				( endfunc )(); } }
	
	#define STACK_SWAP_UINT( stk, tmp1, tmp2,  stylesetptr, caller, scratch, endfunc ) \
		STACK_POP_UINT( ( stk ), ( tmp1 ),  ( stylesetptr ), ( caller ), ( scratch ), ( endfunc ) ) \
		STACK_POP_UINT( ( stk ), ( tmp2 ),  ( stylesetptr ), ( caller ), ( scratch ), ( endfunc)  ) \
		STACK_PUSH_UINT( ( stk ), ( tmp1 ),  ( stylesetptr ), ( caller ), ( scratch ), ( endfunc ) ) \
		STACK_PUSH_UINT( ( stk ), ( tmp2 ),  ( stylesetptr ), ( caller ), ( scratch ), ( endfunc ) )
	
	
		/* Old version: CALL_FRAMEFUNC( rethand, retval, fhand, fval ) */
	#define CALL_FRAMEFUNC( stkpair, rethand, retval, callhand, callval,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = push_retframe( ( stkpair )->ret, (retframe){ (rethand), (retval) } ); \
			if( ( scratch ) ) { \
				return( (retframe){ (callhand), (callval) } ); } \
			else { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "push_retframe", ( caller ), ( scratch ) ); \
				( endfunc )(); } }
		/* Old version: RET_FRAMEFUNC( refname, errnum, ... ) */
		/* Old version: RET_FRAMEFUNC2( refname, subrefnum, ... ) */
	#define RET_FRAMEFUNC( stkpair,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			retframe RET_FRAMEFUNC_ret; \
			( scratch ) = pop_retframe( ( stkpair )->ret, &RET_FRAMEFUNC_ret ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "pop_retframe", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			return( RET_FRAMEFUNC_ret ); }
	
#endif
