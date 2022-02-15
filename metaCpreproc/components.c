#include "../headers.h"

/* This file exists for "component parses" that get built up into more full */
/*  preprocessor parsing runs. */

uintptr_t searchstack_refid;
genname_parrparr *searchstack = (genname_parrparr*)0;
size_t searchstack_used = 0;
genname_parr default =
{
	(size_t)???, /* Length. */
	(genericnamed[])
	{
		(genericnamed){ (char_pascalarray *)0, (void*)0, (uintptr_t)0 },
		(genericnamed){ (char_pascalarray *)0, (void*)0, (uintptr_t)0 },
		...
	}
};

int searchstack_init()
{
	/* Initialize "searchstack" to contain precisely a single reference to */
	/*  "default". */
	
	genname_parrparr *a = (genname_parrparr*)0;
	
	if( searchstack )
	{
		err_interface( &searchstack_refid, REFID_SUBIDS_searchstack__init, 2,  &searchstack );
		return( -2 );
	}
	
#define searchstack_init_ERR( val ) \
		err_interface( &searchstack_refid, REFID_SUBIDS_searchstack__init, 3,  &searchstack, ( val ) ); \
		return( -3 );
	genericnamedparr_pascalarray_result gnpp =
		genericnamedparr_pascalarray_build( &lib4_stdmemfuncs,  4 );
	LIB4_MONAD_EITHER_BODYMATCH( gnpp, LIB4_OP_SETa, searchstack_init_ERR );
	
	if( !a )
	{
		err_interface( &searchstack_refid, REFID_SUBIDS_searchstack__init, 4,  &searchstack, &gnpp );
		return( -4 );
	}
	searchstack = a;
	searchstack->body[ 0 ] = &default;
	searchstack_used = 1;
	
	return( 1 );
}
int searchstack_deinit()
{
	???
}
#define searchstack_ERREXIT( key, ... ) \
	err_interface( &searchstack_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
	return( (retframe){ &end_run, (void*)0 } )
#define searchstack_ALERT( key, ... ) \
	err_interface( &searchstack_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ );

	/* This shouldn't appear directly in any search tables, but instead */
	/*  should be returned in a retframe by another wrapper function, which */
	/*  should also provide a non-null value for the void pointer in the */
	/*  process. */
retframe components_stdsearchinit( stackpair *stkp, void *v_ )
{
	uintptr_t a;
	token_head *tok;
	gennameparr_stdpushentry_entry *v;
	
	if( !stkp || !v_ )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 1,  stkp, v_ );
	}
	v = (gennameparr_stdpushentry_entry*)v_;
	
	
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 2,  stkp, v,  &a );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 3,  stkp, v,  &a );
	}
	tok = (token_head*)a;
		/* tok will now point to the token that caused this function to be */
		/*  run. */
	
		/* Just error detection. */
	if
	(
		tok->toktype != v->toktype &&
		(
			tok->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE ||
			( (tokenbranch*)tok )->subtype != v->toktype
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 4,  stkp, v,  &tok );
	}
	
		/* Build the tokenbranch that'll be used to package the context of */
		/*  the searchtable. */
	tokenbranch *tb = build_tokenbranch( &searchstack_refid,  0 );
	if( !tb )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 5,  stkp, v,  &tok );
	}
	tb->lead = tok;
	
		/* There should be a tokengroup at the top of the stack right now: */
		/*  let's confirm it's presence. */
	if( !peek_uintptr( stk,  0,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 6,  stkp, v,  &tb, &a );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 7,  stkp, v,  &tb, &a );
	}
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 8,  stkp, v,  &tb, &a );
	}
	
		/* We'll build a new tokengroup to receive the tokens that we */
		/*  encounter inside of the searchtable context. */
	tok =
		(token_head*)build_tokengroup
		(
			&searchstack_refid,
			
			size_t elems
		);
	if( !tok )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 9,  stkp, v,  &tb );
	}
	
	
		/* Push the new tokenbranch for use when closing out this */
		/*  searchtable context. */
	if( !push_uintptr( &( stkp->data ),  (uintptr_t)tb ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 10,  stkp, v,  &tb, &tok );
	}
		/* Push the new tokengroup for use WHILE IN this searchtable */
		/*  context. */
	if( !push_uintptr( &( stkp->data ),  (uintptr_t)tok ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 11,  stkp, v,  &tok );
	}
	
	
		/* Push the searchtable itself, so that we'll use the right matches. */
	if
	(
		!lexparse1_pushsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_stdsearchinit__pushfunc,
			
			v->table
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 12,  stkp, v,  &searchstack, &searchstack_used );
	}
	
	
	return( v->followup );
}
retframe components_sequencedsearchproceed( stackpair *stkp, void *v_ )
{
	/* Note to self: finish reviewing this function- it's pieces have been */
	/*  copied'n'pasted without much attempt to rationalize them. */
	
	uintptr_t a;
	token_head *tok;
	gennameparr_stdpushentry_entry *v;
	
	if( !stkp || !v_ )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_sequencedsearchproceed, 1,  stkp, v_ );
	}
	v = (gennameparr_stdpushentry_entry*)v_;
	
		/* The searchtable that we've been using is no longer correct, so pop it. */
	if
	(
		!lexparse1_popsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_sequencedsearchproceed__popfunc
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_sequencedsearchproceed, 2,  stkp, v,  &searchstack, &searchstack_used );
	}
	
		/* The token that matched. */
	if( !peek_uintptr( stk,  0,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_sequencedsearchproceed, 3,  stkp, v,  &a );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_sequencedsearchproceed, 4,  stkp, v,  &a );
	}
	tok = (token_head*)a;
		/* Just error detection. */
	if
	(
		tok->toktype != v->toktype &&
		(
			tok->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE ||
			( (tokenbranch*)tok )->subtype != v->toktype
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_sequencedsearchproceed, 5,  stkp, v,  &tok );
	}
	
		/* Push the next searchtable that we'll be using. */
	if
	(
		!lexparse1_pushsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_sequencedsearchproceed__pushfunc,
			
			v->table
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_sequencedsearchproceed, 6,  stkp, v,  &searchstack, &searchstack_used, &tok );
	}
	
		/* followup should PROBABLY itself return a retframe to a standard */
		/*  searcher function. */
	CALL_FRAMEFUNC(
		v->conclude.handler,
		v->conclude.data,
		
		v->followup.handler,
		v->followup.data
	);
}
	/* Use this if you care about the token that caused the function to be */
	/*  called. */
