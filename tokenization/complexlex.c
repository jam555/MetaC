#include "headers.h"

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
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, funcname, ( ptr ) )

#define MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
		
		#define DECARG( uint ) STDMSG_DECARG_WRAPPER( &errs, ( uint ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )
#define FAILEDPTRFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDPTRFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )



#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )
#define STACKCHECK2( stack, v,  caller, endfunc ) \
	STACK_CHECK2( ( stack ), ( v ),  &err, ( caller ), ( endfunc ) )

#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )

#define CALLFRAMEFUNC( rethand, retval, callhand, callval,  caller ) \
	CALL_FRAMEFUNC( stkp, rethand, retval, callhand, callval,  &errs, ( caller ), res, stack_ENDRETFRAME )
#define RETFRAMEFUNC( caller ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), res, stack_ENDRETFRAME )



retframe complexlex_dealloctoken = (retframe){ &smart_dealloc_token, (void*)0 };

retframe set_dealloctoken( retframe dealc_ )
{
	retframe ret = (retframe){ 0, 0 };
	
	if( dealc_.handler )
	{
		ret = complexlex_dealloctoken;
		complexlex_dealloctoken = dealc_;
		
	} else {
		
		BADNULL( set_dealloctoken, &( dealc_.handler ) );
	}
	
	return( ret );
}
retframe invoke_dealloctoken( stackpair *stkp, void *v )
{
	if( complexlex_dealloctoken.handler )
	{
		BADNONULL( invoke_dealloctoken, &( complexlex_dealloctoken.handler ) );
		return( (retframe){ (framefunc)&end_run, (void*)0 } );
	}
	
	return( complexlex_dealloctoken );
}
retframe smart_dealloc_token( stackpair *stkp, void *v )
{
	uintptr_t a;
	token_head *th = (token_head*)0;
		/* One way or another, this SHOULDN'T be the value actually getting */
		/*  returned. But just in case... */
	retframe ret = (retframe){ &end_run, (void*)0 };
	int res;
	
	STACKCHECK( stkp,  smart_dealloc_token, macroargs_ENDRETFRAME );
	
	STACKPEEK_UINT( &( stk->data ), 0, &a,  smart_dealloc_token, res, macroargs_ENDRETFRAME );
	th = (token_head*)a;
	
	if( th->toktype == TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		tokengroup *tgrp = (tokengroup*)th;
		
		ret =
			dealloc_tokengroup
			(
				stkp, v,
				
				&complexlex_refid,
				REFID_SUBIDS_complexlex__smart_dealloc_token__innerfunc1,
				
				tgrp
			);
		if( !ret.handler && !ret.data )
		{
			BADNULL2( smart_dealloc_token, &( ret.handler ), &( ret.data ) );
			return( (retframe){ (framefunc)&end_run, (void*)0 } );
		}
		
	} else if( th->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		tokenbranch *tb = (tokenbranch*)th;
		
		ret =
			dealloc_tokenbranch
			(
				stkp, v,
				
				&complexlex_refid,
				REFID_SUBIDS_complexlex__smart_dealloc_token__innerfunc2,
				
				tb
			);
		if( !ret.handler && !ret.data )
		{
			BADNULL2( smart_dealloc_token, &( ret.handler ), &( ret.data ) );
			return( (retframe){ (framefunc)&end_run, (void*)0 } );
		}
		
	} else {
		
		/* Wasn't either of the two "complexly structured" categories of */
		/*  tokens, so that means it's one of the simple ones instead. Time */
		/*  to outsource appropriately, with a hardwired return. */
		
		return( (retframe){ &dealloc_token, (void*)0 } );
	}
	
	
	if( ret.handler )
	{
		/* We're dealing with a complexly structured token (it has child */
		/*  tokens), and we haven't finished dealing with the children, so */
		/*  we'll queue up our "standard" deallocator (which is this */
		/*  function by default) as a return route, and then call whichever */
		/*  retframe was returned to us (which will probably ALSO be this */
		/*  function...).  */
		
		/* ??? */
		CALLFRAMEFUNC(
			complexlex_dealloctoken.handler,
			complexlex_dealloctoken.data,
			
			ret.handler, ret.data,
			
			smart_dealloc_token
		);
		
	} else {
		
			/* The children have been previously deallocated, and the */
			/*  complex type was deallocated by the helper function that we */
			/*  got the current value of "ret" from, so it's time to return */
			/*  to OUR caller (or at least the "designated return route")! */
		RETFRAMEFUNC( smart_dealloc_token );
	}
}



