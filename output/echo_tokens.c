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

#define NOTELINE() \
	STDMSG_NOTELINE_WRAPPER( &errs )
#define NOTESPACE() \
	STDMSG_NOTESPACE_WRAPPER( &errs )
#define SIGNEDARG( val ) \
	STDMSG_SIGNEDARG_WRAPPER( &errs, ( val ) )
#define DECARG( uint ) \
	STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
#define DATAPTR( ptr ) \
	STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )



#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )
#define STACKCHECK2( stack, v,  caller, endfunc ) \
	STACK_CHECK2( ( stack ), ( v ),  &err, ( caller ), ( endfunc ) )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &err, ( caller ), ( scratch ), ( endfunc ) )



#define CALLFRAMEFUNC( rethand, retval, callhand, callval,  caller ) \
	CALL_FRAMEFUNC( stkp, rethand, retval, callhand, callval,  &errs, ( caller ), res, stack_ENDRETFRAME )
#define RETFRAMEFUNC( stkpair,  caller ) \
	RET_FRAMEFUNC( ( stkpair ),  &errs, ( caller ), res, stack_ENDRETFRAME )


#define PUTCHAR_CALL( caller, scratch, onerr, errval,  letter ) \
	if( putchar( letter ) == EOF ) { \
		( scratch ) = ferror( stdout ); \
		FAILEDINTFUNC( "putchar", ( caller ), ( scratch ) ); \
			NOTESPACE(); \
			lib4_errno_popresult( &( scratch ) ); SIGNEDARG( ( scratch ) ); \
		onerr( errval ); }
#define PRINTF_CALL( caller, scratch, onerr, errval,  ... ) \
	( scratch ) = printf( __VA_ARGS__ ); /
	if( ( scratch ) < 0 ) { \
		FAILEDINTFUNC( "printf", ( caller ), ( scratch ) ); \
			NOTESPACE(); \
			( scratch ) = ferror( stdout ); SIGNEDARG( ( scratch ) ); \
			NOTESPACE(); \
			lib4_errno_popresult( &( scratch ) ); SIGNEDARG( ( scratch ) ); \
		onerr( errval ); }


#define ENDRETFRAME( ... ) return( (retframe){ &end_run, (void*)0 } )



/* Argument checking should be added to some of this stuff. */

int force_comment = TOKTYPE_INVALID;

#define SOH ( 0x01 )
#define STXT ( 0x02 )
#define ETXT ( 0x03 )
#define USEP ( 0x1F )

int echo_tokenhead( stackpair *stkp, void *v,  token_head **th )
{
	uintptr_t a;
	
	int res = pop_uintptr( stkp,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", echo_tokenhead, res );
		return( -2 );
	}
	*th = (token_head*)a;
	
	lib4_errno_popresult( &res );
	PUTCHAR_CALL( echo_tokenhead, res, return, -3,  SOH );
	switch( force_comment )
	{
		case TOKTYPE_INVALID:
			PRINTF_CALL( echo_tokenhead, res, return, -4,  "%x", (unsigned int)( ( *th )->toktype ) );
			break;
		
		case TOKTYPE_SYM_COMMENTOP:
		case TOKTYPE_SYM_COMMENTLINE:
			PRINTF_CALL( echo_tokenhead, res, return, -4,  "%x", (unsigned int)( TOKTYPE_TOKENGROUP_COMNTMERGE ) );
			break;
	}
	PUTCHAR_CALL( echo_tokenhead, res, return, -5,  USEP );
	PRINTF_CALL( echo_tokenhead, res, return, -6,  "%x", (unsigned int)( ( *th )->length ) );
	PUTCHAR_CALL( echo_tokenhead, res, return, -7,  STXT );
	
	return( 1 );
}


	/* This dispatches to other functions (eg. echo_tokengroup) to do the */
	/*  work, and just exists for the sake of that very dynamic dispatch. */
