#include <stdlib.h>

#include "headers.h"

#include "err/inner_err.h"
#include "command_parse.h"
#include "lexalike.h"
#include "stack.h"
#include "basic_toktools.h"
#include "basic_stktools.h"

#include "eval.h"



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

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )


#define CALL_FFUNC( stkpair,  rethand, retdat,  callhand, calldat,  caller, scratch ) \
	CALL_FRAMEFUNC( stkpair,  rethand, retdat,  callhand, calldat,  &errs, caller, scratch, lexalike_ENDRETFRAME )





retframe common_macrodispatch( stackpair *stkp, void *v )
{
	STACKCHECK( stack,  caller, endfunc );
	
	???
	
	genericnamed *gn = bsearch1_gennamearr( genname_parr *parr, token *tok );
	if( gn )
	{
		switch( gn->reftype )
		{
			case GENNAMETYPE_RETFRAMEFUNC:
				/* ->ref points to a retframe to be called/run. */

					/* Maybe we need to setup additional returns as well? */
				CALL_FFUNC(
					stkp,
					
					rethand, (void*)0,
					( (retframe*)( gn->ref ) )->handler, ( (retframe*)( gn->ref ) )->data,
					
					ommon_macrodispatch, scratch
				);
			case GENNAMETYPE_TABLEENTRY:
				/* ->ref points to a token array. */
				
				???;
			
			case GENNAMETYPE_INVALID:
				/* The default state, doesn't actually represent anything, invalid here. */
			case GENNAMETYPE_NAMEONLY:
				/* Used for tracking includes, invalid here. */
			default:
				/* Unknown ref type, invalid here. */
		}
		
		???
		
		struct genericnamed
		{
			char_pascalarray *name;
			void *ref;
			uintptr_t reftype;
		};
		
	} else {
		
		retframe echo_token( stackpair *stkp, void *v )
	}
	
	???
}
{
	STACKCHECK( stack,  caller, endfunc );
	
	???
	
		/* Use this instead of getANDassemble_token(). */
	retframe token_queue_fetch( stackpair *stkp, void *v )
	
	if( ->toktype == TOKTYPE_NAME )
	{
			/* tok->text must point to an actual string of text. */
		genericnamed* bsearch1_gennamearr( genname_parr *parr, token *tok );
		
	} else {
	}
	
	???
}
retframe common_opdispatch( stackpair *stkp, void *v )
{
	STACKCHECK( stack,  caller, endfunc );
	
	token *tok;
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  common_opdispatch, scratch );
	*t = (token*)a;
	
	if( tok->toktype != TOKTYPE_NAME )
	{
		generictyped *gt = gentyped_bsearch( stkp->ops, tok );
		if( gt )
		{
			switch( gt->reftype )
			{
				case GENNAMETYPE_RETFRAMEFUNC:
					/* ->ref points to a retframe to be called/run. */
					
						/* Maybe we need to setup additional returns as well? */
					CALL_FFUNC(
						stkp,
						
							/* The return route. */
						rethand, retdat,
							/* The immediate jump. */
						( (retframe*)( gt->ref ) )->handler, ( (retframe*)( gt->ref ) )->data,
						
						common_opdispatch, scratch
					);
				case GENNAMETYPE_INVALID:
					/* The default state, doesn't actually represent anything, invalid here. */
				case GENNAMETYPE_NAMEONLY:
					/* Used for tracking includes, invalid here. */
				case GENNAMETYPE_TABLEENTRY:
					/* ->ref points to a lookup table to be used for */
					/*  further searches. Usually it gets pushed onto */
					/*  a stack, but probably invalid here? */
				default:
					/* Unknown ref type, invalid here. */
			}
			
			???
			
		} else {
			
				/* No match found, check for a macro. */
			return( (retframe){ &common_macrodispatch, (void*)0 } );
		}
		
	} else {
		
			/* It's a name, check for a macro. */
		return( (retframe){ &common_macrodispatch, (void*)0 } );
	}
}


	/* Gets a token onto the stack, often as a tokenbranch */
	/*  with whitespace shoved wherever in it: only the */
	/*  body member really matters. */
retframe accumulate_token( stackpair *stkp, void *v )











	/* As with require_preprocopener(). */
