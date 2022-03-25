#include "stdmonads.h"
#include "headers.h"

#include "extrachar.h"

#include "../err/inner_err.h"



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

	#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	#define NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )

	#define DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define BADINT( funcname, objaddr, expectedval ) \
	STDMSG_BADINT_WRAPPER( &errs, ( funcname ), ( objaddr ), ( expectedval ) )



int popas_extrachar( stackpair *stkp, void *v,  extrachar *ec )
{
	if( stkp && ec )
	{
		token_head th;
		
		int res = pop_tokenhead( &( stkp->data ),  &th );
		if( !res )
		{
			FAILEDINTFUNC( "pop_tokenhead", popas_extrachar, res );
				NOTELINE(); DATAPTR( &( stkp->data ) );
			return( -2 );
		}
		if( th.length != 1 )
		{
			BADINT( popas_extrachar, &( th.length ), 1 );
			return( -3 );
		}
		
		res = pop_char( &( stkp->data ),  &( ec->c ) );
		if( !res )
		{
			FAILEDINTFUNC( "pop_char", popas_extrachar, res );
				NOTELINE(); DATAPTR( &( stkp->data ) );
			return( -4 );
		}
		ec->was_freshline = th.was_freshline;
		ec->is_delimited = th.is_delimited;
		
		return( 1 );
	}
	
	BADNULL2( popas_extrachar, &stkp, &v );
	return( -1 );
}


	/* Bit of shallow magic here: backstock holds a "failure value" if it's */
	/*  empty, and a "success value" if it's not empty. The following three */
	/*  functions (get_*, peek_*, and unget_* extrachar() functions) use */
	/*  this, in conjunction with the if() test embedded within */
	/*  *_BODYMATCH(), to implement a single-character get/peek/unget */
	/*  facility. */
extrachar_result backstock = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_GENERICFALSE );
extrachar_result get_extrachar( stackpair *stkp, void *v )
{
	extrachar_result a;
	int res;
	
#define get_extrachar_SUCC( val ) \
		a = ( val ); \
		backstock = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_GENERICFALSE );
#define get_extrachar_FAIL( val ) \
		res = tokenize_char( stkp, v ); \
		if( !res ) { \
			FAILEDINTFUNC( "tokenize_char", get_extrachar, res ); \
				NOTELINE(); DATAPTR( stkp ); \
				NOTESPACE(); DATAPTR( v ); \
			a = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED ); } \
		else { \
			res = popas_extrachar( stkp, v,  &a ); \
			if( !res ) { \
				FAILEDINTFUNC( "popas_extrachar", get_extrachar, res ); \
					NOTELINE(); DATAPTR( stkp ); \
					NOTESPACE(); DATAPTR( v ); \
				a = EXTRACHAR_BUILDFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED ); \
			} }
	EXTRACHAR_BODYMATCH( backstock, get_extrachar_SUCC, get_extrachar_FAIL );
	
	return( a );
}
extrachar_result peek_extrachar( stackpair *stkp, void *v )
{
#define peek_extrachar_FAIL( val ) \
		backstock = get_extrachar( stkp, v );
	EXTRACHAR_BODYMATCH( backstock, EXTRACHAR_RETURNSUCCESS, peek_extrachar_FAIL );
	
	return( backstock );
}
int unget_extrachar( extrachar ec )
{
#define unget_extrachar_SUCC( val ) \
		return( LIB4_RESULT_FAILURE_ABOVEBOUNDS );
#define unget_extrachar_FAIL( val ) \
		backstock = EXTRACHAR_BUILDSUCCESS( ec ); \
		return( 1 );
	EXTRACHAR_BODYMATCH( backstock, succ, fail )
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
