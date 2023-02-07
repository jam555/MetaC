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
#include "lexalike.h"

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

	/* ( uintptr_t*a uintptr_t*b uintptr_t*c -- uintptr_t*b uintptr_t*c uintptr_t*a ) */
retframe raise3rd( stackpair *stkp, void *v )
{
	static retframe_parr
		seq =
			{
				2, /* Number of retframes  */
				{
						/* ( uintptr_t*b uintptr_t*c -- uintptr_t*c uintptr_t*b ) */
					(retframe){ &swap2nd, (void*)0 },
						/* ( uintptr_t*a uintptr_t*c uintptr_t*b --
							uintptr_t*b uintptr_t*c uintptr_t*a ) */
					(retframe){ &swap3rd, (void)*0 }
				}
			};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	/* ( uintptr_t*a uintptr_t*b uintptr_t*c -- uintptr_t*c uintptr_t*a uintptr_t*b ) */
retframe recede_to3rd( stackpair *stkp, void *v )
{
	static retframe_parr
		seq =
			{
				2, /* Number of retframes  */
				{
						/* ( uintptr_t*a uintptr_t*b uintptr_t*c -- uintptr_t*c uintptr_t*b uintptr_t*a ) */
					(retframe){ &swap3rd, (void)*0 },
						/* ( uintptr_t*b uintptr_t*a -- uintptr_t*a uintptr_t*b ) */
					(retframe){ &swap2nd, (void*)0 },
						/* ( -- uintptr_t*c uintptr_t*a uintptr_t*b ) */
				}
			};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}

	/* ( uintptr_t retframe -- retframe uintptr_t ) */
retframe swap2nd_to_retframe( stackpair *stkp, void *v )
{
	return( (retframe){ &raise3rd, (void*)0 } );
}
	/* ( retframe uintptr_t -- uintptr_t retframe ) */
retframe swap2nd_from_retframe( stackpair *stkp, void *v )
{
	return( (retframe){ &recede_to3rd, (void*)0 } );
}
	/* ( uintptr_t*1 uintptr_t*2 retframe -- retframe uintptr_t*1 uintptr_t*2 ) */
retframe swap3rd_retframe( stackpair *stkp, void *v )
{
	static retframe_parr
		seq =
			{
				4, /* Number of retframes  */
				{
						/* ( retframe[1] retframe[2] -- retframe[2] retframe[1] ) */
					(retframe){ &swap2nd, (void*)0 },
						/* ( uintptr_t*1 uintptr_t*2 retframe[2] retframe[1] --
							retframe[1] uintptr_t*2 retframe[2] uintptr_t*1 ) */
					(retframe){ &swap4th, (void)*0 },
						/* ( retframe[2] uintptr_t*1 -- uintptr_t*1 retframe[2] ) */
					(retframe){ &swap2nd, (void*)0 },
						/* ( retframe[1] uintptr_t*2 uintptr_t*1 retframe[2] --
							retframe[1] retframe[2] uintptr_t*1 uintptr_t*2 ) */
					(retframe){ &swap3rd, (void*)0 },
						/* ( .. -- retframe uintptr_t*1 uintptr_t*2 ) */
				}
			};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}

	/* ( uintptr_t -- ret: uintptr_t ) */
retframe swap2ret( stackpair *stkp, void *v )
{
	uintptr_t a;
	int scratch;
	retframe ret;
	
	STACKCHECK( stkp,  swap2ret );
	
	STACK_POP_RETFRAME( stkp->ret, ret,  &errs, swap2ret, scratch, stack_ENDRETFRAME );
	
	STACKPOP_UINT( stkp->data, a,  swap2ret, scratch );
	STACKPUSH_UINT( stkp->ret, a,  swap2ret, scratch );
	
	return( ret );
}
	/* ( ret: uintptr_t -- uintptr_t ) */
retframe swap2data( stackpair *stkp, void *v )
{
	uintptr_t a;
	int scratch;
	retframe ret;
	
	STACKCHECK( stkp,  swap2data );
	
	STACKPOP_UINT( stkp->ret, a,  swap2data, scratch );
	STACKPUSH_UINT( stkp->data, a,  swap2data, scratch );
	
	RETFRAMEFUNC( stkp,  swap2data );
}
	/* ( retframe -- ret: retframe ) */
retframe swap_retframe2ret( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	retframe ret;
	
	STACKCHECK( stkp,  swap_retframe2ret );
	
	STACK_POP_RETFRAME( stkp->ret, ret,  &errs, swap_retframe2ret, scratch, stack_ENDRETFRAME );
	
	
	STACKPOP_UINT( stkp->data, a,  swap_retframe2ret, scratch );
	STACKPOP_UINT( stkp->data, b,  swap_retframe2ret, scratch );
	
	STACKPUSH_UINT( stkp->ret, b,  swap_retframe2ret, scratch );
	STACKPUSH_UINT( stkp->ret, a,  swap_retframe2ret, scratch );
	
	
	return( ret );
}
	/* ( ret: retframe -- retframe ) */
retframe swap_retframe2data( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	retframe ret;
	
	STACKCHECK( stkp,  swap_retframe2data );
	
	
	STACKPOP_UINT( stkp->ret, a,  swap_retframe2data, scratch );
	STACKPOP_UINT( stkp->ret, b,  swap_retframe2data, scratch );
	
	STACKPUSH_UINT( stkp->data, b,  swap_retframe2data, scratch );
	STACKPUSH_UINT( stkp->data, a,  swap_retframe2data, scratch );
	
	
	RETFRAMEFUNC( stkp,  swap_retframe2data );
}

retframe swapdata2shuffle( stackpair *stkp, void *v )
{
	uintptr_t val;
	int scratch;
	
	STACKCHECK( stkp,  swapdata2shuffle );
	
	
	STACKPOP_UINT( stkp->data, val,  swapdata2shuffle, scratch );
	
	PUSH_SHUFFLE( val,  &errs, swapdata2shuffle, scratch, stack_ENDRETFRAME );
	
	
	RETFRAMEFUNC( stkp,  swapdata2shuffle );
}
retframe swapshuffle2data( stackpair *stkp, void *v )
{
	token *val;
	int scratch;
	
	STACKCHECK( stkp,  swapshuffle2data );
	
	
#define swapshuffle2data_ONERR( dummy1, dummy2, scratcharg ) \
		STDMSG_FAILEDINTFUNC_WRAPPER( &errs, "token_queue_shufflepop", swapshuffle2data, scratch ); \
		stack_ENDRETFRAME();
	POP_SHUFFLE( scratch, &val,  swapshuffle2data_ONERR, val, 0 );
	if( !val )
	{
		STDMSG_BADNULL_WRAPPER( &errs, swapshuffle2data, &val );
		stack_ENDRETFRAME();
	}
	
	STACKPUSH_UINT( stkp->data, (uintptr_t)val,  swapshuffle2data, scratch );
	
	
	RETFRAMEFUNC( stkp,  swapshuffle2data );
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

retframe vm_lesser( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  vm_lesser );
	
	
	STACKPOP_UINT( stkp->data, a,  vm_lesser, scratch );
	STACKPOP_UINT( stkp->data, b,  vm_lesser, scratch );
	
	STACKPUSH_UINT( stkp->data, a < b,  vm_lesser, scratch );
	
	RETFRAMEFUNC( stkp,  vm_lesser );
}
retframe vm_equal( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  vm_equal );
	
	
	STACKPOP_UINT( stkp->data, a,  vm_equal, scratch );
	STACKPOP_UINT( stkp->data, b,  vm_equal, scratch );
	
	STACKPUSH_UINT( stkp->data, a < b,  vm_equal, scratch );
	
	RETFRAMEFUNC( stkp,  vm_equal );
}
retframe vm_greater( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  vm_greater );
	
	
	STACKPOP_UINT( stkp->data, a,  vm_greater, scratch );
	STACKPOP_UINT( stkp->data, b,  vm_greater, scratch );
	
	STACKPUSH_UINT( stkp->data, a > b,  vm_greater, scratch );
	
	RETFRAMEFUNC( stkp,  vm_greater );
}

