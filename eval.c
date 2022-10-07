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
#define BADNONULL( caller, objaddr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, ( caller ), ( objaddr ) )

#define MONADICFAILURE( caller, msgstr, failval ) \
	STDMSG_MONADICFAILURE_WRAPPER( &errs, ( caller ), ( msgstr ), ( failval ) )

	#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	
	#define STRARG( msgstr ) STDMSG_STRARG_WRAPPER( &errs, ( msgstr ) )
	#define DATAPTR( ptr ) STDMSG_DATAPTRARG_WRAPPER( &errs, ( ptr ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define BADUINT( caller, objaddr, expectedval ) \
	STDMSG_BADUINT_WRAPPER( &errs, ( caller ), ( objaddr ), ( expectedval ) )

#define TRESPASSPATH( caller, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, ( caller ), ( msgstr ) )

#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), stack_ENDRETFRAME )
#define STACKCHECK2( stack, v,  caller ) \
	STACK_CHECK2( ( stack ), ( v ),  &err, ( caller ), stack_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )


#define CALL_FFUNC( stkpair,  rethand, retdat,  callhand, calldat,  caller, scratch ) \
	CALL_FRAMEFUNC( stkpair,  rethand, retdat,  callhand, calldat,  &errs, caller, scratch, lexalike_ENDRETFRAME )
#define RETFRAMEFUNC( caller ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), res, stack_ENDRETFRAME )



static tokens2macrocllaval_parr_bookmark = UINTPTR_MAX;
retframe tokens2macrocllaval_parr_onerr_( stackpair *stkp, void *v );
static retframe tokens2macrocllaval_parr_onerr =
	(retframe){ &tokens2macrocllaval_parr_onerr_, (void*)0 };

retframe tokens2macrocllaval_parr_conclude( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokens2macrocllaval_parr_conclude );
	
		/* Deinit the longjump stuff. This is needed for some */
		/*  error checking to work right. */
	tokens2macrocllaval_parr_bookmark = UINTPTR_MAX;
	
	RETFRAMEFUNC( tokens2macrocllaval_parr_conclude );
}
	/* ( tokenbranch* macrocllaval_parr* char_parr* -- tokenbranch* macrocllaval_parr* ) */
