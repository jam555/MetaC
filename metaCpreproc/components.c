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



typedef struct macro_link macro_link;
struct macro_link
{
	token_head header;
	
		/* This identifies which element in the argument list should be */
		/*  used. */
	size_t args_offset;
};
typedef struct macro_token macro_token;
struct macro_token
{
	token_head header;
	
	token_head *tok;
};
typedef struct macro_run macro_run;
struct macro_run
{
	token_head header;
	
	macro_call *mac;
		/* The tokens should all be macro_link, macro_token, macro_run, or */
		/*  macro_directive instances. Each one represents a single */
		/*  argument. For the macro_run and macro_directive arguments, their */
		/*  own arguments are obtained from the same context as the */
		/*  macro_run or macro_directive they're being used as arguments */
		/*  FOR. */
	tokhdptr_parr *args;
};
typedef struct macro_directive macro_directive;
struct macro_directive
{
	token_head header;
	
		/* The handler will find it's arguments in the tokhdptr_parr* on the */
		/*  top of the stkp->data stack, but by the time of it's return MUST */
		/*  HAVE MOVED that pointer to the top of the macro_args stack- any */
		/*  actual deallocation will be donme elsewhere. It also MUST push a */
		/*  uintptr_t to the stkp->data stack saying how many token_head* */
		/*  instances it pushed onto the shuffle stack (whether it pushes */
		/*  anything to the shuffle stack is optional, but the top of the */
		/*  data stack MUST say how many token header pointers were pushed, */
		/*  regardless of the numbers): the directive should otherwise leave */
		/*  the stacks alone unless that is EXPLICITLY it's purpose. It's */
		/*  stack signature should thusly be as follows:*/
			/*
					tokenheadptr_pascalarray*
				--
					shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
			*/
		/* By doing this set of actions, the handler will have fully */
		/*  duplicated the required set of actions from the */
		/*  shufflequeue_entry_macro_call() sequence of functions. */
	retframe handler;
		/* Same as macro_run. */
	tokhdptr_parr *args;
};

typedef struct macro_call_argval macro_call_argval;
struct macro_call_argval
{
		/* One and ONLY one of these should be non-null. */
		/* ... */
		/* Actually, does args_offset even belong here? */
	size_t args_offset;
	char_pascalarray *text;
};
LIB4_DEFINE_PASCALARRAY_STDDEFINE( macrocallargval_, macro_call_argval );
typedef macrocallargval_pascalarray macrocllaval_parr;
typedef struct macro_call macro_call;
struct macro_call
{
	token_head header;
	
		/* This gets used while a macro is first getting parsed to figure */
		/*  out if a particular token corresponds to a macro arg or not, and */
		/*  furthermore WHICH arg. */
	macrocllaval_parr *args;
	
		/* This gets used to hold the tokens that represent the macro's */
		/*  body. */
	tokhdptr_parr *body;
	
	???;
};




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

#define STACKCHECK( stack,  refid, errnum, ... ) \
	if( !( stack ) ){ searchstack_ERREXIT( ( refid ), ( errnum ),  __VA_ARGS__  ); }

#define PEEK_UINT( stk, offset, var,  refid, err1, err2, ... ) \
	if( !peek_uintptr( ( stk ),  ( offset ),  &( var ) ) ) { searchstack_ERREXIT( ( refid ), ( err1 ),  __VA_ARGS__,  &( var ) ); } \
	if( !( var ) ) { searchstack_ERREXIT( ( refid ), ( err2 ),  __VA_ARGS__,  &( var ) ); }
#define POP_UINT( stk, var,  refid, err1, err2, ... ) \
	if( !pop_uintptr( ( stk ),  &( var ) ) ) { searchstack_ERREXIT( ( refid ), ( err1 ),  __VA_ARGS__,  &( var ) ); } \
	if( !( var ) ) { searchstack_ERREXIT( ( refid ), ( err2 ),  __VA_ARGS__,  &( var ) ); }
#define PUSH_UINT( stk, val,  refid, err, ... ) \
	if( !push_uintptr( ( stk ),  ( val ) ) ) { searchstack_ERREXIT( ( refid ), ( err ),  __VA_ARGS__, ( val ) ); }

#define PEEK_MACROARGS( offset, var,  refid, err1, err2, ... ) \
	if( !macroargs_peekset( ( offset ),  &( var ) ) ) { searchstack_ERREXIT( ( refid ), ( err1 ),  __VA_ARGS__, &( var ) ); } \
	if( !( var ) ) { searchstack_ERREXIT( ( refid ), ( err2 ),  __VA_ARGS__, &( var ) ); }
