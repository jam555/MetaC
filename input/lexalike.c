#include "stdmonads.h"
#include "headers.h"

#include "../err/inner_err.h"



	/* This file actually shoves something inside "errs", so for the sake of */
	/*  reducing the code in language ifs, we require C++2020 support at the */
	/*  minimum. */
#if defined( __cplusplus ) && __cplusplus >= 202002L
	namespace
	{
		static msg_styleset errs;
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs;
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
	#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	#define NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )
	
	#define SIGNEDARG( integer ) STDMSG_SIGNEDARG_WRAPPER( &errs, integer )
	#define DECARG( uint ) STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
	#define HEXARG( hex ) STDMSG_HEXARG_WRAPPER( &errs, hex )
	#define CHARARG( chara ) STDMSG_CHARARG_WRAPPER( &errs, ( chara ) )
	#define DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define BADCHAR( funcname, objaddr, expectedval ) \
	STDMSG_BADCHAR_WRAPPER( &errs, ( funcname ), ( objaddr ), ( expectedval ) )
#define BADINT( funcname, objaddr, expectedval ) \
	STDMSG_BADINT_WRAPPER( &errs, ( funcname ), ( objaddr ), ( expectedval ) )
#define BADUINT( funcname, objaddr, expectedval ) \
	STDMSG_BADUINT_WRAPPER( &errs, ( funcname ), ( objaddr ), ( expectedval ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, msgstr )

#define I_OVERFLOW( funcname, objaddr, limit ) \
	STDMSG_I_OVERFLOW_WRAPPER( &errs, ( funcname ), ( objaddr ), ( limit ) )
#define OVERFLOW2( funcname, addr, val, limit ) \
	STDMSG_I_OVERFLOW_WRAPPER2( &errs, ( funcname ), ( addr ), ( val ), ( limit ) )


#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), lexalike_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )

#define STACKSWAP_UINT( stk, tmp1, tmp2,  caller, scratch ) \
	STACK_SWAP_UINT( ( stk ), ( tmp1 ), ( tmp2 ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )


#define CALL_FRAMEFUNC( stkpair, rethand, retval, callhand, callval,  caller, scratch ) \
	CALL_FRAMEFUNC( \
		( stkpair ), \
			( rethand ), ( retval ), ( callhand ), ( callval ), \
		&errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME \
	)
#define RETFRAMEFUNC( caller ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), res, lexalike_ENDRETFRAME )
#define RETFRAMEFUNC_ALT( stack,  caller, scratch ) \
	RET_FRAMEFUNC( ( stack ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )



/* The purpose of lexalike.c is to provide wrappers for charin(), isspace(), */
/*  etc., but which convert stuff to tokens so that there's an easier way to */
/*  support character delimiting, by moving it before token lexing and */
/*  assembly. */
/* Note that support for trigraphs belongs in this file. */
/* There's no deep reason for the still_freshline, lexentry, and */
/*  assemble_token stuff to be here, but it makes about as much sense as */
/*  sticking it elsewhere. */



uintptr_t still_freshline = 1;

int is_freshline()
{
	return( !!still_freshline );
}



framefunc lexentry = &head_lex;
intptr_t set_lexentry( lexentry lentry )
{
	intptr_t ret = 0;
	
	if( lentry )
	{
		ret = (intptr_t)lexentry;
		lexentry = lentry;
		
	} else {
		
		BADNULL( set_lexentry, &lentry );
	}
	
	return( ret );
}
retframe assemble_token( stackpair *stkp, void *v )
{
	token_head th;
	token *a = (token*)0;
	char b;
	framefunc hand = &end_run;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		FAILEDINTFUNC( "pop_tokenhead", assemble_token, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
		/* This particular bit of conditionals is so that stuff that wants */
		/*  to only have whitespace separating it from the preceding newline */
		/*  can detect that circumstance. */
	th.was_freshline = still_freshline;
	if( th.toktype == TOKTYPE_NEWLINE )
	{
		still_freshline = 1;
		th.was_freshline = 1;
		
	} else if
	(
		th.toktype == TOKTYPE_SPACE ||
		th.toktype == TOKTYPE_OCTO
	)
	{
		/* Leave still_freshline alone, no change to it. */
		
	} else {
		
		still_freshline = 0;
	}
	
	
#define assemble_token_DISCARD( val ) 
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc
			( lib4_stdmemfuncs.data, sizeof( token ) + ( sizeof( char ) * th.length ) );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, assemble_token_DISCARD )
	if( a )
	{
		a->header = th;
		
		while( th.length )
		{
			--th.length;
			
			res = pop_char( &( stkp->data ),  &b );
			if( !res )
			{
				FAILEDINTFUNC( "pop_char", assemble_token, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			a->text[ th.length ] = b;
		}
		
		res = push_uintptr( &( stkp->data ),  (uintptr_t)a );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", assemble_token, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
	} else {
		
		BADNULL( assemble_token, &a );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	/* Success. */
	
	RETFRAMEFUNC( assemble_token );
}
retframe dealloc_token( stackpair *stkp, void *v )
{
	uintptr_t a;
	
	int res = pop_uintptr( stk,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", dealloc_token, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	lib4_result res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, (void*)a );
#define dealloc_token_ONFAIL( err ) \
		MONADICFAILURE( dealloc_token, "lib4_stdmemfuncs.dealloc()", ( err ) ); \
		return( (retframe){ &end_run, (void*)0 } );
	LIB4_RESULT_BODYMATCH( res, LIB4_OP_SETa, dealloc_token_ONFAIL );
	
	RETFRAMEFUNC( dealloc_token );
}
retframe getANDassemble_token( stackpair *stkp, void *v )
{
	int tmp;
	
		/* Setup a return into assemble_token() to get the token material */
		/*  properly packaged, and then directly head into head_lex() to get */
		/*  that material itself. */
	CALL_FRAMEFUNC(
		stkp,
			&assemble_token, (void*)0, &head_lex, (void*)0,
		
		getANDassemble_token, tmp, lexalike_ENDRETFRAME
	);
}


#define token_queue_STACKSIZE 4
struct
{
	size_t used, shuffleused;
	FILE *f, *shuffle;
	
} token_queue;

int token_queue_init()
{
	token_queue.used = 0;
	token_queue.shuffleused = 0;
	
	token_queue.f = tmpfile();
	if( !token_queue.f )
	{
		BADNULL( token_queue_init, &( token_queue.f ) );
		return( -1 );
	}
	token_queue.shuffle = tmpfile();
	if( !token_queue.shuffle )
	{
		BADNULL( token_queue_init, &( token_queue.shuffle ) );
		fclose( token_queue.f );
		token_queue.f = (FILE*)0;
		return( -2 );
	}
	
	return( 1 );
}

size_t token_queue_used()
{
	return( token_queue.used );
}
int token_queue_push( token *tok )
{
	if( tok )
	{
			/* "&& 0" because we don't have the code here to actually find */
			/*  the maximum size of files. The "0" is where that would be */
			/*  paid attention to. */
		if( token_queue.used >= token_queue_STACKSIZE && 0 )
		{
			TRESPASSPATH( token_queue_push, "ERROR: token_queue_push encountered a used value larger than the stack." );
			return( -2 );
		}
		
		uintptr_t tmp = (uintptr_t)tok;
		
		int res = fseek( token_queue.f, token_queue.used * sizeof( uintptr_t ), SEEK_SET );
		if( res != 0 )
		{
			FAILEDINTFUNC( "fseek", token_queue_push, res );
			return( -3 );
		}
		res = fwrite( (const void*)&tmp, sizeof( uintptr_t ), 1, token_queue.f );
		if( !res )
		{
			FAILEDINTFUNC( "fwrite", token_queue_push, res );
			return( -4 );
		}
		++token_queue.used;
		
		return( 1 );
	}
	
	return( -1 );
}
int token_queue_pop( token **tok )
{
	if( tok )
	{
		if( *tok )
		{
			BADNULL( token_queue_pop, &tok );
			return( -2 );
		}
		if( !token_queue.used )
		{
			BADNULL( token_queue_pop, &( token_queue.used ) );
			return( -3 );
		}
		
		--token_queue.used;
		uintptr_t tmp;
		
		int res = fseek( token_queue.f, token_queue.used * sizeof( uintptr_t ), SEEK_SET );
		if( res != 0 )
		{
			FAILEDINTFUNC( "fseek", token_queue_pop, res );;
			return( -4 );
		}
		res = fread( (void*)&tmp, sizeof( uintptr_t ), 1, token_queue.f );
		if( !res )
		{
			FAILEDINTFUNC( "fread", token_queue_pop, res );
			return( -5 );
		}
		*tok = (token*)tmp;
		
		return( 1 );
	}
	
	return( -1 );
}
retframe token_queue_fetch( stackpair *stkp, void *v )
{
	if( token_queue.used )
	{
		token *tok;
		
		int res = token_queue_pop( &tok );
		if( !res )
		{
			FAILEDINTFUNC( "token_queue_pop", token_queue_fetch, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		res = push_uintptr( &( stkp->data ),  (uintptr_t)a );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", token_queue_fetch, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RETFRAMEFUNC( token_queue_fetch );
		
	} else {
		
		return( (retframe){ &getANDassemble_token, (void*)0 } );
	}
}

size_t token_queue_shuffleused()
{
	return( token_queue.shuffleused );
}
int token_queue_shufflepush( token *tok )
{
	if( tok )
	{
			/* "&& 0" because we don't have the code here to actually find */
			/*  the maximum size of files. */
		if( token_queue.shuffleused >= token_queue_STACKSIZE && 0 )
		{
			TRESPASSPATH( token_queue_shufflepush, "ERROR: token_queue_shufflepush encountered a used value larger than the stack." );
			return( -2 );
		}
		
		uintptr_t tmp = (uintptr_t)tok;
		
		int res = fseek( token_queue.shuffle, token_queue.shuffleused * sizeof( uintptr_t ), SEEK_SET );
		if( res != 0 )
		{
			FAILEDINTFUNC( "fseek", token_queue_shufflepush, res );
			return( -3 );
		}
		res = fwrite( (const void*)&tmp, sizeof( uintptr_t ), 1, token_queue.shuffle );
		if( !res )
		{
			FAILEDINTFUNC( "fwrite", token_queue_shufflepush, res );
			return( -4 );
		}
		++token_queue.shuffleused;
		
		return( 1 );
	}
	
	return( -1 );
}
int token_queue_shufflepop( token **tok )
{
	if( tok )
	{
		if( *tok )
		{
			BADNULL( token_queue_shufflepop, &tok );
			return( -2 );
		}
		if( !token_queue.shuffleused )
		{
			BADNULL( token_queue_shufflepop, &( token_queue.shuffleused ) );
			return( -3 );
		}
		
		--token_queue.shuffleused;
		uintptr_t tmp;
		
		int res = fseek( token_queue.shuffle, token_queue.shuffleused * sizeof( uintptr_t ), SEEK_SET );
		if( res != 0 )
		{
			FAILEDINTFUNC( "fseek", token_queue_shufflepop, res );
			return( -4 );
		}
		res = fread( (void*)&tmp, sizeof( uintptr_t ), 1, token_queue.shuffle );
		if( !res )
		{
			FAILEDINTFUNC( "fread", token_queue_shufflepop, res );
			return( -5 );
		}
		*tok = (token*)tmp;
		
		return( 1 );
	}
	
	return( -1 );
}

int token_queue_shuffle2queue()
{
	if
	(
		!token_queue.shuffleused ||
			/* "&& 0" because we don't have the code here to actually find */
			/*  the maximum size of files. */
		( token_queue.used >= token_queue_STACKSIZE && 0 )
	)
	{
		TRESPASSPATH( token_queue_shuffle2queue, "ERROR: token_queue_shuffle2queue encountered a bounds error." );
		return( -2 );
	}
	
	token *tmp;
	
	int res = token_queue_shufflepop( &tmp );
	if( !res )
	{
		FAILEDINTFUNC( "token_queue_shufflepop", token_queue_shuffle2queue, res );
		return( -3 );
	}
	res = token_queue_push( tmp );
	if( ! )
	{
		FAILEDINTFUNC( "token_queue_push", token_queue_shuffle2queue, res );
		return( -4 );
	}
	
	return( 1 );
}

	/* Note: this DOES NOT attempt to deallocate any tokens, just the file. */
	/*  If you actually care, then empty out the file first. */
int token_queue_deinit()
{
	int res = fclose( token_queue.shuffle );
	if( res == EOF )
	{
		FAILEDINTFUNC( "fclose", token_queue_deinit, res );
		return( -2 );
	}
	token_queue.shuffle = (FILE*)0;
	
	res = fclose( token_queue.f );
	if( res != EOF )
	{
		FAILEDINTFUNC( "fclose", token_queue_deinit, res );
		return( -3 );
	}
	token_queue.f = (FILE*)0;
	
	return( 1 );
}



int is_bslash( int c )
{
	if( c == '\\' )
	{
		return( 1 );
	}
	
	return( 0 );
}

int tokenize_char__accumulate( stackpair *stkp, void *v,  token_head *th, char *a_, char *b_ )
{
	if( stkp &&  th && a_ && b_ )
	{
		char_result res;
		int res2;
		char a = *a_, b = *b_;
		
#define tokenize_char__accumulate_FETCHFAIL( err ) \
	MONADICFAILURE( tokenize_char__accumulate, "charin", err ); \
	return( (retframe){ &end_run, (void*)0 } );
	
		switch( a )
		{
			case '\\':
				/* Fall-through. */
			case 'a': /* Alert. */
				res = charin();
				CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				
				break;
				
			case 'b': /* Binary. */
				res = charin();
				CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				while( b == '0' || b == '1' )
				{
					res2 = push_char( &( stkp->data ),  b );
					if( !res2 )
					{
						FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
						return( (retframe){ &end_run, (void*)0 } );
					}
					th->length += 1;
					
					res = charin();
					CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				}
				
				break;
			case 'd': /* Decimal. */
				res = charin();
				CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				while( isdigit( b ) )
				{
					res2 = push_char( &( stkp->data ),  b );
					if( !res2 )
					{
						FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
						return( (retframe){ &end_run, (void*)0 } );
					}
					th->length += 1;
					
					res = charin();
					CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				}
				
				break;
			case 'o': /* Octal. */
				res = charin();
				CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				while( isdigit( b ) && b != '8' && b != '9' )
				{
					res2 = push_char( &( stkp->data ),  b );
					if( !res2 )
					{
						FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
						return( (retframe){ &end_run, (void*)0 } );
					}
					th->length += 1;
					
					res = charin();
					CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				}
				
				break;
			case 'u':
				/* Unicode, or other "universal" numeric encoding of characters. */
				/*  Other than the u/x character, the encoding is identical to */
				/*  hexadecimal. */
				
				/* Fall-through. */
			case 'x': /* Hexadecimal. */
				res = charin();
				CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				while( isxdigit( b ) )
				{
					res2 = push_char( &( stkp->data ),  b );
					if( !res2 )
					{
						FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
						return( (retframe){ &end_run, (void*)0 } );
					}
					th->length += 1;
					
					res = charin();
					CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char__accumulate_FETCHFAIL )
				}
				
				break;
				
			default:
				b = a;
				th->length = 1;
				
				break;
		}
		res2 = isspace( b );
		if( !res2 )
		{
			FAILEDINTFUNC( "isspace", tokenize_char__accumulate, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		*a_ = a;
		*b_ = b;
		return( 1 );
	}
	
	TRESPASSPATH( tokenize_char__accumulate, "ERROR: tokenize_char__accumulate unexpectedly exited through it's end." );
	return( -1 );
}
	/* TODO: This comment was pulled from headers.h. and reflects the */
	/*  function before it was modified to be called directly instead of via */
	/*  the ITC loop. It needs to be updated. */
	/* Another that's in lexalike.c, and roughly the whole reason for the */
	/*  file: this handles backslashes, including allowing them to "hide" */
	/*  newlines, and encode for number-encoded characters. The character */
	/*  that it "detects" gets put onto the stackpair data stack, currently */
	/*  as a char. */
int tokenize_char( stackpair *stkp, void *v )
{
	token_head th = (token_head){ /* type */TOKTYPE_INVALID, 1, 0, 0 };
	char_result res = charin();
	int res2;
	char a, b;
	unsigned char c;
	
#define tokenize_char_FETCHFAIL( err ) \
		MONADICFAILURE( tokenize_char, "charin", err ); \
		return( (retframe){ &end_run, (void*)0 } );
	
	CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETa, tokenize_char_FETCHFAIL )
	
	if( is_bslash( a ) )
	{
		th.is_delimited = 1;
		
		res = charin();
		CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETa, tokenize_char_FETCHFAIL )
		
		th.length = 0;
		res2 = tokenize_char__accumulate( stkp, v,  &th, &a, &b );
		switch( res2 )
		{
			case 1:
					/* Success. */
				break;
			case -1:
					/* Null argument(s). */
				FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
					NOTELINE(); DATAPTR( &th );
					NOTESPACE(); DATAPTR( &a );
					NOTESPACE(); DATAPTR( &b );
				return( (retframe){ &end_run, (void*)0 } );
			case -2:
					/* Internal error. */
				FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
					NOTELINE(); DATAPTR( &th );
					NOTESPACE(); DATAPTR( &a );
					NOTESPACE(); DATAPTR( &b );
				return( (retframe){ &end_run, (void*)0 } );
			default:
					/* Unforeseen error, verify passable returns. */
				FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
					NOTELINE(); DATAPTR( &th );
					NOTESPACE(); DATAPTR( &a );
					NOTESPACE(); DATAPTR( &b );
				return( (retframe){ &end_run, (void*)0 } );
		}
		
			/* Let's address the whitespace in the room... We essentially try to */
			/*  handle all varieties of ASCII whitespace here, including */
			/*  line-ending variations. */
		if( b == 10 ) /* Ascii line feed. */
		{
			res = charin();
			CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char_FETCHFAIL )
			
			if( b != 13 )
			{
					/* Not followed by carriage return, so shove it back. */
				res2 = charback( b );
				if( !res2 )
				{
					FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
					return( (retframe){ &end_run, (void*)0 } );
				}
			}
			
				/* Restore to newline. */
			b = 10;
			
		} else if( b == 13 ) /* Ascii carriage return. */
		{
			res = charin();
			CHAR_RESULT_BODYMATCH( res, LIB4_OP_SETb, tokenize_char_FETCHFAIL )
			
			if( b != 10 )
			{
					/* Not followed by newline, so NOT DEFINED! */
				BADUINT( tokenize_char, &b, 10 );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
				/* Set to newline. */
			b = 10;
			
		} else if( !( b == 9 || b == 11 || b == 12 || b == 32 ) )
		{
			BADUINT( tokenize_char, &b, 9 );
				NOTESPACE(); DECARG( 11 );
				NOTESPACE(); DECARG( 12 );
				NOTESPACE(); DECARG( 32 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		switch( a )
		{
			case 'a':
					/* "Alert". We're changing to the ASCII "bell" character */
					/*  for this. */
				a = 7;
				
				th.length = 1;
				res2 = push_char( &( stkp->data ),  a );
				if( !res2 )
				{
					FAILEDINTFUNC( "push_char", tokenize_char, res2 );
					return( (retframe){ &end_run, (void*)0 } );
				}
				
				break;
				
			case 'b':
			case 'd':
			case 'o':
			case 'u':
			case 'x':
				size_t len = 0;
				lib4_intresult (*conv)( char c );
				lib4_intresult tmp;
				intmax_t acc = 0, step = 0, mul = 1;
				
				switch( a )
				{
					case 'b':
						conv = &bin2num;
						step = 2;
						break;
					case 'o':
						conv = &oct2num;
						step = 8;
						break;
					case 'd':
						conv = &dec2num;
						step = 10;
						break;
					case 'u':
					case 'x':
						conv = &hexa2num;
						step = 16;
						break;
					default:
							/* Throw an error! */
						BADCHAR( tokenize_char, &a, 'b' );
							NOTESPACE(); CHARARG( 'o' );
							NOTESPACE(); CHARARG( 'd' );
							NOTESPACE(); CHARARG( 'u' );
							NOTESPACE(); CHARARG( 'x' );
						return( (retframe){ &end_run, (void*)0 } );
				}
				
				while( len < th.length )
				{
					res2 = pop_char( &( stkp->data ),  &b );
					if( !res2 )
					{
							/* Throw error. */
						FAILEDINTFUNC( "pop_char", tokenize_char, res2 );
						return( (retframe){ &end_run, (void*)0 } );
					}
					
					tmp = conv( b );
					LIB4_INTRESULT_BODYMATCH( tmp, LIB4_OP_SETb, tokenize_char_FETCHFAIL );
					
						/* Would be desirable to ensure that acc doesn't */
						/*  overflow, but I'll leave that out for now. */
					acc += b * mul;
					len += 1;
					mul *= step;
				}
				
				if( a == 'u' )
				{
						/* Do nothing! We're just going to assume that the */
						/*  "universal encoding" is whatever the current */
						/*  encoding is. It's usually going to be Unicode */
						/*  regardless. */
					;
				}
				
					/* The only place that checks the output character for */
					/*  bounding... */
				if( acc > UCHAR_MAX )
				{
						/* Just report, don't break, we don't really care when */
						/*  the user does stupid shit so long as the user MIGHT */
						/*  get informed. */
						/* "Might" get informed because that's somewhere else's */
						/*  job. */
					I_OVERFLOW( tokenize_char, &acc, UCHAR_MAX );
				}
				c = acc;
				th.length = 1;
				res2 = push_char( &( stkp->data ),  *( (char*)&c ) );
				if( !res2 )
				{
					FAILEDINTFUNC( "push_char", tokenize_char, res2 );
					return( (retframe){ &end_run, (void*)0 } );
				}
				
				break;
				
			default: /* The whitespace stuff. */
				res2 = push_char( &( stkp->data ),  b );
				if( !res2 )
				{
					FAILEDINTFUNC( "push_char", tokenize_char, res2 );
					return( (retframe){ &end_run, (void*)0 } );
				}
				
				break;
		}
		
	} else {
		
		th.length = 1;
		res2 = push_char( &( stkp->data ),  a );
		if( !res2 )
		{
			FAILEDINTFUNC( "push_char", tokenize_char, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
	res2 = push_tokenhead( &( stkp->data ),  th );
	if( !res2 )
	{
		FAILEDINTFUNC( "push_tokenhead", tokenize_char, res2 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( 1 );
}

	/* A helper function to handle most of the conversion between normal */
	/*  character operations & stackpair operations. Unlike some stuff in */
	/*  this file, this should pretty much be complete. */
msg_style stack_testchar_multicharforbidden =
	{
		"ERROR: stack_testchar( %p, ignored, %p ) encountered a multichar when not enabled."
	};
lib4_result stack_testchar( stackpair *stkp, void *v,  int (*testfunc)( int ),  int fail_on_multichar )
{
	if( stkp && testfunc )
	{
		token_head th;
		int res;
		char c;
		
		res = pop_tokenhead( &( stkp->data ),  &th );
		if( !res )
		{
			FAILEDINTFUNC( "pop_tokenhead", stack_testchar, res );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
		if( th.length <= 0 )
		{
			BADNULL( stack_testchar, &( th.length ) );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_BELOWBOUNDS );
		}
		if( th.length > 1 && fail_on_multichar )
		{
			msg_interface( &errs, 1, (void*)stkp, (uintmax_t)&( funcname ) );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_ABOVEBOUNDS );
		}
		
		
		
		res = peek_char( &( stkp->data ),  0,  &c );
		if( !res )
		{
			FAILEDINTFUNC( "peek_char", stack_testchar, res );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		res = push_tokenhead( &( stkp->data ),  th );
		if( !res )
		{
			FAILEDINTFUNC( "push_tokenhead", stack_testchar, res );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
		
		
		LIB4_RESULT_RETURNSUCCESS( testfunc( c ) );
	}
	
		/* We REQUIRE at least stkp and testfunc. */
	LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_DOMAIN );
}
retframe stack_testchar2( stackpair *stkp, void *v,  int (*testfunc)( int ), char *funcname )
{
	if( stkp )
	{
		lib4_result res = stack_testchar( stkp, v,  int (*testfunc)( int ),  0 );
		lib4_success_result a;
		
#define stack_testchar2__TESTFAIL( err ) \
	MONADICFAILURE( stack_testchar2, "stack_testchar", ( err ) ); \
	NOTELINE(); DATAPTR( stkp ); \
	NOTESPACE(); DATAPTR( v ); \
	NOTESPACE(); HEXARG( (uintptr_t)test_func ); \
	NOTESPACE(); DECARG( 0 );
		LIB4_RESULT_BODYMATCH( res, LIB4_OP_SETa, stack_testchar2__TESTFAIL )
		
		int res2 = push_int( &( stkp->data ),  a.val );
		if( !res2 )
		{
			FAILEDINTFUNC( "push_int", stack_testchar2, res2 );
				NOTELINE(); DATAPTR( &( stkp->data ) );
				NOTESPACE(); SIGNEDARG( a.val );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RETFRAMEFUNC( stack_testchar2 );
	}
	
	BADNULL2( stack_testchar2, &stkp, &v );
	return( (retframe){ &end_run, (void*)0 } );
}


/* None of these pop anything (technicaly they do, but then put it back). */

retframe stack_isalnum( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isalnum, "isalnum" ) );
}
retframe stack_isalpha( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isalpha, "isalpha" ) );
}
retframe stack_isblank( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isblank, "isblank" ) );
}
retframe stack_iscntrl( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &iscntrl, "iscntrl" ) );
}
retframe stack_isdigit( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isdigit, "isdigit" ) );
}
retframe stack_isgraph( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isgraph, "isgraph" ) );
}
retframe stack_islower( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &islower, "islower" ) );
}
retframe stack_isprint( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isprint, "isprint" ) );
}
retframe stack_ispunct( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &ispunct, "ispunct" ) );
}
retframe stack_isspace( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isspace, "isspace" ) );
}
retframe stack_isupper( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isupper, "isupper" ) );
}
retframe stack_isxdigit( stackpair *stkp, void *v )
{
	return( stack_testchar2( stkp, v,  &isxdigit, "isxdigit" ) );
}



