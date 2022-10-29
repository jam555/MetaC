#include <ctype.h>


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


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
	#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	#define NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )
	
	#define DECARG( uint ) STDMSG_DECARG_WRAPPER( &errs, ( uint ) )
	#define DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )
#define FAILEDPTRFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDPTRFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )


int gentyped_tokencomp( const void *key_, const void *elem_ );
int gentyped_gencomp( const void *key1_, const void *key2_ );


int match_token( token *tok,  unsigned char *str, size_t len )
{
	if( len && !str )
	{
		TRESPASSPATH( match_token, "ERROR: match_token was given a null string pointer with a positive length." );
		return( -2 );
	}
	
	if( ( !tok ) != ( !str ) )
	{
		TRESPASSPATH( match_token, "ERROR: match_token encountered ( !tok ) != ( !str )." );
		return( -1 );
	}
	
	int ret = 1;
	
	if( tok )
	{
		unsigned l1 = tok->header.length, l2 = 0;
		
		while( l1 < tok->header.length && l2 < len && tok->text[ l1 ] == str[ l2 ] )
		{
			++l1;
			++l2;
		}
		
		if( tok->text[ l1 ] != str[ l2 ] )
		{
			ret = 0;
			
		} else if( tok->header.length != len )
		{
			ret = 2;
		}
	}
	
	return( ret );
}



int charparr_isname( char_parr *cparr )
{
	if( !cparr )
	{
		/* Error. */
		
		???
		
		return( -1 );
	}
	if( cparr->len <= 0 )
	{
		/* Error. */
		
		???
		
		return( -1 );
	}
	
	size_t iter = 0;
	
	if( cparr->body[ 0 ] != '_' && !isalpha( cparr->body[ 0 ] ) )
	{
		???
		
		return( 0 );
	}
	while( iter + 1 < cparr->len )
	{
		if( cparr->body[ 0 ] != '_' && !isalnum( cparr->body[ 0 ] ) )
		{
			???
			
			return( 0 );
		}
		
		++iter;
	}
	if( cparr->body[ iter ] != '\0' )
	{
		/* Error. */
		
		???
		
		return( -1 );
	}
	
	return( 1 );
}



	/* For use with qsort & bsearch. */
int stdcompare_str( char *a_, char *b_, size_t len )
{
	size_t loop = 0;
	char a, b;
	
	while( loop < len )
	{
		a = a_ ? a_[ loop ] : '\0';
		b = b_ ? b_[ loop ] : '\0';
		
		if( a < b )
		{
			return( -1 );
			
		} else if( a > b )
		{
			return( 1 );
		}
		
		++loop;
	}
	
	return( 0 );
}
int stdcompare_token( const void *key_, const void *elem_ )
{
	token *key = (token*)key_;
	genericnamed *elem = (genericnamed*)elem_;
	
	size_t
		loop = 0,
		l1 = key ? key->header.length : 0,
		l2 = elem->name ? elem->name->len : 0,
		l3 = ( l1 > l2 ) ? l1 : l2;
	
	if( !l1 && !l2 )
	{
		return( 0 );
	}
	
	return( stdcompare_str(
		( l1 ? key->text : (char*)0 ),
		( l2 ? elem->name->body : (char*)0 ),
		l3
	) );
}
int stdcompare_genericnamed( const void *a_, const void *b_ )
{
	genericnamed
		*a = (genericnamed*)a_,
		*b = (genericnamed*)b_;
	
	size_t
		loop = 0,
		l1 = a->name ? a->name->len : 0,
		l2 = b->name ? b->name->len : 0,
		l3 = ( l1 > l2 ) ? l1 : l2;
	
	if( !l1 && !l2 )
	{
		return( 0 );
	}
	
	return( stdcompare_str(
		( l1 ? a->name->body : (char*)0 ),
		( l2 ? b->name->body : (char*)0 ),
		l3
	) );
}



