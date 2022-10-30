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

#include "macrotime/arraccess.h"

#include "err/inner_err.h"

#include "stack.h"



stackpair std_stacks;

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

#define STACK_BADNULL( funcname, ptr ) STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define STACK_BADNULL2( funcname, ptr1, ptr2 ) STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define STACK_MONADICFAILURE( funcname, msgstr, err ) STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( msgstr ), ( err ) )
	#define STACK_NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	#define STACK_NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )
	
	#define STACK_DECARG( uint ) STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
	#define STACK_DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )


#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )


#define RET_FRAMEFUNC( stkpair,  caller, scratch ) \
	RET_FRAMEFUNC( ( stkpair ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )



int enable_loop( stackpair *stkp )
{
	if( !stkp )
	{
		STACK_BADNULL( run_loop, &enter );
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	stkp->run = 1;
	
	return( 1 );
}
int run_loop( retframe *enter,  stackpair *stkp )
{
	if( !enter || !stkp )
	{
		STACK_BADNULL2( run_loop, &enter, &stkp );
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	if( !( *enter ) || !( enter->handler ) )
	{
		STACK_BADNULL( run_loop, &enter );
		return( LIB4_STDERRS_BADARGS_DEEP );
	}
	
	while( stkp->run && *enter )
	{
		if( !( enter->handler ) )
		{
			return( LIB4_STDERRS__LAST_UNALLOC_ERR );
		}
		
		*enter = ( enter->handler )( stkp, enter->data );
	}
	
	return( 1 );
}
retframe end_run( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  end_run, stack_ENDRETFRAME );
	
	stkp->run = 0;
	
	return( (retframe){ (framefunc)0, (void*)0 } );
}

retframe noop( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  end_run, stack_ENDRETFRAME );
	
	int scratch;
	
	RET_FRAMEFUNC( stkp,  noop, scratch );
}



int init_stack( stackframe *stk )
{
	if( !stk )
	{
		STACK_BADNULL( init_stack, &stk );
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	if( stk->stack || stk->used )
	{
		STACK_BADNULL2( init_stack, &( stk->stack ), &( stk->used ) );
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	
#define init_stack_SUCCESS( arr ) \
		stk->stack = ( arr ); stk->used = 0;
#define init_stack_FAILURE( err ) \
		STACK_MONADICFAILURE( init_stack, "char_pascalarray_build( 64 )", ( err ) ); \
		return( LIB4_STDERRS_BADARGS_SIMPLE - 2 );
	
	char_pascalarray_result res =
		char_pascalarray_build( 64 );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, init_stack_SUCCESS, init_stack_FAILURE )
	
	return( 1 );
}
int resize_stack( stackframe *stk,  int deltaChars )
{
	int len = stk->stack->len + deltaChars;
	
	if( !stk || len < 0 )
	{
		STACK_BADNULL2( resize_stack, &stk, &len );
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	char_pascalarray_result res =
		char_pascalarray_rebuild( stk->stack, len );
#define resize_stack_SUCCESS( arr ) stk->stack = ( arr );
#define resize_stack_FAILURE( err ) \
		STACK_MONADICFAILURE( resize_stack, "char_pascalarray_rebuild()", ( err ) ); \
		STACK_NOTELINE(); STACK_DATAPTRARG( stk->stack ); \
		STACK_NOTESPACE(); STACK_DECARG( len ); \
		return( err );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, resize_stack_SUCCESS, resize_stack_FAILURE )
	
	if( stk->used > len )
	{
		stk->used = len;
	}
	
	return( 1 );
}
	int tellmark_stack( stackframe *stk,  size_t *res )
	{
		if( !stk || !res )
		{
			STACK_BADNULL2( tellmark_stack, &stk, &len );
			return( LIB4_STDERRS_BADARGS_SIMPLE );
		}

		*res = stk->used;

		return( 1 );
	}
	int rewind_stack( stackframe *stk,  size_t targ )
	{
		if( !stk || !res )
		{
			STACK_BADNULL2( rewind_stack, &stk, &len );
			return( LIB4_STDERRS_BADARGS_SIMPLE );
		}

		if( targ > stk->used )
		{
			/* Error: tried to "shrink" to a LARGER size. */

			return( LIB4_STDERRS_RANGEERR );
		}

		stk->used = targ;

		return( 1 );
	}
int clear_stack( stackframe *stk )
{
	if( !stk )
	{
		STACK_BADNULL( clear_stack, &stk );
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	lib4_result res = char_pascalarray_destroy( stk->stack );
#define pop_frame_SUCCESS( var ) ;
#define pop_frame_FAILURE( err ) \
		STACK_MONADICFAILURE( clear_stack, "char_pascalarray_destroy()", ( err ) ); \
		STACK_NOTELINE(); STACK_DATAPTRARG( stk->stack ); \
		return( err );
	LIB4_RESULT_BODYMATCH( res, pop_frame_SUCCESS, pop_frame_FAILURE )
	
	stk->stack = (char_pascalarray*)0;
	stk->used = 0;
	
	return( 1 );
}


int push_char( stackframe *stk,  char val )
{
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	if( stk->used >= stk->stack->len )
	{
		int res = resize_stack( stk,  stk->stack->len );
		
		if( res < 0 )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
		}
	}
	
	stk->stack->body[ stk->used ] = val;
	++( stk->used );
	
	return( 1 );
}
int pop_char( stackframe *stk,  char *dest )
{
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	if( stk->used <= 0 )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	
	--( stk->used );
	if( dest )
	{
		*dest = stk->stack->body[ stk->used ];
		
		return( 2 );
	}
	
	return( 1 );
}
int peek_char( stackframe *stk,  size_t off,  char *dest )
{
	if( !stk || !dest || off >= stk->used )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	*dest = stk->stack->body[ stk->used - ( off + 1 ) ];
	
	return( 1 );
}


int push_int( stackframe *stk,  int val )
{
	unsigned char tmp;
	unsigned int acc = *( (unsigned int*)&val ), l = 0;
	
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	while( l < sizeof( int ) )
	{
		tmp = acc & ( (unsigned int)( 1 << CHARBITS ) - 1 );
		val = push_char( stk,  *( (char*)&tmp ) );
		
		if( !val )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - ( l + 1 ) );
		}
		
		acc >> CHARBIT;
		++l;
	}
	
	return( 1 );
}
int pop_int( stackframe *stk,  int *dest )
{
	int res, l = 0;
	unsigned int acc = 0;
	char tmp;
	
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	while( l < sizeof( int ) )
	{
		res = pop_char( stk,  &tmp );
		if( !res )
		{
			if( dest )
			{
				*dest = *( (int*)&acc );
			}
			
			return( LIB4_STDERRS_BADARGS_SIMPLE - ( l + 1 ) );
		}
		
		acc << CHARBIT;
		acc |= *( (unsigned char*)&tmp );
		
		++l;
	}
	
	if( dest )
	{
		*dest = *( (int*)&acc );
	}
	
	return( 1 );
}
int peek_int( stackframe *stk,  size_t off,  int *dest )
{
	if( stk && dest )
	{
		int res, l = 0;
		char tmp;
		unsigned int acc = 0;
		
		while( l < sizeof( int ) )
		{
			res = peek_char( stk,  off + l,  &tmp );
			if( !res )
			{
				return( LIB4_STDERRS_BADARGS_SIMPLE - ( l + 1 ) );
			}
			
			acc << CHARBIT;
			acc |= *( (unsigned char*)&tmp );
			
			++l;
		}
		
		*( (unsigned int*)dest ) = acc;
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}

int push_uintptr( stackframe *stk,  uintptr_t val )
{
	unsigned char tmp;
	int res, l = 0;
	
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	while( l < sizeof( uintptr_t ) )
	{
		tmp = val & ( ( 1 << CHARBITS ) - 1 );
		res = push_char( stk,  *( (char*)&tmp ) );
		
		if( !res )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - ( l + 1 ) );
		}
		
		val >> CHARBIT;
		++l;
	}
	
	return( 1 );
}
int pop_uintptr( stackframe *stk,  uintptr_t *dest )
{
	int res, l = 0;
	char tmp;
	
	if( !stk || !dest )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	*dest = 0;
	
	while( l < sizeof( uintptr_t ) )
	{
		res = pop_char( stk,  &tmp );
		if( !res )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - ( l + 1 ) );
		}
		
		*dest << CHARBIT;
		*dest |= *( (unsigned char*)&tmp );
		
		++l;
	}
	
	return( 1 );
}
int peek_uintptr( stackframe *stk,  size_t off,  uintptr_t *dest )
{
	if( dest )
	{
		int res, l = 0;
		char tmp;
		
		*dest = 0;
		
		while( l < sizeof( uintptr_t ) )
		{
			res = peek_char( stk,  off + l,  &tmp );
			if( !res )
			{
				return( LIB4_STDERRS_BADARGS_SIMPLE - ( l + 1 ) );
			}
			
			*dest << CHARBIT;
			*dest |= *( (unsigned char*)&tmp );
			
			++l;
		}
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}

int push_retframe( stackframe *stk,  retframe rf )
{
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	int res = push_uintptr( stk,  (uintptr_t)( rf.handler ) );
	if( res == LIB4_STDERRS_BADARGS_SIMPLE )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	if( !res )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 2 );
	}
	
	res = push_uintptr( stk,  (uintptr_t)( rf.data ) );
	if( res == LIB4_STDERRS_BADARGS_SIMPLE )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 3 );
	}
	if( !res )
	{
		res = -res;
		while( res )
		{
			pop_char( stk,  (char*)0 );
			--res;
		}
		return( LIB4_STDERRS_BADARGS_SIMPLE - 4 );
	}
	
	return( 1 );
}
int pop_retframe( stackframe *stk,  retframe *rf )
{
	uintptr_t tmp;
	
	int res = pop_uintptr( stk,  &tmp );
	if( !res )
	{
		return( res );
	}
	if( rf )
	{
		rf->data = (void*)tmp;
	}
	
	res = pop_uintptr( stk,  &tmp );
	if( !res )
	{
			/* To indicate where the error was, add the size we already */
			/*  popped... */
		return( res - sizeof( uintptr_t ) );
	}
	if( rf )
	{
		rf->handler = (framefunc)tmp;
	}
	
	return( 1 );
}
int peek_retframe( stackframe *stk,  size_t off,  retframe *rf )
{
	if( rf )
	{
		uintptr_t tmp;
		
		int res = peek_uintptr( stk,  off,  &tmp );
		if( !res )
		{
			return( res );
		}
		rf->data = (void*)tmp;
		
		res = peek_uintptr( stk,  off + sizeof( uintptr_t ),  &tmp );
		if( res == LIB4_STDERRS_BADARGS_SIMPLE )
		{
				/* How?!? */
			return( -sizeof( uintptr_t ) );
		}
		if( !res )
		{
			return( res - ( sizeof( uintptr_t ) + 1 ) );
		}
		rf->handler = (framefunc)tmp;
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}

int push_tokenhead( stackframe *stk,  token_head val )
{
	if( !stk )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	if( !push_int( stk,  val.is_delimited ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	if( !push_int( stk,  !( val.column ) ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 2 );
	}
	if( !push_int( stk,  val.length ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 3 );
	}
	if( !push_uintptr( stk,  val.toktype ) )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE - 4 );
	}
	
	return( 1 );
}
int pop_tokenhead( stackframe *stk,  token_head *dest )
{
	if( stk && dest )
	{
		if( !pop_uintptr( stk,  &( dest->toktype ) ) )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
		}
		if( !pop_int( stk,  &( dest->length ) ) )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - 2 );
		}
		if( !pop_int( stk,  &( !( dest->column ) ) ) )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - 3 );
		}
		if( !pop_int( stk,  &( dest->is_delimited ) ) )
		{
			return( LIB4_STDERRS_BADARGS_SIMPLE - 4 );
		}
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}
int peek_tokenhead( stackframe *stk,  size_t off,  token_head *dest )
{
	if( stk && rf )
	{
		token_head tmp;
		
		int res = peek_uintptr( stk,  off,   &( tmp.toktype ) );
		if( !res )
		{
			return( res );
		}
		
		res = peek_int( stk,  off + sizeof( uintptr_t ),  &( tmp.length ) );
		if( res == LIB4_STDERRS_BADARGS_SIMPLE )
		{
				/* How?!? */
			return( -sizeof( uintptr_t ) );
		}
		if( !res )
		{
			return( res - sizeof( uintptr_t ) );
		}
		
		res = peek_int( stk,  off + sizeof( uintptr_t ),  &( !( tmp.column ) ) );
		if( res == LIB4_STDERRS_BADARGS_SIMPLE ) /* ??? */
		{
				/* How?!? */
			return( -sizeof( uintptr_t ) * 2 );
		}
		if( !res )
		{
			return( ( -sizeof( uintptr_t ) * 2 ) + res );
		}
		}
		
		res = peek_int( stk,  off + sizeof( uintptr_t ),  &( tmp.is_delimited ) );
		if( res == LIB4_STDERRS_BADARGS_SIMPLE )
		{
				/* How?!? */
			return( -sizeof( uintptr_t ) * 3 );
		}
		if( !res )
		{
			return( ( -sizeof( uintptr_t ) * 3 ) + res );
		}
		
		*dest = tmp;
		
		return( 1 );
	}
	
	return( LIB4_STDERRS_BADARGS_SIMPLE );
}


