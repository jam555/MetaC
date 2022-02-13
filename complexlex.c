#include "headers.h"



uintptr_t complexlex_refid;
retframe complexlex_dealloctoken = (retframe){ &smart_dealloc_token, (void*)0 };
retframe set_dealloctoken( retframe dealc_ )
{
	retframe ret = (retframe){ 0, 0 };
	
	if( dealc_.handler )
	{
		ret = complexlex_dealloctoken;
		complexlex_dealloctoken = dealc_;
		
	} else {
		
		err_interface( &complexlex_refid, REFID_SUBIDS_complexlex__set_dealloctoken,  &complexlex_dealloctoken );
	}
	
	return( ret );
}
retframe invoke_dealloctoken( stackpair *stkp, void *v )
{
	if( complexlex_dealloctoken.handler )
	{
		err_interface( &complexlex_refid, REFID_SUBIDS_complexlex__invoke_dealloctoken,  &complexlex_dealloctoken );
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
	
	if( !stkp )
	{
		err_interface( &complexlex_refid, REFID_SUBIDS_complexlex__smart_dealloc_token, 1,  stkp, v );
		return( (retframe){ (framefunc)&end_run, (void*)0 } );
	}
	
	if( !peek_uintptr( stk->data,  0,  &a ) )
	{
		err_interface
		(
			&complexlex_refid,
			REFID_SUBIDS_complexlex__smart_dealloc_token,
			2,
			
			stkp,
			v,
			
			&a
		);
		return( (retframe){ (framefunc)&end_run, (void*)0 } );
	}
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
			err_interface
			(
				&complexlex_refid,
				REFID_SUBIDS_complexlex__smart_dealloc_token,
				3,
				
				stkp,
				v,
				
				th
			);
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
			err_interface
			(
				&complexlex_refid,
				REFID_SUBIDS_complexlex__smart_dealloc_token,
				4,
				
				stkp,
				v,
				
				th
			);
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
		
		CALL_FRAMEFUNC(
			complexlex_dealloctoken.handler,
			complexlex_dealloctoken.data,
			
			ret.handler, ret.data
		);
		
	} else {
		
			/* The children have been previously deallocated, and the */
			/*  complex type was deallocated by the helper function that we */
			/*  got the current value of "ret" from, so it's time to return */
			/*  to OUR caller (or at least the "designated return route")! */
		RET_FRAMEFUNC(
			complexlex_refid,
			REFID_SUBIDS_complexlex__smart_dealloc_token,
			5,
			
			stkp,
			v
		);
	}
}



uintptr_t tokengroup_refid;
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
		err_interface( &tokengroup_refid, (uintptr_t)1, refid, 1, ( err ) ); \
		return( (tokengroup*)0 );
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc
			( lib4_stdmemfuncs.data, sizeof( tokengroup ) );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_tokengroup_FAILURE1 )
	if( !a )
	{
		err_interface( &tokengroup_refid, (uintptr_t)1, refid, 2, &ptrres );
		return( (tokengroup*)0 );
	}
	
	ret = (tokengroup*)a;
			
#define build_tokengroup_SUCCESS1( val ) ret->arr = (tokhdptr_parr*)( val );
#define build_tokengroup_FAILURE2( err ) \
		err_interface( &tokengroup_refid, (uintptr_t)1, refid, 3, &ret, ( err ) ); \
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
	if( !refid || !tgrp || newlen < tgrp->used )
	{
		err_interface( &tokengroup_refid, (uintptr_t)2, refid, 0 );
		return( -1 );
	}
	
	tokenheadptr_pascalarray_result res =
		tokenheadptr_pascalarray_rebuild( tgrp->arr, newlen );
	