genname_parr* build_gennamearr( size_t count )
{
	genname_parr *a = (genericnamed_pascalarray*)0;
	
#define build_gennamearr_ONERR( err ) \
		MONADICFAILURE( build_gennamearr, "genericnamed_pascalarray_build()", err ); \
		NOTESPACE(); DECARG( count );
	genericnamed_pascalarray_result res =
		genericnamed_pascalarray_build( count );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETa,  build_gennamearr_ONERR )
	
	if( a )
	{
		while( count )
		{
			--count;
			
			a->body[ count ] = (genericnamed){ (char_pascalarray*)0, (void*)0, 0 };
		}
	}
	
	return( a );
}
genname_parr* resize_gennamearr( genname_parr *parr, size_t newlen )
{
	genname_parr *a = (genericnamed_pascalarray*)0;
	size_t oldlen = parr->len;
	
	genericnamed_pascalarray_result res =
		genericnamed_pascalarray_rebuild( parr, newlen );
#define resize_gennamearr_ONERR( err ) \
		MONADICFAILURE( resize_gennamearr, "genericnamed_pascalarray_rebuild()", err ); \
		NOTELINE(); DATAPTR( parr ); \
		NOTESPACE(); DECARG( newlen );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETa, resize_gennamearr_ONERR )
	
	if( a )
	{
		while( oldlen < newlen )
		{
			a->body[ oldlen ] = (genericnamed){ (char_pascalarray*)0, (void*)0, 0 };
			
			++oldlen;
		}
	}
	
	return( a );
}
void destroy_gennamearr( genname_parr *parr,  void (*release_ref)( genericnamed* ) )
{
	if( parr )
	{
		size_t l = 0;
		while( l < parr->len )
		{
			if( release_ref )
			{
				release_ref( ( parr->body ) + l );
			}
			
			++l;
		}
		
#define destroy_gennamearr_SUCCESS( var ) ;
#define destroy_gennamearr_ONERR( err ) \
	MONADICFAILURE( destroy_gennamearr, "genericnamed_pascalarray_destroy()", err ); \
	NOTELINE(); DATAPTR( parr );
		lib4_result res = genericnamed_pascalarray_destroy( parr );
		LIB4_RESULT_BODYMATCH( res, destroy_gennamearr_SUCCESS, destroy_gennamearr_ONERR )
	}
}

void qsort_gennamearr( genname_parr *parr )
{
	qsort( (void*)( parr->body ), parr->len, sizeof( genericnamed ), &stdcompare_genericnamed );
}
genericnamed* bsearch1_gennamearr( genname_parr *parr, token *tok )
{
	return( (genericnamed*)( bsearch( 
		(const void*)tok, (const void*)( parr->body ),
		parr->len, sizeof( genericnamed ),
		&stdcompare_token
			/* Note that this expects a token, and thus is NOT capable of working with qsort! */
	) ) );
}



int gentyped_tokencomp( const void *key_, const void *elem_ )
{
	uintptr_t
		k = ( (token*)key_ ) ? ( (token*)key_ )->header.toktype : TOKTYPE_INVALID,
		e = ( (generictyped*)elem_ ) ? ( (generictyped*)elem_ )->toktype : TOKTYPE_INVALID;
	
	if( k < e )
	{
		return( -1 );
		
	} else if( k > e )
	{
		return( 1 );
	}
	
	return( 0 );
}
int gentyped_gencomp( const void *key1_, const void *key2_ )
{
	uintptr_t
		k1 = ( (generictyped*)key1_ ) ? ( (generictyped*)key1_ )->toktype : TOKTYPE_INVALID,
		k2 = ( (generictyped*)key2_ ) ? ( (generictyped*)key2_ )->toktype : TOKTYPE_INVALID;
	
	if( k1 < k2 )
	{
		return( -1 );
		
	} else if( k1 > k2 )
	{
		return( 1 );
	}
	
	return( 0 );
}

void gentyped_qsort( gentyped_parr *parr )
{
	qsort( (void*)( parr->body ), parr->len, sizeof( generictyped ), &gentyped_gencomp );
}
generictyped* gentyped_bsearch( gentyped_parr *parr, token *tok )
{
	return( (generictyped*)( bsearch( 
		(const void*)tok, (const void*)( parr->body ),
		parr->len, sizeof( generictyped ),
		&gentyped_tokencomp
			/* Note that this expects a token, and thus is NOT capable of working with qsort! */
	) ) );
}



