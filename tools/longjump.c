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

#include "longjump.h"

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



#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), stack_ENDRETFRAME )
#define STACKCHECK2( stack, v,  caller ) \
	STACK_CHECK2( ( stack ), ( v ),  &errs, ( caller ), stack_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )

#define RETFRAMEFUNC( stkpair,  caller ) \
	RET_FRAMEFUNC( ( stkpair ),  &errs, ( caller ), res, stack_ENDRETFRAME )



#define FAILED_INTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), ( callername ), ( val ) )

#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
#define DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )



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
		FAILED_INTFUNC( "tellmark_stack", setjump_callstack, scratch );
		stack_ENDRETFRAME();
	}
	*bookmark = tmp;
	
	
	retframe ret;
		/* We DO NOT want to directly pop the next retframe, */
		/*  since longjump_*() will want it. */
	scratch = peek_retframe( stkp->ret,  0,  &ret );
	if( !scratch )
	{
		FAILED_INTFUNC( "peek_retframe", setjump_callstack, scratch );
		stack_ENDRETFRAME();
	}
	
	
		/* We delay this just to avoid any stack changes in */
		/*  case of earlier errors. */
	STACKPUSH_UINT( stkp->data, 0,  setjump_callstack, scratch );
	
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
		FAILED_INTFUNC( "rewind_stack", longjump_callstack, scratch );
		stack_ENDRETFRAME();
	}
	
	STACKPUSH_UINT( stkp->data, 1,  longjump_callstack, scratch );
	RETFRAMEFUNC( stkp,  longjump_callstack );
}





/* Herein begins the divertthread system, a rather unweildy dragon that likely as not isn't ready to run yet. And yet, run it someday must. */

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

