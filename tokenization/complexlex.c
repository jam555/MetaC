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
		
		#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
		#define NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )
		
		#define DECARG( uint ) STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
		#define STRARG( strptr ) STDMSG_STRARG_WRAPPER( &errs, ( strptr ) )
		#define DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

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
		
		ret = dealloc_tokenbranch( stkp, v,  tb );
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



tokengroup* build_tokengroup( size_t elems )
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
		
		int res = regrow_tokengroup( tgrp, newlen );
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
int place_tokenhead( token_head **dest, token_head *tok )
{
	int res;
	
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
		
		tokengroup *tg = build_tokengroup( 2 );
		if( !tg )
		{
			FAILEDPTRFUNC( "build_tokengroup", place_tokenhead, tg );
			return( -2 );
		}
		
		res = pushto_tokengroup( tg, *dest );
		if( !res )
		{
			FAILEDINTFUNC( "pushto_tokengroup", place_tokenhead, res );
			return( -3 );
		}
		res = pushto_tokengroup( tg, tok );
		if( !res )
		{
			FAILEDINTFUNC( "pushto_tokengroup", place_tokenhead, res );
			return( -4 );
		}
		
		*dest = &( tg->header );
		
	} else {
		
		/* *dest is a tokengroup, so just push tok to the end of it. */
		
		res = pushto_tokengroup( (tokengroup*)( *dest ), tok );
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


tokenbranch* build_tokenbranch( size_t elems )
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
int set_lead_tokenbranch( tokenbranch *tb, token_head *tok )
{
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
int push_body_tokenbranch( tokenbranch *tb, token_head *tok )
{
	if( !tb || !tok )
	{
		BADNULL2( push_body_tokenbranch, &tb, &tok );
		return( -1 );
	}
	
	int res = place_tokenhead( &( tb->body ), tok );
	if( !res )
	{
		FAILEDINTFUNC( "place_tokenhead", push_body_tokenbranch, res );
		return( -2 );
	}
	
	return( 1 );
}
int set_tail_tokenbranch( tokenbranch *tb, token_head *tok )
{
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


retframe comment_cstyle( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tok, tb;
	
	STACKCHECK( stkp,  comment_cstyle, stack_ENDRETFRAME );
	
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), tb,  comment_cstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tb )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH(
			comment_cstyle,
			"ERROR: comment_cstyle() encountered a non-tokenbranch pointer beneath the top of the stack."
		);
		stack_ENDRETFRAME();
	}
	
	STACKPOP_UINT( stkp->data, tok,  comment_cstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tok )->toktype != TOKTYPE_SYM_COMMENTCL )
	{
		/* Not the end of the comment, so just store and proceed. */
		
		scratch = push_body_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "push_body_tokenbranch", comment_cstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* Fetch & loop. */
		CALLFRAMEFUNC(
			&comment_cstyle, (void*)0,
			&getANDassemble_token, (void*)0,
			
			comment_cstyle
		);
		
	} else {
		
		scratch = set_tail_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "set_tail_tokenbranch", comment_cstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* We're done. */
		RETFRAMEFUNC( comment_cstyle );
	}
}
retframe comment_cppstyle( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tok, tb;
	
	STACKCHECK( stkp,  comment_cppstyle, stack_ENDRETFRAME );
	
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), tb,  comment_cppstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tb )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH(
			comment_cppstyle,
			"ERROR: comment_cppstyle() encountered a non-tokenbranch pointer beneath the top of the stack."
		);
		stack_ENDRETFRAME();
	}
	
	STACKPOP_UINT( stkp->data, tok,  comment_cppstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tok )->toktype != TOKTYPE_NEWLINE )
	{
		/* Not the end of the comment, so just store and proceed. */
		
		scratch = push_body_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "push_body_tokenbranch", comment_cppstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* Fetch & loop. */
		CALLFRAMEFUNC(
			&comment_cstyle, (void*)0,
			&getANDassemble_token, (void*)0,
			
			comment_cppstyle
		);
		
	} else {
		
		scratch = set_tail_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "set_tail_tokenbranch", comment_cppstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* We're done. */
		RETFRAMEFUNC( comment_cppstyle );
	}
}
retframe comment_entry( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tmp, type;
	
	STACKCHECK( stkp,  comment_entry, stack_ENDRETFRAME );
	
		/* So, DO we have an acceptable token pointed to by the top of the */
		/*  stack? */
	STACKPEEK_UINT( stkp->data, 0, tmp,  comment_entry, scratch, stack_ENDRETFRAME );
	type = ( (token_head*)tmp )->toktype;
		/* Note: "commentop" is C style, "commentline" is C++ style. */
	if ( type != TOKTYPE_SYM_COMMENTOP && type != TOKTYPE_SYM_COMMENTLINE )
	{
		TRESPASSPATH(
			comment_entry,
			"ERROR: comment_entry() encountered a non-comment-opening token pointer on top of the stack."
		);
		stack_ENDRETFRAME();
	}
	
		/* The argument doesn't get used, but the function signature */
		/*  currently requires it. */
	tokenbranch *tb = build_tokenbranch( 0 );
	if( !tb )
	{
		BADNULL( comment_entry, &tb );
		stack_ENDRETFRAME();
	}
	
		/* Start accumulating the comment. */
	scratch = set_lead_tokenbranch( tb, (token_head*)tmp );
	if( !scratch )
	{
		FAILEDINTFUNC( "set_lead_tokenbranch", comment_entry, scratch );
		stack_ENDRETFRAME();
	}
	tb->subtype = TOKTYPE_TOKENGROUP_COMNTMERGE;
	
		/* Swap to tracking the wrapper. */
	STACKPOP_UINT( stkp->data, tmp,  comment_entry, scratch, stack_ENDRETFRAME );
	STACKPUSH_UINT( stkp->data, &( tb->header ),  comment_entry, scratch, stack_ENDRETFRAME );
	
		/* Need to pick the correct accumulator function... */
		/* Just to repeat, "commentop" is C style comments, "commentline" is */
		/*  C++ style comments. */
	framefunc accumulator =
		(
			type == TOKTYPE_SYM_COMMENTOP ?
				&comment_cstyle :
				&comment_cppstyle
		);
	
		/* Time to start accumulating the comment. */
	CALLFRAMEFUNC(
		accumulator, (void*)0,
		&getANDassemble_token, (void*)0,
		
		comment_entry
	);
}


	/* You must enter with a pointer to a token_head (and, honestly, */
	/*  probably something more complex) on the top of the data stack. When */
	/*  this returns, it'll have a pointer to a non-whitespace token on the */
	/*  top of the stack, and right below might have a pointer to some sort */
	/*  of token_head-lead whitespace-related object. Accumulating that */
	/*  lower object is the point of the function, but not necessarily */
	/*  possible, depending on what tokens are actually available. */
	/* Note that the top token should NOT be a tokengroup or tokenbranch. */
	/* Addendum: tokenbranches are now allowed for comments. */
