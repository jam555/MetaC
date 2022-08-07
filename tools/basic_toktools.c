#include "../stack.h"
#include "basic_toktools.h"
#include "basic_stktools.h"



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



#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), lexalike_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )

#define RETFRAMEFUNC( stkpair,  caller ) \
	RET_FRAMEFUNC( ( stkpair ),  &errs, ( caller ), res, stack_ENDRETFRAME )
#define CALL_FFUNC( stkpair,  rethand, retdat,  callhand, calldat,  caller, scratch ) \
	CALL_FRAMEFUNC( stkpair,  rethand, retdat,  callhand, calldat,  &errs, caller, scratch, lexalike_ENDRETFRAME )



	/* Our reference token. */
static const token
	prepar =
		{
			{
				TOKTYPE_OPPARUP,
				2,

				0, 0, 0, 0
			},
			"(^"
		},
	presqr =
		{
			{
				TOKTYPE_OPSQRUP,
				2,

				0, 0, 0, 0
			},
			"[^"
		},
	precrl =
		{
			{
				TOKTYPE_OPCRLUP,
				2,

				0, 0, 0, 0
			},
			"{^"
		},
	
	prepar_cl =
		{
			{
				TOKTYPE_SYM_PREPARCL,
				2,

				0, 0, 0, 0
			},
			"^)"
		},
	presqr_cl =
		{
			{
				TOKTYPE_SYM_PRESQRCL,
				2,

				0, 0, 0, 0
			},
			"^]"
		},
	precrl_cl =
		{
			{
				TOKTYPE_SYM_PRECRLCL,
				2,

				0, 0, 0, 0
			},
			"^}"
		};



	/* ( token* -- token* ( 0 | 1 ) ) */
	/* This function MUST be given a token pointer via it's */
	/*  void argument: that's used to comunicatre WHAT is */
	/*  required. */
retframe require_match( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  require_match );
	if( !v )
	{
			/* Fatal error, v must point to a token! */
		???
	}
	
	token *tok, *ref = (token*)v;
	uintptr_t a, ret = 0;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  require_match, scratch );
	*tok = (token*)a;
	
	switch( ref->header.toktype )
	{
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
			a = ( (tokengroup*)ref )->subtype;
			break;
		
		case TOKTYPE_TOKENGROUP_STRMERGE:
		case TOKTYPE_TOKENGROUP_COMNTMERGE:
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
			a = ( (tokenbranch*)ref )->subtype;
			break;
		
		default:
			a = ref->header.toktype;
	}
	switch( tok->header.toktype )
	{
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
			if( ( (tokengroup*)ref )->subtype == a )
			{
				ret = 1;
			}
			break;
		case TOKTYPE_TOKENGROUP_WHITESPACE:
			if
			(
				a == TOKTYPE_TOKENGROUP_WHITESPACE ||
				a == TOKTYPE_SPACE ||
				a == TOKTYPE_NEWLINE
			)
			{
				ret = 1;
			}
			break;
		
		case TOKTYPE_TOKENGROUP_STRMERGE:
		case TOKTYPE_TOKENGROUP_COMNTMERGE:
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
			if( ( (tokenbranch*)ref )->subtype == a )
			{
				ret = 1;
			}
			break;
		
		case TOKTYPE_NAME:
			if
			(
				a == tok->header.toktype &&
				strcmp( ref->text, tok->text ) == 0
			)
			{
				ret = 1;
			}
			break;
		case TOKTYPE_SPACE:
		case TOKTYPE_NEWLINE:
			if
			(
				a == TOKTYPE_TOKENGROUP_WHITESPACE ||
				a == tok->header.toktype
			)
			{
				ret = 1;
			}
			break;
		
		default:
			if( tok->header.toktype == a )
			{
				ret = 1;
			}
			break;
	}
	
	STACKPUSH_UINT( stkp->data, ret,  require_match, scratch );
	RETFRAMEFUNC( stkp,  require_match );
}
	/* ( token* -- token* boolean ) */
	/* Place 1 on the data stack if the token pointer */
	/*  points to a preprocessor opening token, else */
	/*  place 0 on the data stack. */
retframe require_preprocoppar( stackpair *stkp, void *v )
{
	return( (retframe){ &require_match, (void*)&prepar } );
}
retframe require_preprocopsqr( stackpair *stkp, void *v )
{
	return( (retframe){ &require_match, (void*)&presqr } );
}
retframe require_preprocopcrl( stackpair *stkp, void *v )
{
	return( (retframe){ &require_match, (void*)&precrl } );
}
	/* The combination of all three above, has the same */
	/*  in/out sig. */