tokengroup* build_tokengroup
(
	uintptr_t *refid,
	
	size_t elems
)
{
	tokengroup *ret;
	void *a;
	
	/* Build a new tokengroup, and default it's arr & subtype. */
			
#define build_tokengroup_FAILURE1( err ) \
		MONADICFAILURE( build_tokengroup, "lib4_stdmemfuncs.alloc", err ); \
		return( (tokengroup*)0 );
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc
			( lib4_stdmemfuncs.data, sizeof( tokengroup ) );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_tokengroup_FAILURE1 )
	if( !a )
	{
		STACK_BADNULL( build_tokengroup, &a );
		return( (tokengroup*)0 );
	}
	
	ret = (tokengroup*)a;
			
#define build_tokengroup_SUCCESS1( val ) ret->arr = (tokhdptr_parr*)( val );
#define build_tokengroup_FAILURE2( err ) \
		MONADICFAILURE( build_tokengroup, "tokenheadptr_pascalarray_build", err ); \
		return( (tokengroup*)0 );
	tokenheadptr_pascalarray_result res =
		tokenheadptr_pascalarray_build( elems );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH(
		res,
		build_tokengroup_SUCCESS1,
		build_tokengroup_FAILURE2
	)
	
		ret->header.toktype = TOKTYPE_TOKENGROUP_SAMEMERGE;
		ret->header.length = sizeof( tokengroup ) - sizeof( token_head );
	ret->subtype = TOKTYPE_INVALID;
	ret->used = 0;
	
	return( ret );
}
int regrow_tokengroup
(
	uintptr_t *refid,
	
	tokengroup *tgrp,
	size_t newlen
)
{
	if( !tgrp )
	{
		BADNULL( regrow_tokengroup, &tgrp );
		return( -1 );
	}
	if( newlen < tgrp->used )
	{
		TRESPASSPATH( regrow_tokengroup, "ERROR: regrow_tokengroup was given a new size below the used size." );
		return( -2 );
	}
	
	tokenheadptr_pascalarray_result res =
		tokenheadptr_pascalarray_rebuild( tgrp->arr, newlen );
	
#define regrow_tokengroup_SUCC( narr ) tgrp->arr = ( narr );
#define regrow_tokengroup_FAIL( err ) \
		MONADICFAILURE( regrow_tokengroup, "tokenheadptr_pascalarray_rebuild", err ); \
		return( -2 );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH(
		res,
		regrow_tokengroup_SUCC,
		regrow_tokengroup_FAIL
	)
	
	return( 1 );
}
int pushto_tokengroup
(
	uintptr_t *refid,
	int err_subsource,
	
	tokengroup *tgrp,
	token_head *thd
)
{
	if( !tgrp || !thd )
	{
		BADNULL2( pushto_tokengroup, &tgrp, &thd );
		return( -1 );
	}
	
	if( tgrp->used >= tgrp->arr->len )
	{
		size_t newlen = tgrp->arr->len * 2;
		if( !newlen )
		{
			newlen = 1;
		}
		
		int res = regrow_tokengroup( refid,  tgrp, newlen );
		if( !res )
		{
			FAILEDINTFUNC( "regrow_tokengroup", pushto_tokengroup, res );
			return( -2 );
		}
	}
	
	if( tgrp->used >= tgrp->arr->len )
	{
		TRESPASSPATH( pushto_tokengroup, "ERROR: pushto_tokengroup retained a used size larger than the available size." );
		return( -3 );
	}
	
	tgrp->arr->body[ tgrp->used ] = thd;
	++( tgrp->used );
	
	return( 1 );
}
int place_tokenhead( uintptr_t *refid, token_head **dest, token_head *tok )
{
	int res;
	
		/* We actually DON'T CARE if refid is set. */
	if( !dest || !tok )
	{
		BADNULL2( place_tokenhead, &dest, &tok );
		return( -1 );
	}
	
	if( !( *dest ) )
	{
		*dest = tok;
		
	} else if( ( *dest )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		/* Build new tokengroup & push both *dest and tok into it, before */
		/*  putting it into *dest. */
		
		tokengroup *tg = build_tokengroup( refid, 2 );
		if( !tg )
		{
			FAILEDPTRFUNC( "build_tokengroup", place_tokenhead, tg );
			return( -2 );
		}
		
		res = pushto_tokengroup( refid, 0,  tg, *dest );
		if( !res )
		{
			FAILEDINTFUNC( "pushto_tokengroup", place_tokenhead, res );
			return( -3 );
		}
		res = pushto_tokengroup( refid, 0,  tg, tok );
		if( !res )
		{
			FAILEDINTFUNC( "pushto_tokengroup", place_tokenhead, res );
			return( -4 );
		}
		
		*dest = &( tg->header );
		
	} else {
		
		/* *dest is a tokengroup, so just push tok to the end of it. */
		
		res = pushto_tokengroup( refid, 0,  (tokengroup*)( *dest ), tok );
		if( !res )
		{
			FAILEDINTFUNC( "pushto_tokengroup", place_tokenhead, res );
			return( -5 );
		}
	}
	
	return( 1 );
}
retframe dealloc_tokengroup
(
	stackpair *stkp, void *v,
	
	uintptr_t *refid,
	int err_source,
	int err_subsource,
	
	tokengroup *tgrp
)
{
	retframe ret = (retframe){ 0, 0 };
	int res;
	
	STACKCHECK( stkp,  dealloc_tokengroup, macroargs_ENDRETFRAME );
	
	if( !tgrp )
	{
			/* Error! */
		BADNULL( dealloc_tokengroup, &tgrp );
		return( (retframe){ 0, 0 } );
	}
	if( !( tgrp->arr ) )
	{
			/* Error! */
		BADNULL( dealloc_tokengroup, &( tgrp->arr ) );
		return( (retframe){ 0, 0 } );
	}
	
	if( tgrp->used )
	{
		STACKPUSH_UINT(
			&( stkp->data ),  (uintptr_t)( tgrp->arr ),
			dealloc_tokengroup, res, macroargs_ENDRETFRAME
		);
		
		--( tgrp->used );
		ret = complexlex_dealloctoken;
		
	} else {
		
			/* We need to actually pull the tokengroup's pointer off the */
			/*  stack, since we're deleting it now. */
		uintptr_t a;
		STACKPOP_UINT(
			&( stkp->data ), &a,
			dealloc_tokengroup, res, macroargs_ENDRETFRAME
		);
		
		
#define dealloc_tokengroup_ONSUCC( val ) 
		
		
		lib4_result res = tokenheadptr_pascalarray_destroy( tgrp->arr );
#define dealloc_tokengroup_ONFAIL1( err ) \
	MONADICFAILURE( dealloc_tokengroup, "tokenheadptr_pascalarray_destroy", ( err ) ); \
	return( (retframe){ 0, 0 } );
		LIB4_RESULT_BODYMATCH(
			res,
			dealloc_tokengroup_ONSUCC,
			dealloc_tokengroup_ONFAIL1
		);
		
		
		res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, (void*)tgrp );
#define dealloc_tokengroup_ONFAIL2( val ) \
	MONADICFAILURE( dealloc_tokengroup, "lib4_stdmemfuncs.dealloc", ( err ) ); \
	return( (retframe){ 0, 0 } );
		LIB4_RESULT_BODYMATCH(
			res,
			dealloc_tokengroup_ONSUCC,
			dealloc_tokengroup_ONFAIL2
		);
		
		
			/* This is just a signaling value used to indicate that any */
			/*  calling loop should exit. */
		ret = (retframe){ (framefunc)0, (void*)&dealloc_token };
	}
	
	return( ret );
}


