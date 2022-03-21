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



	/* This implements a simple FORTH-ish "return" function. Note that it */
	/*  doesn't expect to deal with any threaded-code registers- the ones */
	/*  usually implemented by e.g. ITC would need to be dealt with */
	/*  elsewhere, this just works with the "double stack" layout. */
retframe plainreturn_leaffunction( stackpair *stkp, void *v )
{
	if( !stkp )
	{
			/* Use a standard error format. */
		STDMSG_BADNULL_WRAPPER( &errs, getANDassemble_token, &stkp );
	}
	
	/* If we WERE going to deal with the normal threaded-code related */
	/*  variations on the "instruction pointer" register, then this is where */
	/*  we would do so. Probably that would culminate in the return retframe */
	/*  that's expected below only being pushed onto the return stack HERE, */
	/*  as opposed to it already existing on the stack as this function */
	/*  assumes. */
	
		/* Perform the return. */
	int res;
	RET_FRAMEFUNC(
		stkp,
		
		&errs, return_leaffunction,
		
		res,
		
		stack_ENDRETFRAME
	);
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