retframe components_stdsearchdeinit( stackpair *stkp, void *v_ )
{
	uintptr_t a;
	token_head *tok_a, *tok_b;
	gennameparr_stdpushentry_entry *v;
	
	if( !stkp || !v_ )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 1,  stkp, v_ );
	}
	v = (gennameparr_stdpushentry_entry*)v_;
	
		/* The ending token. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 2,  stkp, v,  &a );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 3,  stkp, v,  &a );
	}
	tok_a = (token_head*)a;
		/* Just error detection. */
	if
	(
		tok_a->toktype != v->toktype &&
		(
			tok_a->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE ||
			( (tokenbranch*)tok_a )->subtype != v->toktype
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchinit, 4,  stkp, v,  &a, &tok_a );
	}
	
		/* The result-body of the searchtable that we're going to pop. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 5,  stkp, v,  &a, &tok_a );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 6,  stkp, v,  &a, &tok_a );
	}
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 7,  stkp, v,  &a, &tok_a );
	}
	tok_b = (token_head*)a;
	
		/* And "finally" (for this step), the tokebranch that organizes all */
		/*  of that stuff.  */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 8,  stkp, v,  &a, &tok_a, &tok_b );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 9,  stkp, v,  &a, &tok_a, &tok_b );
	}
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 10,  stkp, v,  &a, &tok_a, &tok_b );
	}
	{
			/* And now we actually store it all. */
		tokenbranch *tb = (tokenbranch*)a;
		tb->body = tok_b;
		tb->tail = tok_a;
		
		
			/* And now we reset our tracking for the next stage of our work. */
		tok_a = &( tb->header );
	}
		/* Peek instead of pop because we actually DON'T want to remove this */
		/*  one from the stack. It's "owned" by another context, so we need */
		/*  to NOT interfere with it's "lifetime". */
	if( !peek_uintptr( stk,  0,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 11,  stkp, v,  &a, &tok_a );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 12,  stkp, v,  &a, &tok_a );
	}
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 13,  stkp, v,  &a, &tok_a );
	}
	tok_b = (token_head*)a;
		/* Now we store the tokenbranch into the parent context's */
		/*  tokengroup. */
	if
	(
		pushto_tokengroup
		(
			&searchstack_refid,
			REFID_SUBIDS_searchstack__components_stdsearchdeinit__pushfunc,
			
			(tokengroup*)tok_b,
			tok_a
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 14,  stkp, v,  &tok_a, &tok_b );
	}
	
		/* Pop the actual searchtable: it has no business being used to */
		/*  search anymore. */
	if
	(
		!lexparse1_popsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_stdsearchdeinit__popfunc
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_stdsearchdeinit, 15,  stkp, v,  &tok_a, &tok_b, &searchstack, &searchstack_used );
	}
	
	
		/* Now we transfer control to the "processing function" for the work */
		/*  we've assembled. It'll need to grab the relevant tokenbranch */
		/*  from the end of the tokengroup that's linked on top of the */
		/*  stack. */
	return( v->conclude );
}
	/* Use this if you DON'T care about the token that caused this function */
	/*  to be called. */