tokenbranch* build_tokenbranch
(
	uintptr_t *refid,
	
	size_t elems
)
{
	tokenbranch *ret;
	void *a;
	
	/* Build a new tokenbranch, and set it's defaults. */
			
#define build_tokenbranch_FAILURE1( err ) \
		MONADICFAILURE( build_tokengroup, "lib4_stdmemfuncs.alloc", ( err ) ); \
		return( (tokenbranch*)0 );
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc
			( lib4_stdmemfuncs.data, sizeof( tokenbranch ) );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_tokenbranch_FAILURE1 )
	if( !a )
	{
		BADNULL( build_tokenbranch, &a );
		return( (tokenbranch*)0 );
	}
	
	ret = (tokenbranch*)a;
	
		ret->header.toktype = TOKTYPE_TOKENGROUP_EQUIVMERGE;
		ret->header.length = sizeof( tokenbranch ) - sizeof( token_head );
	ret->subtype = TOKTYPE_INVALID;
	ret->lead = (token_head*)0;
	ret->body = (token_head*)0;
	ret->tail = (token_head*)0;
	
	return( ret );
}
int set_lead_tokenbranch( uintptr_t *refid,  tokenbranch *tb, token_head *tok )
{
		/* We actually DON'T CARE if refid is set. */
	if( !tb || !tok )
	{
		BADNULL2( set_lead_tokenbranch, &tb, &tok );
		return( -1 );
	}
	
	if( tb->lead )
	{
		BADNONULL( set_lead_tokenbranch, &( tb->lead ) );
		return( -2 );
	}
	
	tb->lead = tok;
	
	return( 1 );
}
int push_body_tokenbranch( uintptr_t *refid,  tokenbranch *tb, token_head *tok )
{
		/* We actually DON'T CARE if refid is set. */
	if( !tb || !tok )
	{
		BADNULL2( push_body_tokenbranch, &tb, &tok );
		return( -1 );
	}
	
	int res = place_tokenhead( refid, &( tb->body ), tok );
	if( !res )
	{
		FAILEDINTFUNC( "place_tokenhead", push_body_tokenbranch, res );
		return( -2 );
	}
	
	return( 1 );
}
int set_tail_tokenbranch( uintptr_t *refid,  tokenbranch *tb, token_head *tok )
{
		/* We actually DON'T CARE if refid is set. */
	if( !tb || !tok )
	{
		BADNULL2( set_tail_tokenbranch, &tb, &tok );
		return( -1 );
	}
	
	if( tb->tail )
	{
		BADNONULL( set_tail_tokenbranch, &( tb->lead ) );
		return( -2 );
	}
	
	tb->tail = tok;
	
	return( 1 );
}
retframe dealloc_tokenbranch
(
	stackpair *stkp, void *v,
	
	uintptr_t *refid,
	int err_source,
	int err_subsource,
	
	tokenbranch *tb
)
{
	uintptr_t a;
	int res;
	
	STACKCHECK2( stkp, tb,  dealloc_tokenbranch, macroargs_ENDRETFRAME );
	
	if( tb->lead )
	{
		a = (uintptr_t)( tb->lead );
		tb->lead = (token_head*)0;
		
	} else if( tb->body )
	{
		a = (uintptr_t)( tb->body );
		tb->body = (token_head*)0;
		
	} else if( tb->tail )
	{
		a = (uintptr_t)( tb->tail );
		tb->tail = (token_head*)0;
		
	} else {
		
			/* We need to actually pull the tokenbranch's pointer off the */
			/*  stack, since we're deleting it now. */
		STACKPOP_UINT(
			&( stk->data ), &a,
			dealloc_tokenbranch, res, macroargs_ENDRETFRAME
		);
		
		
		res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, (void*)tgrp );
#define dealloc_tokenbranch_ONSUCC( val ) 
#define dealloc_tokenbranch_ONFAIL( err ) \
	MONADICFAILURE( dealloc_tokenbranch, "lib4_stdmemfuncs.dealloc", ( err ) ); \
	return( (retframe){ 0, 0 } );
		LIB4_RESULT_BODYMATCH(
			res,
			dealloc_tokenbranch_ONSUCC,
			dealloc_tokenbranch_ONFAIL
		);
		
		
			/* This is just a signaling value used to indicate that any */
			/*  calling loop should exit. */
		return( (retframe){ (framefunc)0, (void*)&dealloc_token } );
	}
	
	
	STACKPUSH_UINT( &( stkp->data ), a,  dealloc_tokenbranch, res, macroargs_ENDRETFRAME );
	return( complexlex_dealloctoken );
}


