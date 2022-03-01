#include "stdmonads.h"
#include "headers.h"

#include "../err/inner_err.h"



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


#define STACK_BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define STACK_BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define STACK_MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
	#define STACK_NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	#define STACK_NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )
	
	#define STACK_SIGNEDARG( integer ) STDMSG_SIGNEDARG_WRAPPER( &errs, integer )
	#define STACK_DECARG( uint ) STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
	#define STACK_HEXARG( hex ) STDMSG_HEXARG_WRAPPER( &errs, hex )
	#define STACK_CHARARG( chara ) STDMSG_CHARARG_WRAPPER( &errs, ( chara ) )
	#define STACK_DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

#define STACK_FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define STACK_BADCHAR( funcname, objaddr, expectedval ) \
	STDMSG_BADCHAR_WRAPPER( &errs, ( funcname ), ( objaddr ), ( expectedval ) )
#define STACK_BADINT( funcname, objaddr, expectedval ) \
	STDMSG_BADINT_WRAPPER( &errs, ( funcname ), ( objaddr ), ( expectedval ) )
#define STACK_BADUINT( funcname, objaddr, expectedval ) \
	STDMSG_BADUINT_WRAPPER( &errs, ( funcname ), ( objaddr ), ( expectedval ) )

#define STACK_I_OVERFLOW( funcname, objaddr, limit ) \
	STDMSG_I_OVERFLOW_WRAPPER( &errs, ( funcname ), ( objaddr ), ( limit ) )


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
		
		STACK_BADNULL( set_lexentry, &lentry );
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
		STACK_FAILEDINTFUNC( "pop_tokenhead", assemble_token, res );
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
				STACK_FAILEDINTFUNC( "pop_char", assemble_token, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			a->text[ th.length ] = b;
		}
		
		res = push_uintptr( &( stkp->data ),  (uintptr_t)a );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "pop_char", assemble_token, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
	} else {
		
		STACK_BADNULL( assemble_token, &a );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	/* Success. */
	
	RET_FRAMEFUNC( assemble_token_refid, 2, -6, 0 );
}
retframe dealloc_token( stackpair *stkp, void *v )
{
	uintptr_t a;
	
	int res = pop_uintptr( stk,  &a );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_uintptr", dealloc_token, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	lib4_result res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, (void*)a );
#define dealloc_token_ONFAIL( err ) \
		STACK_MONADICFAILURE( dealloc_token, "lib4_stdmemfuncs.dealloc()", ( err ) ); \
		return( (retframe){ &end_run, (void*)0 } );
	LIB4_RESULT_BODYMATCH( res, LIB4_OP_SETa, dealloc_token_ONFAIL );
	
	RET_FRAMEFUNC( assemble_token_refid, 3, -4,  stkp, v );
}
retframe getANDassemble_token( stackpair *stkp, void *v )
{
		/* Setup a return into assemble_token() to get the token material */
		/*  properly packaged, and then directly head into head_lex() to get */
		/*  that material itself. */
	CALL_FRAMEFUNC( &assemble_token, 0, lexentry, 0 );
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
		return( -1 );
	}
	token_queue.shuffle = tmpfile();
	if( !token_queue.shuffle )
	{
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
			/*  the maximum size of files. */
		if( token_queue.used >= token_queue_STACKSIZE && 0 )
		{
			return( -2 );
		}
		
		uintptr_t tmp = (uintptr_t)tok;
		
		if( fseek( token_queue.f, token_queue.used * sizeof( uintptr_t ), SEEK_SET ) != 0 )
		{
			return( -3 );
		}
		if( !fwrite( (const void*)&tmp, sizeof( uintptr_t ), 1, token_queue.f ) )
		{
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
			return( -2 );
		}
		if( !token_queue.used )
		{
			return( -3 );
		}
		
		--token_queue.used;
		uintptr_t tmp;
		
		if( fseek( token_queue.f, token_queue.used * sizeof( uintptr_t ), SEEK_SET ) != 0 )
		{
			return( -4 );
		}
		if( !fread( (void*)&tmp, sizeof( uintptr_t ), 1, token_queue.f ) )
		{
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
			STACK_FAILEDINTFUNC( "token_queue_pop", token_queue_fetch, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		res = push_uintptr( &( stkp->data ),  (uintptr_t)a );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "push_uintptr", token_queue_fetch, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RET_FRAMEFUNC( token_queue_refid, 5, -3, 0 );
		
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
			return( -2 );
		}
		
		uintptr_t tmp = (uintptr_t)tok;
		
		if( fseek( token_queue.shuffle, token_queue.shuffleused * sizeof( uintptr_t ), SEEK_SET ) != 0 )
		{
			return( -3 );
		}
		if( !fwrite( (const void*)&tmp, sizeof( uintptr_t ), 1, token_queue.shuffle ) )
		{
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
			return( -2 );
		}
		if( !token_queue.shuffleused )
		{
			return( -3 );
		}
		
		--token_queue.shuffleused;
		uintptr_t tmp;
		
		if( fseek( token_queue.shuffle, token_queue.shuffleused * sizeof( uintptr_t ), SEEK_SET ) != 0 )
		{
			return( -4 );
		}
		if( !fread( (void*)&tmp, sizeof( uintptr_t ), 1, token_queue.shuffle ) )
		{
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
		return( -2 );
	}
	
	token *tmp;
	
	if( !token_queue_shufflepop( &tmp ) )
	{
		return( -3 );
	}
	if( !token_queue_push( tmp ) )
	{
		return( -4 );
	}
	
	return( 1 );
}

	/* Note: this DOES NOT attempt to deallocate any tokens, just the file. */
	/*  If you actually care, then empty out the file first. */
int token_queue_deinit()
{
	if( fclose( token_queue.shuffle ) == EOF )
	{
		return( -2 );
	}
	token_queue.shuffle = (FILE*)0;
	
	if( fclose( token_queue.f ) != EOF )
	{
		return( -3 );
	}
	token_queue.f = (FILE*)0;
	
	return( 1 );
}



lib4_intresult bin2num( char c )
{
	if( c == '0' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( 0 );
		
	} else if( c == '1' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( 1 );
		
	} else {
		
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
	}
}
lib4_intresult oct2num( char c )
{
	if( c < '0' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else if( c > '7' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else {
		
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
	}
}
lib4_intresult dec2num( char c )
{
	if( c < '0' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else if( c > '9' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else {
		
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
	}
}
lib4_intresult hexa2num( char c )
{
	if( c >= '0' && c <= '9' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
		
	} else if( c >= 'a' && c <= 'f' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - 'a' ) );
		
	} else if( c >= 'A' && c <= 'F' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - 'A' ) );
		
	} else {
		
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
	}
}
	/* Roughly the system of the ILLIAC 1, the first private (particularly */
	/*  college) owned computer. Yes, it seems odd, but it was very early, */
	/*  and the letters may have started as abbreviations. */
	/* Probably worth noting that this isn't actually used: I just put it in */
	/*  because it came to mind (blame Dave's Garage on Youtube). */
lib4_intresult sexa2num( char c )
{
	if( c >= '0' && c <= '9' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
		
	} else {
		
		switch( c )
		{
			case 'k':
			case 'K':
				LIB4_INTRESULT_RETURNSUCCESS( 10 );
			case 's':
			case 'S':
				LIB4_INTRESULT_RETURNSUCCESS( 11 );
			case 'n':
			case 'N':
				LIB4_INTRESULT_RETURNSUCCESS( 12 );
			case 'j':
			case 'J':
				LIB4_INTRESULT_RETURNSUCCESS( 13 );
			case 'f':
			case 'F':
				LIB4_INTRESULT_RETURNSUCCESS( 14 );
			case 'l':
			case 'L':
				LIB4_INTRESULT_RETURNSUCCESS( 15 );
				
			default:
				LIB4_INTRESULT_RETURNFAILURE( (int)c );
		}
	}
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
	STACK_MONADICFAILURE( tokenize_char__accumulate, "charin", err ); \
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
						STACK_FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
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
						STACK_FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
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
						STACK_FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
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
						STACK_FAILEDINTFUNC( "push_char", tokenize_char__accumulate, res2 );
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
			STACK_FAILEDINTFUNC( "isspace", tokenize_char__accumulate, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		*a_ = a;
		*b_ = b;
		return( 1 );
	}
	
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
		STACK_MONADICFAILURE( tokenize_char, "charin", err ); \
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
				STACK_FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
					STACK_NOTELINE(); STACK_DATAPTR( &th );
					STACK_NOTESPACE(); STACK_DATAPTR( &a );
					STACK_NOTESPACE(); STACK_DATAPTR( &b );
				return( (retframe){ &end_run, (void*)0 } );
			case -2:
					/* Internal error. */
				STACK_FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
					STACK_NOTELINE(); STACK_DATAPTR( &th );
					STACK_NOTESPACE(); STACK_DATAPTR( &a );
					STACK_NOTESPACE(); STACK_DATAPTR( &b );
				return( (retframe){ &end_run, (void*)0 } );
			default:
					/* Unforeseen error, verify passable returns. */
				STACK_FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
					STACK_NOTELINE(); STACK_DATAPTR( &th );
					STACK_NOTESPACE(); STACK_DATAPTR( &a );
					STACK_NOTESPACE(); STACK_DATAPTR( &b );
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
					STACK_FAILEDINTFUNC( "tokenize_char__accumulate", tokenize_char, res2 );
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
				STACK_BADUINT( tokenize_char, &b, 10 );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
				/* Set to newline. */
			b = 10;
			
		} else if( !( b == 9 || b == 11 || b == 12 || b == 32 ) )
		{
			STACK_BADUINT( tokenize_char, &b, 9 );
				STACK_NOTESPACE(); STACK_DECARG( 11 );
				STACK_NOTESPACE(); STACK_DECARG( 12 );
				STACK_NOTESPACE(); STACK_DECARG( 32 );
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
					STACK_FAILEDINTFUNC( "push_char", tokenize_char, res2 );
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
						STACK_BADCHAR( tokenize_char, &a, 'b' );
							STACK_NOTESPACE(); STACK_CHARARG( 'o' );
							STACK_NOTESPACE(); STACK_CHARARG( 'd' );
							STACK_NOTESPACE(); STACK_CHARARG( 'u' );
							STACK_NOTESPACE(); STACK_CHARARG( 'x' );
						return( (retframe){ &end_run, (void*)0 } );
				}
				
				while( len < th.length )
				{
					res2 = pop_char( &( stkp->data ),  &b );
					if( !res2 )
					{
							/* Throw error. */
						STACK_FAILEDINTFUNC( "pop_char", tokenize_char, res2 );
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
					STACK_I_OVERFLOW( tokenize_char, &acc, UCHAR_MAX );
				}
				c = acc;
				th.length = 1;
				res2 = push_char( &( stkp->data ),  *( (char*)&c ) );
				if( !res2 )
				{
					STACK_FAILEDINTFUNC( "push_char", tokenize_char, res2 );
					return( (retframe){ &end_run, (void*)0 } );
				}
				
				break;
				
			default: /* The whitespace stuff. */
				res2 = push_char( &( stkp->data ),  b );
				if( !res2 )
				{
					STACK_FAILEDINTFUNC( "push_char", tokenize_char, res2 );
					return( (retframe){ &end_run, (void*)0 } );
				}
				
				break;
		}
		
	} else {
		
		th.length = 1;
		res2 = push_char( &( stkp->data ),  a );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_char", tokenize_char, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
	res2 = push_tokenhead( &( stkp->data ),  th );
	if( !res2 )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", tokenize_char, res2 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( 1 );
}

	/* A helper function to handle most of the conversion between normal */
	/*  character operations & stackpair operations. Unlike some stuff in */
	/*  this file, this should pretty much be complete. */
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
			STACK_FAILEDINTFUNC( "pop_tokenhead", stack_testchar, res );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
		if( th.length <= 0 )
		{
			STACK_BADNULL( stack_testchar, &( th.length ) );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_BELOWBOUNDS );
		}
		if( th.length > 1 && fail_on_multichar )
		{
			/* TODO: this is probably worthy of a custom message. */
			
			STACK_I_OVERFLOW( stack_testchar, &( th.length ), 1 );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_ABOVEBOUNDS );
		}
		
		
		
		res = peek_char( &( stkp->data ),  0,  &c );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "peek_char", stack_testchar, res );
			LIB4_RESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		res = push_tokenhead( &( stkp->data ),  th );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "push_tokenhead", stack_testchar, res );
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
	STACK_MONADICFAILURE( stack_testchar2, "stack_testchar", ( err ) ); \
	STACK_NOTELINE(); STACK_DATAPTR( stkp ); \
	STACK_NOTESPACE(); STACK_DATAPTR( v ); \
	STACK_NOTESPACE(); STACK_HEXARG( (uintptr_t)test_func ); \
	STACK_NOTESPACE(); STACK_DECARG( 0 );
		LIB4_RESULT_BODYMATCH( res, LIB4_OP_SETa, stack_testchar2__TESTFAIL )
		
		int res2 = push_int( &( stkp->data ),  a.val );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_int", stack_testchar2, res2 );
				STACK_NOTELINE(); STACK_DATAPTR( &( stkp->data ) );
				STACK_NOTESPACE(); STACK_SIGNEDARG( a.val );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RET_FRAMEFUNC( lexalike_refid, REFID_SUBIDS_lexalike__stack_testchar2, 0, -15, 0 );
	}
	
	STACK_BADNULL2( stack_testchar2, &stkp, &v );
	return( (retframe){ &end_run, (void*)0 } );
}