retframe tokens2macrocllaval_parr_continue( stackpair *stkp, void *v )
{
	static uintptr_t thisfuncptr = (uintptr_t)&tokens2macrocllaval_parr_continue;
	
	STACKCHECK( stkp,  tokens2macrocllaval_parr_continue );
	
	if( tokens2macrocllaval_parr_bookmark == UINTPTR_MAX )
	{
		/* Error: the longjump route hasn't been set. */
		
		TRESPASSPATH(
			tokens2macrocllaval_parr_continue,
			"ERROR: tokens2macrocllaval_parr_continue() detected that the longjump bookmark was uninitialized!"
		);
		stack_ENDRETFRAME();
	}
	
	int scratch;
	uintptr_t tmp;
	
	STACKPOP_UINT( &( stkp->data ), tmp,  tokens2macrocllaval_parr_continue, scratch );
	char_parr *cparr = (char_parr*)tmp;
	if( !cparr )
	{
		BADNULL( tokens2macrocllaval_parr_continue, &cparr );
		TRESPASSPATH(
			tokens2macrocllaval_parr_continue,
			"tokens2macrocllaval_parr_continue() received a null char_parr pointer: this should never happen."
		);
		stack_ENDRETFRAME();
	}
	
	STACKPOP_UINT( &( stkp->data ), tmp,  tokens2macrocllaval_parr_continue, scratch );
	macrocllaval_parr *a = (macrocllaval_parr*)tmp; /* argnames grouping. */
	if( !a )
	{
		BADNULL( tokens2macrocllaval_parr_continue, &a );
		TRESPASSPATH(
			tokens2macrocllaval_parr_continue,
			"tokens2macrocllaval_parr_continue() received a null macrocllaval_parr pointer: this should never happen."
		);
		stack_ENDRETFRAME();
	}
		/* Grow the array. */
	{
		macrocallargval_pascalarray_result res =
			macrocallargval_pascalarray_rebuild( a, a->len + 1 );
#define tokens2macrocllaval_parr_continue_BUILDFAIL( err ) \
	MONADICFAILURE( \
		tokens2macrocllaval_parr_continue, \
		"macrocallargval_pascalarray_rebuild() failed to resize an array.", \
		( err ) \
	);
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, tokens2macrocllaval_parr_continue_BUILDFAIL );
		if( !a )
		{
			TRESPASSPATH(
				tokens2macrocllaval_parr_continue,
				"tokens2macrocllaval_parr_continue() received a null macrocllaval_parr pointer from"
				" macrocallargval_pascalarray_rebuild() via *_BODYMATCH(): this should never happen."
			);
			stack_ENDRETFRAME();
		}
	}
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)a,  tokens2macrocllaval_parr_continue, scratch );
	
		/* Actually save the char array. */
	a->body[ a->len - 1 ].text = cparr;
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), tmp,  tokens2macrocllaval_parr_continue, scratch );
	
	
	
	static retframe
		loopframe = (retframe){ &tokens2macrocllaval_parr_continue, (void*)0 },
		noopframe = (retframe){ &noop, (void*)0 };
	
	static const retframe_parr
		on_dropcomma_ =
			(retframe_parr)
			{
				3,
					/* ( macrocllaval_parr* tokenbranch* token* result
						--
						macrocllaval_parr* tokenbranch* 0 ) */
				{
					(retframe){ &drop, (void*)0 },
						(retframe){ &invoke_dealloctoken, (void*)0 },
					(retframe){ &vm_push0, (void*)0 }
				}
			},
		on_notcomma_ =
			(retframe_parr)
			{
				4,
					/* ( macrocllaval_parr* tokenbranch* token* result
						--
						exits via longjump ) */
				{
					(retframe){ &drop, (void*)0 },
					
					/* ( macrocllaval_parr* tokenbranch* token* ) */
					
					(retframe){ &vm_push0, (void*)0 },
					(retframe){ &vm_pushdata, (void*)&tokens2macrocllaval_parr_continue },
					(retframe){ &longjump_callstack, (void*)&tokens2macrocllaval_parr_bookmark }
				}
			},
		on_loosecomma_ =
			(retframe_parr)
			{
				4,
					/* ( macrocllaval_parr* tokenbranch* length )
						--
						exits via longjump */
				{
					(retframe){ &drop, (void*)0 },
					
					/* ( macrocllaval_parr* tokenbranch* ) */
					
					(retframe){ &vm_push1, (void*)0 },
					(retframe){ &vm_pushdata, (void*)&tokens2macrocllaval_parr_continue },
					(retframe){ &longjump_callstack, (void*)&tokens2macrocllaval_parr_bookmark }
				}
			},
		on_name_ =
			(retframe_parr)
			{
				3,
					/* ( macrocllaval_parr* tokenbranch* token* result
						--
						macrocllaval_parr* tokenbranch* char_parr* 0 ) */
				{
					(retframe){ &drop, (void*)0 },
							/* ( macrocllaval_parr* tokenbranch* token* ) */
						(retframe){ &token2char_parr, (void*)&tokens2macrocllaval_parr_bookmark },
							/* ( macrocllaval_parr* tokenbranch* char_parr* ) */
					(retframe){ &vm_push0, (void*)0 }
				}
			},
		on_notname_ =
			(retframe_parr)
			{
				4,
					/* ( macrocllaval_parr* tokenbranch* token* result
						--
						exits via longjump ) */
				{
					(retframe){ &drop, (void*)0 },
					
					/* ( macrocllaval_parr* tokenbranch* token* ) */
					
					(retframe){ &vm_push2, (void*)0 },
					(retframe){ &vm_pushdata, (void*)&tokens2macrocllaval_parr_continue },
					(retframe){ &longjump_callstack, (void*)&tokens2macrocllaval_parr_bookmark }
				}
			};
	static const retframe
		on_dropcomma = (retframe){ &enqueue_returns, (void*)&on_dropcomma_ },
		on_notcomma = (retframe){ &enqueue_returns, (void*)&on_notcomma_ },
		on_loosecomma = (retframe){ &enqueue_returns, (void*)&on_loosecomma_ },
		on_name = (retframe){ &enqueue_returns, (void*)&on_name_ },
		on_notname = (retframe){ &enqueue_returns, (void*)&on_notname_ };
			};
	static const retframe_parr
		on_yes_ =
			(retframe_parr)
			{
				20,
					/* ( tokenbranch* macrocllaval_parr* length
						--
						tokenbranch* macrocllaval_parr* char_parr* 0 ret:yes-loop ) */
				{
					/* ( tokenbranch* macrocllaval_parr* length ) */
					
					(retframe){ &drop, (void*)0 },
					
						/* ( tokenbranch* macrocllaval_parr* ) */
						
						(retframe){ &swap2nd, (void*)0 },
						(retframe){ &vm_popfront_body_tokenbranch, (void*)0 },
						(retframe){ &require_comma, (void*)0 },
								/* ( macrocllaval_parr* tokenbranch* token* result ) */
							(retframe){ &run_if, (void*)&on_dropcomma },
							(retframe){ &run_else, (void*)&on_notcomma },
						(retframe){ &drop, (void*)0 },
						
						(retframe){ &vm_lengthof_body_tokenbranch, (void*)0 },
								/* ( macrocllaval_parr* tokenbranch* length ) */
							(retframe){ &run_else, (void*)&on_loosecomma },
						(retframe){ &drop, (void*)0 },
						
						(retframe){ &vm_popfront_body_tokenbranch, (void*)0 },
						(retframe){ &require_anyname, (void*)0 },
								/* ( macrocllaval_parr* tokenbranch* token* result ) */
							(retframe){ &run_if, (void*)&on_name },
							(retframe){ &run_else, (void*)&on_notname },
						(retframe){ &drop, (void*)0 },
						
						
							/* ( macrocllaval_parr* tokenbranch* char_parr* ) */
						(retframe){ &swap2nd, (void*)0 },
						(retframe){ &swap3rd, (void*)0 },
						(retframe){ &swap2nd, (void*)0 },
							/* ( tokenbranch* macrocllaval_parr* char_parr* ) */
						
						
							/* Store our loop value onto the data stack. */
						(retframe){ &vm_pushretframe, (void*)&loopframe },
						/* Surpress the "run_else" test that will follow. */
					(retframe){ &vm_push0, (void*)0 }
				}
			},
		on_no_ =
			(retframe_parr)
			{
				3,
					/* ( tokenbranch* macrocllaval_parr* length -- tokenbranch* macrocllaval_parr* 0 ret:no-loop ) */
				{
					(retframe){ &drop, (void*)0 },
							/* All we need to do, is to do nothing at all. */
							/* ( tokenbranch* macrocllaval_parr* ) */
						(retframe){ &vm_pushretframe, (void*)&noopframe },
					(retframe){ &vm_push0, (void*)0 }
				}
			};
	static const retframe
		on_yes = (retframe){ &enqueue_returns, (void*)&on_yes_ },
		on_no = (retframe){ &enqueue_returns, (void*)&on_no_ };
	/* This function's stack delta: ( tokenbranch* macrocllaval_parr* char_parr* -- tokenbranch* macrocllaval_parr* ) */
	static const retframe_parr entry =
		(retframe_parr)
		{
			9,
				/* ( tokenbranch* macrocllaval_parr* -- tokenbranch* macrocllaval_parr* ) */
			{
					/* ( tokenbranch* macrocllaval_parr* ) */
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ &vm_lengthof_body_tokenbranch, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ &swap3rd, (void*)0 },
				(retframe){ &swap2nd, (void*)0 },
					/* ( tokenbranch* macrocllaval_parr* length ) */
				
				(retframe){ &run_if, (void*)&on_yes },
				(retframe){ &run_else, (void*)&on_no },
					/* There WILL be a number that we need to pop */
					/*  on top of the stack, we just don't know (or */
					/*  care) what that number is. */
				(retframe){ &drop, (void*)0 },
				
					/* Loop via a retframe stored on the data stack... */
					/*  unless we don't! *insert dramatic chipmunk* */
					/* ( tokenbranch* macrocllaval_parr* ret:retframe ) */
				(retframe){ &vm_datacall, (void*)0 }
					/* ( tokenbranch* macrocllaval_parr* ) */
			}
		};
	
	
	
	int sz = lengthof_body_tokenbranch( (tokenbranch*)tmp );
	if( !sz )
	{
		RETFRAMEFUNC( tokens2macrocllaval_parr_continue );
		
	} else {
		
		return( (retframe){ &enqueue_returns, (void*)&entry } );
	}
}
	/* ( tokenbranch* token* -- tokenbranch* macrocllaval_parr* char_parr* ) */
