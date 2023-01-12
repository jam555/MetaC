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
	/*  void argument: that's used to comunicate WHAT is */
	/*  required. */
retframe require_match( stackpair *stkp, void *v )
{
	STACKCHECK2( stkp, v,  require_match );
	
	token *tok, *ref = (token*)v;
	uintptr_t a, ret = 0;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  require_match, scratch );
	*tok = (token*)a;
	
	??? /* Can this be compacted via use of token.h's simplifytype stuff? */
	
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
retframe require_strmerge( stackpair *stkp, void *v )
{
		/* Our reference token. */
	static token
		tok =
		{
			{
				TOKTYPE_TOKENGROUP_STRMERGE,
				0,
				
				0, 0, 0, 0
			},
			""
		};
	return( (retframe){ &require_match, (void*)( &tok ) } );
}
	/* ( token* -- token* boolean ) */
	/* Place 1 on the data stack if the token pointer */
	/*  points to a preprocessor opening token, else */
	/*  place 0 on the data stack. */
retframe require_anystring( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  require_anystring );
	
	token *tok;
	uintptr_t a, ret = 0;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  require_anystring, scratch );
	*tok = (token*)a;
	
	if
	(
		tok->header.toktype == TOKTYPE_SQSTR ||
		tok->header.toktype == TOKTYPE_DQSTR
	)
	{
		ret = 1;
	}
	
	STACKPUSH_UINT( stkp->data, ret,  require_anystring, scratch );
	RETFRAMEFUNC( stkp,  require_anystring );
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
	
	STACKPOP_UINT( stkp->data, tok,  tokengroup_pushtoken, scratch );
	
	
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



	/* ( char_parr* char_parr* -- char_parr* 1 ) */
retframe grow_string_conclude( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  grow_string_conclude );
	
	int scratch;
	uintptr_t tmp;
	char_parr *tail, *head;
	
	
	
	STACKPEEK_UINT( stkp->data, 0, tmp,  grow_string_conclude, scratch );
	tail = (char_parr*)tmp;
	
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), tmp,  grow_string_conclude, scratch );
	head = (char_parr*)tmp;
	
	
	
		/* We only need one of the terminating nulls, not both. */
	char_pascalarray_result res = char_pascalarray_build( head->len + tail->len - 1 );
	char_parr *a;
#define grow_string_conclude_ONERR1( err ) \
		???
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, grow_string_conclude_ONERR1 );
		/* Copy the two strings together. */
		??? /* Am I interpreting ->len correctly? */
	memcpy
		(
		(void*)( a->body ),
		(const void*)( head->body ),
		
		head->len - 1
	);
	memcpy
		(
		(void*)( (char*)( a->body ) + ( head->len - 1 ) ),
		(const void*)( tail->body ),
		
		tail->len
	);
	
	
	
	lib4_success_result b;
	
	STACKPOP_UINT( stkp->data, tmp,  grow_string_conclude, scratch );
	STACKPOP_UINT( stkp->data, tmp,  grow_string_conclude, scratch );
	
	lib4_result res = char_pascalarray_destroy( head );
#define grow_string_conclude_ONERR2( err ) \
		???
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETb, grow_string_conclude_ONERR2 );
	
	res = char_pascalarray_destroy( tail );
#define grow_string_conclude_ONERR3( err ) \
		???
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETb, grow_string_conclude_ONERR3 );
	
	
	
	STACKPUSH_UINT( stkp->data, (uintptr_t)a,  grow_string_conclude, scratch );
	STACKPUSH_UINT( stkp->data, (uintptr_t)1,  grow_string_conclude, scratch );
	
	RETFRAMEFUNC( stkp,  grow_string );
}
	/* ( char_parr* token* -- char_parr* ( 1 )|( token* 0 ) ) */
retframe grow_string( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  grow_string );
	
	uintptr_t tok;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, tok,  grow_string, scratch );
	
	if
	(
		( (token*)tok )->toktype != TOKTYPE_SQSTR &&
		( (token*)tok )->toktype != TOKTYPE_DQSTR
	)
	{
		STACKPUSH_UINT( stkp->data, (uintptr_t)0,  grow_string, scratch );
		
		RETFRAMEFUNC( stkp,  grow_string );
	}
	
	static const retframe_parr seq =
		(retframe_parr)
		{
			4, /* Number of retframes  */
			{
					/* ( string-token* -- token* char_parr* ) */
				(retframe){ &stringtoken2char_parr, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ &invoke_dealloctoken, (void*)0 },
				(retframe){ &grow_string_conclude, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, &seq } );
}

	/*  The stack should be ( token* &stringtoken2char_parr ). */
