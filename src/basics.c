

#include "basics.h"
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


#define MONADICFAILURE( funcname, calltext, err ) \
	STDMSG_MONADICFAILURE_WRAPPER( &errs, ( funcname ), ( calltext ), ( err ) )

	#define NOTESPACE() \
		STDMSG_NOTESPACE_WRAPPER( &errs )

	#define DATAPTRARG( val ) \
		STDMSG_DATAPTRARG_WRAPPER( &errs, ( val ) )


#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, ( funcname ), ( msgstr ) )



int refed_pstr_incrrefs( refed_pstr *pstr )
{
	if( pstr )
	{
		if( pstr->refs )
		{
			++( pstr->refs );
			return( 1 );
			
		} else {
			
			TRESPASSPATH( refed_pstr_incrrefs, "ERROR: refed_pstr_incrrefs() was given a refed_pstr with a null ref count." );
			return( -2 );
		}
	}
	
	TRESPASSPATH( refed_pstr_incrrefs, "ERROR: refed_pstr_incrrefs() was given a null arg." );
	return( -1 );
}
int refed_pstr_decrrefs( refed_pstr *pstr )
{
	if( pstr )
	{
		if( pstr->refs )
		{
			--( pstr->refs );
			
			if( pstr->refs )
			{
					/* We're done, everything below this is */
					/*  dedicated to deallocation. */
				return( 1 );
			}
			
				/* Default to success. */
			int ret = 2;
			
#define refed_pstr_decrrefs_BADDESTROY( err ) \
	MONADICFAILURE( init, "char_pascalarray_destroy()", ( err ).val ); \
	NOTESPACE(); DATAPTRARG( pstr->text ); \
	ret = -2;
			lib4_result res = char_pascalarray_destroy( pstr->text );
			LIB4_RESULT_BODYMATCH( res, LIB4_NULL_MACRO, refed_pstr_decrrefs_BADDESTROY );
			
#define refed_pstr_decrrefs_BADDEALLOC( err ) \
	MONADICFAILURE( init, "lib4_stdmemfuncs.dealloc()", ( err ).val ); \
	NOTESPACE(); DATAPTRARG( pstr->text ); \
	ret = -3;
			res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, pstr );
			LIB4_RESULT_BODYMATCH( res, LIB4_NULL_MACRO, refed_pstr_decrrefs_BADDEALLOC );
			
			return( ret );
			
		} else {
			
			TRESPASSPATH( refed_pstr_decrrefs, "ERROR: refed_pstr_decrrefs() was given a refed_pstr with a null ref count." );
			return( -4 );
		}
	}
	
	TRESPASSPATH( refed_pstr_decrrefs, "ERROR: refed_pstr_decrrefs() was given a null arg." );
	return( -1 );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
