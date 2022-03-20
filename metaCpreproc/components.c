#include "../headers.h"

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


#define CALLFRAMEFUNC( stkpair, rethand, retval, callhand, callval,  caller, scratch ) \
	CALL_FRAMEFUNC( stkpair, rethand, retval, callhand, callval,  &err, caller, scratch, macroargs_ENDRETFRAME )
#define RETFRAMEFUNC( stkpair,  caller, scratch ) \
	RET_FRAMEFUNC( stkpair,  &err, caller, scratch, macroargs_ENDRETFRAME )

#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
	#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	
	#define DECARG( val ) STDMSG_DECARG_WRAPPER( &errs, ( val ) )
	#define STRARG( str ) STDMSG_STRARG_WRAPPER( &errs, ( str ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )
#define FAILEDPTRFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDPTRFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )

#define PEEKMACROARGS( offset, dest,  caller, scratch, endfunc ) \
	PEEK_MACROARGS( offset, dest,  &err, caller, scratch, endfunc );
#define POPMACROARGS( destptr,  caller, scratch, endfunc ) \
	POP_MACROARGS( destptr,  &err, caller, scratch, endfunc )
#define PUSHMACROARGS( val,  caller, scratch, endfunc ) \
	PUSH_MACROARGS( val,  &err, caller, scratch, endfunc )


#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )
#define STACKCHECK2( stack, v,  caller, endfunc ) \
	STACK_CHECK2( ( stack ), ( v ),  &err, ( caller ), ( endfunc ) )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &err, ( caller ), ( scratch ), ( endfunc ) )


#define PUSHSHUFFLE( tokptr,  caller, scratch, endfunc ) \
	PUSH_SHUFFLE( tokptr,  &err, caller, scratch, endfunc )



/* This file exists for "component parses" that get built up into more full */
/*  preprocessor parsing runs. */

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


genname_parrparr *std_entry_keys;
size_t std_entry_keys_used;
uintptr_t std_entry_refid;