/* Note: build some functions to properly accumulate a token. That should */
/*  include te whitespace AFTER the token, which we now have the function */
/*  for. */

	/* You must enter with a pointer to a token_head (and, honestly, */
	/*  probably something more complex) on the top of the data stack. When */
	/*  this returns, it'll have a pointer to a non-whitespace token on the */
	/*  top of the stack, and right below might have a pointer to some sort */
	/*  of token_head-lead whitespace-related object. Accumulating that */
	/*  lower object is the point of the function, but not necessarily */
	/*  possible, depending on what tokens are actually available. */
	/* Note that the top token should NOT be a tokengroup or tokenbranch. */
retframe accumulate_whitespace( stackpair *stkp, void *v )
{
	uintptr_t a;
	token_head *tmp, *th;
	retframe ret = (retframe){ (framefunc)&end_run, (void*)0 };
	int res;
	
	STACKPOP_UINT( &( stkp->data ), &a,  accumulate_whitespace, res, macroargs_ENDRETFRAME );
	th = (token_head*)a;
	
	STACKPOP_UINT( &( stkp->data ), &a,  accumulate_whitespace, res, macroargs_ENDRETFRAME );
	tmp = (token_head*)a;
	
	if
	(
		th->toktype == TOKTYPE_TOKENGROUP_SAMEMERGE ||
		th->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE
	)
	{
		TRESPASSPATH( accumulate_whitespace, "ERROR: accumulate_whitespace encountered a token type of _SAMEMERGE or _EQUIVMERGE : " );
		DECARG( th->toktype );
		return( ret );
		
	} else if
	(
		th->toktype != TOKTYPE_SPACE &&
		th->toktype != TOKTYPE_NEWLINE &&
		th->toktype != TOKTYPE_OTHER
	)
	{
		RETFRAMEFUNC( accumulate_whitespace );
		
	} else if
	(
		!tmp ||
		(
			tmp->toktype != TOKTYPE_SPACE &&
			tmp->toktype != TOKTYPE_NEWLINE &&
			tmp->toktype != TOKTYPE_OTHER &&
			(
				tmp->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE ||
				( (tokengroup*)tmp )->subtype != TOKTYPE_TOKENGROUP_WHITESPACE
			)
		)
	)
	{
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)tmp,  accumulate_whitespace, res, macroargs_ENDRETFRAME );
		tmp = (token_head*)0;
	}
	
	res = place_tokenhead( &complexlex_refid, &tmp, th );
	if( !res )
	{
		FAILEDINTFUNC( "place_tokenhead", accumulate_whitespace, res );;
		return( ret );
	}
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)tmp,  accumulate_whitespace, res, macroargs_ENDRETFRAME );
	( (tokengroup*)tmp )->subtype = TOKTYPE_TOKENGROUP_WHITESPACE;
	
	CALLFRAMEFUNC(
		&accumulate_whitespace, 0,
		&getANDassemble_token, 0,
		
		accumulate_whitespace
	);
}
	/* This function expects an already ASSEMBLED token to be pointed to by */
	/*  a pointer on the top of the data stack. It will then either directly */
	/*  defer to accumulate_whitespace() (if that token is a SPACE, NEWLINE, */
	/*  or OTHER), or else shove it into a tokenbranch, push a return to */
	/*  conclude_accumulate_token(), then call getANDassemble_token() with a */
	/*  return to accumulate_whitespace(). accumulate_whitespace() will then */
	/*  loop (accumulating whitespace as it goes) until it finds something */
	/*  that isn't whitespace, at which point it will return via the */
	/*  conclude_accumulate_token() reference that was pushed onto the */
	/*  return stack. */