retframe token_touint_binary( stackpair *stkp, void *v )
{
	static const uintptr_t maxv_lastbin = ( ( UINTPTR_MAX ) / 2 ) + 1;
	
	uintptr_t a;
	token_uint *tu;
	token *src;
	int scratch, b;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint_binary, scratch );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		BADUINT( token_touint_binary, &( tu->header.toktype ), TOKTYPE_INVALID );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint_binary, scratch );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		BADUINT( token_touint_binary, &( src->header.toktype ), TOKTYPE_NUMBER );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_binary_FAIL( err ) \
	MONADICFAILURE( token_touint_binary, "bin2um", err ); \
	return( (retframe){ &end_run, (void*)0 } );
		res = bin2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_binary_FAIL );
		if( b < 0 || b > 1 )
		{
			/* Range error! */
			
			TRESPASSPATH( token_touint_binary, "Error: expected a number from 0 to 1, instead received: " );
				SIGNEDARG( b );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lastbin )
		{
			OVERFLOW( token_touint_binary, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
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
	STACKSWAP_UINT( stk, tmp1, tmp2,  token_touint_binary, scratch );
		/* ... and delete the old one. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}
retframe token_touint_octal( stackpair *stkp, void *v )
{
	static const uintptr_t maxv_lastoct = ( ( UINTPTR_MAX ) / 8 ) + 1;
	
	uintptr_t a;
	token_uint *tu;
	token *src;
	int scratch, b;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint_octal, scratch );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		BADUINT( token_touint_octal, &( tu->header.toktype ), TOKTYPE_INVALID );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint_octal, scratch );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		BADUINT( token_touint_octal, &( src->header.toktype ), TOKTYPE_NUMBER );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_octal_FAIL( val ) \
	MONADICFAILURE( token_touint_octal, "oct2um", err ); \
	return( (retframe){ &end_run, (void*)0 } );
		res = oct2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_octal_FAIL );
		if( b < 0 || b > 7 )
		{
			/* Range error! */
			
			TRESPASSPATH( token_touint_octal, "Error: expected a number from 0 to 8, instead received: " );
				SIGNEDARG( b );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lastoct )
		{
			OVERFLOW( token_touint_octal, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
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
	STACKSWAP_UINT( stk, tmp1, tmp2,  token_touint_octal, scratch );
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
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint_decimal, scratch );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		BADUINT( token_touint_decimal, &( tu->header.toktype ), TOKTYPE_INVALID );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint_decimal, scratch );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		BADUINT( token_touint_decimal, &( src->header.toktype ), TOKTYPE_NUMBER );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_decimal_FAIL( val ) \
	MONADICFAILURE( token_touint_decimal, "dec2um", err ); \
	return( (retframe){ &end_run, (void*)0 } );
		res = dec2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_decimal_FAIL );
		if( b < 0 || b > 9 )
		{
			/* Range error! */
			
			TRESPASSPATH( token_touint_decimal, "Error: expected a number from 0 to 9, instead received: " );
				SIGNEDARG( b );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lastdec )
		{
			OVERFLOW( token_touint_decimal, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
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
	STACKSWAP_UINT( stk, tmp1, tmp2,  token_touint_decimal, scratch );
		/* ... and delete the old one. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}
retframe token_touint_hexadecimal( stackpair *stkp, void *v )
{
	static const uintptr_t maxv_lasthex = ( ( UINTPTR_MAX ) / 16 ) + 1;
	
	uintptr_t a;
	token_uint *tu;
	token *src;
	int scratch, b;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint_hexadecimal, scratch );
	tu = (*)a;
	if( tu->header.toktype != TOKTYPE_INVALID )
	{
		BADUINT( token_touint_hexadecimal, &( tu->header.toktype ), TOKTYPE_INVALID );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), &a,  token_touint_hexadecimal, scratch );
	src = (token*)a;
	if( src->header.toktype != TOKTYPE_NUMBER )
	{
		BADUINT( token_touint_hexadecimal, &( src->header.toktype ), TOKTYPE_NUMBER );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	lib4_intresult res;
	scratch = tu->length;
	while( scratch )
	{
#define token_touint_hexadecimal_FAIL( val ) \
	MONADICFAILURE( token_touint_hexadecimal, "hexa2um", err ); \
	return( (retframe){ &end_run, (void*)0 } );
		res = hexa2num( src->text[ tu->length - scratch ] );
		LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, token_touint_hexadecimal_FAIL );
		if( b < 0 || b > 15 )
		{
			/* Range error! */
			
			TRESPASSPATH( token_touint_hexadecimal, "Error: expected a number from 0 to F, instead received: " );
				SIGNEDARG( b );
			
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		if( tu->val >= maxv_lasthex )
		{
			OVERFLOW( token_touint_hexadecimal, &( tu->val ), ( tu->val ), UINTPTR_MAX );
			
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
	STACKSWAP_UINT( stk, tmp1, tmp2,  token_touint_hexadecimal, scratch );
		/* ... and delete the old one. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}


retframe token_touint( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  token_touint );
	
	uintptr_t a;
	int scratch;
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  token_touint, scratch );
		/* STACKPEEK_* already checks for null "a" values. */
	if( ( (token_head*)a )->toktype != TOKTYPE_NUMBER )
	{
		if( ( (token_head*)a )->toktype == TOKTYPE_NUMBER_UINT )
		{
			/* So, looks like we were handed what we're supposed to build, */
			/*  so let's just pretend that we DID build it... */
			
			RETFRAMEFUNC_ALT( stkp,  token_touint, scratch );
		}
		
		BADUINT( token_touint, &( ( (token_head*)a )->toktype ), TOKTYPE_NUMBER );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	token *src = (token*)a;
	
	token_uint *b;
	size_t size = sizeof( token_uint ) + ( sizeof( char ) * src->length );
#define token_touint_ALLOCFAIL( err ) \
		MONADICFAILURE( token_touint, "lib4_stdmemfuncs.alloc", ( err ) ); \
			NOTELINE(); DATAPTR( lib4_stdmemfuncs.data ); \
			NOTESPACE(); DECARG( size ); \
		return( (retframe){ &end_run, (void*)0 } );
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc( lib4_stdmemfuncs.data, size );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETb, token_touint_ALLOCFAIL )
	if( !b )
	{
		BADNULL( token_touint, &b );
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	b->header = src->header;
			/* Temporarily mark "b" as an invalid token. */
		b->header.toktype = TOKTYPE_INVALID;
	b->val = 0;
	
	if( b->length < 1 )
	{
		TRESPASSPATH( token_touint, "Error: the token length was listed as null." );
		
		return( (retframe){ &end_run, (void*)0 } );
		
	} else if( b->length == 1 )
	{
		b->text[ 0 ] = src->text[ 0 ];
	}
	if( b->length < 2 && src->text[ 0 ] == '0' )
	{
		b->header.toktype = TOKTYPE_NUMBER_UINT;
		
			/* Shove the new token under the old one... */
		STACKPOP_UINT( &( stkp->data ), &a,  token_touint, scratch );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)b,  token_touint, scratch );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)src,  token_touint, scratch );
			/* ... and delete the old one. */
		return( (retframe){ &invoke_dealloctoken, (void*)0 } );
	}
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)b,  token_touint, scratch );
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
				TRESPASSPATH( token_touint, "Error: encountered an unexpected character as the second in the token text: " );
					CHARARG( src->text[ 1 ] );
				
				return( (retframe){ &end_run, (void*)0 } );
		}
		
	} else {
		
			/* Go to the decimal parser. */
		return( (retframe){ &token_touint_decimal, (void*)0 } );
	}
	
	TRESPASSPATH( token_touint, "Error: function exited through it's end." );
	
	return( (retframe){ &end_run, (void*)0 } );
}



	/* For "native" compatibility in both C and C++, DO NOT use compound */
	/*  literals. Designated initializers with this specific syntax are C as */
	/*  of C99, and C++ as of C++20; thus, it's the best syntax for this. */
static msg_piece msgs[] =
	{
		{ .style = &stack_testchar_multicharforbidden }, 0
	};

#if defined( __cplusplus ) && __cplusplus >= 202002L
	namespace
	{
		msg_styleset errs =
			{
				msgs,
				sizeof( msgs ) / sizeof( msgs[ 0 ] )
			};
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs =
		(msg_styleset)
		{
			msgs,
			sizeof( msgs ) / sizeof( msgs[ 0 ] )
		};
#endif