#define regrow_tokengroup_SUCC( narr ) tgrp->arr = ( narr );
#define regrow_tokengroup_FAIL( err ) \
		err_interface( &tokengroup_refid, (uintptr_t)2, refid, 1, &res, ( err ) ); \
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
	if( !refid || !tgrp || !thd )
	{
		err_interface( &tokengroup_refid, (uintptr_t)3, refid, 0 );
		return( -1 );
	}
	
	if( tgrp->used >= tgrp->arr->len )
	{
		size_t newlen = tgrp->arr->len * 2;
		if( !newlen )
		{
			newlen = 1;
		}
		
		if
		(
			!regrow_tokengroup
			(
				refid,  tgrp, newlen
			)
		)
		{
			err_interface( &tokengroup_refid, (uintptr_t)3, refid, 1 );
			return( -2 );
		}
	}
	
	if( tgrp->used >= tgrp->arr->len )
	{
		err_interface( &tokengroup_refid, (uintptr_t)3, refid, 2 );
		return( -3 );
	}
	
	tgrp->arr->body[ tgrp->used ] = thd;
	++( tgrp->used );
	
	return( 1 );
}
int place_tokenhead( uintptr_t *refid, token_head **dest, token_head *tok )
{
		/* We actually DON'T CARE if refid is set. */
	if( !dest || !tok )
	{
		err_interface( &tokengroup_refid, (uintptr_t)4, refid, 0, dest, tok );
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
			err_interface( &tokengroup_refid, (uintptr_t)4, refid, 1, dest, tok );
			return( -2 );
		}
		
		if( !pushto_tokengroup( refid, 0,  tg, *dest ) )
		{
			err_interface( &tokengroup_refid, (uintptr_t)4, refid, 2, dest, tok, tg );
			return( -3 );
		}
		if( !pushto_tokengroup( refid, 0,  tg, tok ) )
		{
			err_interface( &tokengroup_refid, (uintptr_t)4, refid, 3, tok, tg );
			return( -4 );
		}
		
		*dest = &( tg->header );
		
	} else {
		
		/* *dest is a tokengroup, so just push tok to the end of it. */
		
		if( !pushto_tokengroup( refid, 0,  (tokengroup*)( *dest ), tok ) )
		{
			err_interface( &tokengroup_refid, (uintptr_t)4, refid, 4, dest, tok );
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
	
	if( !tgrp )
	{
			/* Error! */
		err_interface( refid, err_source, err_subsource,  1,  stkp, v );
		return( (retframe){ 0, 0 } );
	}
	if( !( tgrp->arr ) )
	{
			/* Error! */
		err_interface( refid, err_source, err_subsource,  2,  stkp, v, tgrp );
		return( (retframe){ 0, 0 } );
	}
	
	if( tgrp->used )
	{
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)( tgrp->arr ) ) )
		{
			err_interface( refid, err_source, err_subsource,  3,  stkp, v, tgrp );
			return( (retframe){ 0, 0 } );
		}
		
		--( tgrp->used );
		ret = complexlex_dealloctoken;
		
	} else {
		
			/* We need to actually pull the tokengroup's pointer off the */
			/*  stack, since we're deleting it now. */
		uintptr_t a;
		if( !pop_uintptr( stk->data,  &a ) )
		{
			err_interface( refid, err_source, err_subsource,  4,  stkp, v, tgrp, &a );
			return( (retframe){ 0, 0 } );
		}
		
		
#define dealloc_tokengroup_ONSUCC( val ) 
		
		
		lib4_result res = tokenheadptr_pascalarray_destroy( tgrp->arr );
#define dealloc_tokengroup_ONFAIL1( val ) \
	err_interface( refid, err_source, err_subsource,  5,  stkp, v, tgrp, (val) ); \
	return( (retframe){ 0, 0 } );
		LIB4_RESULT_BODYMATCH(
			res,
			dealloc_tokengroup_ONSUCC,
			dealloc_tokengroup_ONFAIL1
		);
		
		
		res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, (void*)tgrp );
#define dealloc_tokengroup_ONFAIL2( val ) \
	err_interface( refid, err_source, err_subsource,  6,  stkp, v, tgrp, (val) ); \
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