retframe tokens2macrocllaval_parr( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokens2macrocllaval_parr );
	
	if( tokens2macrocllaval_parr_bookmark != UINTPTR_MAX )
	{
		/* Error: the longjump route has been set, which means that we've */
		/*  got an invalid loop. */
		
		TRESPASSPATH(
			tokens2macrocllaval_parr,
			"ERROR: tokens2macrocllaval_parr() detected that the longjump"
			" bookmark was initialized, even though IT should be doing"
			 "the initialization!"
		);
		stack_ENDRETFRAME();
	}
	
	int scratch;
	uintptr_t tmp;
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  tokens2macrocllaval_parr, scratch );
	token *tok = (token*)tmp;
	
	scratch = is_stdtoken( &( tok->header ) );
	switch( scratch )
	{
		case 1:
				/* The one and only 'good' path. */
			break;
			
		case -1:
		default:
				/* Error, full break! */
			FAILEDINTFUNC( "is_stdtoken()", tokens2macrocllaval_parr, scratch );
				NOTELINE();
				STRARG( "is_stdtoken() was somehow handed a null pointer." );
			stack_ENDRETFRAME();
			
		case 0:
				/* Error, use ... longjump isn't setup yet, what do we do? */
			FAILEDINTFUNC( "is_stdtoken()", tokens2macrocllaval_parr, scratch );
				NOTELINE();
				STRARG( "is_stdtoken() returned false: please replace this with a better handler." );
			stack_ENDRETFRAME();
	}
	
	macrocllaval_parr *a; /* argnames grouping. */
	macrocallargval_pascalarray_result res =
		macrocallargval_pascalarray_build( 0 );
