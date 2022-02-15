#include "../headers.h"


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


uintptr_t gentypedarr_refid;


#define preproc_ERREXIT( key, ... ) \
		err_interface( &gentypedarr_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
	/* This is the actual basal lookup table. Searching is based on */
	/*  token_head's toktype value. If an entry's reftype is *_RETFRAMEFUNC */
	/*  then it'll get used directly as the return value of the search */
	/*  function, if it's *_TABLEENTRY then it'll get paired with */
	/*  preproc_search2() as the return vector for a call to */
	/*  accumulate_token(), and any other value results in a call to a */
	/*  "seekother" retframe value: the seekother value for both *_search1() */
	/*  and *_search2() currently just end execution, but that should be */
	/*  changed in the future. */
	/* If an entry's reftype is *_RETFRAMEFUNC, then it's ref must be a */
	/*  pointer to a retframe. If the reftype is *_TABLEENTRY, then it's ref */
	/*  must be a pointer to a genname_parr, the entries of which follow the */
	/*  same rules as preproc_basic_gentyping's entries, excepting that */
	/*  search is done via text value instead of toktype. */
gentyped_parr *preproc_basic_gentyping =
&(
	(gentyped_parr)
	{
			/* The number of elements in the generictyped[] array below, */
			/*  minus one because we don't want to acknowledge the very last */
			/*  element, because it's just there to increase the chance of */
			/*  catching errors. */
		element_count,
		(generictyped[])
		{
				/* Now we should be preprocessing! Except that I need to be */
				/*  checking for "line freshness". */
			(generictyped){ TOKTYPE_OCTO, (void*)ref, reftype },
			
			
			(generictyped){ toktype, (void*)ref, reftype },
			(generictyped){ toktype, (void*)ref, reftype },
			
			/* New elements go here! */
			
			
				/* Do not count this one, leave it at the end as well. */
			(generictyped){ TOKTYPE_INVALID, (void*)0, GENNAMETYPE_INVALID }
		}
	}
GENNAMETYPE_NAMEONLY
GENNAMETYPE_TABLEENTRY
GENNAMETYPE_RETFRAMEFUNC
);





retframe preproc_search1( stackpair *stkp, void *v )
{
	return( gentyped_search( stkp, preproc_basic_gentyping, &preproc_search2 ) );
}
retframe preproc_search2( stackpair *stkp, void *table_ )
{
		/* This is for the preprocessor, so if we encounter a character */
		/*  we're not looking for, then we'll just echo it. */
	static retframe seekother = (retframe){ &echo_tokens_entrypoint, (void*)0 };
	genname_parr *table = (genname_parr*)table_;
	uintptr_t a;
	
	if( !stkp )
	{
		preproc_ERREXIT( 2, (uintptr_t)1, &stkp, &table );
	}
	if( !table || table->len < 1 )
	{
		preproc_ERREXIT( 2, (uintptr_t)2, &stkp, &table );
	}
	
	if( !peek_uintptr( stk,  &a ) )
	{
		preproc_ERREXIT( 2, (uintptr_t)3, &stkp, &table, &a );
	}
	
	genericnamed *found = bsearch1_gennamearr( table->body, (token*)a );
	if( !found )
	{
		preproc_ERREXIT( 2, (uintptr_t)4, &stkp, &table, &a, &found );
	}
	
	switch( found->reftype )
	{
		case GENNAMETYPE_RETFRAMEFUNC:
			/* Found a function, proceed to return it directly. */
			
			return( *( (retframe*)( found->ref ) ) );
		case GENNAMETYPE_TABLEENTRY:
			/* Found a lookup table; queue the string searcher as a return */
			/*  route with it as an argument, and call accumulate_token() to */
			/*  fetch a token for it to interpret. */
			
			CALL_FRAMEFUNC( &preproc_search2, (void*)( found->ref ), &accumulate_token, 0 );
		default:
			return( seekother );
	}
	
	preproc_ERREXIT( 2, (uintptr_t)5, &stkp, &table, &a, &found );
}

retframe gentyped_search( stackpair *stkp, gentyped_parr *table, retframe (*tablehand)( stackpair*, void* ) )
{
		/* This is for the preprocessor, so if we encounter a character */
		/*  we're not looking for, then we'll just echo it. */
	static retframe seekother = (retframe){ &echo_tokens_entrypoint, (void*)0 };
	uintptr_t a;
	
	if( !stkp || !tablehand )
	{
		preproc_ERREXIT( 1, (uintptr_t)1, &stkp, &v );
	}
	
	if( !table || table->len < 1 )
	{
		preproc_ERREXIT( 1, (uintptr_t)2, &stkp, &v, &table );
	}
	
	if( !peek_uintptr( stk,  &a ) )
	{
		preproc_ERREXIT( 1, (uintptr_t)3, &stkp, &v, &table,  &a );
	}
	
	generictyped *found = gentyped_bsearch( table, (token*)a );
	if( !found )
	{
		preproc_ERREXIT( 1, (uintptr_t)4, &stkp, &v, &table,  &a, &found );
	}
	
	switch( found->reftype )
	{
		case GENNAMETYPE_RETFRAMEFUNC:
			/* Found a function, proceed to return it directly. */
			
			return( *( (retframe*)( found->ref ) ) );
			
		case GENNAMETYPE_TABLEENTRY:
			/* Found a lookup table; queue the string searcher as a return */
			/*  route with it as an argument, and call accumulate_token() to */
			/*  fetch a token for it to interpret. */
			
			CALL_FRAMEFUNC( tablehand, (void*)( found->ref ), &accumulate_token, 0 );
			
		default:
			return( seekother );
	}
	
		/* Shouldn't ever get here, but whatever. */
	preproc_ERREXIT( 1, (uintptr_t)8, &stkp, &v, &table,  &a, &found );
}