int push_block( stackframe *stk,  const char *src, size_t len )
{
	if( !stk || !src )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	while( len )
	{
		int ret = push_char( stk,  *src );
		if( !ret )
		{
			return( ret + LIB4_STDERRS_BADARGS_SIMPLE );
		}
		
		++src;
		--len;
	}
	
	return( 1 );
}
int pop_block( stackframe *stk,  char *dest, size_t len )
{
	if( !stk || !dest )
	{
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	while( len )
	{
		--len;
		
		int ret = pop_char( stk,  dest + len );
		if( !ret )
		{
			return( ret + LIB4_STDERRS_BADARGS_SIMPLE );
		}
	}
	
	return( 1 );
}

int push_token( stackframe *stk,  token_head *src )
{
	if( !stk || !src )
	{
		STACK_BADNULL( push_token, &src );
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	/* Note to self: verify that all tokens use ->length correctly, so that */
	/*  this actually works! */
	
	int res = push_block( stk,  ( (const char*)src ) + sizeof( token_head ), src->length );
	if( !res )
	{
		STACK_BADNULL( push_token, &res );
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	res = push_tokenhead( stk,  *src );
	if( !res )
	{
		STACK_BADNULL( push_token, &res );
		return( LIB4_STDERRS_BADARGS_SIMPLE - 2 );
	}
	
	return( 1 );
}
int pop_token( stackframe *stk,  token_head **dest )
{
	token_head tmp, *a = (token_head*)0;
	
	if( !stk || !dest )
	{
		STACK_BADNULL( pop_token, &dest );
		return( LIB4_STDERRS_BADARGS_SIMPLE );
	}
	
	int res = pop_tokenhead( stk,  &tmp );
	if( !res )
	{
		STACK_BADNULL( pop_token, &res );
		return( LIB4_STDERRS_BADARGS_SIMPLE - 1 );
	}
	
	size_t len = sizeof( token ) + ( sizeof( char ) * tmp.length );
#define pop_token_FAIL( err ) \
		STACK_MONADICFAILURE( pop_token, "lib4_stdmemfuncs.alloc()", ( err ) ); \
		STACK_NOTELINE(); STACK_DATAPTR( lib4_stdmemfuncs.data ); \
		STACK_NOTESPACE(); STACK_DECARG( len ); \
		return( LIB4_STDERRS_BADARGS_SIMPLE - 2 );
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc( lib4_stdmemfuncs.data, len );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, pop_token_FAIL )
	if( !a )
	{
		STACK_BADNULL( pop_token, &a );
		return( LIB4_STDERRS_BADARGS_SIMPLE - 3 );
	}
	
	res = pop_block( stk,  ( (const char*)a ) + sizeof( token_head ), tmp.length );
		a->toktype = tmp.toktype;
		a->length = tmp.length;
	if( !res )
	{
		STACK_BADNULL( pop_token, &res );
		return( LIB4_STDERRS_BADARGS_SIMPLE - 4 );
	}
	
	*dest = a;
	
	return( 1 );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