#define tokens2macrocllaval_parr_BUILDFAIL( err ) \
	MONADICFAILURE( \
		tokens2macrocllaval_parr, \
		"macrocallargval_pascalarray_build() failed to create an array.", \
		( err ) \
	);
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, tokens2macrocllaval_parr_BUILDFAIL );
	if( !a )
	{
		TRESPASSPATH(
			tokens2macrocllaval_parr,
			"tokens2macrocllaval_parr() received a null macrocllaval_parr pointer from"
			" macrocallargval_pascalarray_build() via *_BODYMATCH(): this should never happen."
		);
		stack_ENDRETFRAME();
	}
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)a,  tokens2macrocllaval_parr, scratch );
	
	
	
	static const retframe_parr seq =
		(retframe_parr)
		{
			3,
			{
					/* Move argnames below the token. */
				(retframe){ &swap2nd, (void*)0 },
					/* Convert token. */
				(retframe){ &token2char_parr, (void*)&tokens2macrocllaval_parr_bookmark },
					/* And now, we enter the loop... */
				(retframe){ &tokens2macrocllaval_parr_continue, (void*)0 }
			}
		};
	retframe local_entry = (retframe){ &enqueue_returns, &seq };
	
	LOCALIZE_SETJUMP(
		tokens2macrocllaval_parr_bookmark,
		tokens2macrocllaval_parr,
		local_setjump, /* Set this, then build an invocation function for it. */
		
		&local_entry,
		&tokens2macrocllaval_parr_onerr
	);
		/* Queue up our deinitializer. */
	scratch = push_retframe
		(
			&( stkp->ret ),
			
			(retframe){ &tokens2macrocllaval_parr_conclude, (void*)0 }
		);
	if( !scratch )
	{
		FAILEDINTFUNC(
			"push_retframe( &conclude_func )",
			
			tokens2macrocllaval_parr,
			scratch
		);
		stack_ENDRETFRAME();
	}
	return( (retframe){ &enqueue_returns, (void*)&local_setjump } );
}
retframe tokens2macrocllaval_parr_onerr_( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  tokens2macrocllaval_parr_onerr_ );
	
	int scratch;
	uintptr_t setjump_flag;
	
	STACKPOP_UINT( &( stkp->data ), setjump_flag,  tokens2macrocllaval_parr_onerr_, scratch );
	
	{
		uintptr_t errfunc, errid, tmp;
		
		STACKPOP_UINT( &( stkp->data ), errfunc,  tokens2macrocllaval_parr_onerr_, scratch );
		STACKPOP_UINT( &( stkp->data ), errid,  tokens2macrocllaval_parr_onerr_, scratch );
		
		switch( errfunc )
		{
			case ( (uintptr_t)&token2char_parr ):
				if( errid != 0 )
				{
					BADNONULL( tokens2macrocllaval_parr_onerr_, &errid );
					TRESPASSPATH(
						tokens2macrocllaval_parr_onerr_,
						"token2char_parr() dispatched to tokens2macrocllaval_parr_onerr_() with a non-null error id."
					);
					stack_ENDRETFRAME();
				}
				
					/* The top of the stack will point to a token: it */
					/*  was supposed to be a normal one so that we could */
					/*  convert it to a string, but it was one of the */
					/*  fancy complex types instead. */
				TRESPASSPATH(
					tokens2macrocllaval_parr_onerr_,
					"token2char_parr() was given a non-stdtype token to convert to a character array: this is invalid."
				);
				stack_ENDRETFRAME();
			case ( (uintptr_t)&tokens2macrocllaval_parr_continue ):
				switch( errid )
				{
					case 0: /* on_notcomma_ */
						/* ( macrocllaval_parr* tokenbranch* token* ) */
						/* Expected a comma, got something else instead. */
						
						STACKPEEK_UINT(
							&( stkp->data ), 0,
							tmp,
							
							tokens2macrocllaval_parr_onerr_,
							
							scratch
						);
						BADUINT(
							tokens2macrocllaval_parr_onerr_,
							&( ( (token_head*)tmp )->toktype ),
							TOKTYPE_SYM_COMMA
						);
						stack_ENDRETFRAME();
						
					case 1: /* on_loosecomma_ */
						/* ( macrocllaval_parr* tokenbranch* ) */
						/* Expected SOMETHING to follow the most recent */
						/*  token, and yet nothing. */
						
						TRESPASSPATH(
							tokens2macrocllaval_parr_onerr_,
							"An additional token was required by tokens2macrocllaval_parr_continue()"
							" following a comma, but none was found."
						);
						stack_ENDRETFRAME();
						
					case 2: /* on_notname_ */
						/* ( macrocllaval_parr* tokenbranch* token* ) */
						/* That thing following the comma? Should have */
						/*  been a name (or ellipsis which we CAN'T */
						/*  encode at the moment...), but it was some */
						/*  other type instead. */
						
						STACKPEEK_UINT(
							&( stkp->data ), 0,
							tmp,
							
							tokens2macrocllaval_parr_onerr_,
							
							scratch
						);
						BADUINT(
							tokens2macrocllaval_parr_onerr_,
							&( ( (token_head*)tmp )->toktype ),
							TOKTYPE_NAME
						);
						stack_ENDRETFRAME();
					
					default:
						TRESPASSPATH(
							tokens2macrocllaval_parr_onerr_,
							"tokens2macrocllaval_parr_continue() dispatched to"
							" tokens2macrocllaval_parr_onerr_() with an unknown errid."
						);
						stack_ENDRETFRAME();
				}
				
				stack_ENDRETFRAME();
			case ( (uintptr_t)&tokens2macrocllaval_parr ):
				/* Actually, this one should never be entered. */
				
				TRESPASSPATH(
					tokens2macrocllaval_parr_onerr_,
					"tokens2macrocllaval_parr_onerr_() was entered with an errfunc"
					" value of tokens2macrocllaval_parr(): this should never happen."
				);
				stack_ENDRETFRAME();
			
			default:
				
				TRESPASSPATH(
					tokens2macrocllaval_parr_onerr_,
					"tokens2macrocllaval_parr_onerr_() was entered with an unknown"
					" errfunc value: this should never happen."
				);
				stack_ENDRETFRAME();
		}
	}
	
	TRESPASSPATH(
		tokens2macrocllaval_parr_onerr_,
		"tokens2macrocllaval_parr_onerr_() passed through it's switches without returning: this should never happen."
	);
	stack_ENDRETFRAME();
	
	
	/* These two macros should REALLY never be reached. */
	
		/* Restore value (because it needs to be there when it */
		/*  gets popped by LOCALIZE_SETJUMP::prefix##_DISPATCH{} )... */
	STACKPUSH_UINT( &( stkp->data ), setjump_flag,  tokens2macrocllaval_parr_onerr_, scratch );
	
	RETFRAMEFUNC( tokens2macrocllaval_parr_onerr_ );
}














typedef enum
{
	parseheart_legaltoken_overrides__no_override = 0,
	
	parseheart_legaltoken_overrides_allow_comma = 1,
	parseheart_legaltoken_overrides_allow_parens = 2,
	parseheart_legaltoken_overrides_allow_curly = 4,
	parseheart_legaltoken_overrides_allow_square = 8
	
} parseheart_legaltoken_overrides;
	/* ( token* -- token* ) */
int parseheart_legaltoken( stackpair *stkp, void *v,  parseheart_legaltoken_overrides overrides )
{
	if( !stkp )
	{
		BADNULL( parseheart_layer1_call,  &stkp );
		return( -1 );
	}
	if( !( stkp->data ) )
	{
		BADNULL( parseheart_layer1_call, &( stkp->data ) );
		return( -2 );
	}
	
	int scratch;
	uintptr_t tmp;
	scratch = peek_uintptr( &( stkp->data ),  0,  &tmp );
	if( !scratch )
	{
		FAILEDINTFUNC( "peek_uintptr", parseheart_layer1_call, scratch );
		return( -3 );
	}
	if( !tmp )
	{
		BADNULL( parseheart_layer1_call, &tmp );
		return( -4 );
	}
	token *tok = (token*)tmp;
	
	switch( tok->toktype )
	{
		case TOKTYPE_SYM_COMMA:
			if
			(
				overrides & parseheart_legaltoken_overrides_allow_comma ==
					parseheart_legaltoken_overrides_allow_comma
			)
			{
				return( 1 );
			}
			break;
		case TOKTYPE_SYM_PREPARCL:
			if
			(
				overrides & parseheart_legaltoken_overrides_allow_parens ==
					parseheart_legaltoken_overrides_allow_parens
			)
			{
				return( 1 );
			}
			break;
		case TOKTYPE_SYM_PRECRLCL:
			if
			(
				overrides & parseheart_legaltoken_overrides_allow_curly ==
					parseheart_legaltoken_overrides_allow_curly
			)
			{
				return( 1 );
			}
			break;
		case TOKTYPE_SYM_PRESQRCL:
			if
			(
				overrides & parseheart_legaltoken_overrides_allow_square ==
					parseheart_legaltoken_overrides_allow_square
			)
			{
				return( 1 );
			}
			break;
			
		default:
			return( 1 );
	}
	
	return( 0 );
}













	/* ( tokengroup* -- tokengroup* ) */