int popas_extrachar( stackpair *stkp, void *v,  extrachar *ec )
{
	if( stkp && ec )
	{
		token_head th;
		
		int res = pop_tokenhead( &( stkp->data ),  &th );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "pop_tokenhead", popas_extrachar, res );
				STACK_NOTELINE(); STACK_DATAPTR( &( stkp->data ) );
			return( -2 );
		}
		if( th.length != 1 )
		{
			STACK_BADINT( popas_extrachar, &( th.length ), 1 );
			return( -3 );
		}
		
		res = pop_char( &( stkp->data ),  &( ec->c ) );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "pop_char", popas_extrachar, res );
				STACK_NOTELINE(); STACK_DATAPTR( &( stkp->data ) );
			return( -4 );
		}
		ec->was_freshline = th.was_freshline;
		ec->is_delimited = th.is_delimited;
		
		return( 1 );
	}
	
	STACK_BADNULL2( popas_extrachar, &stkp, &v );
	return( -1 );
}


	/* Bit of shallow magic here: backstock holds a "failure value" if it's */
	/*  empty, and a "success value" if it's not empty. The following three */
	/*  functions (get_*, peek_*, and unget_* extrachar() functions) use */
	/*  this, in conjunction with the if() test embedded within */
	/*  *_BODYMATCH(), to implement a single-character get/peek/unget */
	/*  facility. */
