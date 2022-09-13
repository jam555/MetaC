#include "headers.h"
#include "err/inner_err.h"

#include "token.h"



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



#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )

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




int is_stdtoken( token_head *th )
{
	if( !th )
	{
		return( -1 );
	}
	
	switch( th->toktype )
	{
		case TOKTYPE_INVALID:
		case TOKTYPE_PARSEBREAK:
		case TOKTYPE_NUMBER_UINT:
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
		case TOKTYPE_TOKENGROUP_STRMERGE:
		case TOKTYPE_TOKENGROUP_COMNTMERGE:
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
		case TOKTYPE_TOKENGROUP_WHITESPACE:
		case TOKTYPE_TOKENGROUP_DELIMITED:
		case TOKTYPE_TOKENGROUP_MACROLINK:
		case TOKTYPE_TOKENGROUP_MACROTOKEN:
		case TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION:
		case TOKTYPE_TOKENGROUP_MACRORUN:
		case TOKTYPE_TOKENGROUP_MACRODIRECTIVE:
		case TOKTYPE_TOKENGROUP_MACROCALL:
			return( 0 );
		
		default:
			return( 1 );
	}
}
int is_delimited( token_head *th )
{
	if( !th )
	{
		return( -1 );
	}
	
	return( !!( th->is_delimited ) );
}
int was_freshline( token_head *th )
{
	if( !th )
	{
		return( -1 );
	}
	
		/* This is a bad test, but it'll hold for now. */
	return( !( th->column ) );
}



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




#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
