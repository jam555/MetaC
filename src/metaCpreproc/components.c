/*
MetaC Preprocessor - A macro processor for preprocessing usage.
Copyright (C) 2022 Jared A. Maddox

This program is free software; you can redistribute it and/or modify it
under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the:
	Free Software Foundation, Inc.,
	59 Temple Place, Suite 330,
	Boston, MA 02111-1307 USA
*/

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


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, funcname, ( ptr ) )

#define MONADICFAILURE( funcname, calltext, err ) \
	STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )
#define FAILEDPTRFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDPTRFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )


#define STACKCHECK2( stack, v,  caller, endfunc ) \
	STACK_CHECK2( ( stack ), ( v ),  &err, ( caller ), ( endfunc ) )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &err, ( caller ), ( scratch ), ( endfunc ) )


#define CALLFRAMEFUNC( stkpair, rethand, retval, callhand, callval,  caller, scratch ) \
	CALL_FRAMEFUNC( stkpair, rethand, retval, callhand, callval,  &err, caller, scratch, macroargs_ENDRETFRAME )



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
	tokenbranch *tb = build_tokenbranch( 0 );
	if( !tb )
	{
		FAILEDPTRFUNC( "build_tokenbranch", components_stdsearchinit, tb );
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
		/*  encounter inside of the searchtable context. "2" is a nice, */
		/*  bland number of token pointers to start with. */
	tok = (token_head*)build_tokengroup( 2 );
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
			&searchstack,
			&searchstack_used,
			
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
			&searchstack,
			&searchstack_used
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
			&searchstack,
			&searchstack_used,
			
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
			&searchstack,
			&searchstack_used
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
	STACKPOP_UINT( &( stkp->data ), b,  components_skipendersearchdeinit, res, macroargs_ENDRETFRAME );
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
	
		/* And "finally" (for this step), the tokenbranch that organizes all */
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
			&searchstack,
			&searchstack_used
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



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