int searchstack_init()
{
	/* Initialize "searchstack" to contain precisely a single reference to */
	/*  "default". */
	
	genname_parrparr *a = (genname_parrparr*)0;
	
	if( searchstack )
	{
		BADNONULL( searchstack_init, &searchstack );
		return( -2 );
	}
	
#define searchstack_init_ERR( err ) \
		MONADICFAILURE( searchstack_init, "genericnamedparr_pascalarray_build", (err) ); \
		return( -3 );
	genericnamedparr_pascalarray_result gnpp =
		genericnamedparr_pascalarray_build( &lib4_stdmemfuncs,  4 );
	LIB4_MONAD_EITHER_BODYMATCH( gnpp, LIB4_OP_SETa, searchstack_init_ERR );
	
	if( !a )
	{
		BADNULL( searchstack_init, &a );
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

	/* This shouldn't appear directly in any search tables, but instead */
	/*  should be returned in a retframe by another wrapper function, which */
	/*  should also provide a non-null value for the void pointer in the */
	/*  process. */
retframe components_stdsearchinit( stackpair *stkp, void *v_ )
{
	uintptr_t a;
	token_head *tok;
	gennameparr_stdpushentry_entry *v;
	int res;
	
	
	STACKCHECK2( stkp, v_,  components_stdsearchinit, macroargs_ENDRETFRAME );
	v = (gennameparr_stdpushentry_entry*)v_;
	
	
	STACKPOP_UINT( &( stkp->data ), a,  components_stdsearchinit, res, macroargs_ENDRETFRAME );
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
		TRESPASSPATH( components_stdsearchinit, "ERROR: components_stdsearchinit was given a token with the wrong token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Build the tokenbranch that'll be used to package the context of */
		/*  the searchtable. */
	tokenbranch *tb = build_tokenbranch( &searchstack_refid,  0 );
	if( !tb )
	{
		BADNULL( components_stdsearchinit, &tb );
		return( (retframe){ &end_run, (void*)0 } );
	}
	tb->lead = tok;
	
		/* There should be a tokengroup at the top of the stack right now: */
		/*  let's confirm it's presence. */
	STACKPEEK_UINT( &( stkp->data ), 0, a,  components_stdsearchinit, res, macroargs_ENDRETFRAME );
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		TRESPASSPATH( components_stdsearchinit, "ERROR: components_stdsearchinit detected the wrong token type at the top of the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
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
		FAILEDPTRFUNC( "build_tokengroup", components_stdsearchinit, tok );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
		/* Push the new tokenbranch for use when closing out this */
		/*  searchtable context. */
	STACKPUSH_UINT( &( stkp->data ), tb,  components_stdsearchinit, res, macroargs_ENDRETFRAME );
		/* Push the new tokengroup for use WHILE IN this searchtable */
		/*  context. */
	STACKPUSH_UINT( &( stkp->data ), tok,  components_stdsearchinit, res, macroargs_ENDRETFRAME );
	
	
		/* Push the searchtable itself, so that we'll use the right matches. */
	int res =
		lexparse1_pushsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_stdsearchinit__pushfunc,
			
			v->table
		);
	if( !res )
	{
		FAILEDINTFUNC( "lexparse1_pushsearchtable", components_stdsearchinit, res );
		return( (retframe){ &end_run, (void*)0 } );
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
	
	
	STACKCHECK2( stkp, v_,  components_sequencedsearchproceed, macroargs_ENDRETFRAME );
	v = (gennameparr_stdpushentry_entry*)v_;
	
		/* The searchtable that we've been using is no longer correct, so pop it. */
	int res =
		lexparse1_popsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_sequencedsearchproceed__popfunc
		);
	if( !res )
	{
		FAILEDINTFUNC( "lexparse1_popsearchtable", components_sequencedsearchproceed, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* The token that matched. */
	STACKPEEK_UINT( &( stkp->data ), 0, a,  components_sequencedsearchproceed, res, macroargs_ENDRETFRAME );
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
		TRESPASSPATH( components_sequencedsearchproceed, "ERROR: components_sequencedsearchproceed was given a token with the wrong token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Push the next searchtable that we'll be using. */
	res =
		lexparse1_pushsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_sequencedsearchproceed__pushfunc,
			
			v->table
		);
	if( !res )
	{
		FAILEDINTFUNC( "lexparse1_pushsearchtable", components_sequencedsearchproceed, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* followup should PROBABLY itself return a retframe to a standard */
		/*  searcher function. */
	CALLFRAMEFUNC(
		stkp,
		
		v->conclude.handler,
		v->conclude.data,
		
		v->followup.handler,
		v->followup.data,
		
		components_sequencedsearchproceed,
		res
	);
}
	/* Use this if you care about the token that caused the function to be */
	/*  called. */
retframe components_stdsearchdeinit( stackpair *stkp, void *v_ )
{
	uintptr_t a;
	token_head *tok_a, *tok_b;
	gennameparr_stdpushentry_entry *v;
	int res;
	
	
	STACKCHECK2( stkp, v_,  components_stdsearchdeinit, macroargs_ENDRETFRAME );
	v = (gennameparr_stdpushentry_entry*)v_;
	
		/* The ending token. */
	STACKPOP_UINT( &( stkp->data ), a,  components_stdsearchdeinit, res, macroargs_ENDRETFRAME );
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
		TRESPASSPATH( components_stdsearchdeinit, "ERROR: components_stdsearchdeinit was given a token with the wrong token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* The result-body of the searchtable that we're going to pop. */
	STACKPOP_UINT( &( stkp->data ), a,  components_stdsearchdeinit, res, macroargs_ENDRETFRAME );
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		TRESPASSPATH( components_stdsearchdeinit, "ERROR: components_stdsearchdeinit found a non-_SAMEMERGE token on the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	tok_b = (token_head*)a;
	
		/* And "finally" (for this step), the tokebranch that organizes all */
		/*  of that stuff.  */
	STACKPOP_UINT( &( stkp->data ), a,  components_stdsearchdeinit, res, macroargs_ENDRETFRAME );
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH( components_stdsearchdeinit, "ERROR: components_stdsearchdeinit found a non-_EQUIVMERGE token on the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
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
	STACKPEEK_UINT( &( stkp->data ), 0, a,  components_stdsearchdeinit, res, macroargs_ENDRETFRAME );
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		TRESPASSPATH( components_stdsearchdeinit, "ERROR: components_stdsearchdeinit found a non-_SAMEMERGE token on the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	tok_b = (token_head*)a;
		/* Now we store the tokenbranch into the parent context's */
		/*  tokengroup. */
	int res =
		pushto_tokengroup
		(
			&searchstack_refid,
			REFID_SUBIDS_searchstack__components_stdsearchdeinit__pushfunc,
			
			(tokengroup*)tok_b,
			tok_a
		);
	if( !res )
	{
		FAILEDINTFUNC( "pushto_tokengroup", components_stdsearchdeinit, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Pop the actual searchtable: it has no business being used to */
		/*  search anymore. */
	res =
		lexparse1_popsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_stdsearchdeinit__popfunc
		);
	if( !res )
	{
		FAILEDINTFUNC( "lexparse1_popsearchtable", components_stdsearchdeinit, res );
		return( (retframe){ &end_run, (void*)0 } );
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
	int res;
	
	
	STACKCHECK2( stkp, v_,  components_skipendersearchdeinit, macroargs_ENDRETFRAME );
	v = (gennameparr_stdpushentry_entry*)v_;
	
		/* The ending token. */
	STACKPOP_UINT( &( stkp->data ), a,  components_skipendersearchdeinit, res, macroargs_ENDRETFRAME );
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
		TRESPASSPATH( components_skipendersearchdeinit, "ERROR: components_skipendersearchdeinit was given a token with the wrong token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* The result-body of the searchtable that we're going to pop. */
	STACKPOP_UINT( &( stkp->data ), a,  components_skipendersearchdeinit, res, macroargs_ENDRETFRAME );
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		TRESPASSPATH( components_skipendersearchdeinit, "ERROR: components_skipendersearchdeinit found a non-_SAMEMERGE token on the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	tok = (token_head*)a;
	
		/* And "finally" (for this step), the tokebranch that organizes all */
		/*  of that stuff.  */
	STACKPOP_UINT( &( stkp->data ), a,  components_skipendersearchdeinit, res, macroargs_ENDRETFRAME );
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH( components_skipendersearchdeinit, "ERROR: components_skipendersearchdeinit found a non-_EQUIVMERGE token on the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
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
	STACKPEEK_UINT( &( stkp->data ), 0, a,  components_skipendersearchdeinit, res, macroargs_ENDRETFRAME );
	if( ( (token_head*)a )->toktype != TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		TRESPASSPATH( components_skipendersearchdeinit, "ERROR: components_skipendersearchdeinit found a non-_SAMEMERGE token on the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
		/* Now we store the tokenbranch into the parent context's */
		/*  tokengroup. */
	int res =
		pushto_tokengroup
		(
			&searchstack_refid,
			REFID_SUBIDS_searchstack__components_skipendersearchdeinit__pushfunc,
			
			(tokengroup*)( (token_head*)a ),
			tok
		);
	if( !res )
	{
		FAILEDINTFUNC( "pushto_tokengroup", components_skipendersearchdeinit, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Pop the actual searchtable: it has no business being used to */
		/*  search anymore. */
	res =
		lexparse1_popsearchtable
		(
			&searchstack_refid,
			
			&searchstack,
			&searchstack_used,
			
			REFID_SUBIDS_searchstack__components_skipendersearchdeinit__popfunc
		);
	if( !res )
	{
		FAILEDINTFUNC( "lexparse1_popsearchtable", components_skipendersearchdeinit, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
		/* Restore the "triggering token" pointer to the stack, so that we */
		/*  can deallocate it with the standard functions for the purpose. */
	STACKPUSH_UINT( &( stkp->data ), b,  components_skipendersearchdeinit, res, macroargs_ENDRETFRAME );
	
	
		/* Register the "processing function" for the assembled work as the */
		/*  return route for the standard token deallocator, and then */
		/*  proceed to said deallocator. */
		/* Note that the processing function will need to grab the relevant */
		/*  tokenbranch from the end of the tokengroup that's linked on what */
		/*  WILL BE the top of the stack. */
	CALLFRAMEFUNC(
		stkp,
		
		v->conclude.handler,
		v->conclude.data,
		
		&invoke_dealloctoken,
		(void*)0,
		
		components_skipendersearchdeinit,
		res
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
		BADNULL2( macroargs_init, &macro_args, &macro_args_used );
		return( -2 );
	}
	
#define macroargs_init_SUCC( arr ) \
		macro_args = ( arr ); macro_args_used = 0;
#define macroargs_init_FAIL( err ) \
		MONADICFAILURE( macroargs_init, "tokhdptparpt_pascalarray_build", (err) ); \
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
		BADNULL( macroargs_resize, &macro_args );
		return( -2 );
	}
	
	if( len < 0 )
	{
		BADNONULL( macroargs_resize, &len );
		return( -3 );
	}
	
#define macroargs_resize_SUCC( arr ) \
		macro_args = ( arr );
#define macroargs_resize_FAIL( err ) \
		MONADICFAILURE( macroargs_resize, "tokhdptparpt_pascalarray_rebuild", (err) ); \
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
		BADNULL( macroargs_clear, &macro_args );
		return( -2 );
	}
	
#define macroargs_resize_SUCC( arr ) ;
#define macroargs_resize_FAIL( err ) \
		MONADICFAILURE( macroargs_clear, "tokhdptparpt_pascalarray_destroy", (err) ); \
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
		BADNULL( macroargs_pushset, &val );
		return( -1 );
	}
	if( !macro_args )
	{
		BADNULL( macroargs_pushset, &macro_args );
		return( -2 );
	}
	
	if( macro_args_used >= macro_args->len )
	{
		int res = macroargs_resize( macro_args->len );
		
		if( res < 0 )
		{
			BADNONULL( macroargs_pushset, &res );
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
		BADNULL2( macroargs_popset, &macro_args, &macro_args_used );
		return( -2 );
	}
	
	--macro_args_used;
	
	if( dest )
	{
		*dest = macro_args->body[ macro_args_used ];
		
		return( 2 );
	}
	
		/* Not per-say an error, just a warning. */
	NOTELINE(); STRARG( "WARNING: macroargs_popset() exited from it's end, because *dest was null." );
	return( 1 );
}
int macroargs_peekset( size_t off,  tokhdptr_parr **dest )
{
	if( !macro_args || !macro_args_used )
	{
		BADNULL2( macroargs_peekset, &macro_args, &macro_args_used );
		return( -2 );
	}
	if( !dest || off >= macro_args_used )
	{
		TRESPASSPATH( macroargs_peekset, "ERROR: macroargs_peekset received invalid args." );
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
		BADNULL( pack_arglist, &stkp );
		return( -1 );
	}
	
	int res = pop_uintptr( &( stkp->data ),  &shufcount );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", pack_arglist, res );
		return( -2 );
	}
	
#define pack_arglist_FAIL( err ) \
		MONADICFAILURE( pack_arglist, "tokenheadptr_pascalarray_build", (err) ); \
		return( -3 );
	tokenheadptr_pascalarray_result res = tokenheadptr_pascalarray_build( shufcount );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETa, pack_arglist_FAIL );
	
	while( shufcount )
	{
		--shufcount;
		res = token_queue_shufflepop( &tmp );
		if( !res )
		{
			FAILEDINTFUNC( "token_queue_shufflepop", pack_arglist, res );
			return( -4 );
		}
		if( !tmp )
		{
			BADNULL( pack_arglist, &tmp );
			return( -5 );
		}
		a->body[ shufcount ] = &( tmp->header );
	}
	
	res = push_uintptr( &( stkp->data ),  (uintptr_t)a );
	if( !res )
	{
		FAILEDINTFUNC( "push_uintptr", pack_arglist, res );
		return( -6 );
	}
	
	return( 1 );
}
retframe vm_pop_macroarg( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_pop_macroarg, macroargs_ENDRETFRAME );
	
	int res;
	POPMACROARGS( (tokhdptr_parr**)0,  vm_pop_macroarg, res, macroargs_ENDRETFRAME );
	
	RETFRAMEFUNC( stkp,  vm_pop_macroarg, res );
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
	
	
	STACKCHECK( stkp,  shufflequeue_macro_link, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), a,  shufflequeue_macro_link, res, macroargs_ENDRETFRAME );
	mlink = (macro_link*)a;
	if( mlink->header.toktype != TOKTYPE_TOKENGROUP_MACROLINK )
	{
		TRESPASSPATH( shufflequeue_macro_link, "ERROR: shufflequeue_macro_link found a macro_link with a non-_MACROLINK token type on top of the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	PEEKMACROARGS( 0, thdptpr,  shufflequeue_macro_link, res, macroargs_ENDRETFRAME );
	if( thdptpr->len <= mlink->args_offset )
	{
		TRESPASSPATH( shufflequeue_macro_link, "ERROR: shufflequeue_macro_link was given a macro_link with too large an args_offset." );
		return( (retframe){ &end_run, (void*)0 } );
	}
		???
		/* Note that OUTWARDLY ONE token should be pushed here. This is a */
		/*  SINGLE arg that we're dealing with, NOT multiple. */
	PUSHSHUFFLE( thdptpr->body[ mlink->args_offset ],  shufflequeue_macro_link, res, macroargs_ENDRETFRAME );
	
	
	RETFRAMEFUNC( stkp,  shufflequeue_macro_link, res );
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
	
	
	STACKCHECK( stkp,  shufflequeue_macro_token, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), a,  shufflequeue_macro_token, res, macroargs_ENDRETFRAME );
	mtok = (macro_token*)a;
	if( mtok->header.toktype != TOKTYPE_TOKENGROUP_MACROTOKEN )
	{
		TRESPASSPATH( shufflequeue_macro_token, "ERROR: shufflequeue_macro_token found a macro_link with a non-_MACROTOKEN token type on top of the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !( mtok->tok ) )
	{
		BADNULL( shufflequeue_macro_token, &( mtok->tok ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
		???
		/* Is this actually enough? Might need to ensure that a */
		/*  tokengroup is present, and push it's elements individually, */
		/*  or something similar. */
	PUSHSHUFFLE( mtok->tok,  shufflequeue_macro_token, res, macroargs_ENDRETFRAME );
	
	
	RETFRAMEFUNC( stkp,  shufflequeue_macro_token, res );
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
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_macro_run, macroargs_ENDRETFRAME );
	
	STACKPEEK_UINT( &( stkp->data ), 0, a,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	mr = (macro_run*)a;
	if( mr->header.toktype != TOKTYPE_TOKENGROUP_MACRORUN || !( mr->mac ) || !( mr->args ) )
	{
		TRESPASSPATH( shufflequeue_macro_run, "ERROR: shufflequeue_macro_run found an incorrectly configured macro_run on top of the data stack" );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Prep for the call to shufflequeue_step_macro_call(). */
	{
		size_t used = token_queue_shuffleused();
		
		STACKPUSH_UINT( &( stkp->data ), used,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	}
	STACKPUSH_UINT( &( stkp->data ), mr->args,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	STACKPUSH_UINT( &( stkp->data ), 0,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	
	
	CALLFRAMEFUNC(
		stkp,
		
		&shufflequeue_macro_run_continue,
		(void*)0,
		
		&shufflequeue_step_macro_call,
		(void*)0,
		
		shufflequeue_macro_run,
		res
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
		
		
		STACKCHECK( stkp,  shufflequeue_macro_run_continue, macroargs_ENDRETFRAME );
		
			/*
					shuffledlength shufflestack:token*[shuffledlength]
				--
					arglist*
			*/
		int res = pack_arglist( stkp,  REFID_SUBIDS_searchstack__???, 2 );
		if( !res )
		{
			FAILEDINTFUNC( "pack_arglist", shufflequeue_macro_run_continue, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		STACKPOP_UINT( &( stkp->data ), args,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		
		STACKPOP_UINT( &( stkp->data ), b,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		mr = (macro_run*)b;
		if( mr->header.toktype != TOKTYPE_TOKENGROUP_MACRORUN || !( mr->mac ) )
		{
			TRESPASSPATH( shufflequeue_macro_run_continue, "ERROR: shufflequeue_macro_run_continue found an incorrectly configured macro_run on top of the data stack" );
			return( (retframe){ &end_run, (void*)0 } );
		}
		STACKPUSH_UINT( &( stkp->data ), mr->mac,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		
		STACKPUSH_UINT( &( stkp->data ), args,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		
		
		CALLFRAMEFUNC(
			stkp,
			
			&vm_pop_macroarg,
			(void*)0,
			
			&shufflequeue_entry_macro_call,
			(void*)0,
			
			shufflequeue_macro_run_continue,
			res
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
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_macro_directive, macroargs_ENDRETFRAME );
	
	STACKPEEK_UINT( &( stkp->data ), 0, a,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	mdir = (macro_directive*)a;
	if( mdir->header.toktype != TOKTYPE_TOKENGROUP_MACRODIRECTIVE || !( mdir->handler ) || !( mdir->args ) )
	{
		TRESPASSPATH( shufflequeue_macro_directive, "ERROR: shufflequeue_macro_directive found an incorrectly configured macro_run on top of the data stack" );
		return( (retframe){ &end_run, (void*)0 } )
	}
	
		/* Prep for the call to shufflequeue_step_macro_call(). */
	{
		size_t used = token_queue_shuffleused();
		
		STACKPUSH_UINT( &( stkp->data ), used,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	}
	STACKPUSH_UINT( &( stkp->data ), mdir->args,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	STACKPUSH_UINT( &( stkp->data ), 0,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	
	
	CALLFRAMEFUNC(
		stkp,
		
		&shufflequeue_macro_directive_continue,
		(void*)0,
		
		&shufflequeue_step_macro_call,
		(void*)0,
		
		shufflequeue_macro_directive,
		res
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
		
		
		STACKCHECK( stkp,  shufflequeue_macro_directive_continue, macroargs_ENDRETFRAME );
		
			/*
					shuffledlength shufflestack:token*[shuffledlength]
				--
					arglist*
			*/
		int res = pack_arglist( stkp,  REFID_SUBIDS_searchstack__???, 2 );
		if( !res )
		{
			FAILEDINTFUNC( "pack_arglist", shufflequeue_macro_directive_continue, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		STACKPOP_UINT( &( stkp->data ), args,  shufflequeue_macro_directive_continue, res, macroargs_ENDRETFRAME );
		
		STACKPOP_UINT( &( stkp->data ), b,  shufflequeue_macro_directive_continue, res, macroargs_ENDRETFRAME );
		mdir = (macro_directive*)b;
		if( mdir->header.toktype != TOKTYPE_TOKENGROUP_MACRODIRECTIVE || !( mdir->handler ) )
		{
			TRESPASSPATH( shufflequeue_macro_directive_continue, "ERROR: shufflequeue_macro_directive_continue popped an invalidly configured macro_directive from the data stack." );
			return( (retframe){ &end_run, (void*)0 } ); 
		}
		
		STACKPUSH_UINT( &( stkp->data ), args,  shufflequeue_macro_directive_continue, res, macroargs_ENDRETFRAME );
		
		
		CALLFRAMEFUNC(
			stkp,
			
			&vm_pop_macroarg,
			(void*)0,
			
			mdir->handler.handler,
			mdir->handler.data,
			
			shufflequeue_macro_directive_continue,
			res
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
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_entry_macro_call, macroargs_ENDRETFRAME );
	
		/* Move the arguments to the argument stack. */
	STACKPOP_UINT( &( stkp->data ), mcall,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	PUSHMACROARGS( mcall,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), mcall,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
		/* Just verification. */
	if( ( (token_head*)mcall )->toktype != TOKTYPE_TOKENGROUP_MACROCALL )
	{
		TRESPASSPATH( shufflequeue_entry_macro_call, "ERROR: shufflequeue_entry_macro_call found a non-_MACROCALL token on top of the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Push a bookmark for where the tokenqueue shuffle stack currently */
		/*  is: if we have macro calls embedded inside of other macrocalls, */
		/*  then this will make it MUCH easier to avoid causing buffer */
		/*  "overflow/underflow" errors. */
	{
		size_t used = token_queue_shuffleused();
		
		STACKPUSH_UINT( &( stkp->data ), used,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	}
	
		/* Push the body element from mcall instead of mcall itself, and */
		/*  then push a "0" progress indicator, since we haven't processed */
		/*  anything yet. */
	STACKPUSH_UINT( &( stkp->data ), mcall->body,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
		/* Our progress? None! Because we haven't really started. */
	STACKPUSH_UINT( &( stkp->data ), 0,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	
	
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
	
	
	STACKCHECK( stkp,  shufflequeue_step_macro_calltool, macroargs_ENDRETFRAME );
	
		/* Pull these two from the stack, since we need to use and modify */
		/*  the "deeper" one, while otherwise preserving both. */
	int res;
	PEEKMACROARGS( 0, arg_parr,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME )
		/* "0" is actually valid, so we can't use the usual macro. */
	int res = pop_uintptr( stk,  prog );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", shufflequeue_step_macro_calltool, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	STACKPEEK_UINT( &( stkp->data ), 0, a,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME );
	tokhdptr_parr *ops = (tokhdptr_parr*)ops_;
	
	++prog;
	STACKPUSH_UINT( &( stkp->data ), prog,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME );
	
	if( ops->len >= prog )
	{
		token_header *th = ( ops->body[ prog - 1 ] );
		
		STACKPUSH_UINT( &( stkp->data ), th,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME );
		
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
				TRESPASSPATH( shufflequeue_step_macro_calltool, "ERROR: shufflequeue_step_macro_calltool encountered an unrecognized token type: " );
				DECARG( th->toktype );
					/* "hand" contains a reference to end_run, so no need to */
					/*  do anything other than break out of the switch. */
				break;
		}
		
		CALLFRAMEFUNC(
			stkpair,
			
			loop.handler,
			loop.data,
			
			hand,
			(void*)0,
			
			shufflequeue_step_macro_calltool,
			res
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
	
	
	STACKCHECK( stkp,  shufflequeue_exit_macro_calltool, macroargs_ENDRETFRAME );
	
		/* And now pop the shuffle stack/queue/whatever's bookmark, so that */
		/*  we'll know how many token pointers need to be moved from */
		/*  "shuffle" to the token get/unget stack. These will be */
		/*  deallocated elsewhere, so no need to do it here. */
	int res = pop_uintptr( stk,  &shufflebookmark );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", shufflequeue_exit_macro_calltool, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	??? /* Actually, where DO these get freed? That might be a source of */
		/*  errors. */
	
	size_t used = token_queue_shuffleused() - shufflebookmark;
		/* Push the number of tokens on the shuffle stack: some */
		/*  things need to know this for their own purposes. */
	STACKPUSH_UINT( &( stkp->data ), shufflebookmark,  shufflequeue_exit_macro_calltool, res, macroargs_ENDRETFRAME );
	
	
	RETFRAMEFUNC( stkp,  shufflequeue_exit_macro_calltool, res );
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
	
	
	STACKCHECK( stkp,  shufflequeue_exit_macro_call, macroargs_ENDRETFRAME );
	
		/* Pop the pascal array of arguments provided to the macro first: */
		/*  these will need to be deallocated at some point. */
	int res;
	POPMACROARGS( &args,  shufflequeue_exit_macro_call, res, macroargs_ENDRETFRAME );
		/* Do something to deallocate those tokens. */
	???
	
		/* Now pop the progress marker: we're done, so it's done. Just a */
		/*  number, so no need to deallocate. */
	int res = pop_uintptr( stk,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", shufflequeue_exit_macro_call, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Now the operations array. The array itself is */
		/*  owned elsewhere, so no need to deallocate. Also, deallocating */
		/*  now could break something later. */
	res = pop_uintptr( stk,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", shufflequeue_exit_macro_call, res );
		return( (retframe){ &end_run, (void*)0 } );
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
	int res;
	CALLFRAMEFUNC(
		stkp,
		
		&shufflequeue_entry_macro_call,
		(void*)0,
		
		&shufflequeue_exit_macro_wrapper,
		(void*)0,
		
		shufflequeue_entry_macro_wrapper,
		res
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
		int res;
		
		
		STACKCHECK( stkp,  shufflequeue_entry_macro_wrapper, macroargs_ENDRETFRAME );
		
		STACKPOP_UINT( &( stkp->data ), count,  shufflequeue_exit_macro_wrapper, res, macroargs_ENDRETFRAME );
		
		while( count )
		{
			--count;
			int res = token_queue_shuffle2queue();
			if( !res )
			{
				FAILEDINTFUNC( "token_queue_shuffle2queue", shufflequeue_exit_macro_wrapper, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
		}
		
		/* The various produced tokens should now be in the token stream, */
		/*  AND in the correct order. */
		
		
		RETFRAMEFUNC( stkp,  shufflequeue_exit_macro_wrapper, res );
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



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