retframe components_skipendersearchdeinit( stackpair *stkp, void *v_ )
{
	uintptr_t a, b;
	token_head *tok;
	gennameparr_stdpushentry_entry *v;
	
	if( !stkp || !v_ )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 1,  stkp, v_ );
	}
	v = (gennameparr_stdpushentry_entry*)v_;
	
		/* The ending token. */
	if( !pop_uintptr( stk,  &b ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 2,  stkp, v,  &b );
	}
	if( !b )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 3,  stkp, v,  &b );
	}
		/* Just error detection. */
	if
	(
		( (token_head*)b )->toktype != v->toktype &&
		(
			( (token_head*)b )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE ||
			( (tokenbranch*)( (token_head*)b ) )->subtype != v->toktype
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 4,  stkp, v,  &b );
	}
	
		/* The result-body of the searchtable that we're going to pop. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 5,  stkp, v,  &b, &a );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 6,  stkp, v,  &b, &a );
	}
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 7,  stkp, v,  &b, &a );
	}
	tok = (token_head*)a;
	
		/* And "finally" (for this step), the tokebranch that organizes all */
		/*  of that stuff.  */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 8,  stkp, v,  &b, &a, &tok );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 9,  stkp, v,  &b, &a, &tok );
	}
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 10,  stkp, v,  &b, &a, &tok );
	}
	{
			/* And now we actually store what we care about. */
		tokenbranch *tb = (tokenbranch*)a;
		tb->body = tok;
		
		
			/* And now we reset our tracking for the next stage of our work. */
		tok = &( tb->header );
	}
		/* Peek instead of pop because we actually DON'T want to remove this */
		/*  one from the stack. It's "owned" by another context, so we need */
		/*  to NOT interfere with it's "lifetime". */
	if( !peek_uintptr( stk,  0,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 11,  stkp, v,  &b, &a, &tok );
	}
	if( !a )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 12,  stkp, v,  &b, &a, &tok );
	}
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 13,  stkp, v,  &b, &a, &tok );
	}
		/* Now we store the tokenbranch into the parent context's */
		/*  tokengroup. */
	if
	(
		pushto_tokengroup
		(
			&searchstack_refid,
			REFID_SUBIDS_searchstack__components_skipendersearchdeinit__pushfunc,
			
			(tokengroup*)( (token_head*)a ),
			tok
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 14,  stkp, v,  &b, &a, &tok );
	}
	
		/* Pop the actual searchtable: it has no business being used to */
		/*  search anymore. */
	if
	(
		!lexparse1_popsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_skipendersearchdeinit__popfunc
		)
	)
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 15,  stkp, v,  &b, &a, &searchstack, &searchstack_used );
	}
	
	
		/* Restore the "triggering token" pointer to the stack, so that we */
		/*  can deallocate it with the standard functions for the purpose. */
	if( !push_uintptr( &( stkp->data ),  b ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_skipendersearchdeinit, 16,  stkp, v,  &b, &a );
	}
	
	
		/* Register the "processing function" for the assembled work as the */
		/*  return route for the standard token deallocator, and then */
		/*  proceed to said deallocator. */
		/* Note that the processing function will need to grab the relevant */
		/*  tokenbranch from the end of the tokengroup that's linked on what */
		/*  WILL BE the top of the stack. */
	CALL_FRAMEFUNC(
		v->conclude.handler,
		v->conclude.data,
		
		&invoke_dealloctoken,
		(void*)0
	);
}



/* These macroargs_* things really belong in their own pair of files. */

LIB4_DEFINE_PASCALARRAY_STDDEFINE( tokhdptparpt_, tokhdptr_parr* );
typedef tokhdptparpt_pascalarray tokhdptparpt_parr;
tokhdptparpt_parr *macro_args = (tokhdptparpt_parr*)0;
size_t macro_args_used = 0;

int macroargs_init()
{
	if( macro_args || macro_args_used )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used );
		return( -2 );
	}
	
#define macroargs_init_SUCC( arr ) \
		macro_args = ( arr ); macro_args_used = 0;
#define macroargs_init_FAIL( err ) \
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, ( err ) ); \
		return( -3 );
	
	tokhdptparpt_pascalarray_result res =
		tokhdptparpt_pascalarray_build( 64 );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, macroargs_init_SUCC, macroargs_init_FAIL )
	
	return( 1 );
}
int macroargs_resize( int deltaMacros )
{
	int len = macro_args->len + deltaMacros;
	
	if( !macro_args )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &deltaMacros, &len );
		return( -2 );
	}
	
	if( len < 0 )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &deltaMacros, &len );
		return( -3 );
	}
	
