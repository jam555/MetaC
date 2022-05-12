#include "../headers.h"

#include "source.h"
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


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, ( funcname ), ( ptr ) )
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, ( funcname ), ( ptr ) )

#define MONADICFAILURE( funcname, calltext, err ) \
	STDMSG_MONADICFAILURE_WRAPPER( &errs, ( funcname ), ( calltext ), ( err ) )

	#define NOTESPACE() \
		STDMSG_NOTESPACE_WRAPPER( &errs )
	#define DECARG( val ) \
		STDMSG_DECARG_WRAPPER( &errs, ( val ) )
	$define CHARARG( val ) \
		STDMSG_CHARARG_WRAPPER( &errs, ( val ) )
	#define DATAPTRARG( val ) \
		STDMSG_DATAPTRARG_WRAPPER( &errs, ( val ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), ( callername ), ( val ) )

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
			res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, a );
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



struct
{
	char_pascalarray *pushedback;
	size_t used;
	
} inbuf = { (char_pascalarray*)0, 0 };


struct source
{
	source *prev;
	
	FILE *file;
	refed_pstr *name;
	
		/* Max val is UINTMAX_MAX */
	uintmax_t base, progress;
	
} *sources = (source*)0;

#define COMMONINCLUDE "common_include.incl"


static root_name =
	LIB4_DEFINE_PASCALARRAY_LITERAL2( char_, char, COMMONINCLUDE );
static refed_pstr root_pname = (refed_pstr){ 1, &root_name };
static source root_src = (source){ (source*)0,  0, &root_pname,  0, 0 };
int init()
{
	int ret = -1;
	
	if( !sources )
	{
		root_src.file = fopen( root_src.name->text->body, "rb" );
		if( !root_src.file )
		{
			BADNULL( init, &( root_src.file ) );
			return( -2 );
		}
		
		sources = &root_src;
		
		ret = 1;
	}
	
	if( !( inbuf.pushedback ) )
	{
		char_pascalarray_result res =
			char_pascalarray_build( size_t len );
		int res2;
#define init_SUCCESS( arr ) inbuf.pushedback = ( arr );
#define init_FAILURE( err ) \
	MONADICFAILURE( init, "char_pascalarray_build()", ( err ) ); \
	sources = (source*)0; \
	res2 = fclose( root_src.file ); \
	if( res2 != 0 ) { \
		FAILEDINTFUNC( "fclose", init, res2 ); } \
	root_pname = (refed_pstr){ 1, &root_name };
	root_src = (source){ (source*)0,  0, &root_pname,  0, 0 }; \
	return( err );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, init_SUCCESS, init_FAILURE )
	}
	
	if( !ret )
	{
		TRESPASSPATH( init, "ERROR: init() in source.c failed to exit before reaching it's end." );
		BADNONULL( init, &sources );
	}
	return( ret );
}
void deinit()
{
	if( inbuf.pushedback )
	{
		lib4_result res = char_pascalarray_destroy( inbuf.pushedback );
#define deinit_SUCCESS( succ ) \
	inbuf = { (char_pascalarray*)0, 0 };
#define deinit_FAILURE( err ) \
	MONADICFAILURE( deinit, "char_pascalarray_destroy()", ( err ).val ); \
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, deinit_SUCCESS, deinit_FAILURE )
	}
	
	if( sources == &root_src )
	{
		sources = (source*)0;
		
		if( root_src.file )
		{
			int res = fclose( root_src.file );
			if( res != 0 )
			{
				FAILEDINTFUNC( "fclose", deinit, res );
			}
		}
		
		root_pname = (refed_pstr){ 1, &root_name };
		root_src = (source){ (source*)0,  0, &root_pname,  0, 0 };
	}
}
int reinit()
{
	deinit();
	return( init() );
}