retframe convert_tokengroup2string_catcherr1( stackpair *stkp, void *v );
	/* ( tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) -- ??? ) */
retframe convert_tokengroup2string_catcherr1_groupconclude( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t rescategory;
	
	STACKCHECK( stkp,  convert_tokengroup2string_catcherr1_groupconclude );
	
	???
	
	STACKPOP_UINT( &( stkp->data ), rescategory,  convert_tokengroup2string_catcherr1_groupconclude, scratch );
	switch( rescategory )
	{
		default:
		case 0:
		case 1:
		case 2:
	}
	
									/* ( wrapper-token* -- ) */
								(retframe){ &invoke_dealloctoken, (void*)0 },
									/* Let's try this again... */
								(retframe){ &convert_tokengroup2string, (void*)&callhere }
	
	???
}
	/*  The stack should be ( token* &stringtoken2char_parr ). */
retframe convert_tokengroup2string_catcherr1( stackpair *stkp, void *v )
{
	static retframe callhere = (retframe){ &convert_tokengroup2string_catcherr1, (void*)0 };
	static retframe_parr
		indirectionseq =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* ( wrapper-token* refed-token* -- refed-token* wrapper-token* ) */
					(retframe){ &swap2nd, (void*)0 },
						/* ( wrapper-token* -- ) */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* Let's try this again... */
					(retframe){ &convert_tokengroup2string, (void*)&callhere }
				}
			};
	
	int scratch;
	uintptr_t tmp;
	
	STACKCHECK( stkp,  convert_tokengroup2string_catcherr1 );
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  convert_tokengroup2string_catcherr1, scratch );
	if( tmp != (uintptr_t)&stringtoken2char_parr )
	{
		/* Error! Is this breaking or continuable? */
		???
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), tmp,  convert_tokengroup2string_catcherr1, scratch );
	token *tok = (token*)tmp;
	
#define convert_tokengroup2string_catcherr1_BADRET( val ) \
		???
	deep_toktype a;
	deeptoktype_result deepres = get_deeptoktype( &( tok->header ) );
	DEEPTOKTYPE_RESULT_BODYMATCH( deepres, LIB4_OP_SETa, convert_tokengroup2string_catcherr1_BADRET );
	
	
	switch( a.shallow_toktype )
	{
		case TOKTYPE_SQSTR:
		case TOKTYPE_DQSTR:
			???
			
		case TOKTYPE_TOKENGROUP_MACROTOKEN:
		case TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION:
			STACKPUSH_UINT(
				stkp->data,
				(uintptr_t)( ( (macro_token*)tok )->tok ),
				
				convert_tokengroup2string_catcherr1,
				scratch
			);
			return( (retframe){ &enqueue_returns, (void*)&indirectionseq } );
			
		case TOKTYPE_TOKENGROUP_STRMERGE:
		case TOKTYPE_TOKENGROUP_COMNTMERGE:
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
		case TOKTYPE_TOKENGROUP_WHITESPACE:
		case TOKTYPE_TOKENGROUP_DELIMITED:
			{
				int acc =
					( ( (tokenbranch*)tok )->lead ? 1 + 8 : 0 ) +
					( ( (tokenbranch*)tok )->body ? 1 + 16 : 0 ) +
					( ( (tokenbranch*)tok )->tail ? 1 + 32 : 0 );
					
				if( acc & 3 != 1 )
				{
						/* Error! Incompatible branch count! */
					STACKPUSH_UINT(
						&( stkp->data ),
						(uintptr_t)&token2char_parr,
						
						convert_tokengroup2string_catcherr1,
						scratch
					);
					return( *( (retframe*)v_ ) );
				}
				
				switch( acc & 56 )
				{
					case 0:
							/* Error! The code above should render this impossible! */
						???
						
					case 8:
						tok = ( (tokenbranch*)tok )->lead;
						break;
						
					case 16:
						tok = ( (tokenbranch*)tok )->body;
						break;
						
					case 32:
						tok = ( (tokenbranch*)tok )->tail;
						break;
				}
				STACKPUSH_UINT(
					stkp->data,
					(uintptr_t)tok,
					
					convert_tokengroup2string_catcherr1,
					scratch
				);
				switch( acc & 56 )
				{
					case 0:
							/* Error! The code above should render this impossible! */
						???
						
					case 8:
						( (tokenbranch*)tok )->lead = (token_head*)0;
						break;
						
					case 16:
						( (tokenbranch*)tok )->body = (token_head*)0;
						break;
						
					case 32:
						( (tokenbranch*)tok )->tail = (token_head*)0;
						break;
				}
			}
			
			return( (retframe){ &enqueue_returns, (void*)&indirectionseq } );
			
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
			if( ( (tokengroup*)tok )->used != 1 )
			{
				static retframe_parr
					indirectionseq =
						(retframe_parr)
						{
							??? , /* Number of retframes  */
							{
									/* ( tokengroup* -- tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) ) */
								(retframe){ &convert_tokengroup2string, (void*)&callhere },
									/* ( ... -- ??? ) */
								(retframe){ &convert_tokengroup2string_catcherr1_groupconclude, (void*)0 }
							}
						};
				
				
				
				/* Error! Incompatible token count! This function can't handle it! */
				STACKPUSH_UINT( &( stkp->data ), (uintptr_t)&token2char_parr,  convert_tokengroup2string_catcherr1, scratch );
				return( *( (retframe*)v_ ) );

			} else {
				
				/* Why are we even in this function? */
				
				if( !( ( (tokengroup*)&( tok->header ) )->arr ) )
				{
						/* Error! Missing pascal array! */
					???
				}

				tok = ( (tokengroup*)&( tok->header ) )->arr->body[ 0 ];
			}
			
		case TOKTYPE_INVALID:
		case TOKTYPE_PARSEBREAK:
		case TOKTYPE_NUMBER_UINT:
		case TOKTYPE_TOKENGROUP_MACROLINK:
		case TOKTYPE_TOKENGROUP_MACRORUN:
		case TOKTYPE_TOKENGROUP_MACRODIRECTIVE:
		case TOKTYPE_TOKENGROUP_MACROCALL:
			/* Error: none of these are stringifiable! */
			
		default:
			???
	}
	
	
	
	???
					/* We should probably support the embedding of */
					/*  comments into strings, but we will currently */
					/*  not do that. */
	/*
					test token
						if breaking type
							report error
							deallocate token
							call swap3rd_retframe to bring retframe to top
							use vm_datacall() to run provided on-error retframe
								that SHOULD act like a longjump()
	*/
}
	/* ( tokengroup* -- tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) ) */
	/* v_ must be a pointer to a retframe{}. */