#define macroargs_resize_SUCC( arr ) \
		macro_args = ( arr );
#define macroargs_resize_FAIL( err ) \
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &len, ( err ) ); \
		return( -4 );
	
	tokhdptparpt_pascalarray_result res =
		tokhdptparpt_pascalarray_rebuild( macro_args, len );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, macroargs_resize_SUCC, macroargs_resize_FAIL );
	
	if( macro_args_used > len )
	{
		macro_args_used = len;
	}
	
	return( 1 );
}
int macroargs_clear()
{
	if( !macro_args )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used );
		return( -2 );
	}
	
#define macroargs_resize_SUCC( arr ) ;
#define macroargs_resize_FAIL( err ) \
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &res, ( err ) ); \
		return( -3 );
	
	lib4_result res = tokhdptparpt_pascalarray_destroy( macro_args );
	LIB4_RESULT_BODYMATCH( res, macroargs_resize_SUCC, macroargs_resize_FAIL );
	
	macro_args = (tokhdptparpt_parr*)0;
	macro_args_used = 0;
	
	return( 1 );
}

int macroargs_pushset( tokhdptr_parr *val )
{
	if( !val )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &val );
		return( -1 );
	}
	if( !macro_args )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &val );
		return( -2 );
	}
	
	if( macro_args_used >= macro_args->len )
	{
		int res = macroargs_resize( macro_args->len );
		
		if( res < 0 )
		{
			searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &val, &res );
			return( -3 );
		}
	}
	
	macro_args->body[ macro_args_used ] = val;
	++macro_args_used;
	
	return( 1 );
}
int macroargs_popset( tokhdptr_parr **dest )
{
	if( !macro_args || !macro_args_used )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &dest );
		return( -2 );
	}
	
	--macro_args_used;
	
	if( dest )
	{
		*dest = macro_args->body[ macro_args_used ];
		
		return( 2 );
	}
	
		/* Not per-say an error, just a warning. */
	searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &( macro_args->body[ macro_args_used ] ) );
	return( 1 );
}
int macroargs_peekset( size_t off,  tokhdptr_parr **dest )
{
	if( !macro_args || !macro_args_used )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &dest, &off );
		return( -2 );
	}
	if( !dest || off >= macro_args_used )
	{
		searchstack_ALERT( key, ...,  &macro_args, &macro_args_used, &dest, &off );
		return( -1 );
	}
	
	*dest = macro_args->body[ macro_args_used - ( off + 1 ) ];
	
	return( 1 );
}


	/*
			shufflecount shuffle:token*[shufflecount]
		--
			arglist*
	*/
int pack_arglist( stackpair *stkp,  uintptr_t *refid, int errgate )
{
	uintptr_t shufcount;
	token *tmp;
	tokenheadptr_pascalarray *a;
	
	if( !stkp )
	{
		searchstack_ALERT( refid, errgate, 1,  stkp, (void*)0 );
		return( -1 );
	}

	if( !pop_uintptr( &( stkp->data ),  &shufcount ) )
	{
		searchstack_ALERT( refid, errgate, 2,  stkp, (void*)0,  &shufcount );
		return( -2 );
	}
	
#define pack_arglist_FAIL( err ) \
		searchstack_ALERT( refid, errgate, 3,  stkp, (void*)0,  &shufcount, (err) ); \
		return( -3 );
	tokenheadptr_pascalarray_result res = tokenheadptr_pascalarray_build( shufcount );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETa, pack_arglist_FAIL );
	
	while( shufcount )
	{
		--shufcount;
		int res = token_queue_shufflepop( &tmp );
		if( !res )
		{
			searchstack_ALERT( refid, errgate, 4,  stkp, (void*)0,  &shufcount, &a, res, &tmp );
			return( -4 );
		}
		if( !tmp )
		{
			searchstack_ALERT( refid, errgate, 5,  stkp, (void*)0,  &shufcount, &a );
			return( -5 );
		}
		a->body[ shufcount ] = &( tmp->header );
	}
	
	if( !push_uintptr( &( stkp->data ),  (uintptr_t)a ) )
	{
		searchstack_ALERT( refid, errgate, 6,  stkp, (void*)0, &shufcount, &a );
		return( -6 );
	}
	
	return( 1 );
}
retframe vm_pop_macroarg( stackpair *stkp, void *v )
{
	STACKCHECK( stkp, REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 1, stkp, v );
	
	POP_MACROARGS( (tokhdptr_parr**)0,  refid, err1, err2, ... );
	
	RET_FRAMEFUNC( REFID_???, -12, &th );
}



	/*
			*macro_link macro_args:tokenheadptr_pascalarray* shuffle:token*[]
		--
			macro_args:tokenheadptr_pascalarray* shuffle:token*[]+(arglist[subcall->link])
	*/
	/* A pointer to a macro_link should be on top of the stack, and a */
	/*  pointer to the tokhdptr_parr pascal array of actual arguments TO */
	/*  the macro should be on "macro_args". */