retframe parseheart_layer1_exit( stackpair *stkp, void *v )
{
	???
	
	RETFRAMEFUNC( parseheart_layer1_exit );
}
	typedef struct token_retframe
	{
		token_head header;
		retframe closure;
		
	} token_retframe;
??? ???( ???, ??? )
{
	???
	
	if( tok->toktype == TOKTYPE_DEFERED_RETFRAME )
	{
		token_retframe *trf = (token_retframe*)tok;
		
		/* Run it! */
		
		???
	}
	
	???
}
??? ???( ??? )
{
	int scratch;
	uintptr_t tmp;
	
	STACKCHECK( stkp,  parseheart_layer1_directive_inline );
	
	
	
	
	
	
	??? /* These are just markers, to identify the type and purpose of these values. */
		genname_parr *freshline, *inline;
	???
	
	if( !scratch )
	{
		FAILEDINTFUNC(
			"push_retframe( &search )",
			
			parseheart_layer1_directive_freshline,
			scratch
		);
		stack_ENDRETFRAME();
	}
		/* ( tokengroup* ret:retframe -- ... ) */
	return( ( was_freshline( (token_head*)tok ) ? freshline : inline ) );
	
	
	
	
	
	
					/* Turn this into a vm func, and stick it into the search */
					/*  table as associated with '#'. */
				ignore = src.pop_front();
					retframe invoke_dealloctoken( stackpair *stkp, void *v );
					retframe token_queue_fetch( stackpair *stkp, void *v )
				/* Directives & macros get handled identically, */
				/*  so just discard the hash mark and */
				/*  fall-through. */
	
	/* Fetch next token, do search, do another (custom) dispatch. Look at the */
	/*  parseheart_layer1_functionmacro() note on token_retframe{}. */
	
	???
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  parseheart_layer1_directive_freshline, scratch );
	token *tok = (token*)tmp;
	
	genericnamed *found_entry = bsearch1_gennamearr( inline, tok );
	
	if( !found_entry )
	{
		/* It's just a token, it should never have gotten here. */
		
		TRESPASSPATH(
			parseheart_layer1_directive_freshline,
			"Error! parseheart_layer1_directive_freshline() encountered a non-directive token!"
		);
		stack_ENDRETFRAME();
		
	} else if( found_entry->reftype == GENNAMETYPE_RETFRAMEFUNC )
	{
		if( !( found_entry->ref ) )
		{
			BADNULL( parseheart_layer1_directive_freshline, ptr );
			stack_ENDRETFRAME();
		}
		
		return( *( (retframe*)( found_entry->ref ) ) );
		
	} else {
		
		/* Error! Later, we'll add support for macro arrays here, by using double-hash and GENNAMETYPE_TABLEENTRY! */
		
		TRESPASSPATH( parseheart_layer1_directive_freshline, "Error! Unknown found_entry->reftype value: " );
			DECARG( ( found_entry->reftype ) );
		stack_ENDRETFRAME();
	}
}
retframe parseheart_layer1_functionmacro( stackpair *stkp, void *v )
{
	???
	
				/* Somewhere in here, we build a token_retframe{} that we'll */
				/*  push... into a tokengroup{}? With the retframe containing */
				/*  the data for e.g. args? */
				
				if( src.length <= 0 )
				{
					error();
				}
				args = process_arglist( src.pop_front() );
					retframe token_queue_fetch( stackpair *stkp, void *v );
					while() { corefunc( stackpair *stkp, void *v ); }
				if( macro.is_immediate )
				{
					src.push_front( macro( args ) );
						int token_queue_push( token *tok );

				} else {

					dest.push_back( macro );
						int pushto_tokengroup
							(
							tokengroup *tgrp,
							token_head *thd
						);
					dest.push_back( args );
						int pushto_tokengroup
						(
							tokengroup *tgrp,
							token_head *thd
						);
				}
				break;
	
	???
}
	/* ( tokengroup* token* -- tokengroup* token* -- ???  ) */
