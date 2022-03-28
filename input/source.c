#include "headers.h"

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

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), ( callername ), ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, ( funcname ), ( msgstr ) )



struct source
{
	source *prev;
	
	FILE *file;
	char_pascalarray *name;
	
		/* Max val is UINTMAX_MAX */
	uintmax_t base, progress;
	
} *sources = (source*)0;

#define COMMONINCLUDE "common_include.incl"


static root_name =
	LIB4_DEFINE_PASCALARRAY_LITERAL2( char_, char, COMMONINCLUDE );
static source root_src = (source){ (source*)0,  0, &root_name,  0, 0 };
int init()
{
	if( !sources )
	{
		root_src.file = fopen( root_src.name->body, "rb" );
		if( !root_src.file )
		{
			BADNULL( init, &( root_src.file ) );
			return( -2 );
		}
		
		sources = &root_src;
		
		return( 1 );
	}
	
	TRESPASSPATH( init, "ERROR: init() in source.c failed to exit before reaching it's end." );
	BADNONULL( init, &sources );
	return( -1 );
}
void deinit()
{
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
		
		root_src = (source){ (source*)0,  0, &root_name,  0, 0 };
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
				( lib4_stdmemfuncs.data, sizeof( stackframe ) );
		LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_source2_ERR1 )
		if( a )
		{
			*a = (source){ (source*)0,  (FILE*)0, (char_pascalarray*)0,  inclusionpoint, 0 };
		}
		
		char_pascalarray *b = (char_pascalarray*)0;
		char_pascalarray_result res =
			char_pascalarray_build( len );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETb, build_source2_ERR2 )
		if( b )
		{
			--len;
			b->body[ len ] = '\0';
			
			while( len )
			{
				--len;
				b->body[ len ] = name->body[ len ];
			}
			
			a->name = b;
			a->file = fopen( b->name->body, opentype );
			
		} else {
			
			if( lib4_stdmemfuncs.dealloc )
			{
				lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, a );
			}
			a = (source*)0;
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
	if( src )
	{
		src->prev = (source*)0;
		
#define pop_frame_SUCCESS( var ) ;
#define pop_frame_FAILURE( err ) \
	MONADICFAILURE( discard_source, "char_pascalarray_destroy()", ( err ) ); \
	return( err );
		lib4_result res = char_pascalarray_destroy( src->name );
		LIB4_RESULT_BODYMATCH( res, pop_frame_SUCCESS, pop_frame_FAILURE )
		
		int res2 = fclose( src->file );
		if( res2 != 0 )
		{
			FAILEDINTFUNC( "fclose", discard_source, res2 );
			
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



struct
{
	char_pascalarray *pushedback;
	size_t used;
	
} inbuf;


char_result charin()
{
	char val;
	
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
				
				LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
			}
			
			res = fgetc( sources->file );
		}
		if( res == EOF )
		{
				/* Failure. */
			FAILEDINTFUNC( "fgetc", charin, res );
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
		++( sources->progress );
		val = res;
		
	} else {
		
		--( inbuf.used );
		val = inbuf.pushedback->body[ inbuf.used ];
	}
	
		/* Success. */
	LIB4_CHARRESULT_RETURNSUCCESS( val );
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