source* build_source2( char_pascalarray *name, uintmax_t inclusionpoint, const char *opentype )
{
	source *a = (source*)0;
	
#define build_source2_ERR1( err ) \
	MONADICFAILURE( build_source2, "lib4_stdmemfuncs.alloc()", ( err ) )
#define build_source2_ERR2( err ) \
	MONADICFAILURE( build_source2, "char_pascalarray_build()", ( err ) )
	
	if( name )
	{
		size_t len = 0;
		while
		(
			len < name->len &&
			( isspace( name->body[ len ] ) || isgraph( name->body[ len ] ) )
		)
		{
			++len;
		}
		if( name->body[ len ] != '\0' )
		{
			++len;
		}
		
		lib4_ptrresult ptrres =
			lib4_stdmemfuncs.alloc
				( lib4_stdmemfuncs.data, sizeof( source ) );
		LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_source2_ERR1 )
		if( a )
		{
			*a = (source){ (source*)0,  (FILE*)0, (refed_pstr*)0,  inclusionpoint, 0 };
		}
		
		refed_pstr *b = (refed_pstr*)0;
		lib4_ptrresult ptrres =
			lib4_stdmemfuncs.alloc
				( lib4_stdmemfuncs.data, sizeof( refed_pstr ) );
		LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETb, build_source2_ERR1 )
		if( !b )
		{
			if( lib4_stdmemfuncs.dealloc )
			{
				lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, a );
			}
			a = (source*)0;
			
			TRESPASSPATH( build_source2, "ERROR: build_source2() failed to allocate a refed_pstr." );
			
		} else {
			
			*b = (refed_pstr){ 0, (char_pascalarray*)0 };
			
			char_pascalarray *c = (char_pascalarray*)0;
			char_pascalarray_result res =
				char_pascalarray_build( len );
			LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETc, build_source2_ERR2 )
			if( c )
			{
				--len;
				c->body[ len ] = '\0';
				
				while( len )
				{
					--len;
					c->body[ len ] = name->body[ len ];
				}
				
					/* Very important! Both increment & decrement will error */
					/*  out if this isn't positive! */
				b->refs = 1;
				b->text = c;
				
				a->name = b;
				a->file = fopen( a->name->text->body, opentype );
				
			} else {
				
				if( lib4_stdmemfuncs.dealloc )
				{
					lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, b );
					lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, a );
				}
				b = (refed_pstr*)0;
				a = (source*)0;
				
				TRESPASSPATH( build_source2, "ERROR: build_source2() failed to allocate a char_pascalarray of length " );
					DECARG( len );
			}
		}
	}
	
	return( a );
}
source* build_source( char_pascalarray *name, uintmax_t inclusionpoint )
{
	return( build_source2( name, inclusionpoint, "rb" ) );
}
int discard_source( source *src )
{
	if( src == &root_src )
	{
		TRESPASSPATH( discard_source, "ERROR: discard_source() was told to deallocate static source instance \"root_src\"." );
		return( -2 );
	}
	
	if( src )
	{
		src->prev = (source*)0;
		
			/* We'll probably have other references floating around, so */
			/*  don't DIRECTLY delete... */
		int res = refed_pstr_decrrefs( src->name );
		if( res < 0 )
		{
			FAILEDINTFUNC( "refed_pstr_decrrefs", discard_source, res );
				NOTESPACE();
				DATAPTRARG( src->name );
			
			/* Just continue on. */
		}
		src->name = (refed_pstr*)0;
		
		res = fclose( src->file );
		if( res != 0 )
		{
			FAILEDINTFUNC( "fclose", discard_source, res );
				NOTESPACE();
				DATAPTRARG( src->file );
			
			/* Just continue on. */
		}
		
		if( lib4_stdmemfuncs.dealloc )
		{
			return( lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, src ) );
		}
	}
	
	TRESPASSPATH( discard_source, "ERROR: discard_source() failed to exit before reaching it's end." );
	return( -1 );
}