retframe shufflequeue_macro_link( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_link *mlink;
	tokhdptr_parr *thdptpr;
	int res;
	
	STACKCHECK( stkp, REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 1, stkp, v );
	
	POP_UINT( &( stkp->data ), a, REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 2, 3, stkp, v );
	mlink = (macro_link*)a;
	if( mlink->header.toktype != TOKTYPE_TOKENGROUP_MACROLINK )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 4,  stkp, v,  &mlink );
	}
	
	PEEK_MACROARGS( 0, thdptpr, REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 5, 6, stkp, v,  &mlink );
	if( thdptpr->len <= mlink->args_offset )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 7,  stkp, v,  &mlink, &thdptpr );
	}
		???
		/* Note that OUTWARDLY ONE token should be pushed here. This is a */
		/*  SINGLE arg that we're dealing with, NOT multiple. */
	PUSH_SHUFFLE( res, thdptpr->body[ mlink->args_offset ],  REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 8,  stkp, v,  &mlink, &thdptpr );
	
	RET_FRAMEFUNC( head_lex_refid, -12, &th );
}
	/*
			macro_token* args:*arglist shuffle:token*[]
		--
			args:*arglist shuffle:token*[]+(subcall->link)
	*/
retframe shufflequeue_macro_token( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_token *mtok;
	int res;
	
	STACKCHECK( stkp, REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 1, stkp, v );
	
	POP_UINT( &( stkp->data ), a, REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 2, 3, stkp, v );
	mtok = (macro_token*)a;
	if( mtok->header.toktype != TOKTYPE_TOKENGROUP_MACROTOKEN )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 4,  stkp, v,  &mtok );
	}
	if( !( mtok->tok ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 5,  stkp, v,  &mtok );
	}
		???
		/* Is this actually enough? Might need to ensure that a */
		/*  tokengroup is present, and push it's elements individually, */
		/*  or something similar. */
	PUSH_SHUFFLE( res, mtok->tok,  REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 6,  stkp, v,  &mtok );
	
	RET_FRAMEFUNC( head_lex_refid, -12, &th );
}
	/*
		(
				macro_run* macro_args:tokenheadptr_pascalarray*
			--
				macro_run* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
		)
	*/
	/*
		This entire sub-sequence of calls roughly produces this:
		(
				macro_run* macro_args:tokenheadptr_pascalarray*
			--
				macro_run* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
			--
				macro_run* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
			--
				macro_call* tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
			--
				shuffledlength shufflestack:token*[?] macro_args:
		)
	*/
retframe shufflequeue_macro_run( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_run *mr;
	
	STACKCHECK( stkp, REFID_SUBIDS_searchstack__???, 1, stkp, v );
	
	PEEK_UINT( &( stkp->data ), 0, a,  REFID_SUBIDS_searchstack__???, 3, 4, stkp, v );
	mr = (macro_run*)a;
	if( mr->header.toktype != TOKTYPE_TOKENGROUP_MACRORUN || !( mr->mac ) || !( mr->args ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__???, 5,  stkp, v,  &mr );
	}
	
		/* Prep for the call to shufflequeue_step_macro_call(). */
	{
		size_t used = token_queue_shuffleused();
		
		PUSH_UINT( &( stkp->data ), used,  REFID_SUBIDS_searchstack__???, 6,  stkp, v,  &mr );
	}
	PUSH_UINT( &( stkp->data ), mr->args,  REFID_SUBIDS_searchstack__???, 7,  stkp, v,  &mr );
	PUSH_UINT( &( stkp->data ), 0,  REFID_SUBIDS_searchstack__???, 8,  stkp, v,  &mr );
	
	CALL_FRAMEFUNC(
		&shufflequeue_macro_run_continue,
		(void*)0,
		
		&shufflequeue_step_macro_call,
		(void*)0
	);
}
		/*
				(
						macro_run* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
					--
						macro_call* tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
					--
						shuffledlength shufflestack:token*[?] macro_args:
				)
		*/
	retframe shufflequeue_macro_run_continue( stackpair *stkp, void *v )
	{
		uintptr_t args, b;
		macro_run *mr;
		
		STACKCHECK( stkp, REFID_SUBIDS_searchstack__???, 1, stkp, v );
		
			/*
					shuffledlength shufflestack:token*[shuffledlength]
				--
					arglist*
			*/
		if( !pack_arglist( stkp,  REFID_SUBIDS_searchstack__???, 2 ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__???, 3,  stkp, v,   );
		}
		POP_UINT( &( stkp->data ), args,  REFID_SUBIDS_searchstack__???, 4, 5, stkp, v );
		
		POP_UINT( &( stkp->data ), b,  REFID_SUBIDS_searchstack__???, 6, 7, stkp, v,  &args );
		mr = (macro_run*)b;
		if( mr->header.toktype != TOKTYPE_TOKENGROUP_MACRORUN || !( mr->mac ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__???, 8,  stkp, v,  &args, &mr );
		}
		PUSH_UINT( &( stkp->data ), mr->mac,  REFID_SUBIDS_searchstack__???, 9,  stkp, v,  &args, &mr );
		
		PUSH_UINT( &( stkp->data ), args,  REFID_SUBIDS_searchstack__???, 10,  stkp, v );
		
		CALL_FRAMEFUNC(
			&vm_pop_macroarg,
			(void*)0,
			
			&shufflequeue_entry_macro_call,
			(void*)0
		);
	}
		/*
			(
					macro_directive* macro_args:tokenheadptr_pascalarray*
				--
					macro_directive* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
			)
		*/
		/*
			This entire sub-sequence of calls roughly produces this:
			(
					macro_directive* macro_args:tokenheadptr_pascalarray*
				--
					macro_directive* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
				--
					macro_directive* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
				--
					tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
				--
					shuffledlength shufflestack:token*[?] macro_args:
			)
		*/