#define POP_MACROARGS( var,  refid, err1, err2, ... ) \
	if( !macroargs_popset( &( var ) ) ) { searchstack_ERREXIT( ( refid ), ( err1 ),  __VA_ARGS__, &( var ) ); } \
	if( !( var ) ) { searchstack_ERREXIT( ( refid ), ( err2 ),  __VA_ARGS__, &( var ) ); }
#define PUSH_MACROARGS( val,  refid, err, ... ) \
	if( !macroargs_pushset( val ) ) { searchstack_ERREXIT( ( refid ), ( err ),  __VA_ARGS__, ( val ) ); }

	/* This expects to be used within the context of a function which */
	/*  returns a retframe value. */
#define PUSH_SHUFFLE( res, token,  refid, err, ... ) \
	( res ) = token_queue_shufflepush( (token*)( token ) ); \
	if( !( res ) ) { searchstack_ERREXIT( ( refid ), ( err ),  __VA_ARGS__, &( res ) ); }


	/*
		(
				macro_call* tokenheadptr_pascalarray*
			--
				macro_call* tokenheadptr_pascalarray*
		)
	*/
retframe shufflequeue_generify_macro_call( stackpair *stkp, void *v )
{
	if( !stkp )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 1,  stkp, v );
	}
	
		/* This function ONLY exists to setup the call stack correctly. */
	CALL_FRAMEFUNC(
		&shufflequeue_generify_macro_call_conclude,
		(void*)0,
		
		&shufflequeue_entry_macro_call,
		(void*)0
	);
}
	/* ( shuffledlength --  ) */
retframe shufflequeue_generify_macro_call_conclude( stackpair *stkp, void *v )
{
	uintptr_t  a;
	
	if( !stkp )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 1,  stkp, v );
	}
	
		/* We only jumped into this function because we knew ahead of time */
		/*  that we didn't care how many tokens had been shuffled. So, we're */
		/*  just discarding that number. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 2,  stkp, v,  &arg_parr );
	}
	
	RET_FRAMEFUNC( head_lex_refid, -12, &th );
}

	/*
		Forth stack notation:
			(
					macro_call* tokenheadptr_pascalarray*
				--
					shufflebookmark macro_call* '0' macro_args:tokenheadptr_pascalarray*
			)
		Note that the tokenheadptr_pascalarray* is specifically a pointer to
		the pascal array that holds the actual arguments provided TO the macro
		in the macro call (as opposed to the names that those arguments are
		associated with within the body of the macro).
	*/
	/*
		Note also that the stack notation for the ENTIRE sequence of calls
		looks something like this:
			(
					macro_call* tokenheadptr_pascalarray*
				--
					shuffledlength tokensource:token*[]
			)
		with shuffledlength specifically identifying the total number of
		tokens that have been ADDED to the START of the standard token source.
	*/
retframe shufflequeue_entry_macro_call( stackpair *stkp, void *v )
{
	uintptr_t arg_parr, mcall;
	
	if( !stkp )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 1,  stkp, v );
	}
	
		/* Pull these two from the stack, since we need to preserve them. */
	if( !pop_uintptr( stk,  &arg_parr ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 2,  stkp, v,  &arg_parr );
	}
	if( !pop_uintptr( stk,  &mcall ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 3,  stkp, v,  &arg_parr, &mcall );
	}
		/* Just verification. */
	if( ( (token_head*)mcall )->toktype != TOKTYPE_TOKENGROUP_MACROCALL )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 4,  stkp, v,  &arg_parr, &mcall );
	}
	
		/* Push a bookmark for where the tokenqueue shuffle stack currently */
		/*  is: if we have macro calls embedded inside of other macrocalls, */
		/*  then this will make it MUCH easier to avoid causing buffer */
		/*  "overflow/underflow" errors. */
	{
		size_t used = token_queue_shuffleused();
		
		if( !push_uintptr( &( stkp->data ),  used ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 5,  stkp, v,  &arg_parr, &mcall, &used );
		}
	}
	
		/* Restore these two values back to the top of the stack, in the */
		/*  same order, with a "progress" indicator between them. */
	if( !push_uintptr( &( stkp->data ),  mcall ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 6,  stkp, v,  &arg_parr, &mcall );
	}
			/* Our progress? None! Because we haven't really started. */
		if( !push_uintptr( &( stkp->data ),  0 ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 7,  stkp, v,  &arg_parr );
		}
	if( !macroargs_pushset( arg_parr ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 8,  stkp, v,  &arg_parr );
	}
	
		/* We need to start executing the macro, so just returning is a no-no. */
	return( (retframe){ &shufflequeue_step_macro_call, (void*)0 } );
}

	/*
		Forth stack notation:
			(
					shufflebookmark macro_call* x macro_args:tokenheadptr_pascalarray*
				--
					shufflebookmark macro_call* x+1 ( token_header* |  ) macro_args:tokenheadptr_pascalarray*
			)
	*/