retframe parseheart_layer1_directive_freshline( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tmp;
	
	STACKCHECK( stkp,  parseheart_layer1_directive_freshline );
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  parseheart_layer1_directive_freshline, scratch );
	token *tok = (token*)tmp;
	scratch = parseheart_legaltoken( stkp, v,  parseheart_legaltoken_overrides__no_override );
	switch( scratch )
	{
		case 1:
				/* Valid token. */
			break;
		case 0:
				/* Barred token: this is seriously NOT supposed to happen here, */
				/*  so it's a syntax error. */
			TRESPASSPATH(
				parseheart_layer1_directive_freshline,
				"Error: parseheart_layer1_directive_freshline() encountered a"
				" non-directive-able token as a directive name."
			);

			stack_ENDRETFRAME();
			
		default:
			/* Error! */
			
			FAILEDINTFUNC( "parseheart_legaltoken()", parseheart_layer1_directive_freshline, scratch );
			stack_ENDRETFRAME();
	}
	
	genericnamed *found_entry = bsearch1_gennamearr( freshline, tok );
	
	if( !found_entry )
	{
		/* It's just a token, it should never have gotten here. */
		
		TRESPASSPATH(
			parseheart_layer1_directive_freshline,
			"Error! parseheart_layer1_directive_freshline() encountered a non-directive token!"
		);
		stack_ENDRETFRAME();
		
	} else if( found_entry->reftype == GENNAMETYPE_RETFRAMEFUNC )
	{
		if( !( found_entry->ref ) )
		{
			BADNULL( parseheart_layer1_directive_freshline, ptr );
			stack_ENDRETFRAME();
		}
		
		return( *( (retframe*)( found_entry->ref ) ) );
		
	} else {
		
		/* Error! Later, we'll add support for macro arrays here, by using double-hash and GENNAMETYPE_TABLEENTRY! */
		
		TRESPASSPATH( parseheart_layer1_directive_freshline, "Error! Unknown found_entry->reftype value: " );
			DECARG( ( found_entry->reftype ) );
		stack_ENDRETFRAME();
	}
}
retframe parseheart_layer1_directive_inline( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tmp;
	
	STACKCHECK( stkp,  parseheart_layer1_directive_inline );
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  parseheart_layer1_directive_inline, scratch );
	token *tok = (token*)tmp;
	scratch = parseheart_legaltoken( stkp, v,  parseheart_legaltoken_overrides__no_override );
	switch( scratch )
	{
		case 1:
				/* Valid token. */
			break;
		case 0:
				/* Barred token: this is seriously NOT supposed to happen here, */
				/*  so it's a syntax error. */
			TRESPASSPATH(
				parseheart_layer1_directive_inline,
				"Error: parseheart_layer1_directive_inline() encountered a"
				" non-directive-able token as a directive name."
			);

			stack_ENDRETFRAME();
			
		default:
			/* Error! */
			
			FAILEDINTFUNC( "parseheart_legaltoken()", parseheart_layer1_directive_inline, scratch );
			stack_ENDRETFRAME();
	}
	
	genericnamed *found_entry = bsearch1_gennamearr( inline, tok );
	
	if( !found_entry )
	{
		/* It's just a token, it should never have gotten here. */
		
		TRESPASSPATH(
			parseheart_layer1_directive_inline,
			"Error! parseheart_layer1_directive_inline() encountered a non-directive token!"
		);
		stack_ENDRETFRAME();
		
	} else if( found_entry->reftype == GENNAMETYPE_RETFRAMEFUNC )
	{
		if( !( found_entry->ref ) )
		{
			BADNULL( parseheart_layer1_directive_inline, ptr );
			stack_ENDRETFRAME();
		}
		
		return( *( (retframe*)( found_entry->ref ) ) );
		
	} else {
		
		/* Error! Later, we'll add support for macro arrays here, by using double-hash and GENNAMETYPE_TABLEENTRY! */
		
		TRESPASSPATH( parseheart_layer1_directive_inline, "Error! Unknown found_entry->reftype value: " );
			DECARG( ( found_entry->reftype ) );
		stack_ENDRETFRAME();
	}
}
	/* Stick this in the name table as the handler function for '#'. */
	/* ( tokengroup* token* -- ???  ) */
retframe parseheart_layer1_directive_octothorpe( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tok;
	
	STACKCHECK( stkp,  parseheart_layer1_directive_octothorpe );
	
	STACKPEEK_UINT( &( stkp->data ), 0, tok,  parseheart_layer1_directive_octothorpe, scratch );
	
	static const retframe_parr
		fresh =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* We don't actually want the hash symbol, so discard it, */
						/*  it's done its job already. */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* Fetch the next token: we presumably care about this one. */
					(retframe){ &token_queue_fetch, (void*)0 },
						/* And dispatch. */
					(retframe){ &parseheart_layer1_directive_freshline, (void*)0 }
				}
			},
		inline =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* We don't actually want the hash symbol, so discard it, */
						/*  it's done its job already. */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* Fetch the next token: we presumably care about this one. */
					(retframe){ &token_queue_fetch, (void*)0 },
						/* And dispatch. */
					(retframe){ &parseheart_layer1_directive_freshline, (void*)0 }
				}
			};
	return( (retframe){ &enqueue_returns, ( was_freshline( (token_head*)tok ) ? &fresh : &inline ) } );
}
	/* ( tokengroup* token* -- tokengroup*  ) */
retframe parseheart_layer1_search( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  parseheart_layer1_search );
	
	??? /* Replace this with the real variable! It's just to clarify the type & nature! */
	genname_parr *macrolist;
	???
	
	token *tok;
	tokengroup *dest;
	uintptr_t tmp;
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  parseheart_layer1_search, scratch );
	tok = (token*)tmp;
	scratch = parseheart_legaltoken( stkp, v,  parseheart_legaltoken_overrides__no_override );
	switch( scratch )
	{
		case 1:
				/* Valid token. */
			break;
		case 0:
				/* Barred token: push it back, and just return. */
			scratch = token_queue_push( tok );
			if( !scratch )
			{
				FAILEDINTFUNC( "token_queue_push()", parseheart_layer1_search, scratch );
					DATAPTR( tok );
				
				stack_ENDRETFRAME();
			}
			
			STACKPOP_UINT( &( stkp->data ), tmp,  parseheart_layer1_search, scratch );

			RETFRAMEFUNC( parseheart_layer1_search );
			
		default:
			/* Error! */
			
			FAILEDINTFUNC( "parseheart_legaltoken()", parseheart_layer1_search, scratch );
			stack_ENDRETFRAME();
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), tmp,  parseheart_layer1_search, scratch );
	dest = (tokengroup*)tmp;
	
	genericnamed *found_entry = bsearch1_gennamearr( macrolist, tok );
	
	if( !found_entry )
	{
		/* It's just a token, pass it on unmolested. */
		
		scratch = pushto_tokengroup( dest, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "pushto_tokengroup", parseheart_layer1_search, res );
			stack_ENDRETFRAME();
		}
			/* It's in the tokengroup, we don't need it on the stack anymore. */
		STACKPOP_UINT( &( stkp->data ), tmp,  parseheart_layer1_search, scratch );
		
		RETFRAMEFUNC( parseheart_layer1_search );
		
	} else if( found_entry->reftype == GENNAMETYPE_RETFRAMEFUNC )
	{
			/* ( tokengroup* token* -- ???  ) */
		return( *( (retframe*)( found_entry->ref ) ) );
		
	} else {
		
		/* Error! Later, we'll add support for macro arrays here, by using double-hash and GENNAMETYPE_TABLEENTRY! */
		
		BADUINT( parseheart_layer1_search, &( found_entry->reftype ), GENNAMETYPE_RETFRAMEFUNC );
		
		stack_ENDRETFRAME();
	}
}
	/* This passes non-matching tokens on unmolested (and */
	/*  one at a time), everything else it tries to process. */
	/* ( tokengroup* -- ... -- tokengroup* ) */