retframe echo_tokens_entrypoint( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	
	STACKCHECK( stkp,  echo_tokens_entrypoint, macroargs_ENDRETFRAME );
	
	int res;
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  echo_tokens_entrypoint, res, macroargs_ENDRETFRAME );
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
			TRESPASSPATH( echo_tokens_entrypoint, "Error: \"invalid\" token type encountered inside echo_tokens_entrypoint()" );
			return( (retframe){ &end_run, (void*)0 } );
			
			
		case TOKTYPE_SYM_COMMENTOP:
		case TOKTYPE_SYM_COMMENTLINE:
				if( force_comment == TOKTYPE_INVALID )
				{
					force_comment = th->toktype;
				}
				return( (retframe){ (framefunc)&echo_token, (void*)0 } );
			
		case TOKTYPE_SYM_COMMENTCL:
				if( force_comment == TOKTYPE_SYM_COMMENTOP )
				{
					force_comment = th->toktype;
				}
				return( (retframe){ (framefunc)&echo_token, (void*)0 } );
		case TOKTYPE_NEWLINE:
				if( force_comment == TOKTYPE_SYM_COMMENTLINE )
				{
					force_comment = th->toktype;
				}
				return( (retframe){ (framefunc)&echo_token, (void*)0 } );
			
			
		/* These cases that follow all basically get echoed out verbatim, */
		/*  because they're "leaf" tokens instead of ones that can possess */
		/*  child tokens. */
		
		case TOKTYPE_OTHER:
		case TOKTYPE_SYM:
		case TOKTYPE_SYM_CONFUSION:
		case TOKTYPE_SYM_UNKNOWN:
			TRESPASSPATH( echo_tokens_entrypoint, "Warning: unexpected token type encountered inside echo_tokens_entrypoint() : " );
			DECARG( th->toktype );
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
			
				/* That big list of cases, and all of them come down to this. */
			return( (retframe){ (framefunc)&echo_token, (void*)0 } );
			
			
			/* Slightly new stuff: because there's no fun in constantly */
			/*  parsing numbers, we want to support "archived" number */
			/*  parses. */
		case TOKTYPE_NUMBER_UINT:
			return( (retframe){ (framefunc)&echo_tokennum, (void*)0 } );
			
		default:
			TRESPASSPATH( echo_tokens_entrypoint, "Alert: unexpected token type in echo_tokens_entrypoint() : " );
				DECARG( th->toktype );
			/* Unhandled token type, fall through to error exit. */
	}
	
	TRESPASSPATH(
		echo_tokens_entrypoint,
		"Error: echo_tokens_entrypoint() was unexpectedly reached. This function is meant to exit before reaching this point."
	);
	return( (retframe){ &end_run, (void*)0 } );
}



