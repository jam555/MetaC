
/* This file just contains some dummy code that shows how to use some stuff. */

uintptr_t generic_referenceid;
#define ERREXIT( refid, err, ... ) searchstack_ERREXIT( ( refid ), ( err ),  __VA_ARGS__  )
retframe demo_func( stack_pair *stkp, void *v_ )
{
	uintptr_t a;
	
#define ERR_( ... )	ERREXIT( &generic_referenceid, __VA_ARGS__ )
	
		/* Note, one or the other: *2() is assumed for "v" to be defined! */
	STACK_CHECK( stkp,  ERR_, 1,  stkp, v_ );
	STACK_CHECK2( stkp, v_,  ERR_, 1,  stkp, v_ );
		/* This is done to make it easier to use whatever v_ points at, so */
		/*  only relevant for some functions. */
	function_data *v = (function_data*)v_;
	
	STACK_PEEK_UINT( &( stkp->data ), 0, a,  ERR_, 2, 3,  stkp, v );
	STACK_POP_UINT( &( stkp->data ), a,  ERR_, 2, 3,  stkp, v );
	STACK_PUSH_UINT( &( stkp->data ), a,  ERR_, 2,  stkp, v );
	
	return( (retframe){ &next_function, (void*)0 } );
	
#undef ERR_
}
