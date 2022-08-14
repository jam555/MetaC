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
	STACK_CHECK( ( stack ),  &errs, ( caller ), lexalike_ENDRETFRAME )
#define STACKCHECK2( stack, v,  caller ) \
	STACK_CHECK2( ( stack ), ( v ),  &errs, ( caller ), lexalike_ENDRETFRAME )

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



	/* ( token* -- token* ) */
retframe bad_token( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  bad_token );
	bad_token_report *rep = (bad_token_report*)v_;
	
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  bad_token, scratch );
	
	
	msg_interface
	(
		&errs, 1,
		
		( rep->file ),
		(uintmax_t)( rep->caller ), ( rep->callername ),
		( rep->line ),
		
		( rep->message ),
		(unsigned)( ( (token_head*)a )->toktype )
	);
#define bad_token_ERRMSG "\nSyntax error from %s:%jx, %s(), file line %x : %s, ->toktype was %x."
	
	
	if( rep.freethis )
	{
		free( rep );
	}
	RETFRAMEFUNC( stkp,  bad_token );
}



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
	/* As with require_preprocopener(). */
retframe require_comma( stackpair *stkp, void *v )
{
		/* Our reference token. */
	static token
		comma =
		{
			{
				TOKTYPE_SYM_COMMA,
				1,
				
				0, 0, 0, 0
			},
			","
		};
		/* The instructions that comprise this procedure. */
	static const retframe_parr octoreq =
		(retframe_parr)
		{
			3,
			{
				(retframe){ &require_match, &comma },
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ , } /* Call something to discard the token pointer. */
			}
		};
	
	return( (retframe){ &enqueue_returns, (void*)&octoreq } );
}
	/* As with require_preprocopener(). */
retframe require_parenopener( stackpair *stkp, void *v )
{
		/* Our reference token. */
	static token
		tok =
		{
			{
				TOKTYPE_OPPAR,
				1,
				
				0, 0, 0, 0
			},
			"("
		};
	return( (retframe){ &require_match, (void*)( &tok ) } );
}
retframe require_parencloser( stackpair *stkp, void *v )
{
		/* Our reference token. */
	static token
		tok =
		{
			{
				TOKTYPE_SYM_PARENCLOSE,
				1,
				
				0, 0, 0, 0
			},
			")"
		};
	return( (retframe){ &require_match, (void*)( &tok ) } );
}
retframe require_parsebreak( stackpair *stkp, void *v )
{
		/* Our reference token. */
	static token
		tok =
		{
			{
				TOKTYPE_PARSEBREAK,
				1,
				
				0, 0, 0, 0
			},
			")"
		};
	return( (retframe){ &require_match, (void*)( &tok ) } );
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
	STACKCHECK( stkp,  tokenbranch_build );
	
	int scratch;
	tokenbranch *tb = build_tokenbranch( 0 );
	if( !tb )
	{
		???
	}
	
		/* Return the result.. */
	STACKPUSH_UINT( stkp->data, &( tb->header ),  tokenbranch_build, scratch );
	RETFRAMEFUNC( stkp,  tokenbranch_build );
}
	/* ( tokenbranch* token* -- tokenbranch* token* ) */
retframe tokenbranch_initbase( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_initbase );
	
	uintptr_t tok, bran, tmp;
	int scratch;
	
	
	STACKPEEK_UINT( stkp->data, 0, tok,  tokenbranch_initbase, scratch );
	if( !tok )
	{
		???
	}
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), bran,  tokenbranch_initbase, scratch );
	if( !bran )
	{
		???
	}
	
	
	
	tmp = ( (token_head*)bran )->toktype;
	*( (token_head*)bran ) = *( (token_head*)tok );
	( (token_head*)bran )->toktype = tmp;
	
		/* *_setsubtype() does the rest of what's needed. */
	return( (retframe){ &tokenbranch_setsubtype, (void*)0 } );
}
	/* ( tokenbranch* token* -- tokenbranch* token* ) */
retframe tokenbranch_setsubtype( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_setsubtype );
	
	uintptr_t tok, bran, tmp;
	int scratch;
	
	
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
	
	
		/* Note that this is correct: for the SOURCE OF THE TYPE we want */
		/*  to support all three of these, whereas fcor the destination */
		/*  we DON'T want to support all of them. */
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
retframe tokenbranch_dealloc( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokenbranch_dealloc );
	
		/* Honestly, we're best off just delegating to the */
		/*  preexisting deallocation system in complexlex.c, */
		/*  since it already uses the same function we would. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}



	/* ( -- tokengroup* ) */
retframe tokengroup_build( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokengroup_build );
	
	int scratch;
	tokengroup *tg = build_tokengroup( 0 );
	if( !tg )
	{
		???
	}
	
		/* Return the result.. */
	STACKPUSH_UINT( stkp->data, &( tg->header ),  tokengroup_build, scratch );
	RETFRAMEFUNC( stkp,  tokengroup_build );
}
???
/*
	int regrow_tokengroup
	(
		tokengroup *tgrp,
		size_t newlen
	);
*/
???
	/* ( tokengroup* token* -- tokengroup* ) */
retframe tokengroup_pushtoken( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokengroup_pushtoken );
	
	uintptr_t tok, grp;
	int scratch, tmp;
	
	
		/* Peek BENEATH the top of the stack, so that an */
		/*  error here will leave things least molested. */
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), grp,  tokengroup_pushtoken, scratch );
	if( !grp )
	{
		???
	}
	
	STACKPOP_UINT( stkp->data, tok,  tokengroup_pushtoken, scratch );
	if( !tok )
	{
		???
	}
	
	
	tmp = pushto_tokengroup( (tokengroup*)grp, (token_head*)tok );
	if( !tmp )
	{
		???
	}
	
	
	RETFRAMEFUNC( stkp,  tokengroup_pushtoken );
}
	/* ( tokengroup* -- ) */
retframe tokengroup_dealloc( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokengroup_dealloc );
	
		/* Honestly, we're best off just delegating to the */
		/*  preexisting deallocation system in complexlex.c, */
		/*  since it already uses the same function we would. */
	return( (retframe){ &invoke_dealloctoken, (void*)0 } );
}






#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs =
		{
			(msg_piece[]){ BUILD_MSGPIECE_STYLE( (msg_style){ bad_token_ERRMSG } ) },
			1
		};
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
