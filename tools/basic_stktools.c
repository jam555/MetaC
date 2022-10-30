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
#include "stack.h"

#include "basic_stktools.h"



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



#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
#define DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )



#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), lexalike_ENDRETFRAME )
#define STACKCHECK2( stack, v,  caller ) \
	STACK_CHECK2( ( stack ), ( v ),  &errs, ( caller ), lexalike_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )

#define RETFRAMEFUNC( stkpair,  caller ) \
	RET_FRAMEFUNC( ( stkpair ),  &errs, ( caller ), res, stack_ENDRETFRAME )



retframe enqueue_returns( stackpair *stkp, void *v )
{
	retframe_parr *rets;
	uintptr_t a, b;
	int scratch;
	size_t iter;
	
	STACKCHECK( stkp,  enqueue_returns );
	if( !v )
	{
		???
	}
	
	rets = (retframe_parr*)v;
	iter = rets->len;
	while( iter )
	{
		--iter;
		
		scratch = push_retframe( stkp->ret, rets->body[ iter ] );
		if( !scratch )
		{
			STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", enqueue_returns, scratch );
			( endfunc )();
		}
	}
	
	RETFRAMEFUNC( stkp,  enqueue_returns )
}



retframe dup( stackpair *stkp, void *v )
{
	uintptr_t a;
	int scratch;
	
	STACKCHECK( stkp,  dup );
	
	
	STACKPEEK_UINT( stkp->data, 0, a,  dup, scratch );
	STACKPUSH_UINT( stkp->data, a,  dup, scratch );
	
	
	RETFRAMEFUNC( stkp,  dup );
}
retframe drop( stackpair *stkp, void *v )
{
	uintptr_t a;
	int scratch;
	
	STACKCHECK( stkp,  drop );
	
	
	STACKPOP_UINT( stkp->data, a,  drop, scratch );
	
	
	RETFRAMEFUNC( stkp,  drop );
}



retframe swap2nd( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  swap2nd );
	
	
	STACKPOP_UINT( stkp->data, a,  swap2nd, scratch );
	STACKPOP_UINT( stkp->data, b,  swap2nd, scratch );
	
	STACKPUSH_UINT( stkp->data, a,  swap2nd, scratch );
	STACKPUSH_UINT( stkp->data, b,  swap2nd, scratch );
	
	
	RETFRAMEFUNC( stkp,  swap2nd );
}
retframe swap3rd( stackpair *stkp, void *v )
{
	uintptr_t a, b, c;
	int scratch;
	
	STACKCHECK( stkp,  swap3rd );
	
	
	STACKPOP_UINT( stkp->data, a,  swap3rd, scratch );
	STACKPOP_UINT( stkp->data, b,  swap3rd, scratch );
	STACKPOP_UINT( stkp->data, c,  swap3rd, scratch );
	
	STACKPUSH_UINT( stkp->data, a,  swap3rd, scratch );
	STACKPUSH_UINT( stkp->data, b,  swap3rd, scratch );
	STACKPUSH_UINT( stkp->data, c,  swap3rd, scratch );
	
	
	RETFRAMEFUNC( stkp,  swap3rd );
}
retframe swap4th( stackpair *stkp, void *v )
{
	uintptr_t a, b, c, d;
	int scratch;
	
	STACKCHECK( stkp,  swap4th );
	
	
	STACKPOP_UINT( stkp->data, a,  swap4th, scratch );
	STACKPOP_UINT( stkp->data, b,  swap4th, scratch );
	STACKPOP_UINT( stkp->data, c,  swap4th, scratch );
	STACKPOP_UINT( stkp->data, d,  swap4th, scratch );
	
	STACKPUSH_UINT( stkp->data, a,  swap4th, scratch );
		/* Yes, intended: these two don't need to get swapped. */
	STACKPUSH_UINT( stkp->data, c,  swap4th, scratch );
	STACKPUSH_UINT( stkp->data, b,  swap4th, scratch );
	STACKPUSH_UINT( stkp->data, d,  swap4th, scratch );
	
	
	RETFRAMEFUNC( stkp,  swap4th );
}



retframe and2( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  and2 );
	
	
	STACKPOP_UINT( stkp->data, a,  and2, scratch );
	STACKPOP_UINT( stkp->data, b,  and2, scratch );
	
	STACKPUSH_UINT( stkp->data, a & b,  and2, scratch );
	
	RETFRAMEFUNC( stkp,  and2 );
}
retframe and3( stackpair *stkp, void *v )
{
	uintptr_t a, b, c;
	int scratch;
	
	STACKCHECK( stkp,  and3 );
	
	
	STACKPOP_UINT( stkp->data, a,  and3, scratch );
	STACKPOP_UINT( stkp->data, b,  and3, scratch );
	STACKPOP_UINT( stkp->data, c,  and3, scratch );
	
	STACKPUSH_UINT( stkp->data, a & b & c,  and3, scratch );
	
	RETFRAMEFUNC( stkp,  and3 );
}
retframe and4( stackpair *stkp, void *v )
{
	uintptr_t a, b, c, d;
	int scratch;
	
	STACKCHECK( stkp,  and4 );
	
	
	STACKPOP_UINT( stkp->data, a,  and4, scratch );
	STACKPOP_UINT( stkp->data, b,  and4, scratch );
	STACKPOP_UINT( stkp->data, c,  and4, scratch );
	STACKPOP_UINT( stkp->data, d,  and4, scratch );
	
	STACKPUSH_UINT( stkp->data, a & b & c & d,  and4, scratch );
	
	RETFRAMEFUNC( stkp,  and4 );
}

