#include <stdlib.h>

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


#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )



struct
{
	stackpair *stkp;
	
} globals =
	{
		&std_stacks
	};


int main( int argn, char** args )
{
	int res;
	retframe entrance_func = ???;
	
	if( !argn || !args )
	{
		BADNULL2( main, &argn, &args );
		???
	}
	STACK_CHECK( globals.stkp,  &errs, main, abort );
	
	???
	
	res = init_stack( &( globals.stkp->ret ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &ret )", main, res );
		abort();
	}
	res = init_stack( &( globals.stkp->data ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &data )", main, res );
		abort();
	}
	
	res = init();
	if( !res )
	{
		FAILEDINTFUNC( "init()", main, res );
		abort();
	}
	
	res = token_queue_init();
	if( !res )
	{
		FAILEDINTFUNC( "token_queue_init()", main, res );
		abort();
	}
	
	res = searchstack_init();
	if( !res )
	{
		FAILEDINTFUNC( "searchstack_init()", main, res );
		abort();
	}
	
	res = macroargs_init();
	if( !res )
	{
		FAILEDINTFUNC( "macroargs_init()", main, res );
		abort();
	}
	
	
	???
	
	
	res = enable_loop( globals.stkp );
	if( !res )
	{
		FAILEDINTFUNC( "enable_loop()", main, res );
		???
		abort();
	}
	
	res = run_loop( &entrance_func,  globals.stkp );
	if( !res )
	{
		FAILEDINTFUNC( "run_loop()", main, res );
		???
		abort();
	}
	
	
	???
	
	
	res = macroargs_clear();
	if( !res )
	{
		FAILEDINTFUNC( "macroargs_clear()", main, res );
		abort();
	}
	
	res = int searchstack_deinit();
	if( !res )
	{
		FAILEDINTFUNC( "searchstack_deinit()", main, res );
		abort();
	}
	
	res = token_queue_deinit();
	if( !res )
	{
		FAILEDINTFUNC( "token_queue_deinit()", main, res );
		abort();
	}
	
	deinit();
	
	res = clear_stack( &( globals.stkp->data ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &data )", main, res );
		abort();
	}
	res = clear_stack( &( globals.stkp->ret ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &ret )", main, res );
		abort();
	}
	
	???
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