retframe parseheart_layer1_enter( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  parseheart_layer1_enter );
	
		/* ( tokengroup* ret:retframe -- ... ) */
	static const retframe_parr seq =
		(retframe_parr)
		{
			3, /* Number of retframes  */
			{
				(retframe){ &token_queue_fetch, (void*)0 },
				(retframe){ &parseheart_layer1_search, (void*)0 },
				(retframe){ &parseheart_layer1_exit, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, &seq } );
}

retframe parseheart_layer1_conclude( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  parseheart_layer1_conclude );
	
	/* I think this should just be passing stuff along to the next layer, or maybe echoing? */
	
	???
}
	/* ( -- tokengroup* ) */
retframe parseheart_layer1_call( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  parseheart_layer1_call );
	
	tokengroup *dest = build_tokengroup( 0 );
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)dest,  parseheart_layer1_call, scratch );
	
	static const retframe_parr seq =
		(retframe_parr)
		{
			2, /* Number of retframes  */
			{
				(retframe){ &parseheart_layer1_enter, (void*)0 },
				(retframe){ &parseheart_layer1_conclude, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, &seq } );
}


retframe corefunc( stackpair *stkp, void *v )
{
	/* The parseheart_layer1_*() stuff exists to implement what this was. */
}
retframe func( stackpair *stkp, void *v )
{
	???
	
		/* Loop. */
	if( src.length )
	{
		pushframe( (retframe){ &func, (void*)0 } );
		return( corefunc( stkp, v ) );
	}
	
	???
}













/* This parsecall_*() stuff seems similar to what components.c does- */
/*  perhaps that file should be dropped, or these functions rewritten */
/*  in terms of it? */

/* To play well with exec.c, we'll eventually need to convert that */
/*  lower tokengroup into a tokenheadptr_pascalarray{}. Probably */
/*  should be donme here, but worth some thought. */
	/* ( tokengroup* tokengroup* token* -- ... -- ??? ) */
retframe parsecall_bodytail( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tmp;
	
	STACKCHECK( stkp,  parsecall_bodytail );
	
	
	scratch =
		parseheart_legaltoken
		(
			stkp, v,
			
			parseheart_legaltoken_overrides_allow_comma |
				parseheart_legaltoken_overrides_allow_parens
		);
	if( !scratch )
	{
		/* Error! The encountered token is totally wrong for this context! */
		
		???
	}
	
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  parsecall_bodytail, scratch );
	token *tok = (token*)tmp;
	
	
	static const retframe_parr
		discarder_epilogue =
			(retframe_parr)
			{
				2, /* Number of retframes  */
				{
						/* Discard the toke, and push the top tokengroup */
						/*  into the lower tokengroup. */
					(retframe){ &invoke_dealloctoken, (void*)0 },
					(retframe){ &vm_pushto_tokengroup, (void*)0 }
				}
			},
		recursion_prologue =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* Accumulate token (and any resulting tokens) */
						/*  into the top tokengroup, then recurse. */
					(retframe){ &parseheart_layer1_enter, (void*)0 },
					(retframe){ &token_queue_fetch, (void*)0 },
					(retframe){ &parsecall_bodytail, (void*)0 }
				}
			},
		
		on_comma =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* Discard the token, store the upper */
						/*  tokengroup, and proceed to the next */
						/*  argument sequence. */
					(retframe){ &enqueue_returns, &discarder_epilogue },
					(retframe){ &vm_buildempty_tokengroup(), (void*)0 },
					(retframe){ &enqueue_returns, &recursion_prologue }
				}
			},
		on_normal =
			(retframe_parr)
			{
				2, /* Number of retframes  */
				{
						/* Accumulate & recurse. */
					(retframe){ &token_queue_unfetch, (void*)0 },
					(retframe){ &enqueue_returns, &recursion_prologue }
				}
			};
	
	if( tok->toktype == TOKTYPE_SYM_PREPARCL )
	{
		/* We're done, time to drop out of the parsecall_*() */
		/*  sequence. */
		
			/* Afterwards, this should transparently return to */
			/*  caller of parsecall_enter(). */
		return( (retframe){ &enqueue_returns, &discarder_epilogue } );
		
	} else if( tok->toktype == TOKTYPE_SYM_COMMA )
	{
			/* Finish up this argument sequence, and proceed */
			/*  to the next. */
		return( (retframe){ &enqueue_returns, &on_comma } );
		
	} else {
		
			/* Continue accumulating tokens. */
		return( (retframe){ &enqueue_returns, &on_normal } );
	}
}
	/* ( tokengroup* token* -- ??? ) */
retframe parsecall_body( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  parsecall_body );
	
	
	if( tok->toktype == TOKTYPE_SYM_PREPARCL )
	{
		/* Closing symbol! */
		
		return( (retframe){ &invoke_dealloctoken, (void*)0 } );
		
	} else
	if
	(
		tok->toktype == TOKTYPE_OPPARUP
		tok->toktype == TOKTYPE_SYM_COMMA
	)
	{
		tokengroup *dest = build_tokengroup( 0 );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)dest,  parsecall_body, scratch );
		
		
		static const retframe_parr seq =
			(retframe_parr)
			{
				5, /* Number of retframes  */
				{
					(retframe){ &swap2nd, (void*)0 },
						/* Ditch the comma: it was passed to us by */
						/*  parsecall_bodytail(), which has ALREADY */
						/*  done everything it needs to in regards */
						/*  to it... */
						/* Note that the same is true of the opening */
						/*  bracket. */
					(retframe){ &invoke_dealloctoken, (void*)0 },
					(retframe){ &parseheart_layer1_enter, (void*)0 },
					(retframe){ &token_queue_fetch, (void*)0 },
					(retframe){ &parsecall_bodytail, (void*)0 }
				}
			};
		return( (retframe){ &enqueue_returns, &seq } );
		
	} else {
		
		/* Error! */
		
		???
	}
}
	/* This function is the entry point to parse a macro or directive */
	/*  call with the "(^" "^)" brackets. Note that a pointer to a */
	/*  "(^" token MUST be on the stack when this function is entered. */
	/* ( token* -- ... -- tokengroup* ) */