retframe shufflequeue_macro_directive( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_directive *mdir;
	
	STACKCHECK( stkp, REFID_SUBIDS_searchstack__???, 1, stkp, v );
	
	PEEK_UINT( &( stkp->data ), 0, a,  REFID_SUBIDS_searchstack__???, 3, 4, stkp, v );
	mdir = (macro_directive*)a;
	if( mdir->header.toktype != TOKTYPE_TOKENGROUP_MACRODIRECTIVE || !( mdir->handler ) || !( mdir->args ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__???, 5,  stkp, v,  &mdir );
	}
	
		/* Prep for the call to shufflequeue_step_macro_call(). */
	{
		size_t used = token_queue_shuffleused();
		
		PUSH_UINT( &( stkp->data ), used,  REFID_SUBIDS_searchstack__???, 6,  stkp, v,  &mdir );
	}
	PUSH_UINT( &( stkp->data ), mdir->args,  REFID_SUBIDS_searchstack__???, 7,  stkp, v,  &mdir );
	PUSH_UINT( &( stkp->data ), 0,  REFID_SUBIDS_searchstack__???, 8,  stkp, v,  &mdir );
	
	CALL_FRAMEFUNC(
		&shufflequeue_macro_directive_continue,
		(void*)0,
		
		&shufflequeue_step_macro_call,
		(void*)0
	);
}
		/*
				(
						macro_directive* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
					--
						tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
					--
						shuffledlength shufflestack:token*[?] macro_args:
				)
		*/
	retframe shufflequeue_macro_directive_continue( stackpair *stkp, void *v )
	{
		uintptr_t args, b;
		macro_directive *mdir;
		
		STACKCHECK( stkp, REFID_SUBIDS_searchstack__???, 1, stkp, v );
		
			/*
					shuffledlength shufflestack:token*[shuffledlength]
				--
					arglist*
			*/
		if( !pack_arglist( stkp,  REFID_SUBIDS_searchstack__???, 2 ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__???, 3,  stkp, v,   );
		}
		POP_UINT( &( stkp->data ), args,  REFID_SUBIDS_searchstack__???, 4, 5, stkp, v );
		
		POP_UINT( &( stkp->data ), b,  REFID_SUBIDS_searchstack__???, 6, 7, stkp, v,  &args );
		mdir = (macro_directive*)b;
		if( mdir->header.toktype != TOKTYPE_TOKENGROUP_MACRODIRECTIVE || !( mdir->handler ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__???, 8,  stkp, v,  &args, &mdir );
		}
		
		PUSH_UINT( &( stkp->data ), args,  REFID_SUBIDS_searchstack__???, 9,  stkp, v, &dir );
		
		CALL_FRAMEFUNC(
			&vm_pop_macroarg,
			(void*)0,
			
			mdir->handler.handler,
			mdir->handler.data
		);
	}



/*
	The signature of the whole call sequence:
	(
			*callbody args:*arglist
		--
			shufflecount shuffle:token*[shufflecount]
	)
*/
	/*
		Forth stack notation:
			(
					macro_call* tokenheadptr_pascalarray*
				--
					shufflebookmark tokhdptr_parr* '0' macro_args:tokenheadptr_pascalarray*
			)
		Note that the tokenheadptr_pascalarray* is specifically a pointer to
		the pascal array that holds the actual arguments provided TO the macro
		in the macro call (as opposed to the names that those arguments are
		associated with within the body of the macro).
	*/
