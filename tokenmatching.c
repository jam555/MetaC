#include "headers.h"



int match_token( token *tok,  unsigned char *str, size_t len )
{
	if( len && !str )
	{
		return( -2 );
	}
	
	if( !tok != !str )
	{
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



genname_parr* build_gennamearr( size_t count, uintptr_t *errkey )
{
	genname_parr *a = (genericnamed_pascalarray*)0;
	
#define build_gennamearr_ONERR( err ) \
		err_interface( &gennamearr_refid, (lib4_failure_result){ 0 }, errkey, err );
	genericnamed_pascalarray_result res =
		genericnamed_pascalarray_build( len );
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
		err_interface( &gennamearr_refid, (lib4_failure_result){ 1 }, errkey, err );
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
	err_interface( &gennamearr_refid, (lib4_failure_result){ 2 }, errkey, err );
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



#define lexparse1_ERREXIT( key, ... ) \
		err_interface( &lexparse1_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
int lexparse1_pushsearchtable( uintptr_t *refid, genname_parrparr **keys, size_t *keys_used, int err_subsource, int errsubsub, genname_parr *pushable )
{
	if( !keys || !keys_used )
	{
		err_interface( refid, err_subsource, (lib4_failure_result){ keys }, errsubsub, 0, keys_used );
		return( -1 );
	}
	
	if( ( *keys )->len <= *keys_used )
	{
		/* Grow the lookup stack. */
		
		genericnamedparr_pascalarray_result res =
			genericnamedparr_pascalarray_rebuild( *keys, ( *keys )->len + 1 );
#define lexparse1_tokensearch_SUCCESS( arr ) *keys = ( arr );
#define lexparse1_tokensearch_FAILURE( err ) \
	err_interface( refid, err_subsource, (lib4_failure_result){ keys }, errsubsub, 1, pushable, (err) ); \
	return( -2 );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, lexparse1_tokensearch_SUCCESS, lexparse1_tokensearch_FAILURE )
	}
	
	( *keys )->body[ *keys_used ] = pushable;
	++( *keys_used );
	
	return( 1 );
}
int lexparse1_popsearchtable( uintptr_t *refid, genname_parrparr **keys, size_t *keys_used, int err_subsource, int errsubsub )
{
	if( !keys || !keys_used )
	{
		err_interface( refid, err_subsource, (lib4_failure_result){ keys }, errsubsub, keys_used );
		return( -1 );
	}
	
	--( *keys_used );
	
	return( 1 );
}
retframe lexparse1_tokensearch
(
	stackframe *stk,
	
	uintptr_t *refid,
	
	genname_parrparr **keys,
	size_t *keys_used,
	
	int err_subsource,
	
	retframe seekother
)
{
	uintptr_t a;
	
	if( !keys || !( *keys ) || ( *keys )->len < 1 || !keys_used )
	{
		err_interface( refid, err_subsource, (lib4_failure_result){ keys }, 1, keys_used );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( *keys_used <= 0 )
	{
		err_interface( refid, err_subsource, (lib4_failure_result){ keys }, 2 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !( ( *keys )->body[ *keys_used ] ) )
	{
		err_interface( refid, err_subsource, (lib4_failure_result){ keys }, 3 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if( !peek_uintptr( stk,  &a ) )
	{
		err_interface( refid, err_subsource, (lib4_failure_result){ keys }, 4 );
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
		err_interface( refid, err_subsource, (lib4_failure_result){ keys }, 5, &a );
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
					refid,
					
					keys,
					keys_used,
					
					err_subsource,
					6,
					
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
			
			RET_FRAMEFUNC( ( *refid ), err_subsource, 7, found );
		default:
			return( seekother );
	}
	
		/* Shouldn't ever get here, but whatever. */
	err_interface( refid, err_subsource, (lib4_failure_result){ keys }, 8, found );
	return( (retframe){ &end_run, (void*)0 } );
}

/****************************************************************************/
/****************************************************************************/
/** !!! WARNING !!! The handler functions must remove the tokens they use, **/
/**  *_tokensearch() DOES NOT do so! ****************************************/
/****************************************************************************/
/****************************************************************************/

	/* This one is meant to be fixed, not change. */
genname_parr *lexparse1_octo_keys;