retframe shufflequeue_step_macro_call( stackpair *stkp, void *v )
{
	uintptr_t prog, mcall;
	tokhdptr_parr *arg_parr;
	
	if( !stkp )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 1,  stkp, v );
	}
	
		/* Pull these two from the stack, since we need to use and modify */
		/*  the "deeper" one, while otherwise preserving both. */
	if( !macroargs_peekset( 0,  arg_parr ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 2,  stkp, v,  arg_parr );
	}
	if( !pop_uintptr( stk,  &prog ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 3,  stkp, v,  arg_parr, &prog );
	}
	
	if( !peek_uintptr( stk,  0,  &mcall ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 4,  stkp, v,  arg_parr, &prog, &mcall );
	}
	
	++prog;
	if( !push_uintptr( &( stkp->data ),  prog ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 5,  stkp, v,  arg_parr, &prog, &mcall );
	}
	
	macro_call *mc = (macro_call*)mcall;
	
	if( !mc )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 7,  stkp, v,  arg_parr, &prog, &mc );
	}
	if( !( mc->body ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 8,  stkp, v,  arg_parr, &prog, &mc );
	}
	
	if( mc->body->len >= prog )
	{
		token_header *th = ( mc->body->body[ prog - 1 ] );
		
		if( !push_uintptr( &( stkp->data ),  (uintptr_t)th ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 9,  stkp, v,  arg_parr, &prog, &mc );
		}
		
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
				searchstack_ALERT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 9,  stkp, v,  arg_parr, &prog, &mc, &th );
					/* "hand" contains a reference to end_run, so no need to */
					/*  do anything other than break out of the switch. */
				break;
		}
		
		CALL_FRAMEFUNC(
			&shufflequeue_step_macro_call,
			(void*)0,
			
			hand,
			(void*)0
		);
		
	} else {
		
		return( (retframe){ &shufflequeue_exit_macro_call, (void*)0 } );
	}
}

	/* These should ONLY be executed within (and as a result of) an */
	/*  enclosing shufflequeue_entry_macro_call()/ */
	/*  shufflequeue_exit_macro_call() call pair: they configure and then */
	/*  de-configure the stack so that this will work correctly, and */
	/*  *_exit_*() also moves the stuff pushed onto the token shufflequeue */
	/*  onto the normal token get/unget queue so that they'll both be */
	/*  available for immediate usage, AND in the correct order for */
	/*  immediate usage. */
	
		/* A pointer to a macro_link should be on top of the stack, and a */
		/*  pointer to the tokhdptr_parr pascal array of actual arguments TO */
		/*  the macro should be on "macro_args". */
	retframe shufflequeue_macro_link( stackpair *stkp, void *v )
	{
		uintptr_t a;
		macro_link *mlink;
		tokhdptr_parr *thdptpr;
		int res;
		
		if( !stkp )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 1,  stkp, v );
		}
		
		if( !pop_uintptr( stk,  &a ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 2,  stkp, v,  &a );
		}
		if( !a )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 3,  stkp, v,  &a );
		}
		mlink = (macro_link*)a;
		if( mlink->header.toktype != TOKTYPE_TOKENGROUP_MACROLINK )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 4,  stkp, v,  &mlink );
		}
		
		if( !macroargs_peekset( 0,  &thdptpr ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 5,  stkp, v,  &mlink, &thdptpr );
		}
		if( !thdptpr )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 6,  stkp, v,  &mlink, &thdptpr );
		}
		
		if( thdptpr->len <= mlink->args_offset )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 7,  stkp, v,  &mlink, &thdptpr );
		}
			???
			/* Note that OUTWARDLY ONE token should be pused here. This is a */
			/*  SINGLE arg that we're dealing with, NOT multiple. */
		res = token_queue_shufflepush( (token*)( thdptpr->body[ mlink->args_offset ] ) );
		if( !res )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 8,  stkp, v,  &mlink, &thdptpr, &res );
		}
		
		RET_FRAMEFUNC( head_lex_refid, -12, &th );
	}
	
	retframe shufflequeue_macro_token( stackpair *stkp, void *v )
	{
		uintptr_t a;
		macro_token *mtok;
		int res;
		
		if( !stkp )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 1,  stkp, v );
		}
		
		if( !pop_uintptr( stk,  &a ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 2,  stkp, v,  &a );
		}
		if( !a )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 3,  stkp, v,  &a );
		}
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
		res = token_queue_shufflepush( (token*)( mtok->tok ) );
		if( !res )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_token, 6,  stkp, v,  &mtok, &res );
		}
		
		RET_FRAMEFUNC( head_lex_refid, -12, &th );
	}
	
		/* This is an adapter used to make macro calls from outside the */
		/*  scope of the shufflequeue_*_macro_call() functions. Very */
		/*  important for actually executing macros. Does still require */
			/* ( macro_run* macro_args:tokenheadptr_pascalarray* --) */
		/*  so that it'll have the arguments that the macro actually */
		/*  needs... */
	retframe shufflequeue_macro_runANDcollapse( stackpair *stkp, void *v )
	{
		???
		
			/* Setup *_conclude() as the return route, then proceed on */
			/*  to the actual macro execution. */
		CALL_FRAMEFUNC(
			&shufflequeue_macro_runANDcollapse_conclude,
			(void*)0,
			
			&shufflequeue_macro_run,
			(void*)0
		);
	}
		/* Should only be entered from shufflequeue_macro_run_conclude(). */
	retframe shufflequeue_macro_runANDcollapse_conclude( stackpair *stkp, void *v )
	{
			/* Something, something, return to caller... */
		???
		
		return( (retframe){ echo_tokens_entrypoint, (void*)0} );
	}
		/*
			(
					macro_run* macro_args:tokenheadptr_pascalarray*
				--
					macro_run* x args* macro_args:tokenheadptr_pascalarray*
			)
		*/
		/*
			This entire sub-sequence of calls produces this:
			(
					macro_run* macro_args:tokenheadptr_pascalarray*
				--
					shufflestack:token* macro_args:tokenheadptr_pascalarray*
			)
		*/
	retframe shufflequeue_macro_run( stackpair *stkp, void *v )
	{
		uintptr_t a;
		tokenheadptr_pascalarray *args = ???;
		macro_run *mrun;
		
		if( !stkp )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 1,  stkp, v );
		}
		
		
		if( !peek_uintptr( stk,  0,  &a ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_run, 2,  stkp, v,  &a );
		}
		mrun = (macro_run*)a;
		if( !mrun || !( mrun->mac ) || !( mrun->args ) )
		{
			/* Error. */
		}
		if( mrun->header.toktype != TOKTYPE_TOKENGROUP_MACRORUN )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_run, 4,  stkp, v,  &a );
		}
		
		if( mrun->args->len == 0 )
		{
			/* Early exit: no args needed, so just prep for the call. */
			
			push( args );
			return( (retframe){ & ???, (void*)0 } );
		}
		
		???
		
		if( !push_uintptr( &( stkp->data ),  0 ) )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 9,  stkp, v,  arg_parr, &prog, &mc );
		}
		push( args );
		
		???
		
		return( (retframe){ &shufflequeue_macro_run_body, (void*)0 } );
	}
		/*
			(
					macro_run* x args* macro_args:tokenheadptr_pascalarray*
				--
					(macro_run* x args* | macro_run* macro_call* args* ) macro_args:tokenheadptr_pascalarray*
			)
		*/
	retframe shufflequeue_macro_run_body( stackpair *stkp, void *v )
	{
		tokenheadptr_pascalarray *args;
		uintptr_t iter;
		macro_run *mr;
		
		if( !stkp )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call, 1,  stkp, v );
		}
		
		
		pop( &args );
		if !args )
		{
			/* Error. */
		}
		
		pop( &iter );
		
		peek( &mr );
		if( !mr || !( mr->mac ) || !( mr->args ) || mr->args->len < iter )
		{
			/* Error. */
		}
		if( mr->args->len == iter )
		{
			/* Done, proceed on to actual call. */
			
			if( !push_uintptr( &( stkp->data ),  (uintptr_t)( mr->mac ) ) )
			{
				searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 9,  stkp, v,  arg_parr, &prog, &mc );
			}
			if( !push_uintptr( &( stkp->data ),  (uintptr_t)args ) )
			{
				searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 9,  stkp, v,  arg_parr, &prog, &mc );
			}
			
			/*
				Total result of this CALL_FRAMEFUNC():
				(
						macro_call* tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
					--
						shuffledlength tokensource:token*[] macro_args:tokenheadptr_pascalarray*
				)
			*/
			/*
				Note that *_entry_macro_call has roughly the following
				signature:
				(
						macro_call* tokenheadptr_pascalarray*
					--
						shuffledlength tokensource:token*[]
				)
			*/
				/* Setup *_conclude() as the return route, then proceed on */
				/*  to the actual macro execution. */
			CALL_FRAMEFUNC(
				&shufflequeue_macro_run_conclude,
				(void*)0,
				
				&shufflequeue_entry_macro_call,
				(void*)0
			);
		}
		
		
		++iter;
		push( iter );
		
		push( args );
		
			/* Does... something? */
		???( mr->args->body[ iter ] );
		
		???
		
			/* One of these gets used to grab arguments from the "parent" */
			/*  arguments. Also, all but the lone "else" handle the macro_* */
			/*  token types, since that handles all of the stuff that we */
			/*  should need. */
		if( ??? )
		{
			return( (retframe){ ???, (void*)0 } );
			
		} else if( ??? )
		{
			return( (retframe){ ???, (void*)0 } );
			
		} else if( ??? )
		{
			return( (retframe){ ???, (void*)0 } );
			
		} else {
			
			/* Error. */
		}
	}
		/*
			(
					shuffledlength tokensource:token*[] macro_args:tokenheadptr_pascalarray*
				--
					shufflestack:token* macro_args:tokenheadptr_pascalarray*
			)
		*/
	retframe shufflequeue_macro_run_conclude( stackpair *stkp, void *v )
	{
		???
		
		/* Package everything up into a single token, then push back onto */
		/*  the token source? */
		
		???
		
		res = token_queue_shufflepush( (token*)( thdptpr->body[ mlink->args_offset ] ) );
		if( !res )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_macro_link, 8,  stkp, v,  &mlink, &thdptpr, &res );
		}
		
		???
	}
	
	retframe shufflequeue_macro_directive( stackpair *stkp, void *v )
	{
		macro_directive *mdir;
		
		TOKTYPE_TOKENGROUP_MACRODIRECTIVE
		
		???
	}
	
	/*
		Forth stack notation:
			(
					shufflebookmark macro_call* x macro_args:tokenheadptr_pascalarray* shufflestack:token*[]
				--
					shuffledlength tokensource:token*[]
			)
		Yes, upon exit, all of that stuff will be gone.
	*/