retframe vm_not( stackpair *stkp, void *v )
{
	uintptr_t val;
	int scratch;
	
	STACKCHECK( stkp,  vm_not );
	
	STACKPOP_UINT( stkp->data, val,  vm_not, scratch );
	STACKPUSH_UINT( stkp->data, !val,  vm_not, scratch );
	
	RETFRAMEFUNC( stkp,  vm_not );
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

retframe vm_push_noop( stackpair *stkp, void *v )
{
	return( vm_pushretframe( stkp, (void*)&noop_retframe ) );
}
retframe vm_push_placeholder( stackpair *stkp, void *v )
{
	return( vm_pushretframe( stkp, (void*)&vm_placeholder_retframe ) );
}


retframe just_run( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  just_run );
	
	return( *( (retframe*)v ) );
}
	/* ( uintptr_t -- uintptr_t ) */
	/* Requires a pointer to a retframe as v. */
retframe run_if( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  run_if );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_if, scratch );
	
	if( a )
	{
		return( *( (retframe*)v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_if );
	}
}
retframe run_else( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  run_else );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_else, scratch );
	
	if( !a )
	{
		return( *( (retframe*)v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_else );
	}
}
retframe run_on0( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  run_on0 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on0, scratch );
	
	if( a == 0 )
	{
		return( *( (retframe*)v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on0 );
	}
}
retframe run_on1( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  run_on1 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on1, scratch );
	
	if( a == 1 )
	{
		return( *( (retframe*)v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on1 );
	}
}
retframe run_on2( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  run_on2 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on2, scratch );
	
	if( a == 2 )
	{
		return( *( (retframe*)v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on2 );
	}
}
retframe run_on3( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  run_on3 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on3, scratch );
	
	if( a == 3 )
	{
		return( *( (retframe*)v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on3 );
	}
}
retframe vm_datacall( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_datacall );
	
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
	/* Runs enqueue_returns() with v_ upon condition match. */
	/* ( uintptr_t -- uintptr_t ) */