retframe parsecall_enter( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tmp;
	
	STACKCHECK( stkp,  parsecall_enter );
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  parsecall_enter, scratch );
	token *tok = (token*)tmp;
	
	if( tok->toktype != TOKTYPE_OPPARUP )
	{
		/* Error, wrong type. Except technically, we need to handle */
		/*  tokenbranch but don't. Make a function for that. */
		
		???
	}
	
	
	tokengroup *dest = build_tokengroup( 0 );
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)dest,  parseheart_layer1_call, scratch );
	
	
		/* ( tokengroup* -- ... ) */
	static const retframe_parr seq =
		(retframe_parr)
		{
			4, /* Number of retframes  */
			{
				
				(retframe){ &swap2nd, (void*)0 },
					??? /* We probably want to shove the token into a tokenbranch holding the tokengroup, but we can do that later. */
				(retframe){ &invoke_dealloctoken, (void*)0 },
				(retframe){ &token_queue_fetch, (void*)0 },
				(retframe){ &parsecall_body, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, &seq } );
}














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
					
					common_macrodispatch, scratch
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
					(retframe){ &tokenbranch_build, (void*)0 },
					
					(retframe){ &swap2nd, (void*)0 },
					(retframe){ &tokenbranch_pushbody, (void*)0 },
					
					(retframe){ &swap2nd, (void*)0 },
						/* We might be better off using a different */
						/*  token to set this. If so, then verify that */
						/*  the number of elements in the retframe_parr */
						/*  gets updated. */
					(retframe){ &tokenbranch_setsubtype, (void*)0 },
					(retframe){ &tokenbranch_setlead, (void*)0 },
					
						/* Dispatch to the directive-recognizer code. */
					(retframe){ &, (void*)0 }
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
					(retframe){ &accumulate_token, (void*)0 },
					(retframe){ &require_anyname, (void*)0 },
					
					(retframe){ &conclude_try_directive, (void*)0 }
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







	/* We absolutely want an error if this is used before */
	/*  initialization: this is the best I can do in C. */
uintptr_t pardef_bookmark = UINTPTR_MAX;
	??? /* We should replace noop with something else, but no idea what. */
retframe hook_PARSEBREAK = { &noop, (void*)0 };
		/* ( uintptr_t -- ) */
const retframe_parr on_TOKTYPE_PARSEBREAK_ =
	(retframe_parr)
	{
		3 /* The number of instructions. */,
		{
			/* We won't need that test result again. */
			(retframe){ &drop, (void*)0 },
			
				/* There should be something that needs to be done here, right? */
			(retframe){ &just_run, &hook_PARSEBREAK },
			
				/* This needs to break us out of this bracket match. */
			(retframe){ &longjump_callstack, (void*)&pardef_bookmark }
		}
	};
const retframe
	on_PARSEBREAK = { &enqueue_returns, &on_TOKTYPE_PARSEBREAK_ };

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
				5 /* The number of instructions. */,
				{
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
						/* Now we test for our "absolute break" hazard. */
					(retframe){ &require_parsebreak, (void*)0 },
						/* This needs to break us out of this bracket match. */
					(retframe){ &run_if, (void*)&on_PARSEBREAK },
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
				13 /* The number of instructions. */,
				{
						/* We won't need that test result again. */
					(retframe){ &drop, (void*)0 },
					
					
					/* Fetch the next token and confirm it's validity: it MUST be a */
					/*  name, or maybe a triple ellipsis... except we don't currently */
					/*  HAVE triple ellipsis support in simplelex.c OR token.h, so skip */
					/*  that. */
					
					(retframe){ &accumulate_token, (void*)0 },
					
					(retframe){ &require_parsebreak, (void*)0 },
						/* This needs to break us out of this bracket match. */
					(retframe){ &run_if, (void*)&on_PARSEBREAK },
					(retframe){ &drop, (void*)0 },
					
					
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
				/* Actually, since these are the "def" functions, I */
				/*  think there SHOULDN'T BE any macros allowed here. */
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
			13 /* The number of instructions. */,
			{
				(retframe){ &accumulate_token, (void*)0 },
				
				
					??? /* Do we have the code to trigger parse breaks? If not, then it needs to be put somewhere. */
				(retframe){ &require_parsebreak, (void*)0 },
					/* This needs to break us out of this bracket match. */
				(retframe){ &run_if, (void*)&on_PARSEBREAK },
				(retframe){ &drop, (void*)0 },
				
				
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
	static const retframe_parr
		on_parsebreak_ =
			(retframe_parr)
			{
				???,
				{
					/* Handling code goes here! */
				}
			},
		mainbody_ =
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
	
	retframe
		mainbody = (retframe){ &enqueue_returns, &mainbody_ },
		on_parsebreak = (retframe){ &enqueue_returns, &on_parsebreak_ };
	LOCALIZE_SETJUMP(
		pardef_bookmark,
		enter_try_pardefopen,
		setjump,
		
		&mainbody,
		&on_parsebreak
	);
		/* setjump will setup a longjump_callstack() context for use */
		/*  in case a TOKTYPE_PARSEBREAK (or maybe other erroneous?) */
		/*  token was encountered. It'll later undo that same setup, */
		/*  just in case we're dealing with nested contexts. */
	
	return( (retframe){ &enqueue_returns, (void*)&setjump } );
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