retframe require_preprocopener( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  require_match );
	
		/* The instructions that comprise this procedure. */
	static const retframe_parr seq =
		(retframe_parr)
		{
			8 /* The number of instructions. */,
			{
				/* Setup the three tests that we'll be running: */
				/*  for each test, we'll then be swapping the */
				/*  result and the token pointer's places, so */
				/*  that the token pointer stays on top. */
				
				(retframe){ &require_preprocopcrl, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				(retframe){ &require_preprocopsqr, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				(retframe){ &require_preprocoppar, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				
					/* Move the pointer back below the results... */
				(retframe){ &swap4th, (void*)0 },
					/* ... and merge the results into one. */
				(retframe){ &ior3, (void*)0 }
			}
		};
	
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	/* The closing bracket versions. */
retframe require_preprocclpar( stackpair *stkp, void *v )
{
	return( (retframe){ &require_match, (void*)&prepar_cl } );
}
retframe require_preprocclsqr( stackpair *stkp, void *v )
{
	return( (retframe){ &require_match, (void*)&presqr_cl } );
}
retframe require_preprocclcrl( stackpair *stkp, void *v )
{
	return( (retframe){ &require_match, (void*)&precrl_cl } );
}
retframe require_preproccloser( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  require_match );
	
		/* The instructions that comprise this procedure. */
	static const retframe_parr seq =
		(retframe_parr)
		{
			8 /* The number of instructions. */,
			{
				/* Setup the three tests that we'll be running: */
				/*  for each test, we'll then be swapping the */
				/*  result and the token pointer's places, so */
				/*  that the token pointer stays on top. */
				
				(retframe){ &require_preprocclcrl, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				(retframe){ &require_preprocclsqr, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				(retframe){ &require_preprocclpar, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				
				
					/* Move the pointer back below the results... */
				(retframe){ &swap4th, (void*)0 },
					/* ... and merge the results into one. */
				(retframe){ &ior3, (void*)0 }
			}
		};
	
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	/* As with require_preprocopener(). */
retframe require_octothorp( stackpair *stkp, void *v )
{
		/* Our reference token. */
	static token
		octo =
		{
			{
				TOKTYPE_OCTO,
				1,
				
				0, 0, 0, 0
			},
			"#"
		};
		/* The instructions that comprise this procedure. */
	static const retframe_parr octoreq =
		(retframe_parr)
		{
			3,
			{
				(retframe){ &require_match, &octo },
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ , } /* Call something to discard the token pointer. */
			}
		};
	
	return( (retframe){ &enqueue_returns, (void*)&octoreq } );
}
	/* As with require_preprocopener(). */
retframe require_anyname( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  require_anyname );
	
	token *tok;
	uintptr_t a, ret = 0;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  require_anyname, scratch );
	*tok = (token*)a;
	
	if
	(
		tok->header.toktype == TOKTYPE_NAME &&
		strlen( tok->text ) > 0
	)
	{
		ret = 1;
	}
	
	STACKPUSH_UINT( stkp->data, ret,  require_anyname, scratch );
	RETFRAMEFUNC( stkp,  require_anyname );
}



	/* token* token* -- tokenbranch* */
retframe tokenbranch_buildleadbody( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  buildbranch_leadbody );
	
	uintptr_t body, lead;
	int scratch, tmp;
	
	
	STACKPEEK_UINT( stkp->data, 0, body,  buildbranch_leadbody, scratch );
	if( !body )
	{
		???
	}
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), lead,  buildbranch_leadbody, scratch );
	if( !lead )
	{
		???
	}
	
		/* That argument probably shouldn't be in the func sig... */
	tokenbranch *tb = build_tokenbranch( 0 );
	if( !tb )
	{
		???
	}
	
	tmp = set_lead_tokenbranch( tb, (token_head*)lead );
	if( !tmp )
	{
		???
	}
	tmp = push_body_tokenbranch( tb, (token_head*)body );
	if( !tmp )
	{
		???
	}
	tb->subtype = ( (token_head*)body )->header.toktype;
	
		/* Discard both of those tokens: we already have them linked. */
	STACKPOP_UINT( stkp->data, body,  buildbranch_leadbody, scratch );
	STACKPOP_UINT( stkp->data, lead,  buildbranch_leadbody, scratch );
	
	
		/* Return the result.. */
	STACKPUSH_UINT( stkp->data, &( tb->header ),  buildbranch_leadbody, scratch );
	RETFRAMEFUNC( stkp,  require_anyname );
}

	/* ( -- tokenbranch* ) */
