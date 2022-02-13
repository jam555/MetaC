#include "headers.h"



stackpair std_stacks;

int init_stack( stackframe *stk )
{
	if( !stk )
	{
		err_interface( (uintptr_t)&std_stacks, (lib4_failure_result){ -1 }, (int)-1, &stk );
		return( -1 );
	}
	if( stk->stack || stk->used )
	{
		err_interface( (uintptr_t)&std_stacks, (lib4_failure_result){ -1 }, (int)-2, &stk );
		return( -2 );
	}
	
#define init_stack_SUCCESS( arr ) \
		stk->stack = ( arr ); stk->used = 0;
#define init_stack_FAILURE( err ) \
		err_interface( (uintptr_t)&std_stacks, (lib4_failure_result){ -1 }, (int)-3, &stk, (err) ); \
		return( -3 );
	
	char_pascalarray_result res =
		char_pascalarray_build( 64 );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, init_stack_SUCCESS, init_stack_FAILURE )
	
	return( 1 );
}
int resize_stack( stackframe *stk,  int deltaChars )
{
	int len = stk->stack->len + deltaChars;
	
	if( len < 0 )
	{
		err_interface( (uintptr_t)&std_stacks, (lib4_failure_result){ -2 }, (int)-1, &stk, &len );
		return( -1 );
	}
	
	char_pascalarray_result res =
		char_pascalarray_rebuild( stk->stack, len );
#define resize_stack_SUCCESS( arr ) stk->stack = ( arr );
#define resize_stack_FAILURE( err ) \
		err_interface( (uintptr_t)&std_stacks, (lib4_failure_result){ -2 }, (int)-2, &stk, &len, &res, (err) ); \
		return( err );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, resize_stack_SUCCESS, resize_stack_FAILURE )
	
	if( stk->used > len )
	{
		stk->used = len;
	}
	
	return( 1 );
}
int clear_stack( stackframe *stk )
{
	if( !stk )
	{
		err_interface( (uintptr_t)&std_stacks, (lib4_failure_result){ -3 }, (int)-1, &stk );
		return( -1 );
	}
	
	lib4_result res = char_pascalarray_destroy( stk->stack );
#define pop_frame_SUCCESS( var ) ;
#define pop_frame_FAILURE( err ) \
		err_interface( (uintptr_t)&std_stacks, (lib4_failure_result){ -3 }, (int)-2, &stk, &res, (err) ); \
		return( err );
	LIB4_RESULT_BODYMATCH( res, pop_frame_SUCCESS, pop_frame_FAILURE )
	
	stk->stack = (char_pascalarray*)0;
	stk->used = 0;
	
	return( 1 );
}


