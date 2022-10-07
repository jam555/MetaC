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
tokenheadptr_result popfront_tokengroup( tokengroup *tgrp )
{
	token_head *th;
	
	if( !tgrp )
	{
		BADNULL( popfrom_tokengroup, &tgrp );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 1 );
	}
	if( !( tgrp->used && tgrp->arr ) )
	{
		BADNULL2( popfrom_tokengroup, &( tgrp->used ), &( tgrp->arr ) );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 2 );
	}
	if( !( tgrp->arr->len ) )
	{
		BADNULL( popfrom_tokengroup, &( tgrp->arr->len ) );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 3 );
	}
	if( tgrp->used >= tgrp->arr->len )
	{
		TRESPASSPATH( popfrom_tokengroup, "ERROR: popfrom_tokengroup detected a used size larger than the available size." );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 4 );
	}
	
	th = tgrp->arr->body[ 0 ];
	--( tgrp->used );
	size_t i = 0;
	while( i < tgrp->used )
	{
		tgrp->arr->body[ i ] = tgrp->arr->body[ i + 1 ];
		i += 1;
	}
	
	TOKENHEADPTR_RESULT_RETURNSUCCESS( th );
}
tokenheadptr_result popfrom_tokengroup( tokengroup *tgrp )
{
	token_head *th;
	
	if( !tgrp )
	{
		BADNULL( popfrom_tokengroup, &tgrp );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 1 );
	}
	if( !( tgrp->used && tgrp->arr ) )
	{
		BADNULL2( popfrom_tokengroup, &( tgrp->used ), &( tgrp->arr ) );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 2 );
	}
	if( !( tgrp->arr->len ) )
	{
		BADNULL( popfrom_tokengroup, &( tgrp->arr->len ) );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 3 );
	}
	if( tgrp->used >= tgrp->arr->len )
	{
		TRESPASSPATH( popfrom_tokengroup, "ERROR: popfrom_tokengroup detected a used size larger than the available size." );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 4 );
	}
	
	--( tgrp->used );
	th = tgrp->arr->body[ tgrp->used ];
	
	TOKENHEADPTR_RESULT_RETURNSUCCESS( th );
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
int lengthof_tokengroup( tokengroup *tg )
{
	if( !tg )
	{
		return( -1 );
	}
	if( !( tg->arr ) )
	{
		return( -2 );
	}
	
	return( tg->arr->len );
}

retframe vm_buildempty_tokengroup( stackpair *stkp, void *v )
{
	int res;
	
	STACKCHECK( stkp,  vm_buildempty_tokengroup, macroargs_ENDRETFRAME );
	
	tokengroup *tg = build_tokengroup( 0 );
	if( !tg )
	{
		???
	}
	
	STACKPUSH_UINT(
		&( stkp->data ), (uintptr_t)&( tg->header ),
		
		vm_buildempty_tokengroup, res, macroargs_ENDRETFRAME
	);
	
	RETFRAMEFUNC( vm_buildempty_tokengroup );
}
	/* ( tokengroup* token_head* -- tokengroup* token_head* ) */
retframe vm_setsubtype_tokengroup( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_setsubtype_tokengroup, macroargs_ENDRETFRAME );
	
	uintptr_t tok, grp;
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &tok,
		vm_setsubtype_tokengroup, res, macroargs_ENDRETFRAME
	);
	STACKPEEK_UINT(
		&( stkp->data ), sizeof( uintptr_t ), &grp,
		vm_setsubtype_tokengroup, res, macroargs_ENDRETFRAME
	);
	
	( (tokengroup*)grp )->subtype = ( (token_head*)tok )->toktype;
	
	RETFRAMEFUNC( vm_setsubtype_tokengroup );
}
	/* ( tokengroup* token_head* -- tokengroup* ) */
retframe vm_pushto_tokengroup( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_pushto_tokengroup, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPOP_UINT(
		&( stkp->data ), &a,
		vm_pushto_tokengroup, res, macroargs_ENDRETFRAME
	);
	token_head *th = (token_head*)a;
	if( !th )
	{
		???
	}
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_pushto_tokengroup, res, macroargs_ENDRETFRAME
	);
	tokengroup *tg = (tokengroup*)a;
	if( !tg )
	{
		???
	}
	
	res = pushto_tokengroup( tg, th );
	if( !res )
	{
		???
	}
	
	RETFRAMEFUNC( vm_pushto_tokengroup );
}
retframe vm_popfront_tokengroup( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_popfront_tokengroup, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_popfront_tokengroup, res, macroargs_ENDRETFRAME
	);
	tokengroup *tg = (tokengroup*)a;
	if( !tg )
	{
		???
	}
	
