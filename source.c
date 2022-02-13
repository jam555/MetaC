#include "headers.h"



typedef struct source source;
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
			return( -2 );
		}
		
		sources = &root_src;
		
		return( 1 );
	}
	
	return( -1 );
}
void deinit()
{
	if( sources == &root_src )
	{
		sources = (source*)0;
		
		if( root_src.file )
		{
			fclose( root_src.file );
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
	
#define build_source_DISCARD( val ) 
	
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
		LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_source_DISCARD )
		if( a )
		{
			*a = (source){ (source*)0,  (FILE*)0, (char_pascalarray*)0,  inclusionpoint, 0 };
		}
		
		char_pascalarray *b = (char_pascalarray*)0;
		char_pascalarray_result res =
			char_pascalarray_build( len );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETb, build_source_DISCARD )
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
#define pop_frame_FAILURE( err ) return( err );
		lib4_result res = char_pascalarray_destroy( src->name );
		LIB4_RESULT_BODYMATCH( res, pop_frame_SUCCESS, pop_frame_FAILURE )
		
		if( fclose( src->file ) != 0 )
		{
			/* Just ignore failure. */
		}
		
		if( lib4_stdmemfuncs.dealloc )
		{
			return( lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, src ) );
		}
	}
	
	return( -1 );
}



struct
{
	char_pascalarray *pushedback;
	size_t used;
	
} inbuf;

LIB4_MONAD_EITHER_BUILDTYPE_DEFINITION(
		char_result,
		
		char,
		lib4_failure_result
	);
		/* Note: the on* args must be the names of either functions or */
		/*  function macros, that take one arg of the relevant type. Use of */
		/*  the set* macros is advised, for simplicity. */
#define CHAR_RESULT_BODYMATCH( var, onsucc, onfail ) \
	LIB4_MONAD_EITHER_BODYMATCH( var, onsucc, onfail )
#define CHAR_RESULT_EXPRMATCH( var, onsucc, onfail ) \
	LIB4_MONAD_EITHER_EXPRMATCH( var, onsucc, onfail )


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
			if( !discard_source( tmp ) )
			{
				LIB4_MONAD_EITHER_RETURNRIGHT( char_result, lib4_failure_result, LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
			}
			
			res = fgetc( sources->file );
		}
		if( res == EOF )
		{
				/* Failure. */
			LIB4_MONAD_EITHER_RETURNRIGHT( char_result, lib4_failure_result, LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
		++( sources->progress );
		val = res;
		
	} else {
		
		--( inbuf.used );
		val = pushedbck->body[ inbuf.used ];
	}
	
		/* Success. */
	LIB4_MONAD_EITHER_RETURNLEFT( char_result, char, val );
}
int charback( char val )
{
	if( inbuf.used >= inbuf.pushedback->len )
	{
		char_pascalarray_result res =
			char_pascalarray_rebuild( inbuf.pushedback, inbuf.used * 2 );
#define charback_SUCCESS( arr ) inbuf.pushedback = ( arr );
#define charback_FAILURE( err ) return( err );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, charback_SUCCESS, charback_FAILURE )
	}
	
	pushedbck->body[ inbuf.used ] = val;
	++inbuf.used;
	
	return( 1 );
}