retframe enqueue_if( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  enqueue_if );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  enqueue_if, scratch );
	
	if( a )
	{
		return( enqueue_returns( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  enqueue_if );
	}
}
	/* ( uintptr_t -- uintptr_t ) */
retframe enqueue_else( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  enqueue_else );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  enqueue_else, scratch );
	
	if( !a )
	{
		return( enqueue_returns( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  enqueue_else );
	}
}
retframe enqueue_on0( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  enqueue_on0 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  enqueue_on0, scratch );
	
	if( a == 0 )
	{
		return( enqueue_returns( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  enqueue_on0 );
	}
}
retframe enqueue_on1( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  enqueue_on1 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  enqueue_on1, scratch );
	
	if( a == 1 )
	{
		return( enqueue_returns( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  enqueue_on1 );
	}
}
retframe enqueue_on2( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  enqueue_on2 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  enqueue_on2, scratch );
	
	if( a == 2 )
	{
		return( enqueue_returns( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  enqueue_on2 );
	}
}
retframe enqueue_on3( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  enqueue_on3 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  enqueue_on3, scratch );
	
	if( a == 3 )
	{
		return( enqueue_returns( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  enqueue_on3 );
	}
}
	/* Runs vm_pushretframe() with v_ upon condition match. */
	/* ( uintptr_t -- uintptr_t ) */
retframe vm_pushretframe_if( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  vm_pushretframe_if );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  vm_pushretframe_if, scratch );
	
	if( a )
	{
		return( vm_pushretframe( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  vm_pushretframe_if );
	}
}
	/* ( uintptr_t -- uintptr_t ) */
retframe vm_pushretframe_else( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  vm_pushretframe_else );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  vm_pushretframe_else, scratch );
	
	if( !a )
	{
		return( vm_pushretframe( stkp, v_ ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  vm_pushretframe_else );
	}
}



	/* ( char_parr* --  ) */
retframe dealloc_cparr( stackpair *stkp, void *v_ )
{
	/* Does the obvious. This is here because it doesn't */
	/*  really belong with the token tools. Used by source.c */
	
	???
#define refed_pstr_decrrefs_BADDESTROY( err ) \
	MONADICFAILURE( init, "char_pascalarray_destroy()", ( err ).val ); \
	NOTESPACE(); DATAPTRARG( pstr->text ); \
	ret = -2;
			lib4_result res = char_pascalarray_destroy( pstr->text );
			LIB4_RESULT_BODYMATCH( res, LIB4_NULL_MACRO, refed_pstr_decrrefs_BADDESTROY );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