retframe ior2( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  ior2 );
	
	
	STACKPOP_UINT( stkp->data, a,  ior2, scratch );
	STACKPOP_UINT( stkp->data, b,  ior2, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b,  ior2, scratch );
	
	RETFRAMEFUNC( stkp,  ior2 );
}
retframe ior3( stackpair *stkp, void *v )
{
	uintptr_t a, b, c;
	int scratch;
	
	STACKCHECK( stkp,  ior3 );
	
	
	STACKPOP_UINT( stkp->data, a,  ior3, scratch );
	STACKPOP_UINT( stkp->data, b,  ior3, scratch );
	STACKPOP_UINT( stkp->data, c,  ior3, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b | c,  ior3, scratch );
	
	RETFRAMEFUNC( stkp,  ior3 );
}
retframe ior4( stackpair *stkp, void *v )
{
	uintptr_t a, b, c, d;
	int scratch;
	
	STACKCHECK( stkp,  ior4 );
	
	
	STACKPOP_UINT( stkp->data, a,  ior4, scratch );
	STACKPOP_UINT( stkp->data, b,  ior4, scratch );
	STACKPOP_UINT( stkp->data, c,  ior4, scratch );
	STACKPOP_UINT( stkp->data, d,  ior4, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b | c | d,  ior4, scratch );
	
	RETFRAMEFUNC( stkp,  ior4 );
}

retframe xor2( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  xor2 );
	
	
	STACKPOP_UINT( stkp->data, a,  xor2, scratch );
	STACKPOP_UINT( stkp->data, b,  xor2, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b,  xor2, scratch );
	
	RETFRAMEFUNC( stkp,  xor2 );
}


retframe vm_push0( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  vm_push0 );
	
	STACKPUSH_UINT( stkp->data, 0,  vm_push0, scratch );
	
	RETFRAMEFUNC( stkp,  vm_push0 );
}
retframe vm_push1( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  vm_push1 );
	
	STACKPUSH_UINT( stkp->data, 1,  vm_push1, scratch );
	
	RETFRAMEFUNC( stkp,  vm_push1 );
}
retframe vm_push2( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  vm_push2 );
	
	STACKPUSH_UINT( stkp->data, 2,  vm_push2, scratch );
	
	RETFRAMEFUNC( stkp,  vm_push2 );
}
retframe vm_pushmax( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  vm_pushmax );
	
	STACKPUSH_UINT( stkp->data, UINTPTR_MAX,  vm_pushmax, scratch );
	
	RETFRAMEFUNC( stkp,  vm_pushmax );
}

retframe vm_pushdata( stackpair *stkp, void *v_ )
{
	int scratch;
	
	STACKCHECK2( stkp, v_,  vm_pushdata );
	
	STACKPUSH_UINT( stkp->data, *( (uintptr_t*)v_ ),  vm_pushdata, scratch );
	
	RETFRAMEFUNC( stkp,  vm_pushdata );
}
retframe vm_popdata( stackpair *stkp, void *v_ )
{
	uintptr_t val;
	int scratch;
	
	STACKCHECK2( stkp, v_,  vm_popdata );
	
	STACKPOP_UINT( stkp->data, val,  vm_popdata, scratch );
	*( (uintptr_t*)v_ ) = val;
	
	RETFRAMEFUNC( stkp,  vm_popdata );
}
retframe vm_pushretframe( stackpair *stkp, void *v_ )
{
	int scratch;
	
	STACKCHECK2( stkp, v_,  vm_pushretframe );
	
	scratch = push_retframe( stkp->data,  (retframe*)v_ );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &errs, "push_retframe", vm_pushretframe, scratch );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RETFRAMEFUNC( stkp,  vm_pushretframe );
}


retframe just_run( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  just_run );
	
	return( *( (retframe*)v ) );
}
	/* ( uintptr_t -- uintptr_t ) */
	/* Requires a pointer to a retframe as v. */
retframe run_if( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  run_if );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_if, scratch );
	
	if( a )
	{
		return( *( (retframe*)v ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_if );
	}
}
retframe run_else( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  run_else );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_else, scratch );
	
	if( !a )
	{
		return( *( (retframe*)v ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_else );
	}
}
retframe vm_datacall( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  vm_datacall );
	
	int scratch;
	retframe rf;
	
	scratch = pop_retframe( stkp->data,  &rf );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &errs, "pop_retframe", vm_datacall, scratch );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( rf );
}


retframe setjump_callstack( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v,  bookmark_stack );
	
	uintptr_t *bookmark = (uintptr_t*)v_;
	size_t tmp;
	int scratch;
	
	scratch = tellmark_stack( &( stkp->ret ),  &tmp );
	if( !scratch )
	{
			/* Error! */
		???;
	}
	
	*bookmark = tmp;
	STACKPUSH_UINT( stkp->data, 0,  setjump_callstack, scratch );
	
	
	retframe ret;
		/* We DO NOT want to directly pop the next retframe, */
		/*  since longjump_*() will want it. */
	scratch = peek_retframe( stkp->ret,  0,  &ret );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &errs, "pop_retframe", setjump_callstack, scratch );
		stack_ENDRETFRAME();
	}
	return( ret );
}
retframe longjump_callstack( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v,  longjump_callstack );
	
	uintptr_t *bookmark = (uintptr_t*)v_;
	int scratch;
	
	scratch = rewind_stack( &( stkp->ret ),  *bookmark );
	if( !scratch )
	{
			/* Error! */
		???;
	}
	
	STACKPUSH_UINT( stkp->data, 1,  longjump_callstack, scratch );
	RETFRAMEFUNC( stkp,  longjump_callstack );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