retframe shufflequeue_entry_macro_call( stackpair *stkp, void *v )
{
	uintptr_t mcall;
	
	STACKCHECK( stkp, REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 1, stkp, v );
	
		/* Move the arguments to the argument stack. */
	POP_UINT( &( stkp->data ), mcall, REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 3, 4, stkp, v );
	PUSH_MACROARGS( mcall,  REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 5, stkp, v );
	
	POP_UINT( &( stkp->data ), mcall, REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 6, 7, stkp, v );
		/* Just verification. */
	if( ( (token_head*)mcall )->toktype != TOKTYPE_TOKENGROUP_MACROCALL )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 8,  stkp, v,  &arg_parr, &mcall );
	}
	
		/* Push a bookmark for where the tokenqueue shuffle stack currently */
		/*  is: if we have macro calls embedded inside of other macrocalls, */
		/*  then this will make it MUCH easier to avoid causing buffer */
		/*  "overflow/underflow" errors. */
	{
		size_t used = token_queue_shuffleused();
		
		PUSH_UINT( &( stkp->data ), used, REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 9,  stkp, v,  &mcall );
	}
	
		/* Push the body element from mcall instead of mcall itself, and */
		/*  then push a "0" progress indicator, since we haven't processed */
		/*  anything yet. */
	PUSH_UINT( &( stkp->data ), mcall->body,  REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 10,  stkp, v,  &mcall );
		/* Our progress? None! Because we haven't really started. */
	PUSH_UINT( &( stkp->data ), 0,  REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 11,  stkp, v,  &mcall );
	
	
	return( (retframe){ &shufflequeue_step_macro_call, (void*)0 } );
}

	/*
		Forth stack notation:
			(
					shufflebookmark tokhdptr_parr* x macro_args:tokenheadptr_pascalarray* shuffle:token*[?]
				--
					shufflebookmark tokhdptr_parr* x+1 ( token_header* |  ) macro_args:tokenheadptr_pascalarray* shuffle:token*[?]
			)
	*/
retframe shufflequeue_step_macro_calltool( stackpair *stkp, void *v,  retframe loop, retframe ret )
{
	uintptr_t prog, ops_;
	tokhdptr_parr *arg_parr;
	
	STACKCHECK( stkp,  REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 1,  stkp, v );
	
		/* Pull these two from the stack, since we need to use and modify */
		/*  the "deeper" one, while otherwise preserving both. */
	PEEK_MACROARGS( 0, arg_parr,  REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 2, 3, stkp, v );
		/* "0" is actually valid, so we can't use the usual macro. */
	if( !pop_uintptr( stk,  prog ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 4,  stkp, v,  arg_parr, &prog );
	}
	
	PEEK_UINT( stk,  0,  ops_,  REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 5, 6,  stkp, v,  arg_parr, &prog );
	tokhdptr_parr *ops = (tokhdptr_parr*)ops_;
	
	++prog;
	PUSH_UINT( &( stkp->data ), prog,  REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 7,  stkp, v,  arg_parr, &prog, &ops );
	
	if( ops->len >= prog )
	{
		token_header *th = ( ops->body[ prog - 1 ] );
		
		PUSH_UINT( &( stkp->data ),  (uintptr_t)th,  REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 8,  stkp, v,  arg_parr, &prog, &ops );
		
		framefunc hand = &end_run;
		switch( th->toktype )
		{
			case TOKTYPE_TOKENGROUP_MACROLINK:
				hand = &shufflequeue_macro_link;
				break;
			case TOKTYPE_TOKENGROUP_MACROTOKEN:
				hand = &shufflequeue_macro_token;
				break;
			case TOKTYPE_TOKENGROUP_MACRORUN:
				hand = &shufflequeue_macro_run;
				break;
			case TOKTYPE_TOKENGROUP_MACRODIRECTIVE:
				hand = &shufflequeue_macro_directive;
				break;
			
			default:
				searchstack_ALERT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 9,  stkp, v,  arg_parr, &prog, &ops, &th );
					/* "hand" contains a reference to end_run, so no need to */
					/*  do anything other than break out of the switch. */
				break;
		}
		
		CALL_FRAMEFUNC(
			loop.handler,
			loop.data,
			
			hand,
			(void*)0
		);
		
	} else {
		
		return( ret );
	}
}
	/* Just a wrapper. Will need an imitator for directives. */
retframe shufflequeue_step_macro_call( stackpair *stkp, void *v )
{
	return
	(
		shufflequeue_step_macro_calltool
		(
			stkp, v,
			
			(retframe){ &shufflequeue_step_macro_call, (void*)0 },
			retframe ret /* Should be shufflequeue_exit_macro_call() or similar. */
		)
	);
}
	
		/*
				shufflebookmark shuffle:token*[shufflecount]
			--
				shufflecount shuffle:token*[shufflecount]
		*/