int push_char( stackframe *stk,  char val )
{
	if( stk->used >= stk->stack->len )
	{
		int res = resize_stack( stk,  stk->stack->len );
		
		if( res < 0 )
		{
			return( -2 );
		}
	}
	
	stk->stack->body[ stk->used ] = val;
	++( stk->used );
	
	return( 1 );
}
int pop_char( stackframe *stk,  char *dest )
{
	if( stk->used <= 0 )
	{
		return( -2 );
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
	if( !dest || off >= stk->used )
	{
		return( -1 );
	}
	
	*dest = stk->stack->body[ stk->used - ( off + 1 ) ];
	
	return( 1 );
}


int push_int( stackframe *stk,  int val )
{
	unsigned char tmp;
	unsigned int acc = *( (unsigned int*)&val ), l = 0;
	
	while( l < sizeof( int ) )
	{
		tmp = acc & ( (unsigned int)( 1 << CHARBITS ) - 1 );
		val = push_char( stk,  *( (char*)&tmp ) );
		
		if( !val )
		{
			return( -l );
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
	
	while( l < sizeof( int ) )
	{
		res = pop_char( stk,  &tmp );
		if( !res )
		{
			if( dest )
			{
				*dest = *( (int*)&acc );
			}
			
			return( -( l + 1 ) );
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
	if( dest )
	{
		int res, l = 0;
		char tmp;
		unsigned int acc = 0;
		
		while( l < sizeof( int ) )
		{
			res = peek_char( stk,  off + l,  &tmp );
			if( !res )
			{
				return( -( l + 3 ) );
			}
			
			acc << CHARBIT;
			acc |= *( (unsigned char*)&tmp );
			
			++l;
		}
		
		*( (unsigned int*)dest ) = acc;
		
		return( 1 );
	}
	
	return( -1 );
}

int push_uintptr( stackframe *stk,  uintptr_t val )
{
	unsigned char tmp;
	int res, l = 0;
	
	while( l < sizeof( uintptr_t ) )
	{
		tmp = val & ( ( 1 << CHARBITS ) - 1 );
		res = push_char( stk,  *( (char*)&tmp ) );
		
		if( !res )
		{
			return( -l );
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
	
	*dest = 0;
	
	while( l < sizeof( uintptr_t ) )
	{
		res = pop_char( stk,  &tmp );
		if( !res )
		{
			return( -( l + 1 ) );
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
				return( -( l + 3 ) );
			}
			
			*dest << CHARBIT;
			*dest |= *( (unsigned char*)&tmp );
			
			++l;
		}
		
		return( 1 );
	}
	
	return( -1 );
}

int push_retframe( stackframe *stk,  retframe rf )
{
	int res = push_uintptr( stk,  (uintptr_t)( rf.handler ) );
	if( !res )
	{
		return( -3 );
	}
	
	res = push_uintptr( stk,  (uintptr_t)( rf.data ) );
	if( !res )
	{
		res = -res;
		while( res )
		{
			pop_char( stk,  (char*)0 );
			--res;
		}
			return( -4 );
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
		if( res == -1 )
		{
				/* How?!? */
			return( -2 );
		}
		if( !res )
		{
			return( res - sizeof( uintptr_t ) );
		}
		rf->handler = (framefunc)tmp;
		
		return( 1 );
	}
	
	return( -1 );
}

int push_tokenhead( stackframe *stk,  token_head val )
{
	if( !push_int( stk,  val.is_delimited ) )
	{
		return( -2 );
	}
	if( !push_int( stk,  val.was_freshline ) )
	{
		return( -3 );
	}
	if( !push_int( stk,  val.length ) )
	{
		return( -4 );
	}
	if( !push_uintptr( stk,  val.toktype ) )
	{
		return( -5 );
	}
	
	return( 1 );
}
int pop_tokenhead( stackframe *stk,  token_head *dest )
{
	if( dest )
	{
		if( !pop_uintptr( stk,  &( dest->toktype ) ) )
		{
			return( -2 );
		}
		if( !pop_int( stk,  &( dest->length ) ) )
		{
			return( -3 );
		}
		if( !pop_int( stk,  &( dest->was_freshline ) ) )
		{
			return( -4 );
		}
		if( !pop_int( stk,  &( dest->is_delimited ) ) )
		{
			return( -5 );
		}
		
		return( 1 );
	}
	
	return( -1 );
}
int peek_tokenhead( stackframe *stk,  size_t off,  token_head *dest )
{
	if( rf )
	{
		token_head tmp;
		
		int res = peek_uintptr( stk,  off,   &( tmp.toktype ) );
		if( !res )
		{
			return( res );
		}
		
		res = peek_int( stk,  off + sizeof( uintptr_t ),  &( tmp.length ) );
		if( res == -1 )
		{
				/* How?!? */
			return( -2 );
		}
		if( !res )
		{
			return( res - sizeof( uintptr_t ) );
		}
		
		res = peek_int( stk,  off + sizeof( uintptr_t ),  &( tmp.was_freshline ) );
		if( res == -1 )
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
		if( res == -1 )
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
	
	return( -1 );
}


int push_block( stackframe *stk,  const char *src, size_t len )
{
	if( !src )
	{
		return( -1 );
	}
	
	while( len )
	{
		int ret = push_char( stk,  *src );
		if( !ret )
		{
			return( ret - 1 );
		}
		
		++src;
		--len;
	}
	
	return( 1 );
}
int pop_block( stackframe *stk,  char *dest, size_t len )
{
	if( !dest )
	{
	}
	
	while( len )
	{
		--len;
		
		int ret = pop_char( stk,  dest + len );
		if( !ret )
		{
			return( ret - 1 );
		}
	}
	
	return( 1 );
}

uintptr_t stack_token_refid;
int push_token( stackframe *stk,  token_head *src )
{
	if( !src )
	{
		err_interface( &stack_token_refid, (uintptr_t)0, 0, &res, &dest );
		return( -1 );
	}
	
	/* Note to self: verify that all tokens use ->length correctly, so that */
	/*  this actually works! */
	
	int res = push_block( stk,  ( (const char*)src ) + sizeof( token_head ), src->length );
	if( !res )
	{
		err_interface( &stack_token_refid, (uintptr_t)0, 1, &src );
		return( -2 );
	}
	res = push_tokenhead( stk,  *src );
	if( !res )
	{
		err_interface( &stack_token_refid, (uintptr_t)0, 2, &src );
		return( -3 );
	}
	
	return( 1 );
}
int pop_token( stackframe *stk,  token_head **dest )
{
	token_head tmp, *a = (token_head*)0;
	
	if( !dest )
	{
		err_interface( &stack_token_refid, (uintptr_t)1, 0, &res, &dest );
		return( -1 );
	}
	
	int res = pop_tokenhead( stk,  &tmp );
	if( !res )
	{
		err_interface( &stack_token_refid, (uintptr_t)1, 1, &res, &dest );
		return( -2 );
	}
	
#define pop_token_FAIL( err ) err_interface( &stack_token_refid, (uintptr_t)1, 2, &dest, (err) ); return( -3 );
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc
			( lib4_stdmemfuncs.data, sizeof( token ) + ( sizeof( char ) * tmp.length ) );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, pop_token_FAIL )
	if( !a )
	{
		err_interface( &stack_token_refid, (uintptr_t)1, 3, &ptrres, &dest );
		return( -4 );
	}
	
	res = pop_block( stk,  ( (const char*)a ) + sizeof( token_head ), tmp.length );
		a->toktype = tmp.toktype;
		a->length = tmp.length;
	if( !res )
	{
		err_interface( &stack_token_refid, (uintptr_t)1, 4, &res, &a, &dest );
		return( -5 );
	}
	
	*dest = a;
	
	return( 1 );
}