extrachar_result backstock = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_GENERICFALSE );
extrachar_result get_extrachar( stackpair *stkp, void *v )
{
	extrachar_result a;
	int res;
	
#define get_extrachar_SUCC( val ) \
		a = ( val ); \
		backstock = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_GENERICFALSE );
#define get_extrachar_FAIL( val ) \
		res = tokenize_char( stkp, v ); \
		if( !res ) { \
			STACK_FAILEDINTFUNC( "tokenize_char", get_extrachar, res ); \
				STACK_NOTELINE(); STACK_DATAPTR( stkp ); \
				STACK_NOTESPACE(); STACK_DATAPTR( v ); \
			a = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED ); } \
		else { \
			res = popas_extrachar( stkp, v,  &a ); \
			if( !res ) { \
				STACK_FAILEDINTFUNC( "popas_extrachar", get_extrachar, res ); \
					STACK_NOTELINE(); STACK_DATAPTR( stkp ); \
					STACK_NOTESPACE(); STACK_DATAPTR( v ); \
				a = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED ); \
			} }
	EXTRACHAR_BODYMATCH( backstock, get_extrachar_SUCC, get_extrachar_FAIL );
	
	return( a );
}
extrachar_result peek_extrachar( stackpair *stkp, void *v )
{
#define peek_extrachar_FAIL( val ) \
		backstock = get_extrachar( stkp, v );
	EXTRACHAR_BODYMATCH( backstock, EXTRACHAR_RETURNSUCCESS, peek_extrachar_FAIL );
	
	return( backstock );
}
int unget_extrachar( extrachar ec )
{
#define unget_extrachar_SUCC( val ) \
		return( LIB4_RESULT_FAILURE_ABOVEBOUNDS );
#define unget_extrachar_FAIL( val ) \
		backstock = EXTRACHAR_BUILDSUCCESS( ec ); \
		return( 1 );
	EXTRACHAR_BODYMATCH( backstock, succ, fail )
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



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