int lexparse1_pushsearchtable( genname_parrparr **keys, size_t *keys_used,  genname_parr *pushable )
{
	if( !keys || !keys_used )
	{
		BADNULL2( lexparse1_pushsearchtable, &keys, &keys_used );
		return( -1 );
	}
	
	if( ( *keys )->len <= *keys_used )
	{
		/* Grow the lookup stack. */
		
		genericnamedparr_pascalarray_result res =
			genericnamedparr_pascalarray_rebuild( *keys, ( *keys )->len + 1 );
#define lexparse1_tokensearch_SUCCESS( arr ) *keys = ( arr );
#define lexparse1_tokensearch_FAILURE( err ) \
	MONADICFAILURE( lexparse1_pushsearchtable, "genericnamedparr_pascalarray_rebuild()", err ); \
	NOTELINE(); DATAPTR( *keys ); \
	NOTESPACE(); DECARG( ( *keys )->len + 1 ); \
	NOTESPACE(); NOTESPACE(); DATAPTR( pushable ); \
	return( -2 );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, lexparse1_tokensearch_SUCCESS, lexparse1_tokensearch_FAILURE )
	}
	
	( *keys )->body[ *keys_used ] = pushable;
	++( *keys_used );
	
	return( 1 );
}
int lexparse1_popsearchtable( genname_parrparr **keys, size_t *keys_used )
{
	if( !keys || !keys_used )
	{
		BADNULL2( lexparse1_popsearchtable, &keys, &keys_used );
		return( -1 );
	}
	
	--( *keys_used );
	
	return( 1 );
}
retframe lexparse1_tokensearch
(
	stackpair *stkp,
	
	genname_parrparr **keys,
	size_t *keys_used,
	
	retframe seekother
)
{
	uintptr_t a;
	
	if( !keys || !( *keys ) || ( *keys )->len < 1 || !keys_used )
	{
		BADNULL2( lexparse1_tokensearch, &keys, &keys_used );
		NOTESPACE(); DATAPTR( keys );
		if( keys )
		{
			NOTESPACE();
			DATAPTR( &( ( *keys )->len ) );
		}
		
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( *keys_used <= 0 )
	{
		BADNULL( lexparse1_tokensearch, keys_used );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !( ( *keys )->body[ *keys_used ] ) )
	{
		BADNULL( lexparse1_tokensearch, &( ( *keys )->body[ *keys_used ] ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	int res = peek_uintptr( stkp->data,  &a );
	if( !res )
	{
		FAILEDUINTFUNC( "peek_uintptr", lexparse1_tokensearch, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	genericnamed *found =
		bsearch1_gennamearr
		(
			( *keys )->body[ *keys_used - 1 ],
			(token*)a
		);
	if( !found )
	{
		FAILEDPTRFUNC( "bsearch1_gennamearr", lexparse1_tokensearch, (void*)found );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	switch( found->reftype )
	{
		case GENNAMETYPE_RETFRAMEFUNC:
			/* Found a function, proceed to return it directly. */
			
			return( *( (retframe*)( found->ref ) ) );
		case GENNAMETYPE_TABLEENTRY:
			/* Found a lookup table, push it onto search stack. */
			
			switch
			(
				lexparse1_pushsearchtable
				(
					keys,
					keys_used,
					
					(genname_parr*)( found->ref )
				)
			)
			{
				case 1:
					break;
				case -1:
				case -2:
					return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( stkp,  &errs, lexparse1_tokensearch, res, stack_ENDRETFRAME );
		default:
			return( seekother );
	}
	
		/* Shouldn't ever get here, but whatever. */
	TRESPASSPATH( lexparse1_tokensearch, "lexparse1_tokensearch() is meant to exit in the final switch()!" );
	return( (retframe){ &end_run, (void*)0 } );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif



/****************************************************************************/
/****************************************************************************/
/** !!! WARNING !!! The handler functions must remove the tokens they use, **/
/**  *_tokensearch() DOES NOT do so! ****************************************/
/****************************************************************************/
/****************************************************************************/

	/* This one is meant to be fixed, not change. */
genname_parr *lexparse1_octo_keys;
