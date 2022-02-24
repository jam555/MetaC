/* demo_errs.c */

/* Note that this is essentially a cut-down version of ../stack.c, which was */
/*  the first file that I converted to using the new error system. Note the */
/*  macros defined in this file: they are useful, and you should pretty much */
/*  always write your own versions, with the macro that they wrap choosen */
/*  per your need. */



#include "headers.h"

#include "err/inner_err.h"



stackpair std_stacks;

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


/* Note that these all throw "&errs" in at the start: because this example */
/*  only uses standard messages, "err" is null-initialized, lacking */
/*  specialized error formaters, but in most cases there should be only one */
/*  "msg_styleset" for an entire file/subsystem anyways. This relates to the */
/*  initialization of "errs" at the end of the file: by placing the */
/*  initialization there, all of the members can be simply defined before */
/*  they're actually needed. */
#define STACK_BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define STACK_BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

	/* "calltext" here should be a text string representing the call that */
	/*  failed: the indented defines below it are used in this file to show */
	/*  the arguments, while the actual error is provided via the */
	/*  monad-failure macro itself. Look in "errs/inner_err.h" for the full */
	/*  list of options. */
#define STACK_MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
	#define STACK_NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	#define STACK_NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )
	
	#define STACK_DECARG( uint ) STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
	#define STACK_DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )



int init_stack( stackframe *stk )
{
	if( !stk )
	{
		STACK_BADNULL( init_stack, &stk );
		return( -1 );
	}
	if( stk->stack || stk->used )
	{
		STACK_BADNULL2( init_stack, &( stk->stack ), &( stk->used ) );
		return( -2 );
	}
	
#define init_stack_SUCCESS( arr ) \
		stk->stack = ( arr ); stk->used = 0;
#define init_stack_FAILURE( err ) \
		STACK_MONADICFAILURE( init_stack, "char_pascalarray_build( 64 )", ( err ) ); \
		return( -3 );
	
	char_pascalarray_result res =
		char_pascalarray_build( 64 );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, init_stack_SUCCESS, init_stack_FAILURE )
	
	return( 1 );
}
int resize_stack( stackframe *stk,  int deltaChars )
{
	int len = stk->stack->len + deltaChars;
	
	if( !stk || len < 0 )
	{
		STACK_BADNULL2( resize_stack, &stk, &len );
		return( -1 );
	}
	
	char_pascalarray_result res =
		char_pascalarray_rebuild( stk->stack, len );
#define resize_stack_SUCCESS( arr ) stk->stack = ( arr );
#define resize_stack_FAILURE( err ) \
		STACK_MONADICFAILURE( resize_stack, "char_pascalarray_rebuild()", ( err ) ); \
		STACK_NOTELINE(); STACK_DATAPTRARG( stk->stack ); \
		STACK_NOTESPACE(); STACK_DECARG( len ); \
		return( err );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, resize_stack_SUCCESS, resize_stack_FAILURE )
	
	if( stk->used > len )
	{
		stk->used = len;
	}
	
	return( 1 );
}
int clear_stack( stackframe *stk )
{
	if( !stk )
	{
		STACK_BADNULL( clear_stack, &stk );
		return( -1 );
	}
	
	lib4_result res = char_pascalarray_destroy( stk->stack );
#define pop_frame_SUCCESS( var ) ;
#define pop_frame_FAILURE( err ) \
		STACK_MONADICFAILURE( clear_stack, "char_pascalarray_destroy()", ( err ) ); \
		STACK_NOTELINE(); STACK_DATAPTRARG( stk->stack ); \
		return( err );
	LIB4_RESULT_BODYMATCH( res, pop_frame_SUCCESS, pop_frame_FAILURE )
	
	stk->stack = (char_pascalarray*)0;
	stk->used = 0;
	
	return( 1 );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
