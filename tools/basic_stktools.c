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
retframe run_on0( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  run_on0 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on0, scratch );
	
	if( a == 0 )
	{
		return( *( (retframe*)v ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on0 );
	}
}
retframe run_on1( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  run_on1 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on1, scratch );
	
	if( a == 1 )
	{
		return( *( (retframe*)v ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on1 );
	}
}
retframe run_on2( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  run_on2 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on2, scratch );
	
	if( a == 2 )
	{
		return( *( (retframe*)v ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on2 );
	}
}
retframe run_on3( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  run_on3 );
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  run_on3, scratch );
	
	if( a == 3 )
	{
		return( *( (retframe*)v ) );
		
	} else {
		
		RETFRAMEFUNC( stkp,  run_on3 );
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


	/* ( -- 0 ) */
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
	/* ( -- 1 ) */
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



retframe divertthread_earlyexit_ptr_placeholder( stackpair *ignore1, divertthread_info *ignore2, unsigned ignore3 )
{
	return( (retframe){ &vm_placeholder, (void*)0 } );
}

int push_divertthread_callerinfo( stackframe *stk, divertthread_callerinfo val )
{
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	
	int res = push_retframe( stk,  val.longjump );
	if( !res )
	{
			/* Yes, we're numbering errors in reverse. */
		return( res - 3 );
	}
	
	if( !push_uintptr( stk,  (uintptr_t)( val.earlyexit.data ) ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 3 );
	}
	if( !push_uintptr( stk,  (uintptr_t)( val.earlyexit.handle ) ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 2 );
	}
	if( !push_uintptr( stk,  val.user_typeid ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	
	
	return( 1 );
}
int peek_divertthread_callerinfo( stackframe *stk,  size_t off,  divertthread_callerinfo *val, uintptr_t *user_typeid )
{
	if( val || user_typeid )
	{
		uintptr_t tmp;
		
		int res = peek_uintptr( stk,  off,  &tmp );
		if( !res )
		{
			return( res );
		}
		if( user_typeid )
		{
			*user_typeid = tmp;
		}
		if( val )
		{
			val->user_typeid = tmp;
			
			res = peek_uintptr( stk,  off + sizeof( uintptr_t ),  &tmp );
			if( res == LIB4_STDERRS_BADARGS_SIMPLE )
			{
					/* How?!? */
				return( -sizeof( uintptr_t ) * 1 );
			}
			if( !res )
			{
				return( res - ( sizeof( uintptr_t ) * 1 + 1 ) );
			}
			val->earlyexit.handle = (divertthread_earlyexit_ptr)tmp;
			
			res = peek_uintptr( stk,  off + sizeof( uintptr_t ) * 2,  &tmp );
			if( res == LIB4_STDERRS_BADARGS_SIMPLE )
			{
					/* How?!? */
				return( -sizeof( uintptr_t ) * 2 );
			}
			if( !res )
			{
				return( res - ( sizeof( uintptr_t ) * 2 + 1 ) );
			}
			val->earlyexit.data = (divertthread_info*)tmp;
			
			res = peek_retframe( stk,  off + sizeof( uintptr_t ) * 3,  &( val->longjump ) );
			if( !res )
			{
				return( res - sizeof( uintptr_t ) * 3 );
			}
		}
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}
int pop_divertthread_callerinfo( stackframe *stk,  divertthread_callerinfo *val )
{
	if( val )
	{
		uintptr_t tmp;
		
		
		int res = pop_uintptr( stk,  &tmp );
		if( !res )
		{
			return( res );
		}
		val->user_typeid = tmp;
		
		res = pop_uintptr( stk,  &tmp );
		if( res == LIB4_STDERRS_BADARGS_SIMPLE )
		{
				/* How?!? */
			return( -sizeof( uintptr_t ) * 1 );
		}
		if( !res )
		{
			return( res - ( sizeof( uintptr_t ) * 1 + 1 ) );
		}
		val->earlyexit.handle = (divertthread_earlyexit_ptr)tmp;
		
		res = pop_uintptr( stk,  &tmp );
		if( res == LIB4_STDERRS_BADARGS_SIMPLE )
		{
				/* How?!? */
			return( -sizeof( uintptr_t ) * 2 );
		}
		if( !res )
		{
			return( res - ( sizeof( uintptr_t ) * 2 + 1 ) );
		}
		val->earlyexit.data = (divertthread_info*)tmp;
		
		res = pop_retframe( stk,  &( val->longjump ) );
		if( !res )
		{
			return( res - sizeof( uintptr_t ) * 3 );
		}
		
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}

int push_divertthread_info( stackframe *stk, divertthread_info val )
{
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	
	if( !push_uintptr( stk,  (uintptr_t)( val.recepdata ) ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - sizeof( uintptr_t ) * 3 );
	}
	
	int res = push_retframe( stk,  val.setfunc );
	if( !res )
	{
			/* Yes, we're numbering errors in reverse. */
		return( res - sizeof( uintptr_t ) * 2 );
	}
	res = push_retframe( stk,  val.jumpfunc );
	if( !res )
	{
			/* Yes, we're numbering errors in reverse. */
		return( res - sizeof( uintptr_t ) * 1 );
	}
	
	if( !push_uintptr( stk,  val.bookmark ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	
	
	return( 1 );
}
int peek_divertthread_info( stackframe *stk,  size_t off,  divertthread_info *val )
{
	if( val )
	{
		int res = peek_uintptr( stk,  off,  &( val->bookmark ) );
		if( !res )
		{
			return( res );
		}
		
		res = peek_retframe( stk,  off + sizeof( uintptr_t ) * 1,  &( val->jumpfunc ) );
		if( !res )
		{
			return( res - sizeof( uintptr_t ) * 1 );
		}
		
		res = peek_retframe( stk,  off + sizeof( uintptr_t ) * 3,  &( val->setfunc ) );
		if( !res )
		{
			return( res - sizeof( uintptr_t ) * 2 );
		}
		
		uintptr_t tmp;
		res = peek_uintptr( stk,  off + sizeof( uintptr_t ) * 5,  &tmp );
		if( !res )
		{
			return( res - sizeof( uintptr_t ) * 3 );
		}
		val->recepdata = (divertthread_callerinfo*)tmp;
		
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}
int pop_divertthread_info( stackframe *stk,  divertthread_info *val )
{
	if( val )
	{
		int res = pop_uintptr( stk,  &( val->bookmark ) );
		if( !res )
		{
			return( res );
		}
		
		res = pop_retframe( stk,  &( val->jumpfunc ) );
		if( !res )
		{
			return( res - sizeof( uintptr_t ) * 1 );
		}
		
		res = pop_retframe( stk,  &( val->setfunc ) );
		if( !res )
		{
			return( res - sizeof( uintptr_t ) * 2 );
		}
		
		uintptr_t tmp;
		res = pop_uintptr( stk,  &tmp );
		if( !res )
		{
			return( res - sizeof( uintptr_t ) * 3 );
		}
		val->recepdata = (divertthread_callerinfo*)tmp;
		
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}

static divertthread_info hooks;
int divertthread_adjust( divertthread_info *alt )
{
	if( alt )
	{
		retframe swap;
		
		swap = alt->setfunc;
		alt->setfunc = hooks.setfunc;
		hooks.setfunc = swap;
		
		swap = alt->jumpfunc;
		alt->jumpfunc = hooks.jumpfunc;
		hooks.jumpfunc = swap;
		
		
		divertthread_callerinfo *recepdata = alt->recepdata;
		alt->recepdata = hooks.recepdata;
		hooks.recepdata = recepdata;
		
		return( 1 );
	}
	
	return( -1 );
}
		/* Acts as divertthread_exit(), but for when using */
		/*  longjump() to jump past EARLIER setjump() instances. */
		/*  Provided as a retframe in divertthread_callerinfo->earlyexit */
		/* ( bookmark data[count] -- data[count] bookmark ) */
	retframe divertthread_earlyexit( stackpair *stkp, divertthread_info *v, unsigned count )
	{
		int scratch;
		STACKCHECK2( stkp, v,  divertthread_earlyexit );
		
		
			/* Transfer loop. */
		uintptr_t iter;
			iter = 0;
			while( iter < count )
			{
					/* Move from normal stack to shuffle stack. */
				???
				
				++iter;
			}
				uintptr_t bookmark;
				STACKPOP_UINT( &( stkp->data ), bookmark,  divertthread_earlyexit, scratch );
			iter = 0;
			while( iter < count )
			{
					/* Move from shuffle stack to normal stack. */
				???
				
				++iter;
			}
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)bookmark,  divertthread_earlyexit, scratch );
		
		
		if( !divertthread_adjust( v ) )
		{
			???
		}
		
		
		RETFRAMEFUNC( divertthread_earlyexit );
	}
	/* ( bookmark data -- data bookmark ) */
retframe divertthread_exit( stackpair *stkp, void *v_ )
{
	int scratch;
	STACKCHECK2( stkp, v_,  divertthread_exit );
	
	
		/* ( bookmark data[count] count -- ... ) */
	uintptr_t count;
	STACKPOP_UINT( &( stkp->data ), count,  divertthread_exit, scratch );
	
	return( divertthread_earlyexit( stkp, (divertthread_info*)v_, count ) );
}
retframe divertthread( stackpair *stkp, void *v_ )
{
				/* This commenmt should be replaced- this code is no longer */
				/*  tied to a bracket accumulator. */
			/* (
				token* bookmark --
					bookmark token* --
					... --
				( token* 0 ) | ( tokengroup* 1 ) | ( tokengroup* tokengroup* token* 2 )
			) */
	static retframe_parr
		onset_ =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* ( uintptr_t bookmark -- bookmark uintptr_t ) */
					(retframe){ &swap2nd, (void*)0 },
							/* This requires a pointer to a retframe as v. */
						(retframe){ &just_run, (void*)&( hooks.setfunc ) },
					(retframe){ &divertthread_exit, (void*)0 }
				}
			};
	
		/* Remember, the adjusted stuff will get handed directly to */
		/*  enqueue_returns(): it'll be dealt with before there's a */
		/*  chance of the value getting changed again. */
#define divertthread_ADJUST_ONSET() \
		onset_.body[ 2 ].data = v_;
	
		/* These two should only be tied to statically allocated objects, so I */
		/*  expect them to both be fine. */
	static retframe
		onset = { &enqueue_returns, (void*)&onset_ },
			/* This requires a pointer to a retframe as v. */
		onjump = { &just_run, (void*)&( hooks.jumpfunc ) };
	
	
		/* The bookmark gets used to store the value of the return stack size. */
		/* "callable" gets used as follows: */
			/* (retframe){ &enqueue_returns, (void*)&localname } : */
			/*  (  -- uintptr_t-bookmark ) -> onset ->  ( uintptr_t-bookmark --  ) */
		/* "space" gets prefixed to the start of several variables that */
		/*  LOCALIZE_SETJUMP() creates: you don't directly use ANY of these. */
		/* onset and onjump are pointers to retframe instances with obvious purposes. */
		/* Note that the onset and onjump retframes have to contend with a uintptr_t */
		/*  on top of the data stack: THEY MUST NOT SCREW AROUND WITH THIS (other than */
		/*  to VERY CAREFULLY move stuff underneath it), lest the entire system break, */
		/*  IT MUST BE ON TOP OF THE STACK UPON BOTH onset{} AND onjump{}'s RETURNS. */
		/*  Seriously, here there be dragons. */
	LOCALIZE_SETJUMP( v->bookmark, callable, space,  &onset, &onjump );
	
	
	int scratch;
	STACKCHECK2( stkp, v_,  divertthread );
	
	if( !v_ )
	{
		???
	}
	divertthread_info *v = (divertthread_info*)v_;
	if( v->recepdata )
	{
		v->recepdata->earlyexit.handle = &divertthread_earlyexit;
			/* Yes, the divertthread_info pointer submitted as our v_ IS */
			/*  correct, because it's needed so that it's values can be swapped */
			/*  with hook's values... */
		v->recepdata->earlyexit.data = v;
		
		v->recepdata->longjump.handle = &longjump_callstack;
		v->recepdata->longjump.data = &( v->bookmark );
	}
	
		/* Swap hook's values with *v's values: this will be reversed by either */
		/*  divertthread_exit() or divertthread_earlyexit() at a later date, to */
		/*  preserve a sort of longjump() stack. */
	if( !divertthread_adjust( v ) )
	{
		???
	}
	
	divertthread_ADJUST_ONSET();
	return( (retframe){ &enqueue_returns, (void*)&callable } );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