retframe conclude_try_directive( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  conclude_try_directive );
	
	uintptr_t a;
	int scratch;
	
	STACKPOP_UINT( stkp->data, a,  conclude_try_directive, scratch );
	if( a )
	{
		static const retframe_parr octoreq =
			(retframe_parr)
			{
				7,
				{
					(retframe(){ &tokenbranch_build, (void*)0 },
					
					(retframe(){ &swap2nd, (void*)0 },
					(retframe(){ &tokenbranch_pushbody, (void*)0 },
					
					(retframe(){ &swap2nd, (void*)0 },
						/* We might be better off using a different */
						/*  token to set this. If so, then verify that */
						/*  the number of elements in the retframe_parr */
						/*  gets updated. */
					(retframe(){ &tokenbranch_setsubtype, (void*)0 },
					(retframe(){ &tokenbranch_setlead, (void*)0 },
					
						/* Dispatch to the directive-recognizer code. */
					(retframe(){ &, (void*)0 }
				}
			};
		return( (retframe){ &enqueue_returns, (void*)&octoreq } );
		
	} else {
		
			/* Not a directive, queue another loop then go check something else. */
		CALL_FFUNC(
			stkp,
			
			&enter_try_directive, (void*)0, /* Prep loop. */
			???, (void*)0, /* Proceed to the next stage of the parse checks. */
			
			conclude_try_directive, scratch
		);
	}
}
retframe try_directive( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  try_directive );
	
	uintptr_t a;
	int scratch;
	
	STACKPOP_UINT( stkp->data, a,  try_directive, scratch );
	if( a )
	{
		/* We have an octothorpe! */
		
		static const retframe_parr tryname =
			(retframe_parr)
			{
				3,
				{
					(retframe(){ &accumulate_token, (void*)0 },
					(retframe(){ &require_anyname, (void*)0 },
					
					(retframe(){ &conclude_try_directive, (void*)0 }
				}
			};
		return( (retframe){ &enqueue_returns, (void*)&tryname } );
		
	} else {
		
			/* Not a directive, queue another loop then go check something else. */
		CALL_FFUNC(
			stkp,
			
			&enter_try_directive, (void*)0, /* Prep loop. */
			???, (void*)0, /* Proceed to the next stage of the parse checks. */
			
			try_directive, scratch
		);
	}
}
retframe enter_try_directive( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  enter_try_directive );
	
	static const retframe_parr seq =
		(retframe_parr)
		{
			3 /* The number of instructions. */,
			{
					/* Get the token. */
				(retframe){ &accumulate_token, (void*)0 },
				(retframe){ &require_octothorp, (void*)0 },
				
					/* Proceed into the handler. */
				(retframe){ &try_directive, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}







	/* ( tokenbranch* -- tokenbranch* token* par_bool sqrcrl_bool ) */
retframe enter_try_pardefclose( stackpair *stkp, void *v );
	/* ( tokenbranch* token* -- tokenbranch* ) */
retframe pack_pardefclose( stackpair *stkp, void *v )
{
		/* Make const? */
	static bad_token_report
		needcomma =
		{
			0, /* Do not free, lest segfault. */
			
			(char*)( __FILE__ ),
			&pack_pardefclose, "pack_pardefclose",
			(unsigned)( __LINE__ ),
			
			"Encountered a non-comma when requiring a comma"
		},
		badcomma =
		{
			0, /* Do not free, lest segfault. */
			
			(char*)( __FILE__ ),
			&pack_pardefclose, "pack_pardefclose",
			(unsigned)( __LINE__ ),
			
			"Encountered a comma when requiring a non-comma"
		};
	static const retframe_parr
			/* ( uintptr_t -- ) */
		on_bad_nocomma_ =
			(retframe_parr)
			{
				3 /* The number of instructions. */,
				{
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
					(retframe){ &bad_token, &needcomma },
					(retframe){ &end_run, (void*)0 }
				}
			},
			/* ( uintptr_t -- ) */
		on_bad_comma_ =
			(retframe_parr)
			{
				3 /* The number of instructions. */,
				{
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
					(retframe){ &bad_token, &badcomma },
					(retframe){ &end_run, (void*)0 }
				}
			},
		on_good_nocomma_ =
			(retframe_parr)
			{
				3 /* The number of instructions. */,
				{
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
					(retframe){ &tokenbranch_pushbody, (void*) }
				}
			};
	retframe
		on_bad_nocomma = { &enqueue_returns, &on_bad_nocomma_ },
		on_bad_comma = { &enqueue_returns, &on_bad_comma_ };
	static const retframe_parr
		on_good_comma_ =
			(retframe_parr)
			{
				10 /* The number of instructions. */,
				{
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
					
					/* Fetch the next token and confirm it's validity: it MUST be a */
					/*  name, or maybe a triple ellipsis... except we don't currently */
					/*  HAVE triple ellipsis support in simplelex.c OR token.h, so skip */
					/*  that. */
					
					(retframe){ &accumulate_token, (void*)0 },
					
					(retframe){ &require_anyname, (void*)0 },
						/* And this is the reason we're in another declaration. */
					(retframe){ &run_else, (void*)&on_bad_comma },
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
					
					/* Push the tokens into the body, and do it in the correct order. */
					/*  Note that they should probably be in a tokenbranch instead of */
					/*  directly being pushed in. Or maybe just ditch the comma? */
					
						/* tokenbranch* token*a token*b -- token*b token*a tokenbranch* */
					(retframe){ &swap3rd, (void*)0 },
						/* token*a tokenbranch* -- tokenbranch* token*a */
					(retframe){ &swap2nd, (void*)0 },
						/* tokenbranch* token*a -- tokenbranch* */
					(retframe){ &tokenbranch_pushbody, (void*)0 },
					
						/* token*b tokenbranch* -- tokenbranch* token*b */
					(retframe){ &swap2nd, (void*)0 },
						/* tokenbranch* token*b -- tokenbranch* */
					(retframe){ &tokenbranch_pushbody, (void*)0 }
				}
			};
	retframe
		on_good_nocomma = { &enqueue_returns, &on_bad_nocomma_ },
		on_good_comma = { &enqueue_returns, &on_bad_comma_ };
	
	
	STACKCHECK( stkp,  pack_pardefclose );
	
	tokenbranch *tb;
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), a,  pack_pardefclose, scratch );
	tb = (tokenbranch*)a;
	
	if( tb->body )
	{
		/* We already have a body member, so this better be a comma. */
		
		static const retframe_parr seq =
			(retframe_parr)
			{
				3 /* The number of instructions. */,
				{
					(retframe){ &require_comma, (void*)0 },
					(retframe){ &run_if, (void*)&on_good_comma },
					(retframe){ &run_else, (void*)&on_bad_nocomma }
				}
			};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
		
	} else {
		
		/* We don't yet have a body member, so this better NOT be a comma. */
		
		static const retframe_parr seq =
			(retframe_parr)
			{
				3 /* The number of instructions. */,
				{
					(retframe){ &require_comma, (void*)0 },
					(retframe){ &run_if, (void*)&on_bad_comma },
					(retframe){ &run_else, (void*)&on_good_nocomma }
				}
			};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
}
	/*
		(
				tokenbranch* token* par_bool sqrcrl_bool
			--
				looping:( tokenbranch* token* ) -> ( tokenbranch* ) |
				returning:( tokenbranch* )
		)
	*/
retframe try_pardefclose( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  try_pardefclose );
	
	uintptr_t a;
	int scratch;
	
	STACKPOP_UINT( stkp->data, a,  try_pardefclose, scratch );
	if( a )
	{
		/* Error! We encountered an unpaired square or curly closer instead! */
		
		???
	}
	
	STACKPOP_UINT( stkp->data, a,  try_pardefclose, scratch );
	if( a )
	{
		/* Success, we have a parenthese closer! Shove into the branch, */
		/*  sort ->body, and return to the initial caller. */
		CALL_FFUNC(
			stkp,
			
				/* ( tokenbranch* -- tokenbranch* ) */
				/* This needs to go through the contents of */
				/*  tokenbranch->body, and process them into some more */
				/*  meaningful form. Macros need to ALREADY be */
				/*  processed, so that any brackets that the macros */
				/*  insert will have ALREADY been inserted into ->body */
				/*  via the very process of accumulation. */
				/* This is the function that returns to */
				/*  enter_try_upparopen()'s caller. */
			&???, (void*)0,
				/* ( tokenbranch* token* -- tokenbranch* ) */
				/* Store the token. */
			&tokenbranch_settail, (void*)0,
			
			try_pardefclose, scratch
		);
		
	} else {
		
			/* Not a directive, queue another loop and accumulate stuff. */
		CALL_FFUNC(
			stkp,
			
				/* ( tokenbranch* -- tokenbranch* token* par_bool sqrcrl_bool ) */
				/* Loop back through. */
			&enter_try_upparclose, (void*)0,
				/* ( tokenbranch* token* -- tokenbranch* ) */
				/* Store the token, including by grabbing a following */
				/*  token if appropriate to also store. */
			&pack_pardefclose, (void*)0,
			
			try_pardefclose, scratch
		);
	}
}
	/* ( tokenbranch* -- tokenbranch* token* par_bool sqrcrl_bool ) */
retframe enter_try_pardefclose( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  enter_try_pardefclose );
	
		/* The instructions that comprise this procedure. */
	static const retframe_parr seq =
		(retframe_parr)
		{
			10 /* The number of instructions. */,
			{
				(retframe){ &accumulate_token, (void*)0 },
				
				
				/* Test for the desired case. */
				
				(retframe){ &require_preprocclpar, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				
				/* Test for the erroneous cases. */
				
				(retframe){ &require_preprocclsqr, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				(retframe){ &require_preprocclpar, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
					/* Move the pointer back below the results... */
				(retframe){ &swap4th, (void*)0 },
					/* ... and merge the most recent two results */
					/*  instead of all three: no point combining */
					/*  error with success. */
				(retframe){ &ior2, (void*)0 },
				
				
					/* And now to check our results. */
				(retframe){ &try_upparclose, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}

	/* ( token* -- tokenbranch* ) */
	/* This requires the appropriate token to ALREADY have been */
	/*  confirmed, it doesn't do ANY such thing itself. */
retframe enter_try_pardefopen( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  enter_try_pardefopen );
	
		/* The instructions that comprise this procedure. */
	static const retframe_parr seq =
		(retframe_parr)
		{
			5 /* The number of instructions. */,
			{
					/* (  ) */
				(retframe){ &tokenbranch_build, (void*)0 },
					/* ( -- tokenbranch* ) */
				
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ &tokenbranch_initbase, (void*)0 },
					/* ( -- tokenbranch* token* ) */
				
				(retframe){ &tokenbranch_setlead, (void*)0 },
					/* ( -- tokenbranch* ) */
				
					/* The function that searches for our closer */
					/*  shall handle the rest. Note that THIS is */
					/*  the only difference between the assorted */
					/*  versions of this function... */
				(retframe){ &enter_try_pardefclose, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}




















	static bad_token_report
		needcomma =
		{
			0, /* Do not free, lest segfault. */
			
			(char*)( __FILE__ ),
			&pack_pardefclose, "pack_pardefclose",
			(unsigned)( __LINE__ ),
			
			"Encountered a non-comma when requiring a comma"
		};
	static const retframe_parr
			/* ( uintptr_t -- ) */
		on_bad_nocomma_ =
			(retframe_parr)
			{
				3 /* The number of instructions. */,
				{
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
					(retframe){ &bad_token, &needcomma },
					(retframe){ &end_run, (void*)0 }
				}
			};
		static const retframe_parr seq =
			(retframe_parr)
			{
				3 /* The number of instructions. */,
				{
					(retframe){ &require_comma, (void*)0 },
				}
			};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	retframe require_parenopener( stackpair *stkp, void *v );
	retframe require_parencloser( stackpair *stkp, void *v );
	retframe require_comma( stackpair *stkp, void *v );
	/* ( tokenbranch* tokengroup* -- ??? ) */
retframe enter_parenclose( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  enter_parenclose );
	
	static const retframe_parr
		is_comma_ =
			(retframe_parr)
			{
				??? /* The number of instructions. */,
				{
					/* We don't care about the token anymore. */
						/* ( tokenbranch* tokengroup* token* result -- ... ) */
					(retframe){ &swap2nd, (void*)0 },
					(retframe){ &invoke_dealloctoken, (void*)0 },
					(retframe){ &swap3rd, (void*)0 },
					(retframe){ &swap2nd, (void*)0 },
						/* ( ... -- result tokenbranch* tokengroup* ) */
					
					/* Push and replace the list that we've been filling with tokens. */
					(retframe){ &tokenbranch_pushbody, (void*)0 },
					(retframe){ &tokengroup_build, (void*)0 },
					
					???
					
				}
			},
		not_comma_ =
			(retframe_parr)
			{
				??? /* The number of instructions. */,
				{
					/* Hide the result. */
						/* ( tokenbranch* tokengroup* token* result -- ... ) */
					(retframe){ &swap4th, (void*)0 },
					(retframe){ &swap3rd, (void*)0 },
					(retframe){ &swap2nd, (void*)0 },
						/* ( ... -- result tokenbranch* tokengroup* token* ) */
					
					???
						/* Before we do this, check for macros, directives, etc. */
					(retframe){ &tokengroup_pushtoken, (void*)0 },
					???
					
					
						/* ( result tokenbranch* tokengroup* -- ... ) */
					(retframe){ &swap2nd, (void*)0 },
					(retframe){ &swap3rd, (void*)0 }
						/* ( ... -- tokenbranch* tokengroup* result ) */
					
					/* And now we return to the caller, for it to dispose */
					/*  of the result that it put on the stack in the first */
					/*  place. */
				}
			};
	static retframe
		is_comma = (retframe){ &enqueue_returns, (void*)&seq },
		not_comma = (retframe){ &enqueue_returns, (void*)&seq };
		/* The instructions that comprise this procedure. */
	static const retframe_parr seq =
		(retframe_parr)
		{
			??? /* The number of instructions. */,
			{
				(retframe){ &accumulate_token, (void*)0 },
				(retframe){ &require_comma, (void*)0 },
				
					/* And now to check our results. */
				(retframe){ &run_if, (void*)&is_comma },
				(retframe){ &run_else, (void*)&not_comma },
					/* We won't need that test result again. */
				(retframe){ &drop, (void*)0 },
				
				???
				
			}
		};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	/* ( token* -- tokenbranch* ) */
	/* Requires the token to ALREADY be verified. */
retframe enter_parenopen( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  enter_parenopen );
	
	static const retframe_parr seq =
		(retframe_parr)
		{
			5 /* The number of instructions. */,
			{
					/* (  ) */
				(retframe){ &tokenbranch_build, (void*)0 },
					/* ( -- tokenbranch* ) */
				
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ &tokenbranch_initbase, (void*)0 },
					/* ( -- tokenbranch* token* ) */
				
				(retframe){ &tokenbranch_setlead, (void*)0 },
					/* ( -- tokenbranch* ) */
				
				
				
				(retframe){ &tokengroup_build, (void*)0 },
				
				
				
				
					/* The function that searches for our closer */
					/*  shall handle the rest. Note that THIS is */
					/*  the only difference between the assorted */
					/*  versions of this function... */
				(retframe){ &enter_parenclose, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}














		This type:
			struct gennameparr_stdpushentry_entry
			{
				uintptr_t toktype;
				genname_parr *table;
					/* "followup" is invoked after a searchtable has been entered, while */
					/*  "conclude" is invoked after a searchtable has been removed. Both */
					/*  need to AT LEAST ensure that the caller of the function that */
					/*  entered/removed the searchtable is returned to. */
				retframe followup, conclude;
			};
	... is ALSO used with the functions below, with a pointer to a
	gennameparr_stdpushentry_entry used as the v_ argument to all four.
	
		These functions:
			retframe components_stdsearchinit( stackpair *stkp, void *v_ );
			retframe components_sequencedsearchproceed( stackpair *stkp, void *v_ );
			retframe components_stdsearchdeinit( stackpair *stkp, void *v_ );
			retframe components_skipendersearchdeinit( stackpair *stkp, void *v_ );
	... work in conjunction with the searchstack. All of them require a pointer
	to a gennameparr_stdpushentry_entry as their "v_" argument (so in general
	use wrapper functions for all of them). In essence, then handle situations
	like comments or argument lists where you have an "opening token" to
	indicate a new context is being entered, thus being deserving of a change
	in treatment. gennameparr_stdpushentry_entry->followup is used after a new
	context has been entered, whereas conclude is used after a context has been
	left.
		components_stdsearchinit() is used to setup a search context. It
	expects a pointer to a token on top of the data stack, and a pointer to a
	tokengroup right under it. The token will be popped, placed in the "lead"
	element of a new tokenbranch, a pointer to that tokebnbranch will be pushed
	onto the stack, then a pointer to a !NEW! tokenGROUP,
	lexparse1_pushsearchtable() will be called to push v_->table onto the
	searchstack, and finally v_->followup will be returned as the next retframe
	to execute.
		components_sequencedsearchproceed() is used to replace one context with
	another. It expects a token on the top of the stack (which it does NOT
	remove), pops the top of the searchstack, pushes a new context back on top,
	and uses a CALLFRAME invocation to tripper a call both to v_->followup, and
	v_->conclude.
		components_stdsearchdeinit() and components_skipendersearchdeinit() are
	roughly equivalent. Both expect a pointer to a token_head derived struct on
	top of the stack with either a ->toktype == v_->toktype, or a toktype of
	EQUIVMERGE (a tokenbranch I think?) with the subtype equal to v_->toktype,
	which both will pop. Under that, both expect a token_head derivation with a
	->toktype of SAMEMERGE (a tokengroup), which again they both pop. Then,
	they both expect a pointer to a tokenbranch, which yes, they both pop.
	Stuff then gets stored into that final tokenbranch (producing the result),
	with both storing the tokengroup into ->body, but only
	components_stdsearchdeinit() storing the triggering token, which goes into
	->tail. They then peek (not pop!) another tokengroup, push the result
	tokenbranch into it, and pop the search table that has been getting used.
	At this point behavior once again diverges, as components_stdsearchdeinit()
	just proceeds straight to v_->conclude, while
	components_skipendersearchdeinit() first pushes the triggering token's
	pointer back onto the stack, before registering v_->conclude for execution,
	and passing execution to invoke_dealloctoken() so that the triggering token
	that it re-pushed will be properly deallocated.
	
		So, a decent framework to handle context-specific search exists here,
	but there doesn't seem to actually be a way to search INSIDE it- that will
	need to be provided elsewhere, or added to this.





			struct macro_directive
			{
				token_head header;
				
				retframe handler;
				tokhdptr_parr *args;
			};
	... represent code invocations of two types: macro invocations, and
	directive invocations. macro_run obviously involves the execution of a
	user-defined macro, while macro_directive corresponds to the execution of a
	built-in directive. Both of them use "args" for the same purpose, that
	being a per-argument set of instructions for how to actually obtain the
	arguments- the allowable set of token_head derived types is limited to
	macro_run and macro_directive themselves, as well as macro_link (used when
	the argument is to be obtained from an argument to the ENCLOSING macro_run)
	and macro_token (which directly holds a pointer to the argument value).
		macro_run's "mac" holds a pointer to the macro_call instances that
	actually DEFINES the behavior of the macro, while macro_directive's
	"handler" is a retframe that implements that particular directive.



	
		This function:
			retframe shufflequeue_macro_run( stackpair *stkp, void *v );
	... is the "caller side" of a macro call. It builds an argument list for
	the designated macro from it's own context, then calls the same function
	that shufflequeue_entry_macro_wrapper() most immediately delegates to. It's
	a good target of study if you want to execute macros, and may even be
	usable directly... but it's honestly intended for internal usage.
		It expects a pointer to a macro_run on top of the data stack, and a
	pointer to a tokenheadptr_pascalarray that holds the CALLER'S argument
	values on top of the macro_args stack. On the "final" return, it will
	leave a uintptr_t on the data stack, a set of token pointers on the shuffle
	stack equal in number to the value of the uintptr_t, and the argument list
	on macro_args will have been popped... but I'm not certain that it SHOULD
	be popped.
	
		This function:
			retframe shufflequeue_macro_directive( stackpair *stkp, void *v );
	... is the directive equivalent of shufflequeue_macro_run(). It executes
	macro_directive->handler with the arguments of :
		( tokenheadptr_pascalarray* macro_args: )
	(... except I think macro_asrgs: DOES have an argument list?), which in
	turn MUST return
		( shuffledlength shufflestack:token*[shuffledlength] macro_args: tokenheadptr_pascalarray* ),
	because a function will be enqueued that pops the tokenheadptr_pascalarray
	pointer. The shufflelength MUST be generated by the handler, and the
	handler MUST put a number of token pointers onto the shuffle stack that
	equals shufflelength.




			int pack_arglist( stackpair *stkp );
	... takes a number of token pointers, and packages it up into an array for
	convenience. It expects a uintptr_t on the data stack, and a set of token
	pointers on the shuffle stack of number equal to the value of the
	uintptr_t. The pointers on the shuffle stack get moved into a
	tokenheadptr_pascalarray, with the pointer on the top of the stack being
	placed at the end of the tokenheadptr_pascalarray, and a pointer to the
	tokenheadptr_pascalarray then gets pushed onto the data stack.




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
		int res = pack_arglist( stkp );
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
			TRESPASSPATH(
				shufflequeue_macro_run_continue,
				"ERROR: shufflequeue_macro_run_continue found an incorrectly configured macro_run on top of the data stack"
			);
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













#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