retframe shufflequeue_exit_macro_calltool( stackpair *stkp, void *v )
{
	uintptr_t shufflebookmark;
	
	STACKCHECK( stkp,  REFID_SUBIDS_???, 1,  stkp, v );
	
		/* And now pop the shuffle stack/queue/whatever's bookmark, so that */
		/*  we'll know how many token pointers need to be moved from */
		/*  "shuffle" to the token get/unget stack. These will be */
		/*  deallocated elsewhere, so no need to do it here. */
	if( !pop_uintptr( stk,  &shufflebookmark ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_???, 5,  stkp, v,  &shufflebookmark );
	}
	??? /* Actually, where DO these get freed? That might be a source of */
		/*  errors. */
	
	size_t used = token_queue_shuffleused() - shufflebookmark;
		/* Push the number of tokens on the shuffle stack: some */
		/*  things need to know this for their own purposes. */
	PUSH_UINT( &( stkp->data ), shufflebookmark,  REFID_SUBIDS_???, 9,  stkp, v,  arg_parr, &prog, &mc );
	
	RET_FRAMEFUNC( refname, errnum, ... );
}
	/*
		Forth stack notation:
			(
					shufflebookmark tokhdptr_parr* x macro_args:tokenheadptr_pascalarray* shufflestack:token*[shuffledlength]
				--
					shuffledlength shufflestack:token*[shuffledlength]
			)
		Yes, upon exit, all of that stuff will be gone.
	*/
retframe shufflequeue_exit_macro_call( stackpair *stkp, void *v )
{
	uintptr_t a;
	tokhdptr_parr *args;
	
	STACKCHECK( stkp,  REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 1,  stkp, v );
	
		/* Pop the pascal array of arguments provided to the macro first: */
		/*  these will need to be deallocated at some point. */
	POP_MACROARGS( args,  REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 2, 3, stkp, v );
		/* Do something to deallocate those tokens. */
	???
	
		/* Now pop the progress marker: we're done, so it's done. Just a */
		/*  number, so no need to deallocate. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 3,  stkp, v,  &a );
	}
	
		/* Now the operations array. The array itself is */
		/*  owned elsewhere, so no need to deallocate. Also, deallocating */
		/*  now could break something later. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 4,  stkp, v,  &a );
	}
	
	return( shufflequeue_exit_macro_calltool( stkp, v ) );
}



	/*
			macro_call* tokenheadptr_pascalarray*
		--
			shuffledlength shufflestack:token*[shuffledlength]
		--
			source:token*
	*/
retframe shufflequeue_entry_macro_wrapper( stackpair *stkp, void *v )
{
	CALL_FRAMEFUNC(
		&shufflequeue_entry_macro_call,
		(void*)0,
		
		&shufflequeue_exit_macro_wrapper,
		(void*)0
	);
}
		/*
				shufflecount shuffle:token*[shufflecount]
			--
				source:token*
		*/
	retframe shufflequeue_exit_macro_wrapper( stackpair *stkp, void *v )
	{
		uintptr_t count;
		
		STACKCHECK( stkp,  REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 1,  stkp, v );
		
		POP_UINT( &( stkp->data ), count,  refid, err1, err2, ... )
		
		while( count )
		{
			--count;
			int res = token_queue_shuffle2queue();
			if( !res )
			{
				searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 6,  stkp, v,  count, res );
			}
		}
		
		/* The various produced tokens should now be in the token stream, */
		/*  AND in the correct order. */
		
		RET_FRAMEFUNC( refname, errnum, ... );
	}








typedef struct ???1 ???1;
struct ???1
{
	???;
};

typedef struct ???2 ???2;
struct ???2
{
	charparrptr_parr *argnames;
	
	???1_parr *tree;
};

retframe ???( stackpair *stkp, void *v )
{
	???
	
	if( !stkp )
	{
		preproc_ERREXIT( 2, (uintptr_t)1, &stkp, &table );
	}
	
	???
}



/*
	The top of the stack must be a pointer to a _SPACE, _NEWLINE, or _OTHER.
	The next elemet must be either a null pointer, or a pointer to NOT a
	_SPACE, _NEWLINE, _OTHER, or TOKTYPE_TOKENGROUP_WHITESPACE. Upon returning,
	it'll have a pointer to a TOKTYPE_TOKENGROUP_WHITESPACE on the top of the
	stack. Not actually sure if it'll work right.
retframe accumulate_whitespace( stackpair *stkp, void *v );

retframe accumulate_token( stackpair *stkp, void *v );

*/