retframe accumulate_whitespace( stackpair *stkp, void *v )
{
	uintptr_t a;
	token_head *tmp, *th;
	retframe ret = (retframe){ (framefunc)&end_run, (void*)0 };
	int res;
	
	STACKPOP_UINT( &( stkp->data ), &a,  accumulate_whitespace, res, macroargs_ENDRETFRAME );
	th = (token_head*)a;
	if
	(
		th->toktype == TOKTYPE_TOKENGROUP_SAMEMERGE ||
		(
			th->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE &&
			( (tokenbranch*)th )->subtype != TOKTYPE_TOKENGROUP_COMNTMERGE
		)
	)
	{
		TRESPASSPATH( accumulate_whitespace, "ERROR: accumulate_whitespace() encountered a top-most token type of _SAMEMERGE or _EQUIVMERGE : " );
		DECARG( th->toktype );
		return( ret );
		
	} else if
	(
		th->toktype == TOKTYPE_SYM_COMMENTOP ||
		th->toktype == TOKTYPE_SYM_COMMENTLINE
	)
	{
			/* Our "whitespace" token is a comment opener, so we'll defer to */
			/*  the comment accumulator, then reconvene at the start of this */
			/*  function again. */
		CALLFRAMEFUNC(
			&accumulate_whitespace, 0,
			&comment_entry, 0,
			
			accumulate_whitespace
		);
		
	} else if
	(
		th->toktype != TOKTYPE_SPACE &&
		th->toktype != TOKTYPE_NEWLINE &&
		th->toktype != TOKTYPE_OTHER &&
		( (tokenbranch*)th )->subtype != TOKTYPE_TOKENGROUP_COMNTMERGE
	)
	{
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)th,  accumulate_whitespace, res, macroargs_ENDRETFRAME );
		RETFRAMEFUNC( accumulate_whitespace );
	}
	
	
	STACKPOP_UINT( &( stkp->data ), &a,  accumulate_whitespace, res, macroargs_ENDRETFRAME );
	tmp = (token_head*)a;
	if
	(
		!tmp ||
		(
			tmp->toktype != TOKTYPE_SPACE &&
			tmp->toktype != TOKTYPE_NEWLINE &&
			tmp->toktype != TOKTYPE_OTHER &&
			(
				tmp->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE ||
					/* This is a tokenbranch, so won't get treated as a */
					/*  merge recipient. */
				( (tokenbranch*)tmp )->subtype != TOKTYPE_TOKENGROUP_COMNTMERGE
				
			) &&
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
	
	res = place_tokenhead( &tmp, th );
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
	uintptr_t top;
	retframe ret = (retframe){ (framefunc)&end_run, (void*)0 };
	int res;
	
	STACKPEEK_UINT( &( stkp->data ), 0, &top,  accumulate_token, res, macroargs_ENDRETFRAME );
	
	if
	(
		( (token_head*)top )->toktype == TOKTYPE_SPACE ||
		( (token_head*)top )->toktype == TOKTYPE_NEWLINE ||
		( (token_head*)top )->toktype == TOKTYPE_OTHER ||
		( (token_head*)top )->toktype == TOKTYPE_SYM_COMMENTOP ||
		( (token_head*)top )->toktype == TOKTYPE_SYM_COMMENTLINE
	)
	{
		return( (retframe){ &accumulate_whitespace, (void*)0 } );
	}
	
	
	tokenbranch *tb = build_tokenbranch( 1 );
	if( !tb )
	{
		FAILEDPTRFUNC( "build_tokenbranch", accumulate_token, tb );
		return( ret );
	}
	
	res = push_body_tokenbranch( tb, (token_head*)top );
	if( !res )
	{
		FAILEDINTFUNC( "push_body_tokenbranch", accumulate_token, res );
		return( ret );
	}
	tb->subtype = ( (token_head*)top )->toktype;
	
		/* Swap the token for the tokenbranch that holds a reference to it. */
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
	uintptr_t breaking_token, white, bottom;
	int res;
	
	/* Upon entry, the top element should be a pointer to a non-whitespace */
	/*  token that caused accumulate_whitespace() to exit(), there may or */
	/*  may not be a pointer to a whitespace token or tokengroup under that, */
	/*  and finally there SHOULD be a pointer to a non-whitespace */
	/*  tokenbranch. The job of this function is ultimately to take any */
	/*  POSSIBLE whitespace pointer, and shove it into the ->tail element of */
	/*  the tokenbranch. */
	
	STACKPOP_UINT( &( stkp->data ), &breaking_token,  conclude_accumulate_token, res, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), &white,  conclude_accumulate_token, res, macroargs_ENDRETFRAME );
	if
	(
		( (token_head*)white )->toktype != TOKTYPE_SPACE &&
		( (token_head*)white )->toktype != TOKTYPE_NEWLINE &&
		( (token_head*)white )->toktype != TOKTYPE_OTHER &&
		(
			( (token_head*)white )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE ||
				/* This is a tokenbranch, so won't get treated as a */
				/*  merge recipient. */
			( (tokenbranch*)white )->subtype != TOKTYPE_TOKENGROUP_COMNTMERGE
			
		) &&
		(
			( (token_head*)white )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE ||
			( (tokengroup*)white )->subtype != TOKTYPE_TOKENGROUP_WHITESPACE
		)
	)
	{
		/* Since there's no whitespace, "white" must instead be the token */
		/*  that ideally would be waiting to get peeked into "bottom": */
		/*  compensate. */
		
		bottom = white;
		white = 0;
		
			/* "bottom" is supposed to stay on the stack, so put it there. */
		STACKPUSH_UINT(
			&( stkp->data ), bottom,
			conclude_accumulate_token, res, macroargs_ENDRETFRAME
		);
		
	} else {
		
		STACKPEEK_UINT( &( stkp->data ), 0, &bottom,  conclude_accumulate_token, res, macroargs_ENDRETFRAME );
	}
	
	if
	(
		( (token_head*)bottom )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE ||
		(
			( (tokenbranch*)bottom )->subtype == TOKTYPE_TOKENGROUP_COMNTMERGE ||
				/* This whitespace token type should only be used with */
				/*  tokengroup, but just in case... */
			( (tokenbranch*)bottom )->subtype == TOKTYPE_TOKENGROUP_WHITESPACE
		)
	)
	{
		TRESPASSPATH( accumulate_whitespace, "ERROR: conclude_accumulate_token() encountered an improper \"bottom\" token." );
		NOTELINE();
			STRARG( "bottom: " );
				DATAPTR( bottom );
			STRARG( " whitespace: " );
				DATAPTR( white );
			STRARG( " breaking token: " );
				DATAPTR( breaking_token );
		
		return( (retframe){ (framefunc)&end_run, (void*)0 } );
	}
	
		/* Just an assert, made more sense to always test. */
	if( ( (tokenbranch*)bottom )->tail != (token_head*)0 )
	{
		
		BADNONULL( conclude_accumulate_token, &( ( (tokenbranch*)bottom )->tail ) );
		NOTELINE();
			STRARG( "whitespace: " );
				DATAPTR( white );
			STRARG( " breaking token: " );
				DATAPTR( breaking_token );
		
		return( ret );
	}
	if( white )
	{
		res = set_tail_tokenbranch( (tokenbranch*)bottom, (token_head*)white );
		if( !res )
		{
			FAILEDINTFUNC( "set_tail_tokenbranch", conclude_accumulate_token, res );
			NOTELINE();
				STRARG( "whitespace: " );
					DATAPTR( white );
				STRARG( " breaking token: " );
					DATAPTR( breaking_token );
			
			return( ret );
		}
	}
	
	STACKPUSH_UINT(
		&( stkp->data ), breaking_token,
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