#define vm_popfront_tokengroup_ONSUCC( val ) \
		STACKPUSH_UINT( &( stkp->data ),  (uintptr_t)( val ), \
			vm_popfront_tokengroup, res, macroargs_ENDRETFRAME );
#define vm_popfront_tokengroup_ONERR( err ) \
		???
	
	tokenheadptr_result thp = popfront_tokengroup( tg );
	TOKENHEADPTR_RESULT_BODYMATCH(
		thp,
		vm_popfront_tokengroup_ONSUCC,
		vm_popfront_tokengroup_ONERR
	);
	
	RETFRAMEFUNC( vm_popfront_tokengroup );
}
retframe vm_popfrom_tokengroup( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_popfrom_tokengroup, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_popfrom_tokengroup, res, macroargs_ENDRETFRAME
	);
	tokengroup *tg = (tokengroup*)a;
	if( !tg )
	{
		???
	}
	
#define vm_popfrom_tokengroup_ONSUCC( val ) \
		STACKPUSH_UINT( &( stkp->data ),  (uintptr_t)( val ), \
			vm_popfrom_tokengroup, res, macroargs_ENDRETFRAME );
#define vm_popfrom_tokengroup_ONERR( err ) \
		???
	
	tokenheadptr_result thp = popfrom_tokengroup( tg );
	TOKENHEADPTR_RESULT_BODYMATCH(
		thp,
		vm_popfrom_tokengroup_ONSUCC,
		vm_popfrom_tokengroup_ONERR
	);
	
	RETFRAMEFUNC( vm_popfrom_tokengroup );
}
retframe vm_lengthof_tokengroup( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_lengthof_tokengroup, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_lengthof_tokengroup, res, macroargs_ENDRETFRAME
	);
	
	
	int sz = lengthof_tokengroup( (tokengroup*)a );
	if( !res )
	{
		???
	}
	
	
	STACKPUSH_UINT(
		&( stkp->data ), (uintptr_t)sz,
		
		vm_lengthof_tokengroup, res, macroargs_ENDRETFRAME
	);
	
	RETFRAMEFUNC( vm_popfrom_tokengroup );
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
tokenheadptr_result popfront_body_tokenbranch( tokenbranch *tb )
{
	if( !tb )
	{
		BADNULL( popfront_body_tokenbranch, &tb );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 1 );
	}
	if( !( tb->body ) )
	{
		BADNULL( popfront_body_tokenbranch, &( tb->body ) );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 2 );
	}
	
	
	if( tb->body->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		token_head *th = tb->body;
		tb->body = (token_head*)0;
		TOKENHEADPTR_RESULT_RETURNSUCCESS( th );
		
	} else {
		
		tokenheadptr_result res = popfront_tokengroup( (tokengroup*)( tb->body ) );
		
#define pop_body_tokenbranch_RETURN_POPFROM_ERROR( val ) \
	TOKENHEADPTR_RESULT_RETURNFAILURE( val + 3 );
		
		TOKENHEADPTR_RESULT_BODYMATCH( res, TOKENHEADPTR_RESULT_RETURNSUCCESS, pop_body_tokenbranch_RETURN_POPFROM_ERROR );
	}
}
tokenheadptr_result pop_body_tokenbranch( tokenbranch *tb )
{
	if( !tb )
	{
		BADNULL( pop_body_tokenbranch, &tb );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 1 );
	}
	if( !( tb->body ) )
	{
		BADNULL( pop_body_tokenbranch, &( tb->body ) );
		TOKENHEADPTR_RESULT_RETURNFAILURE( 2 );
	}
	
	
	if( tb->body->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		token_head *th = tb->body;
		tb->body = (token_head*)0;
		TOKENHEADPTR_RESULT_RETURNSUCCESS( th );
		
	} else {
		
		tokenheadptr_result res = popfrom_tokengroup( (tokengroup*)( tb->body ) );
		
#define pop_body_tokenbranch_RETURN_POPFROM_ERROR( val ) \
	TOKENHEADPTR_RESULT_RETURNFAILURE( val + 3 );
		
		TOKENHEADPTR_RESULT_BODYMATCH( res, TOKENHEADPTR_RESULT_RETURNSUCCESS, pop_body_tokenbranch_RETURN_POPFROM_ERROR );
	}
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
int lengthof_body_tokenbranch( tokenbranch *tb )
{
	if( !tb )
	{
		return( -1 );
	}
	if( !( tb->body ) )
	{
		return( -2 );
	}
	
	if( tb->body->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		return( ( tb->body ) ? 1 : 0 );
		
	} else {
		
		return( lengthof_tokengroup( (tokengroup*)( tb->body ) ) );
	}
}