uintptr_t tokenbranch_refid;
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
		err_interface( &tokenbranch_refid, (uintptr_t)1, refid, 1, ( err ) ); \
		return( (tokenbranch*)0 );
	lib4_ptrresult ptrres =
		lib4_stdmemfuncs.alloc
			( lib4_stdmemfuncs.data, sizeof( tokenbranch ) );
	LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_tokenbranch_FAILURE1 )
	if( !a )
	{
		err_interface( &tokenbranch_refid, (uintptr_t)1, refid, 2, &ptrres );
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
		err_interface( &tokenbranch_refid, (uintptr_t)2, refid, 0, tb, tok );
		return( -1 );
	}
	
	if( tb->lead )
	{
		err_interface( &tokenbranch_refid, (uintptr_t)2, refid, 1, tb, tok );
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
		err_interface( &tokenbranch_refid, (uintptr_t)3, refid, 0, tb, tok );
		return( -1 );
	}
	
	if( !place_tokenhead( refid, &( tb->body ), tok ) )
	{
		err_interface( &tokenbranch_refid, (uintptr_t)3, refid, 1, tb, tok );
		return( -2 );
	}
	
	return( 1 );
}
int set_tail_tokenbranch( uintptr_t *refid,  tokenbranch *tb, token_head *tok )
{
		/* We actually DON'T CARE if refid is set. */
	if( !tb || !tok )
	{
		err_interface( &tokenbranch_refid, (uintptr_t)4, refid, 0, tb, tok );
		return( -1 );
	}
	
	if( tb->tail )
	{
		err_interface( &tokenbranch_refid, (uintptr_t)4, refid, 1, tb, tok );
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
	
	if( !stkp || !tb )
	{
			/* Error! */
		err_interface( refid, err_source, err_subsource,  1,  stkp, v,  tb );
		return( (retframe){ 0, 0 } );
	}
	
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
		if( !pop_uintptr( stk->data,  &a ) )
		{
			err_interface( refid, err_source, err_subsource,  2,  stkp, v,  tb, &a );
			return( (retframe){ 0, 0 } );
		}
		
		
		res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, (void*)tgrp );
#define dealloc_tokenbranch_ONSUCC( val ) 
#define dealloc_tokenbranch_ONFAIL( val ) \
	err_interface( refid, err_source, err_subsource,  3,  stkp, v,  tb, (val) ); \
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
	
	
	if( !push_uintptr( &( stkp->data ),  a ) )
	{
		err_interface( refid, err_source, err_subsource,  4,  stkp, v,  tb, &a );
		return( (retframe){ 0, 0 } );
	}
	return( complexlex_dealloctoken );
}


/* Note: build some functions to properly accumulate a token. That should */
/*  include te whitespace AFTER the token, which we now have the function */
/*  for. */

