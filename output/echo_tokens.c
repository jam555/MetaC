#include <stdio.h>

#include "../headers.h"

#include "err/inner_err.h"


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


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )

#define STACK_NOTELINE() \
	STDMSG_NOTELINE_WRAPPER( &errs )
#define STACK_DECARG( uint ) \
	STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
#define STACK_DATAPTR( ptr ) \
	STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define STACK_TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )



/* Argument checking should be added to some of this stuff. */

#define SOH ( 0x01 )
#define STXT ( 0x02 )
#define ETXT ( 0x03 )
#define USEP ( 0x1F )

int echo_tokenhead( stackpair *stkp, void *v,  token_head **th )
{
	uintptr_t a;
	
	if( !pop_uintptr( stkp,  &a ) )
	{
		return( -2 );
	}
	*th = (token_head*)a;
	
	putc( SOH );
	printf( "%x", (unsigned int)( ( *th )->toktype ) );
	putc( USEP );
	printf( "%x", (unsigned int)( ( *th )->length ) );
	putc( STXT );
	
	return( 1 );
}


	/* This dispatches to other functions (eg. echo_tokengroup) to do the */
	/*  work, and just exists for the sake of that very dynamic dispatch. */
retframe echo_tokens_entrypoint( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	
	int res = peek_uintptr( &( stkp->data ),  0,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "peek_uintptr", echo_tokens_entrypoint, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	th = (token_head*)a;
	if( !th )
	{
		BADNULL( echo_tokens_entrypoint, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	switch( th->toktype )
	{
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
			return( (retframe){ (framefunc)&echo_tokenbranch, (void*)0 } );
			
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
			return( (retframe){ (framefunc)&echo_tokengroup, (void*)0 } );
			
			
		case TOKTYPE_INVALID:
			STACK_TRESPASSPATH( echo_tokens_entrypoint, "Error: \"invalid\" token type encountered inside echo_tokens_entrypoint()" );
			return( (retframe){ &end_run, (void*)0 } );
			
			
		/* These cases that follow all basically get echoed out verbatim, */
		/*  because they're "leaf" tokens instead of ones that can possess */
		/*  child tokens. */
		
		case TOKTYPE_OTHER:
		case TOKTYPE_SYM:
		case TOKTYPE_SYM_CONFUSION:
		case TOKTYPE_SYM_UNKNOWN:
			STACK_TRESPASSPATH( echo_tokens_entrypoint, "Warning: unexpected token type encountered inside echo_tokens_entrypoint() : " );
			STACK_DECARG( th->toktype );
				/* These two don't exit the function, so we fall */
				/*  through. We do this because none of these four are meant */
				/*  to be valid, but are not NECESSARILY wrong either */
				/*  depending on the type of file being processed, so we */
				/*  fall-through instead of ending execution. */
			
		case TOKTYPE_AT:
		case TOKTYPE_DOLLAR:
		case TOKTYPE_DQSTR:
		case TOKTYPE_GRAVE:
		case TOKTYPE_NAME:
		case TOKTYPE_NEWLINE:
		case TOKTYPE_NUMBER:
		case TOKTYPE_OCTO:
		case TOKTYPE_SPACE:
		case TOKTYPE_SQSTR:
			
		case TOKTYPE_SYM_ADDITION:
		case TOKTYPE_SYM_ADDROF:
		case TOKTYPE_SYM_BINARYAND:
		case TOKTYPE_SYM_BSLASH:
		case TOKTYPE_SYM_COLON:
		case TOKTYPE_SYM_COMMA:
		case TOKTYPE_SYM_DECREMENT:
		case TOKTYPE_SYM_DEREFERENCE:
		case TOKTYPE_SYM_DIVIDE:
		case TOKTYPE_SYM_DIVISIVSHIFT:
		case TOKTYPE_SYM_DOT:
		case TOKTYPE_SYM_ELEMEDDEREF:
		case TOKTYPE_SYM_EQUALITY:
		case TOKTYPE_SYM_GREATEQUAL:
		case TOKTYPE_SYM_GREATERTHAN:
		case TOKTYPE_SYM_INCREMENT:
		case TOKTYPE_SYM_LESSEREQUAL:
		case TOKTYPE_SYM_LESSERTHAN:
		case TOKTYPE_SYM_LOGICAND:
		case TOKTYPE_SYM_LOGICIOR:
		case TOKTYPE_SYM_MODULO:
		case TOKTYPE_SYM_MULTIPLY:
		case TOKTYPE_SYM_MULTIPLYSHIFT:
		case TOKTYPE_SYM_NOT:
		case TOKTYPE_SYM_NOTEQUAL:
		case TOKTYPE_SYM_PLACEDADD:
		case TOKTYPE_SYM_PLACEDDIV:
		case TOKTYPE_SYM_PLACEDBINARYAND:
		case TOKTYPE_SYM_PLACEDBINARYIOR:
		case TOKTYPE_SYM_PLACEDDIVISIVSHIFT:
		case TOKTYPE_SYM_PLACEDMODULO:
		case TOKTYPE_SYM_PLACEDMULT:
		case TOKTYPE_SYM_PLACEDMULTIPLYSHIFT:
		case TOKTYPE_SYM_PLACEDSUB:
		case TOKTYPE_SYM_PLACEDXOR:
		case TOKTYPE_SYM_QUERY:
		case TOKTYPE_SYM_SCOPE:
		case TOKTYPE_SYM_SET:
		case TOKTYPE_SYM_SUBTRACT:
		case TOKTYPE_SYM_TILDE:
		case TOKTYPE_SYM_XOR:
			
			
		case TOKTYPE_OPCRL:
		case TOKTYPE_OPSQR:
		case TOKTYPE_OPPAR:
			
		case TOKTYPE_SYM_CURLYCLOSE:
		case TOKTYPE_SYM_SQUARECLOSE:
		case TOKTYPE_SYM_PARENCLOSE:
			
			
		case TOKTYPE_OPCRLUP:
		case TOKTYPE_OPSQRUP:
		case TOKTYPE_OPPARUP:
			
		case TOKTYPE_SYM_PRECRLCL:
		case TOKTYPE_SYM_PRESQRCL:
		case TOKTYPE_SYM_PREPARCL:
			
			
		case TOKTYPE_OPCRLANG:
		case TOKTYPE_OPSQRANG:
		case TOKTYPE_OPPARANG:
			
		case TOKTYPE_SYM_COMPTCRLCL:
		case TOKTYPE_SYM_COMPTSQRCL:
		case TOKTYPE_SYM_COMPTPARCL:
			
			
		case TOKTYPE_OPCRLCOL:
		case TOKTYPE_OPSQRCOL:
		case TOKTYPE_OPPARCOL:
			
		case TOKTYPE_SYM_DECLCRLCL:
		case TOKTYPE_SYM_DECLSQRCL:
		case TOKTYPE_SYM_DECLPARCL:
			
			
		case TOKTYPE_OPCRLAST:
		case TOKTYPE_OPSQRAST:
		case TOKTYPE_OPPARAST:
			
		case TOKTYPE_SYM_DECLCASTCLOSE:
		case TOKTYPE_SYM_ARRCASTCLOSE:
		case TOKTYPE_SYM_CASTCLOSE:
			
			
		case TOKTYPE_SYM_COMMENTOP:
		case TOKTYPE_SYM_COMMENTCL:
			
		case TOKTYPE_SYM_COMMENTLINE:
			
				/* That big list of cases, and all of them come down to this. */
			return( (retframe){ (framefunc)&echo_token, (void*)0 } );
			
		default:
			STACK_TRESPASSPATH( echo_tokens_entrypoint, "Alert: unexpected token type in echo_tokens_entrypoint() : " );
				STACK_DECARG( th->toktype );
			/* Unhandled token type, fall through to error exit. */
	}
	
	STACK_TRESPASSPATH(
		echo_tokens_entrypoint,
		"Error: echo_tokens_entrypoint() was unexpectedly reached. This function is meant to exit before reaching this point."
	);
	return( (retframe){ &end_run, (void*)0 } );
}



retframe echo_token( stackpair *stkp, void *v )
{
	token_head *th;
	token *t;
	int len;
	
	len = echo_tokenhead( stkp, v,  &th );
	if( !len )
	{
		FAILEDINTFUNC( "echo_tokenhead", echo_token, len );
			STACK_NOTELINE();
			STACK_DATAPTR( &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	t = (token*)th;
	len = th->length;
	while( len )
	{
		/* Initially had this expecting the token string to be on the stack, */
		/*  THAT was certainly out of data (and inconvenient)... */
		
		putc( t->text[ th->length - len ] );
		
		--len;
	}
	
	putc( ETXT );
	
	
	/* Success. */
	
	RET_FRAMEFUNC( echo_tokenhead_refid, 2, -3, 0 );
}

retframe echo_tokengroup_extension( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	uintptr_t iter = (uintptr_t)v;
	
	int res = peek_uintptr( &( stkp->data ),  0, &a );
	if( !res )
	{
		FAILEDINTFUNC( "peek_uintptr", echo_tokengroup_extension, res );
			STACK_NOTELINE();
			STACK_DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	th = (token_head*)a;
	
	if( th->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		STACK_TRESPASSPATH(
			echo_tokengroup_extension,
			"Error: echo_tokengroup_extension() encountered a non-samemerge token type: "
		);
			STACK_DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( ( (tokengroup*)th )->used > iter )
	{
		if( !( ( (tokengroup*)th )->arr ) )
		{
			BADNULL( echo_tokengroup_extension, &( ( ( (tokengroup*)th )->arr ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
			/* This will need to be popped by whatever gets defered to by */
			/*  echo_tokengroup_extension(). Note that echo_tokenhead() */
			/*  already does so... */
		res = push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokengroup*)th )->arr->body[ iter ] ) );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", echo_tokengroup_extension, res );
				STACK_NOTELINE();
				STACK_DATAPTR( &( stkp->data ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		putc( ',' );
		
		CALL_FRAMEFUNC( &echo_tokengroup_extension, (void*)( iter + 1 ),  &echo_tokens_entrypoint, (void*)0 );
		
	} else {
		
		/* Empty group, so short-circuit. We should pop "th" here, to keep */
		/*  the stack clean. */
		
		putc( ETXT );
		
			/* "th" got repushed onto the stack before this function was */
			/*  even called, so we need to repop it here, because we've now */
			/*  finished echoing it out. */
		res = pop_uintptr( &( stkp->data ),  &a );
		if( !res )
		{
			FAILEDINTFUNC( "pop_uintptr", echo_tokengroup_extension, res );
				STACK_NOTELINE();
				STACK_DATAPTR( &( stkp->data ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RET_FRAMEFUNC( echo_tokenhead_refid, 4, -5, &stkp, &v, &th, &tmp );
	}
}
retframe echo_tokengroup( stackpair *stkp, void *v )
{
	token_head *th;
	
	int res = echo_tokenhead( &( stkp->data ), v,  &th );
	if( !res )
	{
		FAILEDINTFUNC( "echo_tokenhead", echo_tokengroup, res );
			STACK_NOTELINE();
			STACK_DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		STACK_TRESPASSPATH(
			echo_tokengroup,
			"Error: echo_tokengroup() encountered a non-samemerge token type: "
		);
			STACK_DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	printf( "%x", (unsigned int)( ( (tokengroup*)th )->subtype ) );
	putc( ',' );
	printf( "%x", (unsigned int)( ( (tokengroup*)th )->used ) );
	
	if( ( (tokengroup*)th )->used )
	{
		if( !( ( (tokengroup*)th )->arr ) )
		{
			BADNULL( echo_tokengroup_extension, &( ( ( (tokengroup*)th )->arr ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		res = push_uintptr( &( stkp->data ),  (uintptr_t)th );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", echo_tokengroup, res );
				STACK_NOTELINE();
				STACK_DATAPTR( &( stkp->data ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
			/* This second uintptr_t will need to be popped by */
			/*  echo_tokengroup_extension(). Note that echo_tokenhead() does */
			/*  that already. */
		res = push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokengroup*)th )->arr->body[ 0 ] ) );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", echo_tokengroup, res );
				STACK_NOTELINE();
				STACK_DATAPTR( &( stkp->data ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		putc( ',' );
		
			/* Note that echo_tokengroup_extension() needs to dynamically */
			/*  dispatch according to the type of token pointed to by the */
			/*  top of the stack. Also, it needs to (via whatever it is that */
			/*  it delegate to) pop the top, for the sake of keeping the */
			/*  stack organized & clean. */
		CALL_FRAMEFUNC( &echo_tokengroup_extension, (void*)( (uintptr_t)1 ),  &echo_tokens_entrypoint, (void*)0  );
		
	} else {
		
		/* Empty group, so short-circuit. We should pop "th" here, to keep */
		/*  the stack clean. */
		
		putc( ETXT );
		
		RET_FRAMEFUNC( echo_tokenhead_refid, 3, -7, &stkp, &v, &th, &tmp );
	}
}

retframe echo_tokenbranch_conclude( stackpair *stkp, void *v )
{
	uintptr_t a;
	
	putc( ETXT );
	
		/* "th" got repushed onto the stack before this function was */
		/*  even called, so we need to repop it here, because we've now */
		/*  finished echoing it out. */
	int res = pop_uintptr( &( stkp->data ),  &a );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", echo_tokenbranch_conclude, res );
			STACK_NOTELINE();
			STACK_DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RET_FRAMEFUNC( echo_tokenhead_refid, 8, -3, &stkp, &v, &a );
}
retframe echo_tokenbranch_tail( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	
	int res = peek_uintptr( &( stkp->data ),  0,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "peek_uintptr", echo_tokenbranch_tail, res );
			STACK_NOTELINE();
			STACK_DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	th = (token_head*)a;
	if( !th )
	{
		BADNULL( echo_tokenbranch_tail, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		STACK_TRESPASSPATH(
			echo_tokenbranch_tail,
			"Error: echo_tokenbranch_tail() encountered a non-equivmerge token type: "
		);
			STACK_DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	putc( ',' );
	
	if( ( (tokenbranch*)th )->tail )
	{
			/* As with the second push in echo_tokenbranch()... */
		res = push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokenbranch*)th )->tail ) );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", echo_tokenbranch_tail, res );
				STACK_NOTELINE();
				STACK_DATAPTR( &( stkp->data ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		CALL_FRAMEFUNC( &echo_tokenbranch_conclude, (void*)0,  &echo_tokens_entrypoint, (void*)0  );
	}
	
	return( (retframe){ (framefunc)&echo_tokenbranch_conclude, (void*)0 } );
}
retframe echo_tokenbranch_body( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	
	int res = peek_uintptr( &( stkp->data ),  0,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "peek_uintptr", echo_tokenbranch_body, res );
			STACK_NOTELINE();
			STACK_DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	th = (token_head*)a;
	if( !th )
	{
		BADNULL( echo_tokenbranch_body, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		STACK_TRESPASSPATH(
			echo_tokenbranch_body,
			"Error: echo_tokenbranch_body() encountered a non-equivmerge token type: "
		);
			STACK_DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	putc( ',' );
	
	if( ( (tokenbranch*)th )->body )
	{
			/* As with the second push in echo_tokenbranch()... */
		res = push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokenbranch*)th )->body ) );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", echo_tokenbranch_body, res );
				STACK_NOTELINE();
				STACK_DATAPTR( &( stkp->data ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		CALL_FRAMEFUNC( &echo_tokenbranch_tail, (void*)0,  &echo_tokens_entrypoint, (void*)0  );
	}
	
	return( (retframe){ (framefunc)&echo_tokenbranch_tail, (void*)0 } );
}
retframe echo_tokenbranch( stackpair *stkp, void *v )
{
	token_head *th;
	
	int res = echo_tokenhead( &( stkp->data ), v,  &th );
	if( !res )
	{
		FAILEDINTFUNC( "echo_tokenhead", echo_tokenbranch, res );
			STACK_NOTELINE();
			STACK_DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		STACK_TRESPASSPATH(
			echo_tokenbranch,
			"Error: echo_tokenbranch() encountered a non-equivmerge token type: "
		);
			STACK_DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	printf( "%x", (unsigned int)( ( (tokenbranch*)th )->subtype ) );
	putc( ',' );
	
		/* We'll need to refer to "th" in later calls, so repush it. */
	res = push_uintptr( &( stkp->data ),  (uintptr_t)th );
	if( !res )
	{
		FAILEDINTFUNC( "push_uintptr", echo_tokenbranch, res );
			STACK_NOTELINE();
			STACK_DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( ( (tokenbranch*)th )->lead )
	{
			/* This second uintptr_t will need to be popped by */
			/*  echo_tokengroup_extension(), albeit likely in what it defers */
			/*  to. Note that echo_tokenhead() does that already. */
		res = push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokenbranch*)th )->lead ) );
		if( !res )
		{
			FAILEDINTFUNC( "push_uintptr", echo_tokenbranch, res );
				STACK_NOTELINE();
				STACK_DATAPTR( &( stkp->data ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
			/* Note that echo_tokenbranch_body() needs to be able to */
			/*  dynamically dispatch according to the type of token pointed */
			/*  to by the top of the stack. Also, it needs to (via whatever */
			/*  it is that it delegate to) pop the top, for the sake of */
			/*  keeping the stack organized & clean. */
		CALL_FRAMEFUNC( &echo_tokenbranch_body, (void*)0,  &echo_tokens_entrypoint, (void*)0  );
	}
	
	/* If ->lead is null, then we don't need to echo it, so we can just */
	/*  precede to the next stage of the echo process. */
	
	return( (retframe){ (framefunc)&echo_tokenbranch_body, (void*)0 } );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