retframe vm_push_body_tokenbranch( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_push_body_tokenbranch, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPOP_UINT(
		&( stkp->data ), &a,
		vm_push_body_tokenbranch, res, macroargs_ENDRETFRAME
	);
	token_head *th = (token_head*)a;
	if( !th )
	{
		???
	}
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_push_body_tokenbranch, res, macroargs_ENDRETFRAME
	);
	tokengroup *tb = (tokenbranch*)a;
	if( !tb )
	{
		???
	}
	
	if( !push_body_tokenbranch( tb, th ) )
	{
		???
	}
	
	RETFRAMEFUNC( vm_push_body_tokenbranch );
}
retframe vm_popfront_body_tokenbranch( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_popfront_body_tokenbranch, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_popfront_body_tokenbranch, res, macroargs_ENDRETFRAME
	);
	tokenbranch *tb = (tokenbranch*)a;
	if( !tb )
	{
		???
	}
	
#define vm_popfront_tokenbranch_ONSUCC( val ) \
		STACKPUSH_UINT( &( stkp->data ),  (uintptr_t)( val ), \
			vm_popfront_body_tokenbranch, res, macroargs_ENDRETFRAME );
#define vm_popfront_tokenbranch_ONERR( err ) \
		???
	
	tokenheadptr_result thp = popfront_body_tokenbranch( tb );
	TOKENHEADPTR_RESULT_BODYMATCH(
		thp,
		vm_popfront_tokenbranch_ONSUCC,
		vm_popfront_tokenbranch_ONERR
	);
	
	RETFRAMEFUNC( vm_popfront_body_tokenbranch );
}
retframe vm_pop_body_tokenbranch( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_pop_body_tokenbranch, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_pop_body_tokenbranch, res, macroargs_ENDRETFRAME
	);
	tokenbranch *tb = (tokenbranch*)a;
	if( !tb )
	{
		???
	}
	
#define vm_popfrom_tokenbranch_ONSUCC( val ) \
		STACKPUSH_UINT( &( stkp->data ),  (uintptr_t)( val ), \
			vm_pop_body_tokenbranch, res, macroargs_ENDRETFRAME );
#define vm_popfrom_tokenbranch_ONERR( err ) \
		???
	
	tokenheadptr_result thp = pop_body_tokenbranch( tb );
	TOKENHEADPTR_RESULT_BODYMATCH(
		thp,
		vm_popfrom_tokenbranch_ONSUCC,
		vm_popfrom_tokenbranch_ONERR
	);
	
	RETFRAMEFUNC( vm_pop_body_tokenbranch );
}
retframe vm_lengthof_body_tokenbranch( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_lengthof_body_tokenbranch, macroargs_ENDRETFRAME );
	
	uintptr_t a;
	int res;
	
	STACKPEEK_UINT(
		&( stkp->data ), 0, &a,
		vm_lengthof_body_tokenbranch, res, macroargs_ENDRETFRAME
	);
	tokenbranch *tb = (tokenbranch*)a;
	if( !tb )
	{
		???
	}
	if( !( tb->body ) )
	{
		???
	}
	
	if( tb->body->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		STACKPUSH_UINT(
			&( stkp->data ),  (uintptr_t)1,
			vm_lengthof_body_tokenbranch, res, macroargs_ENDRETFRAME
		);
		
		RETFRAMEFUNC( vm_pop_body_tokenbranch );
		
	} else {
		
		STACKPUSH_UINT(
			&( stkp->data ),  (uintptr_t)( tb->body ),
			vm_lengthof_body_tokenbranch, res, macroargs_ENDRETFRAME
		);
		
		static const retframe_parr seq =
			(retframe_parr)
			{
				3,
				{
					(retframe){ &vm_lengthof_tokengroup, (void*)0 },
					(retframe){ &swap2nd, (void*)0 },
						/* DON'T deallocate, JUST drop. The tokengroup is */
						/*  still referenced elsewhere. */
					(retframe){ &drop, (void*)0 }
				}
			};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
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
		&token_queue_fetch, 0,
		
		accumulate_whitespace
	);
}
	/* This function expects an already ASSEMBLED token to be pointed to by */
	/*  a pointer on the top of the data stack. It will then either directly */
	/*  defer to accumulate_whitespace() (if that token is a SPACE, NEWLINE, */
	/*  or OTHER), or else shove it into a tokenbranch, push a return to */
	/*  conclude_accumulate_token(), then call token_queue_fetch() with a */
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
		( (token_head*)top )->toktype == TOKTYPE_OTHER
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
		&token_queue_fetch, 0,
		
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