uintptr_t complexlex_refid;
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
	
	if( !pop_uintptr( &( stkp->data ),  &a ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)1, 2, &stkp, &v );
		return( ret );
	}
	th = (token_head*)a;
	
	if( !pop_uintptr( &( stkp->data ),  0,  &a ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)1, 3, &stkp, &v, &th );
		return( ret );
	}
	tmp = (token_head*)a;
	
	if
	(
		th->toktype == TOKTYPE_TOKENGROUP_SAMEMERGE ||
		th->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE
	)
	{
		err_interface( &complexlex_refid, (uintptr_t)1, 4, &stkp, &v, &th, &tmp );
		return( ret );
		
	} else if
	(
		th->toktype != TOKTYPE_SPACE &&
		th->toktype != TOKTYPE_NEWLINE &&
		th->toktype != TOKTYPE_OTHER
	)
	{
		RET_FRAMEFUNC( complexlex_refid, -5, &stkp, &v, &th, &tmp );
		
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
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)tmp ) )
		{
			err_interface( &complexlex_refid, (uintptr_t)1, 6, &stkp, &v, &tmp );
			return( ret );
		}
		tmp = (token_head*)0;
	}
	
	if( !place_tokenhead( &complexlex_refid, &tmp, th ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)1, 7, &stkp, &v, &th, &tmp );
		return( ret );
	}
	
	if( !push_uintptr( &( stkp->data ),  (uintptr_t)tmp ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)1, 8, &stkp, &v, &tmp );
		return( ret );
	}
	( (tokengroup*)tmp )->subtype = TOKTYPE_TOKENGROUP_WHITESPACE;
	
	CALL_FRAMEFUNC( &accumulate_whitespace, 0, &getANDassemble_token, 0 );
}
retframe conclude_accumulate_token( stackpair*, void* );
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
	
	if( peek_uintptr( &( stkp->data ),  0,  &top ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)2, 2, &stkp, &v, &tmp );
		return( ret );
	}
	
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
		err_interface( &complexlex_refid, (uintptr_t)2, 3, &stkp, &v, &tmp );
		return( ret );
	}
	
	if( push_body_tokenbranch( uintptr_t *refid,  tb, (token_head*)top ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)2, 4, &stkp, &v, &tmp );
		return( ret );
	}
	tb->subtype = ( (token_head*)top )->toktype;
	
	if( pop_uintptr( &( stkp->data ),  &top ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)2, 5, &stkp, &v, &tmp );
		return( ret );
	}
	if( push_uintptr( &( stkp->data ),  (uintptr_t)&( tb->header ) ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)2, 6, &stkp, &v, &tmp );
		return( ret );
	}
	
	
	if( push_retframe( &( stkp->ret ),  (retframe){ &conclude_accumulate_token, (void*)0 } ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)2, 7, &stkp, &v, &tmp );
		return( ret );
	}
	CALL_FRAMEFUNC( &accumulate_whitespace, 0, &getANDassemble_token, 0 );
}
retframe conclude_accumulate_token( stackpair *stkp, void *v )
{
	uintptr_t top, white, bottom;
	if( !pop_uintptr( &( stkp->data ),  &top ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)3, 2, &stkp, &v );
		return( ret );
	}
	
	if( !pop_uintptr( &( stkp->data ),  &white ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)3, 3, &stkp, &v,  &top );
		return( ret );
	}
	if
	(
		( (token_head*)white )->toktype != TOKTYPE_SPACE &&
		( (token_head*)white )->toktype != TOKTYPE_NEWLINE &&
		( (token_head*)white )->toktype != TOKTYPE_OTHER
	)
	{
		return( (retframe){ &accumulate_whitespace, (void*)0 } );
	}
	
	if( !peek_uintptr( &( stkp->data ),  0,  &bottom ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)3, 4, &stkp, &v,  &top, &white );
		return( ret );
	}
	
	if( ( (token_head*)bottom )->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		if( ( (tokenbranch*)bottom )->tail == (token_head*)0 )
		{
			if( !set_tail_tokenbranch( uintptr_t *refid,  (tokenbranch*)bottom, (token_head*)white ) )
			{
				err_interface( &complexlex_refid, (uintptr_t)3, 5, &stkp, &v,  &top, &white, &bottom );
				return( ret );
			}
			
		} else {
			
			err_interface( &complexlex_refid, (uintptr_t)3, 6, &stkp, &v,  &top, &white, &bottom );
			return( ret );
		}
		
	} else {
		
		if( !push_uintptr( &( stkp->data ),  white ) )
		{
			err_interface( &complexlex_refid, (uintptr_t)3, 7, &stkp, &v,  &top, &white, &bottom );
			return( ret );
		}
	}
	
	if( !push_uintptr( &( stkp->data ),  top ) )
	{
		err_interface( &complexlex_refid, (uintptr_t)3, 8, &stkp, &v,  &top, &white, &bottom );
		return( ret );
	}
	
	RET_FRAMEFUNC2( &complexlex_refid, (uintptr_t)3, &stkp, &v,  &top, &white, &bottom );
}