retframe echo_token( stackpair *stkp, void *v )
{
	token_head *th;
	token *t;
	int len, scratch;
	
	STACKCHECK( stkp,  echo_token, macroargs_ENDRETFRAME );
	
	len = echo_tokenhead( stkp, v,  &th );
	if( !len )
	{
		FAILEDINTFUNC( "echo_tokenhead", echo_token, len );
			NOTELINE();
			DATAPTR( &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	t = (token*)th;
	len = th->length;
	while( len )
	{
		/* Initially had this expecting the token string to be on the stack, */
		/*  THAT was certainly out of data (and inconvenient)... */
		
		PUTCHAR_CALL( echo_token, scratch, ENDRETFRAME, ignore,  ( t->text[ th->length - len ] ) );
		
		--len;
	}
	
	lib4_errno_popresult( &scratch );
	PUTCHAR_CALL( echo_token, scratch, ENDRETFRAME, ignore,  ETXT );
	
	
	/* Success. */
	
	RETFRAMEFUNC( stkp,  echo_token );
}

retframe echo_tokennum( stackpair *stkp, void *v )
{
	token_head *th;
	int len, scratch;
	
	STACKCHECK( stkp,  echo_tokennum, macroargs_ENDRETFRAME );
	
	len = echo_tokenhead( stkp, v,  &th );
	if( !len )
	{
		FAILEDINTFUNC( "echo_tokenhead", echo_tokennum, len );
			NOTELINE();
			DATAPTR( &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype == TOKTYPE_NUMBER_UINT )
	{
		token_uint *t = (token_uint*)th;
		len = th->length;
		while( len )
		{
			PUTCHAR_CALL( echo_tokennum, scratch, ENDRETFRAME, ignore,  ( t->text[ th->length - len ] ) );
			
			--len;
		}
		
	} else {
		
		TRESPASSPATH( echo_tokennum, "Error: echo_tokennum() encountered bad toktype: " );
			DECARG( th->toktype );
			NOTELINE();
			DATAPTR( th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* One or both of these might need to be moved into an if body in */
		/*  the future. */
	lib4_errno_popresult( &scratch );
	PUTCHAR_CALL( echo_tokennum, scratch, ENDRETFRAME, ignore,  ETXT );
	
	
	/* Success. */
	
	RETFRAMEFUNC( stkp,  echo_tokennum );
}

retframe echo_tokengroup_extension( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	uintptr_t iter = (uintptr_t)v;
	
	STACKCHECK( stkp,  echo_tokengroup_extension, macroargs_ENDRETFRAME );
	
	int res;
#define echo_tokengroup_extension_END1() \
		NOTELINE(); DATAPTR( &( stkp->data ) ); \
		return( (retframe){ &end_run, (void*)0 } );
	STACKPEEK_UINT(&( stkp->data ), 0, &a ,  echo_tokengroup_extension, res, echo_tokengroup_extension_END1 );
	th = (token_head*)a;
	
	if( th->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		TRESPASSPATH(
			echo_tokengroup_extension,
			"Error: echo_tokengroup_extension() encountered a non-samemerge token type: "
		);
			DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Just to clear errno. */
	lib4_errno_popresult( &res );
	
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
		STACKPUSH_UINT(
			&( stkp->data ),
			(uintptr_t)( ( (tokengroup*)th )->arr->body[ iter ] ),
			
			echo_tokengroup_extension,
			res,
			echo_tokengroup_extension_END1
		);
		
			/* TODO: set onerr and errval to something appropriate. */
		PUTCHAR_CALL( echo_tokengroup_extension, res, ENDRETFRAME, ignore,  ',' );
		
		CALLFRAMEFUNC(
			&echo_tokengroup_extension, (void*)( iter + 1 ),
			&echo_tokens_entrypoint, (void*)0,
			
			echo_tokengroup_extension
		);
		
	} else {
		
		/* Empty group, so short-circuit. We should pop "th" here, to keep */
		/*  the stack clean. */
		
			/* TODO: set onerr and errval to something appropriate. */
		PUTCHAR_CALL( echo_tokengroup_extension, res, ENDRETFRAME, ignore,  ETXT );
		
			/* "th" got repushed onto the stack before this function was */
			/*  even called, so we need to repop it here, because we've now */
			/*  finished echoing it out. */
		STACKPOP_UINT(
			&( stkp->data ),
			&a,
			
			echo_tokengroup_extension,
			res,
			echo_tokengroup_extension_END1
		);
		
		RETFRAMEFUNC( stkp,  echo_tokengroup_extension );
	}
}
retframe echo_tokengroup( stackpair *stkp, void *v )
{
	token_head *th;
	
	int res = echo_tokenhead( &( stkp->data ), v,  &th );
	if( !res )
	{
		FAILEDINTFUNC( "echo_tokenhead", echo_tokengroup, res );
			NOTELINE();
			DATAPTR( &( stkp->data ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		TRESPASSPATH(
			echo_tokengroup,
			"Error: echo_tokengroup() encountered a non-samemerge token type: "
		);
			DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Clear errno. */
	lib4_errno_popresult( &res );
		/* TODO: set onerr and errval to something appropriate. */
	PRINTF_CALL( echo_tokengroup, res, ENDRETFRAME, ignore,  "%x", (unsigned int)( ( (tokengroup*)th )->subtype ) );
	PUTCHAR_CALL( echo_tokengroup, res, ENDRETFRAME, ignore,  ',' );
	PRINTF_CALL( echo_tokengroup, res, ENDRETFRAME, ignore,  "%x", (unsigned int)( ( (tokengroup*)th )->used ) );
	
	if( ( (tokengroup*)th )->used )
	{
		if( !( ( (tokengroup*)th )->arr ) )
		{
			BADNULL( echo_tokengroup, &( ( ( (tokengroup*)th )->arr ) );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		/* ??? */
#define echo_tokengroup_END1() \
	NOTELINE(); DATAPTR( &( stkp->data ) ); \
	return( (retframe){ &end_run, (void*)0 } );
		STACKPUSH_UINT(
			&( stkp->data ),
			(uintptr_t)th,
			
			echo_tokengroup,
			res,
			echo_tokengroup_END1
		);
			/* This second uintptr_t will need to be popped by */
			/*  echo_tokengroup_extension(). Note that echo_tokenhead() does */
			/*  that already. */
		STACKPUSH_UINT(
			&( stkp->data ),
			(uintptr_t)( ( (tokengroup*)th )->arr->body[ 0 ] ),
			
			echo_tokengroup,
			res,
			echo_tokengroup_END1
		);
		
			/* TODO: set onerr and errval to something appropriate. */
		PUTCHAR_CALL( echo_tokengroup, res, ENDRETFRAME, ignore,  ',' );
		
			/* Note that echo_tokengroup_extension() needs to dynamically */
			/*  dispatch according to the type of token pointed to by the */
			/*  top of the stack. Also, it needs to (via whatever it is that */
			/*  it delegate to) pop the top, for the sake of keeping the */
			/*  stack organized & clean. */
		CALLFRAMEFUNC(
			&echo_tokengroup_extension, (void*)( (uintptr_t)1 ),
			&echo_tokens_entrypoint, (void*)0,
			
			echo_tokengroup
		);
		
	} else {
		
		/* Empty group, so short-circuit. We should pop "th" here, to keep */
		/*  the stack clean. */
		
			/* TODO: set onerr and errval to something appropriate. */
		PUTCHAR_CALL( echo_tokengroup, res, ENDRETFRAME, ignore,  ETXT );
		
		RETFRAMEFUNC( stkp,  echo_tokengroup );
	}
}

retframe echo_tokenbranch_conclude( stackpair *stkp, void *v )
{
	uintptr_t a;
	int res;
	
		/* Clear errno. */
	lib4_errno_popresult( &res );
		/* TODO: set onerr and errval to something appropriate. */
	PUTCHAR_CALL( echo_tokenbranch_conclude, res, ENDRETFRAME, ignore,  ETXT );
	
		/* "th" got repushed onto the stack before this function was */
		/*  even called, so we need to repop it here, because we've now */
		/*  finished echoing it out. */
#define echo_tokenbranch_conclude_END1() \
		NOTELINE(); DATAPTR( &( stkp->data ) ); \
		return( (retframe){ &end_run, (void*)0 } );
	STACKPOP_UINT( &( stkp->data ), &a,  echo_tokenbranch_conclude, res, echo_tokenbranch_conclude_END1 );
	
	RETFRAMEFUNC( stkp,  echo_tokenbranch_conclude );
}
retframe echo_tokenbranch_tail( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	int res;
	
#define echo_tokenbranch_tail_END1() \
		NOTELINE(); DATAPTR( &( stkp->data ) ); \
		return( (retframe){ &end_run, (void*)0 } );
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  echo_tokenbranch_tail, res, echo_tokenbranch_tail_END1 );
	th = (token_head*)a;
	if( !th )
	{
		BADNULL( echo_tokenbranch_tail, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH(
			echo_tokenbranch_tail,
			"Error: echo_tokenbranch_tail() encountered a non-equivmerge token type: "
		);
			DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Clear errno. */
	lib4_errno_popresult( &res );
		/* TODO: set onerr and errval to something appropriate. */
	PUTCHAR_CALL( echo_tokenbranch_tail, res, ENDRETFRAME, ignore,  ',' );
	
	if( ( (tokenbranch*)th )->tail )
	{
			/* As with the second push in echo_tokenbranch()... */
		STACKPUSH_UINT(
			&( stkp->data ), (uintptr_t)( ( (tokenbranch*)th )->tail ),
			echo_tokenbranch_tail, res, echo_tokenbranch_tail_END1
		);
		
		CALLFRAMEFUNC(
			&echo_tokenbranch_conclude, (void*)0,
			&echo_tokens_entrypoint, (void*)0,
			
			echo_tokenbranch_tail
		);
	}
	
	return( (retframe){ (framefunc)&echo_tokenbranch_conclude, (void*)0 } );
}
retframe echo_tokenbranch_body( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	int res;
	
#define echo_tokenbranch_body_END1() \
		NOTELINE(); DATAPTR( &( stkp->data ) ); \
		return( (retframe){ &end_run, (void*)0 } );
	STACKPEEK_UINT( &( stkp->data ), 0, &a,  echo_tokenbranch_body, res, echo_tokenbranch_body_END1 );
	th = (token_head*)a;
	if( !th )
	{
		BADNULL( echo_tokenbranch_body, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH(
			echo_tokenbranch_body,
			"Error: echo_tokenbranch_body() encountered a non-equivmerge token type: "
		);
			DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Clear errno. */
	lib4_errno_popresult( &errno );
		/* TODO: set onerr and errval to something appropriate. */
	PUTCHAR_CALL( echo_tokenbranch_body, res, ENDRETFRAME, ignore,  ',' );
	
	if( ( (tokenbranch*)th )->body )
	{
			/* As with the second push in echo_tokenbranch()... */
		STACKPUSH_UINT(
			&( stkp->data ), (uintptr_t)( ( (tokenbranch*)th )->body ),
			echo_tokenbranch_body, res, echo_tokenbranch_body_END1
		);
		
		CALLFRAMEFUNC(
			&echo_tokenbranch_tail, (void*)0,
			&echo_tokens_entrypoint, (void*)0,
			
			echo_tokenbranch_body
		);
	}
	
	return( (retframe){ (framefunc)&echo_tokenbranch_tail, (void*)0 } );
}
retframe echo_tokenbranch( stackpair *stkp, void *v )
{
	token_head *th;
	int res;
	
#define echo_tokenbranch_END1() \
		NOTELINE(); DATAPTR( &( stkp->data ) ); \
		return( (retframe){ &end_run, (void*)0 } );
	res = echo_tokenhead( &( stkp->data ), v,  &th );
	if( !res )
	{
		FAILEDINTFUNC( "echo_tokenhead", echo_tokenbranch, res );
		echo_tokenbranch_END1();
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH(
			echo_tokenbranch,
			"Error: echo_tokenbranch() encountered a non-equivmerge token type: "
		);
			DECARG( th->toktype );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Clear errno. */
	lib4_errno_popresult( &errno );
		/* TODO: set onerr and errval to something appropriate. */
	PRINTF_CALL( echo_tokenbranch, res, ENDRETFRAME, ignore,  "%x", (unsigned int)( ( (tokenbranch*)th )->subtype ) );
	PUTCHAR_CALL( echo_tokenbranch, res, ENDRETFRAME, ignore,  ',' );
	
		/* We'll need to refer to "th" in later calls, so repush it. */
	STACKPUSH_UINT(
		&( stkp->data ), (uintptr_t)th,
		echo_tokenbranch, res, echo_tokenbranch_END1
	);
	if( ( (tokenbranch*)th )->lead )
	{
			/* This second uintptr_t will need to be popped by */
			/*  echo_tokengroup_extension(), albeit likely in what it defers */
			/*  to. Note that echo_tokenhead() does that already. */
		STACKPUSH_UINT(
			&( stkp->data ), (uintptr_t)( ( (tokenbranch*)th )->lead ),
			echo_tokenbranch, res, echo_tokenbranch_END1
		);
		
			/* Note that echo_tokenbranch_body() needs to be able to */
			/*  dynamically dispatch according to the type of token pointed */
			/*  to by the top of the stack. Also, it needs to (via whatever */
			/*  it is that it delegate to) pop the top, for the sake of */
			/*  keeping the stack organized & clean. */
		CALLFRAMEFUNC(
			&echo_tokenbranch_body, (void*)0,
			&echo_tokens_entrypoint, (void*)0,
			
			echo_tokenbranch
		);
	}
	
	/* If ->lead is null, then we don't need to echo it, so we can just */
	/*  precede to the next stage of the echo process. */
	
	return( (retframe){ (framefunc)&echo_tokenbranch_body, (void*)0 } );
}


retframe emit_C_stringifier( stackpair *stkp, void *v )
{
		/* We can use token_head directly because we'll be outputing the */
		/*  text manually. */
	static token_head th =
		{
			TOKTYPE_OCTO,
			1,
			
			0,
			
			(refed_pstr*)???,
			line???, column???
		};
	token_head *ignore;
	int len, scratch;
	
	STACKCHECK( stkp,  emit_C_stringifier, macroargs_ENDRETFRAME );
	
	push_uintptr( (uintptr_t)&th )
	
	len = echo_tokenhead( stkp, v,  &ignore );
	if( !len )
	{
		FAILEDINTFUNC( "echo_tokenhead", emit_C_stringifier, len );
			NOTELINE();
			DATAPTR( &th );
				/* ENDRETFRAME() results in a return(). */
		ENDRETFRAME( scratch );
	}
	
	PUTCHAR_CALL( emit_C_stringifier, scratch, ENDRETFRAME, ignore,  '#' );
	
	RETFRAMEFUNC( stkp,  emit_C_stringifier );
}
retframe emit_C_concatenizer( stackpair *stkp, void *v )
{
		/* We can use token_head directly because we'll be outputing the */
		/*  text manually. */
	static token_head th =
		{
			TOKTYPE_DOUBLEOCTO,
			2,
			
			0,
			
			(refed_pstr*)???,
			line???, column???
		};
	token_head *ignore;
	int len, scratch;
	
	STACKCHECK( stkp,  emit_C_stringifier, macroargs_ENDRETFRAME );
	
	push_uintptr( (uintptr_t)&th )
	
	len = echo_tokenhead( stkp, v,  &ignore );
	if( !len )
	{
		FAILEDINTFUNC( "echo_tokenhead", emit_C_stringifier, len );
			NOTELINE();
			DATAPTR( &th );
				/* ENDRETFRAME() results in a return(). */
		ENDRETFRAME( scratch );
	}
	
	PUTCHAR_CALL( emit_C_stringifier, scratch, ENDRETFRAME, ignore,  '#' );
	PUTCHAR_CALL( emit_C_stringifier, scratch, ENDRETFRAME, ignore,  '#' );
	
	RETFRAMEFUNC( stkp,  emit_C_stringifier );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