retframe shufflequeue_exit_macro_call( stackpair *stkp, void *v )
{
	uintptr_t a;
	tokhdptr_parr *args;
	int res;
	
	if( !stkp )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 1,  stkp, v );
	}
	
		/* Pop the pascal array of arguments provided to the macro first: */
		/*  these will need to be deallocated at some point. */
	if( !macroargs_popset( &args ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 2,  stkp, v,  &a );
	}
		/* Do something to deallocate those tokens. */
	???
	
		/* Now pop the progress marker: we're done, so it's done. Just a */
		/*  number, so no need to deallocate. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 3,  stkp, v,  &a );
	}
	
		/* Now the macro_call structure pointer. The macro_call itself is */
		/*  owned elsewhere, so no need to deallocate. Also, deallocating */
		/*  now could break something later. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 4,  stkp, v,  &a );
	}
	
		/* And now pop the shuffle stack/queue/whatever's bookmark, so that */
		/*  we'll know how many token pointers need to be moved from */
		/*  "shuffle" to the token get/unget stack. These will be */
		/*  deallocated elsewhere, so no need to do it here. */
	if( !pop_uintptr( stk,  &a ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 5,  stkp, v,  &a );
	}
	??? /* Actually, where DO these get freed? That might be a source of */
		/*  errors. */
	
	size_t used = token_queue_shuffleused(), moved = 0;
	while( used - moved > a )
	{
		res = token_queue_shuffle2queue();
		if( !res )
		{
			searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call, 6,  stkp, v,  &a, &used, &res );
		}
		
		++moved;
	}
		/* All of the tokens that got queued up should now be in the token */
		/*  source stream, and in correct order to boot. */
	
		/* Push the number of tokens that we shuffled onto the stack: some */
		/*  things need to know this for their own purposes. */
	if( !push_uintptr( &( stkp->data ),  (uintptr_t)moved ) )
	{
		searchstack_ERREXIT( REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call, 9,  stkp, v,  arg_parr, &prog, &mc );
	}
	
		/* Unlike the *_entry_*() function, we CAN just return like normal. */
	RET_FRAMEFUNC( head_lex_refid, -12, &th );
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
