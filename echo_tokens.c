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


retframe echo_token( stackpair *stkp, void *v );
retframe echo_tokengroup( stackpair *stkp, void *v );
retframe echo_tokenbranch( stackpair *stkp, void *v );

uintptr_t echo_tokens_refid;
	/* I had four cases that needed a warning, but aren't necessarily an */
	/*  error... */
#define echotokens_ERRALERT( key, ... ) \
		err_interface( &echo_tokens_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ )
#define echotokens_ERREXIT( key, ... ) \
		echotokens_ERRALERT( key, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
	/* This dispatches to other functions (eg. echo_tokengroup) to do the */
	/*  work, and just exists for the sake of that very dynamic dispatch. */
retframe echo_tokens_entrypoint( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	
	if( !peek_uintptr( &( stkp->data ),  0,  &a ) )
	{
		echotokens_ERREXIT( 1, -2, &stkp, &v, &a );
	}
	th = (token_head*)a;
	if( !th )
	{
		echotokens_ERREXIT( 1, -3, &stkp, &v, &a, &th );
	}
	
	
	switch( th->toktype )
	{
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
			return( (retframe){ (framefunc)&echo_tokenbranch, (void*)0 } );
			
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
			return( (retframe){ (framefunc)&echo_tokengroup, (void*)0 } );
			
			
		case TOKTYPE_INVALID:
			echotokens_ERREXIT( 1, -4, &stkp, &v, &a, &th );
			
			
		/* These cases that follow all basically get echoed out verbatim, */
		/*  because they're "leaf" tokens instead of ones that can possess */
		/*  child tokens. */
		
		case TOKTYPE_OTHER:
		case TOKTYPE_SYM:
		case TOKTYPE_SYM_CONFUSION:
		case TOKTYPE_SYM_UNKNOWN:
			echotokens_ERRALERT( 1, -5, &stkp, &v, &a, &th );
				/* *_ERRALERT() doesn't exit the function, so we fall */
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
			/* Unhandled token type, fall through to error exit. */
	}
	
	echotokens_ERREXIT( 1, -6, &stkp, &v, &a );
}



retframe echo_token( stackpair *stkp, void *v )
{
	token_head *th;
	token *t;
	int len;
	
	if( !echo_tokenhead( stkp, v,  &th ) )
	{
		echotokens_ERREXIT( 2, -2, 0 );
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
	
	if( !peek_uintptr( &( stkp->data ),  0, &a ) )
	{
		echotokens_ERREXIT( 4, -2, 0 );
	}
	th = (token_head*)a;
	
	if( th->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		echotokens_ERREXIT( 4, -2, 0 );
	}
	
	if( ( (tokengroup*)th )->used > iter )
	{
		if( !( ( (tokengroup*)th )->arr ) )
		{
			echotokens_ERREXIT( 4, -2, 0 );
		}
		
			/* This will need to be popped by whatever gets defered to by */
			/*  echo_tokengroup_extension(). Note that echo_tokenhead() */
			/*  already does so... */
		if( !push_uintptr_t( &( stkp->data ),  (uintptr_t)( ( (tokengroup*)th )->arr->body[ iter ] ) ) )
		{
			echotokens_ERREXIT( 4, -2, 0 );
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
		if( !pop_uintptr( &( stkp->data ),  &a ) )
		{
			echotokens_ERREXIT( 4, -2, 0 );
		}
		
		RET_FRAMEFUNC( echo_tokenhead_refid, 4, -5, &stkp, &v, &th, &tmp );
	}
}
retframe echo_tokengroup( stackpair *stkp, void *v )
{
	token_head *th;
	
	if( !echo_tokenhead( &( stkp->data ), v,  &th ) )
	{
		echotokens_ERREXIT( 3, -2, 0 );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		echotokens_ERREXIT( 3, -3, 0 );
	}
	
	printf( "%x", (unsigned int)( ( (tokengroup*)th )->subtype ) );
	putc( ',' );
	printf( "%x", (unsigned int)( ( (tokengroup*)th )->used ) );
	
	if( ( (tokengroup*)th )->used )
	{
		if( !( ( (tokengroup*)th )->arr ) )
		{
			echotokens_ERREXIT( 3, -4, 0 );
		}
		
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)th ) )
		{
			echotokens_ERREXIT( 3, -5, 0 );
		}
			/* This second uintptr_t will need to be popped by */
			/*  echo_tokengroup_extension(). Note that echo_tokenhead() does */
			/*  that already. */
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokengroup*)th )->arr->body[ 0 ] ) ) )
		{
			echotokens_ERREXIT( 3, -6, 0 );
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
	if( !pop_uintptr( &( stkp->data ),  &a ) )
	{
		echotokens_ERREXIT( 8, -2, &stkp, &v, &a );
	}
	
	RET_FRAMEFUNC( echo_tokenhead_refid, 8, -3, &stkp, &v, &a );
}
retframe echo_tokenbranch_tail( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	
	if( !peek_uintptr( &( stkp->data ),  0,  &a ) )
	{
		echotokens_ERREXIT( 7, -2, &stkp, &v, &a );
	}
	th = (token_head*)a;
	if( !th )
	{
		echotokens_ERREXIT( 7, -3, &stkp, &v, &a, &th );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		echotokens_ERREXIT( 7, -4, &stkp, &v, &th );
	}
	
	putc( ',' );
	
	if( ( (tokenbranch*)th )->tail )
	{
			/* As with the second push in echo_tokenbranch()... */
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokenbranch*)th )->tail ) ) )
		{
			echotokens_ERREXIT( 7, -5, &stkp, &v, &th );
		}
		
		CALL_FRAMEFUNC( &echo_tokenbranch_conclude, (void*)0,  &echo_tokens_entrypoint, (void*)0  );
	}
	
	return( (retframe){ (framefunc)&echo_tokenbranch_conclude, (void*)0 } );
}
retframe echo_tokenbranch_body( stackpair *stkp, void *v )
{
	token_head *th;
	uintptr_t a;
	
	if( !peek_uintptr( &( stkp->data ),  0,  &a ) )
	{
		echotokens_ERREXIT( 6, -2, &stkp, &v, &a );
	}
	th = (token_head*)a;
	if( !th )
	{
		echotokens_ERREXIT( 6, -3, &stkp, &v, &a, &th );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		echotokens_ERREXIT( 6, -4, &stkp, &v, &th );
	}
	
	putc( ',' );
	
	if( ( (tokenbranch*)th )->body )
	{
			/* As with the second push in echo_tokenbranch()... */
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokenbranch*)th )->body ) ) )
		{
			echotokens_ERREXIT( 6, -5, &stkp, &v, &th );
		}
		
		CALL_FRAMEFUNC( &echo_tokenbranch_tail, (void*)0,  &echo_tokens_entrypoint, (void*)0  );
	}
	
	return( (retframe){ (framefunc)&echo_tokenbranch_tail, (void*)0 } );
}
retframe echo_tokenbranch( stackpair *stkp, void *v )
{
	token_head *th;
	
	if( !echo_tokenhead( &( stkp->data ), v,  &th ) )
	{
		echotokens_ERREXIT( 5, -2, &stkp, &v, &th );
	}
	
	if( th->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		echotokens_ERREXIT( 5, -3, &stkp, &v, &th );
	}
	
	printf( "%x", (unsigned int)( ( (tokenbranch*)th )->subtype ) );
	putc( ',' );
	
		/* We'll need to refer to "th" in later calls, so repush it. */
	if( !push_uintptr( &( stkp->data ),  (uintptr_t)th ) )
	{
		echotokens_ERREXIT( 5, -4, &stkp, &v, &th );
	}
	if( ( (tokenbranch*)th )->lead )
	{
			/* This second uintptr_t will need to be popped by */
			/*  echo_tokengroup_extension(), albeit likely in what it defers */
			/*  to. Note that echo_tokenhead() does that already. */
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)( ( (tokenbranch*)th )->lead ) ) )
		{
			echotokens_ERREXIT( 5, -5, &stkp, &v, &th );
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
