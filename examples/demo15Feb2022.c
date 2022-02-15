
/* This file just contains some dummy code that shows how to use some stuff. */

uintptr_t generic_referenceid;
#define ERREXIT( refid, err, ... ) searchstack_ERREXIT( ( refid ), ( err ),  __VA_ARGS__  )
retframe demo_func( stack_pair *stkp, void *v_ )
{
	uintptr_t a;
	
	/* This seems like like useless boilerplate, but moving that refid */
	/*  REALLY shrinks the line lengths. Also, it's required for all of */
	/*  those other macros, and particularly lets them encode error */
	/*  reporting much more cleanly (though it COULD be replaced with a */
	/*  direct function call if you REALLY wanted). */
#define ERR_( ... )	ERREXIT( &generic_referenceid, __VA_ARGS__ )
	
		/* Note, one or the other: *2() is assumed for "v" to be defined! */
		/*  The code for these is essentially identical each time, so you */
		/*  could honestly just have an argumentless macro that translates */
		/*  into the same thing if you wanted to, just make sure that ERR_() */
		/*  is defined before you use the thing. */
	STACK_CHECK( stkp,  ERR_, 1,  stkp, v_ );
	STACK_CHECK2( stkp, v_,  ERR_, 1,  stkp, v_ );
		/* This is done to make it easier to use whatever v_ points at, so */
		/*  only relevant for some functions. */
	function_data *v = (function_data*)v_;
	
		/* You would obviously only use ONE of these like this, or at least */
		/*  wouldn't have that pop with that peek without a test of "a", and */
		/*  wouldn't have the push without something inbetween. */
	STACK_PEEK_UINT( &( stkp->data ), 0, a,  ERR_, 2, 3,  stkp, v );
	STACK_POP_UINT( &( stkp->data ), a,  ERR_, 2, 3,  stkp, v );
	STACK_PUSH_UINT( &( stkp->data ), a,  ERR_, 2,  stkp, v );
	
		/* This is how you do a tailcall! */
	return( (retframe){ &next_function, (void*)0 } );
	
	/* Undef the macro to save yourself errors later... */
#undef ERR_
}