retframe tokenbranch_build( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  buildbranch_leadbody );
	
	tokenbranch *tb = build_tokenbranch( 0 );
	if( !tb )
	{
		???
	}
	
		/* Return the result.. */
	STACKPUSH_UINT( stkp->data, &( tb->header ),  buildbranch_leadbody, scratch );
	RETFRAMEFUNC( stkp,  require_anyname );
}
	/* ( tokenbranch* token* -- tokenbranch* ) */
retframe tokenbranch_setlead( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_setlead );
	
	uintptr_t tok, bran;
	int scratch, tmp;
	
	
		/* Peek BENEATH the top of the stack, so that an */
		/*  error here will leave things least molested. */
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), bran,  tokenbranch_setlead, scratch );
	if( !bran )
	{
		???
	}
	if( ( (tokenbranch*)bran )->lead )
	{
		???
	}
	
	STACKPOP_UINT( stkp->data, tok,  tokenbranch_setlead, scratch );
	if( !tok )
	{
		???
	}
	
	
	tmp = set_lead_tokenbranch( (tokenbranch*)bran, (token_head*)tok );
	if( !tmp )
	{
		???
	}
	
	
	RETFRAMEFUNC( stkp,  tokenbranch_setlead );
}
	/* ( tokenbranch* token* -- tokenbranch* ) */
retframe tokenbranch_pushbody( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_pushbody );
	
	uintptr_t tok, bran;
	int scratch, tmp;
	
	
		/* Peek BENEATH the top of the stack, so that an */
		/*  error here will leave things least molested. */
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), bran,  tokenbranch_pushbody, scratch );
	if( !bran )
	{
		???
	}
	STACKPOP_UINT( stkp->data, tok,  tokenbranch_pushbody, scratch );
	if( !tok )
	{
		???
	}
	
	
	tmp = push_body_tokenbranch( (tokenbranch*)bran, (token_head*)tok );
	if( !tmp )
	{
		???
	}
	
	
	RETFRAMEFUNC( stkp,  tokenbranch_pushbody );
}
	/* ( tokenbranch* token* -- tokenbranch* token* ) */
retframe tokenbranch_setsubtype( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_setsubtype );
	
	uintptr_t tok, bran;
	int scratch, tmp;
	
	
	STACKPEEK_UINT( stkp->data, 0, tok,  tokenbranch_setsubtype, scratch );
	if( !tok )
	{
		???
	}
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), bran,  tokenbranch_setsubtype, scratch );
	if( !bran )
	{
		???
	}
	
	
	if( ( (token_head*)tok )->toktype == TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		tok = ( (tokengroup*)tok )->subtype;
		
	} else if( ( (token_head*)tok )->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		tok = ( (tokenbranch*)tok )->subtype;
		
	} else {
		
		tok = ( (token_head*)tok )->toktype;
	}
	( (tokenbranch*)bran )->subtype = tok;
	
	
	RETFRAMEFUNC( stkp,  tokenbranch_setsubtype );
}
	/* ( tokenbranch* token* -- tokenbranch* ) */
retframe tokenbranch_settail( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_settail );
	
	uintptr_t tok, bran;
	int scratch, tmp;
	
	
		/* Peek BENEATH the top of the stack, so that an */
		/*  error here will leave things least molested. */
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), bran,  tokenbranch_settail, scratch );
	if( !bran )
	{
		???
	}
	if( ( (tokenbranch*)bran )->tail )
	{
		???
	}
	
	STACKPOP_UINT( stkp->data, tok,  tokenbranch_settail, scratch );
	if( !tok )
	{
		???
	}
	
	
	tmp = set_tail_tokenbranch( (tokenbranch*)bran, (token_head*)tok );
	if( !tmp )
	{
		???
	}
	
	
	RETFRAMEFUNC( stkp,  tokenbranch_settail );
}
	/* ( tokenbranch* -- ) */
retframe tokenbranch_settail( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_settail );
	
		/* Honestly, we're best off just delegating to the */
		/*  preexisting deallocation system in complexlex.c, */
		/*  since it already uses the same function we would. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}
