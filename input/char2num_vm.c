#include <stdint.h>
#include <limits.h>

#include "stdmonads.h"

#include "../headers.h"

#include "char2num.h"
#include "char2num_vm.h"

#include "../tokenization/token.h"

#include "../err/inner_err.h"



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


#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), ( endfunc ) )

#define STACKSWAP_UINT( stk, tmp1, tmp2,  caller, scratch, endfunc ) \
	STACK_SWAP_UINT( ( stk ), ( tmp1 ), ( tmp2 ),  &errs, ( caller ), ( scratch ), ( endfunc ) )


#define OVERFLOW2( funcname, addr, val, limit ) \
	STDMSG_I_OVERFLOW_WRAPPER2( &errs, ( funcname ), ( addr ), ( val ), ( limit ) )


#define RETFRAMEFUNC( stkpair,  caller ) \
	RET_FRAMEFUNC( ( stkpair ),  &errs, ( caller ), res, stack_ENDRETFRAME )


#define char2num_ENDRETFRAME( ... ) return( (retframe){ &end_run, (void*)0 } )



retframe token_touint_binary( stackpair *stkp, void *v )
{
	static const uintptr_t maxv_lastdec = ( ( UINTPTR_MAX ) / 10 ) + 1;
	
	uintptr_t a;
	token_uint *tu;
	token *src;
	int scratch, b;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint, scratch, char2num_ENDRETFRAME );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint, scratch, char2num_ENDRETFRAME );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_decimal_FAIL( val ) \
	???(); \
	return( (retframe){ &end_run, (void*)0 } );
		res = bin2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_decimal_FAIL );
		if( b < 0 || b > 9 )
		{
			??? /* Error! */
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lastdec )
		{
			OVERFLOW( token_touint_, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		tu->val *= 10;
		tu->val += b;
		
		tu->text[ tu->length - scratch ] = src->text[ tu->length - scratch ];
		
		--scratch;
	}
	
	
		/* Remember, it's been marked as invalid. */
	tu->header.toktype = TOKTYPE_NUMBER_UINT;
		/* Move the new token under the old one... */
	STACKSWAP_UINT( stk, tmp1, tmp2,  token_touint_binary, scratch, char2num_ENDRETFRAME );
		/* ... and delete the old one. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}
retframe token_touint_octal( stackpair *stkp, void *v )
{
	static const uintptr_t maxv_lastdec = ( ( UINTPTR_MAX ) / 10 ) + 1;
	
	uintptr_t a;
	token_uint *tu;
	token *src;
	int scratch, b;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint, scratch, char2num_ENDRETFRAME );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint, scratch, char2num_ENDRETFRAME );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_decimal_FAIL( val ) \
	???(); \
	return( (retframe){ &end_run, (void*)0 } );
		res = oct2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_decimal_FAIL );
		if( b < 0 || b > 9 )
		{
			??? /* Error! */
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lastdec )
		{
			OVERFLOW( token_touint_, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		tu->val *= 10;
		tu->val += b;
		
		tu->text[ tu->length - scratch ] = src->text[ tu->length - scratch ];
		
		--scratch;
	}
	
	
		/* Remember, it's been marked as invalid. */
	tu->header.toktype = TOKTYPE_NUMBER_UINT;
		/* Move the new token under the old one... */
	STACKSWAP_UINT( stk, tmp1, tmp2,  token_touint_binary, scratch, char2num_ENDRETFRAME );
		/* ... and delete the old one. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}
retframe token_touint_decimal( stackpair *stkp, void *v )
{
	static const uintptr_t maxv_lastdec = ( ( UINTPTR_MAX ) / 10 ) + 1;
	
	uintptr_t a;
	token_uint *tu;
	token *src;
	int scratch, b;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint, scratch, char2num_ENDRETFRAME );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint, scratch, char2num_ENDRETFRAME );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_decimal_FAIL( val ) \
	???(); \
	return( (retframe){ &end_run, (void*)0 } );
		res = dec2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_decimal_FAIL );
		if( b < 0 || b > 9 )
		{
			??? /* Error! */
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lastdec )
		{
			OVERFLOW( token_touint_, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		tu->val *= 10;
		tu->val += b;
		
		tu->text[ tu->length - scratch ] = src->text[ tu->length - scratch ];
		
		--scratch;
	}
	
	
		/* Remember, it's been marked as invalid. */
	tu->header.toktype = TOKTYPE_NUMBER_UINT;
		/* Move the new token under the old one... */
	STACKSWAP_UINT( stk, tmp1, tmp2,  caller, scratch, char2num_ENDRETFRAME );
		/* ... and delete the old one. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}
retframe token_touint_hexadecimal( stackpair *stkp, void *v )
{
	static const uintptr_t maxv_lastdec = ( ( UINTPTR_MAX ) / 10 ) + 1;
	
	uintptr_t a;
	token_uint *tu;
	token *src;
	int scratch, b;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint, scratch, char2num_ENDRETFRAME );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint, scratch, char2num_ENDRETFRAME );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_decimal_FAIL( val ) \
	???(); \
	return( (retframe){ &end_run, (void*)0 } );
		res = hexa2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_decimal_FAIL );
		if( b < 0 || b > 9 )
		{
			??? /* Error! */
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lastdec )
		{
			OVERFLOW( token_touint_, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		tu->val *= 10;
		tu->val += b;
		
		tu->text[ tu->length - scratch ] = src->text[ tu->length - scratch ];
		
		--scratch;
	}
	
	
		/* Remember, it's been marked as invalid. */
	tu->header.toktype = TOKTYPE_NUMBER_UINT;
		/* Move the new token under the old one... */
	STACKSWAP_UINT( stk, tmp1, tmp2,  token_touint_binary, scratch, char2num_ENDRETFRAME );
		/* ... and delete the old one. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}


retframe token_touint( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  token_touint, char2num_ENDRETFRAME );
	
	uintptr_t a;
	int scratch;
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint, scratch, char2num_ENDRETFRAME );
		/* STACKPEEK_* already checks for null "a" values. */
	if( ( (token_head*)a )->toktype != TOKTYPE_NUMBER )
	{
		if( ( (token_head*)a )->toktype == TOKTYPE_NUMBER_UINT )
		{
			/* So, looks like we were handed what we're supposed to build, */
			/*  so let's just pretend that we DID build it... */
			
			RETFRAMEFUNC( stkp,  token_touint );
		}
		
		??? /* Error! */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	token *src = (token*)a;
	
	token_uint *b;
#define token_touint_DISCARD( val )
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc
			( lib4_stdmemfuncs.data, sizeof( token_uint ) + ( sizeof( char ) * src->length ) );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETb, token_touint_DISCARD )
	if( b )
	{
		??? /* Error. */
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	b->header = src->header;
			/* Temporarily mark "b" as an invalid token. */
		b->header.toktype = TOKTYPE_INVALID;
	b->val = 0;
	
	if( b->length < 1 )
	{
		??? /* Trespass path: there wasn't any characters in the number. */
	} else if( b->length == 1 )
	{
		b->text[ 0 ] = src->text[ 0 ];
	}
	if( b->length < 2 && src->text[ 0 ] == '0' )
	{
		b->header.toktype = TOKTYPE_NUMBER_UINT;
		
			/* Shove the new token under the old one... */
		STACKPOP_UINT( &( stkp->data ), &a,  token_touint, scratch, char2num_ENDRETFRAME );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)b,  token_touint, scratch, char2num_ENDRETFRAME );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)src,  token_touint, scratch, char2num_ENDRETFRAME );
			/* ... and delete the old one. */
		return( (retframe){ &invoke_dealloctoken, (void*)0 } ); ???
	}
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)b,  token_touint, scratch, char2num_ENDRETFRAME );
	if( src->text[ 0 ] == '0' )
	{
			/* We already know there's AT LEAST two characters. */
		switch( src->text[ 1 ] )
		{
			case 'x':
			case 'X':
					/* Hexadecimal. */
				return( (retframe){ &token_touint_hexadecimal, (void*)0 } );
				
			case 'b':
			case 'B':
					/* Binary. */
				return( (retframe){ &token_touint_binary, (void*)0 } );
				
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
					/* Octal. */
				return( (retframe){ &token_touint_octal, (void*)0 } );
				
			default:
				??? /* What??? Error! */
				
				return( (retframe){ &end_run, (void*)0 } );
		}
		
	} else {
		
			/* Go to the decimal parser. */
		return( (retframe){ &token_touint_decimal, (void*)0 } );
	}
	
	??? /* Trespass path. */
	
	return( (retframe){ &end_run, (void*)0 } );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
