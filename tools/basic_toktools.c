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
	/* Places 1 on the data stack if the token pointer */
	/*  points to a preprocessor opening token, else */
	/*  places 0 on the data stack. */
retframe require_preprocopener( stackpair *stkp, void *v )
{
	int scratch;
	static token
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
		};
	
	typedef struct token token;
	struct token
	{
		token_head header;
		char text[];
	};
	
	
	
	/* A WORD OF WARNING!!! */
	/* This function just pushes a bunch of "return calls" onto the */
	/*  return stack to setup a sequence of calls, so the calls are */
	/*  listed in OPPOSITE ORDER from how they will actually be */
	/*  executed. YOU HAVE BEEN WARNED. */
	
	
	
	STACKCHECK( stkp,  require_match );
	
	
	
	/* Push the cleanup sequence: we'll discard the token pointer, */
	/*  then and together the three result values that were placed */
	/*  on the stack. */
	
	scratch = push_retframe( stkp->ret, (retframe){ &and3, (void*)0 } );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", require_preprocopener, scratch );
		( endfunc )();
	}
		/* Move the pointer back below the results. */
	scratch = push_retframe( stkp->ret, (retframe){ &swap4th, (void*)0 } );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", require_preprocopener, scratch );
		( endfunc )();
	}
	
	
	
	/* Setup the three tests that we'll be running: for each test, */
	/*  we'll then be swapping the result and the token pointer's */
	/*  places, so that the token pointer stays on top. */
	
	scratch = push_retframe( stkp->ret, (retframe){ &swap2nd, (void*)0 } );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", require_preprocopener, scratch );
		( endfunc )();
	}
	scratch = push_retframe( stkp->ret, (retframe){ &require_match, &prepar } );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", require_preprocopener, scratch );
		( endfunc )();
	}
	
	
	scratch = push_retframe( stkp->ret, (retframe){ &swap2nd, (void*)0 } );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", require_preprocopener, scratch );
		( endfunc )();
	}
	scratch = push_retframe( stkp->ret, (retframe){ &require_match, &presqr } );
	if( !scratch )
	{
		STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", require_preprocopener, scratch );
		( endfunc )();
	}
	
	CALL_FFUNC(
		stkp,
		
		&swap2nd, (void*)0,
		&require_match, &precrl, /* For the call, put in something to discard the token pointer that we're comparing against. */
		
		require_preprocopener, scratch
	);
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
				(retframe(){ , } /* Call something to discard the token pointer. */
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
retframe buildbranch_leadbody( stackpair *stkp, void *v )
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
	
		/* Discard both of those tokens: we already have them linked. */
	STACKPOP_UINT( stkp->data, body,  buildbranch_leadbody, scratch );
	STACKPOP_UINT( stkp->data, lead,  buildbranch_leadbody, scratch );
	
	
		/* Return the result.. */
	STACKPUSH_UINT( stkp->data, &( tb->header ),  buildbranch_leadbody, scratch );
	RETFRAMEFUNC( stkp,  require_anyname );
}





