retframe accumulate_token( stackpair *stkp, void *v )
{
	uintptr_t a;
	retframe ret = (retframe){ (framefunc)&end_run, (void*)0 };
	int res;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &top,  accumulate_token, res, macroargs_ENDRETFRAME );
	
	if
	(
		( (token_head*)top )->toktype == TOKTYPE_SPACE ||
		( (token_head*)top )->toktype == TOKTYPE_NEWLINE ||
		( (token_head*)top )->toktype == TOKTYPE_OTHER
	)
	{
		return( (retframe){ &accumulate_whitespace, (void*)0 } );
	}
	
	
	tokenbranch *tb = build_tokenbranch( uintptr_t *refid,  1 );
	if( !tb )
	{
		FAILEDPTRFUNC( "build_tokenbranch", accumulate_token, tb );
		return( ret );
	}
	
	res = push_body_tokenbranch( uintptr_t *refid,  tb, (token_head*)top );
	if( !res )
	{
		FAILEDINTFUNC( "push_body_tokenbranch", accumulate_token, res );
		return( ret );
	}
	tb->subtype = ( (token_head*)top )->toktype;
	
	STACKPOP_UINT( &( stkp->data ), &top,  accumulate_token, res, macroargs_ENDRETFRAME );
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)&( tb->header ),  accumulate_token, res, macroargs_ENDRETFRAME );
	
	
	res = push_retframe( &( stkp->ret ),  (retframe){ &conclude_accumulate_token, (void*)0 } );
	if( !res )
	{
		FAILEDINTFUNC( "push_retframe", accumulate_token, res );
		return( ret );
	}
	CALLFRAMEFUNC(
		&accumulate_whitespace, 0,
		&getANDassemble_token, 0,
		
		accumulate_token
	);
}
retframe conclude_accumulate_token( stackpair *stkp, void *v )
{
	uintptr_t top, white, bottom;
	int res;
	
	STACKPOP_UINT( &( stkp->data ), &top,  conclude_accumulate_token, res, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), &white,  conclude_accumulate_token, res, macroargs_ENDRETFRAME );
	if
	(
		( (token_head*)white )->toktype != TOKTYPE_SPACE &&
		( (token_head*)white )->toktype != TOKTYPE_NEWLINE &&
		( (token_head*)white )->toktype != TOKTYPE_OTHER
	)
	{
		return( (retframe){ &accumulate_whitespace, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), 0, &bottom,  conclude_accumulate_token, res, macroargs_ENDRETFRAME );
	
	if( ( (token_head*)bottom )->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		if( ( (tokenbranch*)bottom )->tail == (token_head*)0 )
		{
			res = set_tail_tokenbranch( uintptr_t *refid,  (tokenbranch*)bottom, (token_head*)white );
			if( !res )
			{
				FAILEDINTFUNC( "set_tail_tokenbranch", conclude_accumulate_token, res );
				return( ret );
			}
			
		} else {
			
			BADNONULL( conclude_accumulate_token, &( ( (tokenbranch*)bottom )->tail ) );
			return( ret );
		}
		
	} else {
		
		STACKPUSH_UINT(
			&( stkp->data ), white,
			conclude_accumulate_token, res, macroargs_ENDRETFRAME
		);
	}
	
	STACKPUSH_UINT(
		&( stkp->data ), top,
		conclude_accumulate_token, res, macroargs_ENDRETFRAME
	);
	
	RETFRAMEFUNC( conclude_accumulate_token );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