char_result charin( refed_pstr **refresh_srcname, uintmax_t *prog )
{
	char val;
	
	if( refresh_srcname && !prog )
	{
		LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_DOMAIN );
	}
	
	if( !inbuf.used )
	{
		int res = fgetc( sources->file );
		while( res == EOF && sources )
		{
				/* Delink. */
			source *tmp = sources;
			sources = sources->prev;
			
			tmp->prev = (source*)0;
			res = discard_source( tmp );
			if( !res )
			{
				FAILEDINTFUNC( "discard_source", charin, res );
					NOTESPACE();
					DATAPTRARG( tmp );
				
				if( refresh_srcname && *refresh_srcname )
				{
					res = refed_pstr_incrrefs( *refresh_srcname );
					if( res < 0 )
					{
						FAILEDINTFUNC( "refed_pstr_incrrefs", charin, res );
							NOTESPACE();
							DATAPTRARG( *refresh_srcname );
					}
				}
				
				LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
			}
			
			if( refresh_srcname )
			{
				*refresh_srcname = sources->name;
				prog = sources->base + sources->progress;
			}
			res = fgetc( sources->file );
		}
		if( res == EOF )
		{
				/* Failure. */
			FAILEDINTFUNC( "fgetc", charin, res );
				NOTESPACE();
					/* This should be accurate REGARDLESS of which fgetc() */
					/*  call we're dealing with. */
				DATAPTRARG( sources->file );
			
			if( refresh_srcname && *refresh_srcname )
			{
				res = refed_pstr_incrrefs( *refresh_srcname );
				if( res < 0 )
				{
					FAILEDINTFUNC( "refed_pstr_incrrefs", charin, res );
						NOTESPACE();
						DATAPTRARG( *refresh_srcname );
				}
			}
			
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
		++( sources->progress );
		val = res;
		
	} else {
		
		--( inbuf.used );
		val = inbuf.pushedback->body[ inbuf.used ];
	}
	
	if( refresh_srcname && *refresh_srcname )
	{
		res = refed_pstr_incrrefs( *refresh_srcname );
		if( res < 0 )
		{
			FAILEDINTFUNC( "refed_pstr_incrrefs", charin, res );
				NOTESPACE();
				DATAPTRARG( *refresh_srcname );
			
			/* We should maybe have an error here, but I can't be bothered */
			/*  right now. */
		}
	}
	
		/* Success. */
	LIB4_CHARRESULT_RETURNSUCCESS( val );
}
char_result charpeek( int *isEOF )
{
	char val;
	
	if( isEOF )
	{
		*isEOF = 0;
	}
	
	if( !inbuf.used )
	{
		int res = fgetc( sources->file );
		if( res == EOF )
		{
			if( isEOF )
			{
				*isEOF = 1;
			}
			
				/* Failure. */
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_EOF );
		}
		
		++( sources->progress );
		val = res;
		
		res = charback( val );
		if( res != 1 )
		{
			FAILEDINTFUNC( "charback", charpeek, res );
				NOTESPACE();
				CHARARG( val );
			
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
	} else {
		
		val = inbuf.pushedback->body[ inbuf.used - 1 ];
	}
	
		/* Success. */
	LIB4_CHARRESULT_RETURNSUCCESS( val );
}
int char_dropeof( refed_pstr **refresh_srcname, uintmax_t *prog )
{
	if( refresh_srcname && !prog )
	{
		return( -2 );
	}
	if( inbuf.used )
	{
		return( -3 );
	}
	
	char_result res = charpeek( (int*)0 );
	if( res == LIB4_CHARRESULT_BUILDFAILURE( LIB4_RESULT_FAILURE_EOF ) )
	{
		/* Eof found! Rembember to only drop 1 level. */
		
		source *tmp = sources;
		sources = sources->prev;
		
		tmp->prev = (source*)0;
		int res2 = discard_source( tmp );
		if( !res2 )
		{
			FAILEDINTFUNC( "discard_source", char_dropeof, res2 );
				NOTESPACE();
				DATAPTRARG( tmp );
			
			if( refresh_srcname && *refresh_srcname )
			{
				res2 = refed_pstr_incrrefs( *refresh_srcname );
				if( res2 < 0 )
				{
					FAILEDINTFUNC( "refed_pstr_incrrefs", char_dropeof, res2 );
						NOTESPACE();
						DATAPTRARG( *refresh_srcname );
				}
			}
			
			return( -4 );
		}
	}
	
	if( refresh_srcname && *refresh_srcname )
	{
			/* Don't forget: we need to track name reference counts! */
		*refresh_srcname = sources->name;
		prog = sources->base + sources->progress;
		res = refed_pstr_incrrefs( *refresh_srcname );
		if( res < 0 )
		{
			FAILEDINTFUNC( "refed_pstr_incrrefs", char_dropeof, res );
				NOTESPACE();
				DATAPTRARG( *refresh_srcname );
			
			return( -5 );
		}
	}
	
	return( 1 );
}
int charback( char val )
{
	if( inbuf.used >= inbuf.pushedback->len )
	{
		char_pascalarray_result res =
			char_pascalarray_rebuild( inbuf.pushedback, inbuf.used * 2 );
#define charback_SUCCESS( arr ) inbuf.pushedback = ( arr );
#define charback_FAILURE( err ) \
	MONADICFAILURE( charback, "char_pascalarray_rebuild()", ( err ) ); \
	return( err );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, charback_SUCCESS, charback_FAILURE )
	}
	
	inbuf.pushedback->body[ inbuf.used ] = val;
	++inbuf.used;
	
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