retframe convert_tokengroup2string( stackpair *stkp, void *v_ )
{
	static retframe push_noop = (retframe){ &vm_push_noop, (void*)0 };
	static retframe
		not_retframe = (retframe){ &vm_not, (void*)0 },
		push0_retframe = (retframe){ &vm_push0, (void*)0 },
		push1_retframe = (retframe){ &vm_push1, (void*)0 },
		push_firsthook_onelems_loop,
		firsthook_catcherr = (retframe){ &enqueue_returns, (void*)&firsthook_catcherr_ };
	static retframe
		firsthook_onelems = (retframe){ &enqueue_returns, (void*)&firsthook_onelems_ },
		firsthook_onelemless = (retframe){ &enqueue_returns, (void*)&firsthook_onelemless_ };
	
	static retframe_parr
			/* ( retframe tokengroup* cparr* token* 0 -- retframe tokengroup* cparr* 0 ) */
		canonical_growstring_from_tg_catcherr =
			{
				??? , /* Number of retframes  */
				{
						/* ( flag -- ... ) */
					(retframe){ &drop, (void*)0 },
					
					(retframe){ &convert_tokengroup2string_catcherr, (void*)0 },
					
						/* ( token* --  ) */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* Ding, dong, the token's dead, the token's dead, ... */
					
						/* ( ... -- flag ) */
					(retframe){ &vm_push0, (void*)0 }
				}
			};
	
	
	??? ??? ???
	
	static retframe
		loop_body,
		on_done;
	static retframe_parr
			/* ( retframe cparr* tokengroup* -- retframe cparr* tokengroup* ) */
		loop_testAndDispatch =
			{
				7, /* Number of retframes  */
				{
						(retframe){ &vm_lengthof_tokengroup, (void*)0 },
								/* ( ... -- flag ) */
							(retframe){ &vm_not, (void*)0 },
							(retframe){ &vm_pushretframe_if, (void*)&on_done },
								/* ( tokengroup* cparr* tokengroup* flag -- ... ) */
							(retframe){ &vm_pushretframe_else, (void*)&loop_body },
								/* ( flag retframe -- retframe flag ) */
							(retframe){ &swap2nd_to_retframe, (void*)0 },
							/* ( flag --  ) */
						(retframe){ &drop, (void*)0 },
						(retframe){ &vm_datacall, (void*)0 }
				}
			};
	
	
	static retframe_parr
		on_done_ =
			{
				??? , /* Number of retframes  */
				{
					???
				}
			},
			/* ( retframe tokengroup* cparr* token* 0 -- ??? ) */
		on_err_ =
			{
				??? , /* Number of retframes  */
				{
					???
				}
			},
			/* ( retframe cparr* tokengroup* -- ... -- cparr* tokengroup* ) */
		loop_body_ =
			{
				9, /* Number of retframes  */
				{
					/* Accumulate string: ( retframe cparr* tokengroup*-- retframe cparr* tokengroup* ) */
						/* ( cparr* tokengroup* -- cparr* tokengroup* token* ) */
					(retframe){ &vm_popfront_tokengroup, (void*)0 },
						/* ( ... -- tg* tok* cparr* ) */
					(retframe){ &raise3rd, (void*)0 },
						/* ( ... -- tg* cparr* tok* ) */
					(retframe){ &swap2nd, (void*)0 },
						/* ( char_parr* token* -- char_parr* ( 1 )|( token* 0 ) ) */
					(retframe){ &grow_string, (void*)0 },
							/* Catch the syntax error. */
						(retframe){ &enqueue_else, (void*)&on_err_ },
						/* Don't care about the flag anymore. */
					(retframe){ &drop, (void*)0 },
					
					???
					
					(retframe){ &enqueue_returns, (void*)&loop_testAndDispatch }
				}
			},
			/* ( retframe tokengroup* flag -- tokengroup* cparr* ( 1 | token* 2 ) ) */
		on_elems =
			{
				9, /* Number of retframes  */
				{
						/* ( flag -- ... ) */
					(retframe){ &drop, (void*)0 },
					
						/* ( tokengroup* -- tokengroup* token* ) */
					(retframe){ &vm_popfront_tokengroup, (void*)0 },
						/* ( string-token* -- token* char_parr* ) */
					(retframe){ &stringtoken2char_parr, (void*) ??? },
						/* ( tg* tok* cparr* -- tg* cparr* tok* ) */
					(retframe){ &swap2nd, (void*)0 },
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* ( tg* cparr* -- cparr* tg* ) */
					(retframe){ &swap2nd, (void*)0 },
					
					(retframe){ &enqueue_returns, (void*)&loop_testAndDispatch },
					
						/* ( ... -- flag ) */
					(retframe){ &vm_push0, (void*)0 }
				}
			},
			/* ( retframe tokengroup* flag -- tokengroup* 0result flag ) */
		on_noelems =
			{
				6, /* Number of retframes  */
				{
						/* ( flag -- ... ) */
					(retframe){ &drop, (void*)0 },
							/* ( retframe tokengroup* -- tokengroup* retframe ) */
						(retframe){ &swap2nd_from_retframe, (void*)0 },
							/* ( retframe[1] -- ... ) */
						(retframe){ &drop, (void*)0 },
							/* ( retframe[0] -- ... ) */
						(retframe){ &drop, (void*)0 },
						(retframe){ &vm_push0, (void*)0 },
					(retframe){ &vm_push1, (void*)0 }
						/* ( ... -- 0result flag ) */
				}
			};
	static retframe_parr
		seq =
			(retframe_parr)
			{
				5, /* Number of retframes  */
				{
						/* ( tokengroup* retframe -- retframe tokengroup* ) */
					(retframe){ &swap2nd_to_retframe, (void*)0 },
					
					(retframe){ &vm_lengthof_tokengroup, (void*)0 },
					(retframe){ &vm_not, (void*)0 },
					(retframe){ &enqueue_if, (void*)&on_noelems },
					(retframe){ &enqueue_else, (void*)&on_elems },
					(retframe){ &drop, (void*)0 }
					
					/* The retframe MUST be removed BEFORE we reach here. */
				}
			};
	
	int scratch;
	uintptr_t tok;
	
	static int hasrun = 0;
	if( !hasrun )
	{
		loop_body = (retframe){ &enqueue_returns, (void*)&loop_body_ };
		on_done = (retframe){ &enqueue_returns, (void*)&on_done_ };
		
		hasrun = 1;
	}
	
	STACKCHECK2( stkp, v_,  convert_tokengroup2string );
	
	STACKPEEK_UINT( &( stkp->data ), 0, tok,  convert_tokengroup2string, scratch );
	
	if( ( (token*)tok )->toktype != ??? )
	{
		???
	}
	
	??? push( *( (retframe*)v_ ) );
	
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	/* ( tokengroup* -- tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) ) */
	/* v_ must be a pointer to a retframe{}. */
retframe convert_tokengroup2string( stackpair *stkp, void *v_ )
{
	static retframe_parr
		on_err =
			{
					/* ( tg* 1 token* &stringtoken2char_parr -- tg* tok* &strtok2cparr 1 ) */
				raise3rd
				drop
				drop
					/* ( ... -- tg* tok* 0 ) */
				push0
					/* ( ... -- tg* 0 tok* ) */
				swap2nd
				push0
					/* ( ... -- tg* 0 tok* 0 ) */
			};
	
	int scratch;
	uintptr_t tok;
	
	static int hasrun = 0;
	if( !hasrun )
	{
		loop_body = (retframe){ &enqueue_returns, (void*)&loop_body_ };
		on_done = (retframe){ &enqueue_returns, (void*)&on_done_ };
		
		hasrun = 1;
	}
	
	STACKCHECK2( stkp, v_,  convert_tokengroup2string );
	
	???
	
	static loop =
		{
			
		}
	if( is tg )
	{
		if( > 0 )
		{
				/* tg* -- tg* tok* */
			popfront
				/* tg* tok* -- tg* tok* 1 */
			push1
				/* tg* tok* 1 -- tg* 1 tok* */
			swap2nd
				/* ( tg* 1 string-token* -- tg* flag token* char_parr* ) */
			stringtoken2char_parr( on_err )
				/* ( tg* flag token* char_parr* -- tg* token* char_parr* flag ) */
			raise3rd
			vm_if
					/* ( ... -- tg* token* char_parr* ) */
				drop
						/* ( ... -- tg* char_parr* token* ) */
					swap2nd
						/* ( ... -- tg* char_parr* ) */
					invoke_dealloctoken
						/* ( ... -- char_parr* tg* ) */
					swap2nd
						/* ( ... -- char_parr* tg* len ) */
					grouplen
						/* ( ... -- char_parr* tg* !len ) */
					not
						if
							drop
									/* ( ... -- tg* char_parr* ) */
								swap
							push1
								/* ( ... -- tg* char_parr* 1 1 ) */
							push1
						else
								/* ( ... -- char_parr* tg* ) */
							drop
								/* ( ... -- char_parr* tg* tok* ) */
							popfront
								/* ( ... -- tg* tok* char_parr* ) */
							raise3rd
								/* ( ... -- tg* char_parr* tok* ) */
							swap
							???
/* Prospective loop code: */
	/* ( tg* char_parr* token* -- tg* char_parr* ( 1 )|( token* 0 ) ) */
retframe grow_string( stackpair *stkp, void *v );
if
		/* ( tg* char_parr* 1 -- tg* char_parr* ) */
	drop
			/* ( ... -- tg* char_parr* len ) */
		grouplen
			/* ( ... -- tg* char_parr* !len ) */
		not
		if
				/* ( ... -- tg* char_parr* 1 noop-retframe ) */
			push_noop
		else
				/* ( ... -- tg* char_parr* ) */
			drop
				/* ( ... -- tg* char_parr* loop-retframe ) */
			push_loop
		/* ( ... -- tg* char_parr* ( | 1 ) retframe 1 ) */
	push1
else
		/* ( tg* char_parr* token* 0 -- tg* char_parr* token* ) */
	drop
			/* ( ... -- tg* char_parr* token* 2 ) */
		push2 /* result value */
			/* ( ... -- tg* char_parr* token* 2 noop-retframe ) */
		push_noop
		/* ( ... -- tg* char_parr* token* 2 noop-retframe 0 ) */
	push0
	/* ( tg* char_parr* ( | 1 | token* 2 ) retframe ( 0 | 1 ) -- tg* char_parr* ( | 1 | token* 2 ) retframe ) */
drop
stack_run
	/* ( ... -- tokengroup* char_parr* ( 1 | token* 2 | or just keep looping ) */
								loop
							push0
						/* ( ??? !len -- ??? ) */
					drop
					/* ( ... -- char_parr* tg* val ??? ) */
					/* Note: need ( ... -- tokengroup* char_parr* ( 1 | token* 2 ) */
				push1
			vm_else
					/* ( ... -- tg* token* dummy0 0flag ) */
				drop
						/* ( tg* token* dummy0 -- tg* dummy0 token* ) */
					swap2nd
						/* ( ... -- tg* dummy0 token* 2 ) */
					push2
				push0
			drop
				/* ( ... -- tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) ) */
			
		} else {
			
			error "No elements"
		}
		
	} else {
		
		error "Wrong type"
	}
	
	
	???
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
