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

/* This file is yet another attempt to basically restart the macro */
/*  handling process. Should be a bit more sensible this time around. */
/*  It is not intended to use any of the stuff from the metaCpreproc */
/*  directory, excepting it'll certainly grab some of the types & */
/*  such. */


/* Parsing is essentially divided into three stages: */
/* Phase 1 MOSTLY just packages up tokens into a sequence of macros */
/*  or directives that are packaged with their arguments, and */
/*  individual tokens, but allows for some stuff to be executed */
/*  immediately so that it can alter the allocation of arguments, */
/*  which needs to be done in phase 1. */
/* Phase 2 executes most of the directives, and most macro calls. */
/* Phase 3 is mostly just token export, but there are some special */
/*  "emit text" directives that might be executed here instead of */
/*  later. */


#include <stdlib.h>

#include "headers.h"

#include "err/inner_err.h"
#include "token.h"
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


#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), stack_ENDRETFRAME )
#define STACKCHECK2( stack, v,  caller ) \
	STACK_CHECK2( ( stack ), ( v ),  &err, ( caller ), stack_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )


#define CALL_FFUNC( stkpair,  rethand, retdat,  callhand, calldat,  caller, scratch ) \
	CALL_FRAMEFUNC( stkpair,  rethand, retdat,  callhand, calldat,  &errs, caller, scratch, stack_ENDRETFRAME )
#define RETFRAMEFUNC( caller, scratch ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )


#define FETCH_SIMPLETYPE( tokhead, destuintp,  caller, scratch ) \
	TOKEN_FETCH_SIMPLETYPE( ( tokhead ), ( destuintp ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define CHECK_SIMPLETYPE( tokhead, testval, on_yes, on_no,  caller, scratch ) \
	TOKEN_CHECK_SIMPLETYPE( ( tokhead ), ( testval ), ( on_yes ), ( on_no ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )


lib4_intresult not_contextspecials( token_head *tok_, context_specials *ctx )
{
	int scratch;
	
	if( !tok || !ctx )
	{
		LIB4_INTRESULT_RETURNFAILURE(
			(lib4_failure_uipresult){ LIB4_STDERRS_BADARGS_SIMPLE }
		);
	}
	if( !( ctx->th.toktype == TOKTYPE_CONTEXTSPECIALS ) )
	{
		LIB4_INTRESULT_RETURNFAILURE(
			(lib4_failure_uipresult){ LIB4_STDERRS_BADARGS_COMPLEX }
		);
	}
	if( !( ctx->breaks ) || !( ctx->end ) )
	{
		LIB4_INTRESULT_RETURNFAILURE(
			(lib4_failure_uipresult){ LIB4_STDERRS_BADARGS_DEEP }
		);
	}
	
	uintptr_t tok, tmp;
	FETCH_SIMPLETYPE( *tok_, tok,  not_contextspecials, scratch );
#define not_contextspecials_ONYES( ... ) LIB4_INTRESULT_RETURNSUCCESS( -1 );
	CHECK_SIMPLETYPE( *( ctx->end ), tmp, not_contextspecials_ONYES, LIB4_NULL_MACRO,  not_contextspecials, scratch );
	
	size_t loop = 0;
	while( loop < ctx->breaks->len )
	{
#define not_contextspecials_LOOPYES( ... ) LIB4_INTRESULT_RETURNSUCCESS( 0 );
		CHECK_SIMPLETYPE(
			*( ctx->breaks->body[ loop ] ), tmp, not_contextspecials_LOOPYES, LIB4_NULL_MACRO,
			not_contextspecials, scratch
		);
		loop++;
	}
	
	LIB4_INTRESULT_RETURNSUCCESS( 1 );
}



	/* The actually available stuff. Maybe belongs here, maybe */
	/*  doesn't, worry about that later. */
???
genname_parr *freshline, *inline;
???



	/* (  --  ) */
retframe phase1_comonexec( stackpair *stkp, void *v_ )
{
	int scratch;
	STACKCHECK2( stkp, v_,  phase1_comonexec );
	
	???
}



retframe bracketgather_invalidcall( stackpair *stkp, void *v_ )
{
	/* This should never be entered, it exists to alert when */
	/*  something fails to get set correctly. */
	
	???
}
	/* ( token* char_parr* -- ) */
retframe bracketgather_badtoken( stackpair *stkp, void *v_ )
{
	/* This should never be entered, it exists to alert when */
	/*  something fails to get set correctly. */
	
	???
}
retframe bracketgather_exit( stackpair *stkp, void *v_ )
{
	???
}
	/* ( tokengroup* tokengroup* token* char_parr* -- ??? ) */
retframe bracketgather_loop_search( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  bracketgather_loop );
	
	context_specials ctx = (context_specials*)v_;
	
	uintptr_t parr, tok;
	STACKPOP_UINT( &( stkp->data ), parr,  bracketgather_loop, scratch );
	STACKPEEK_UINT( &( stkp->data ), 0, tok,  bracketgather_loop, scratch );
	
		??? /* Does this belong here? Aren't we searching for macros? */
	genname_parr *searchbatch = ( was_freshline( (token_head*)tok ) ? freshline : inline );

		/* genericnamed* bsearch1_gennamearr( genname_parr *parr, token *tok ); */
	genericnamed *found_entry = bsearch1_gennamearr( searchbatch, (char_parr*)parr );
		/* We no longer need the pascal array, so time to delete it. */
	lib4_result res = char_pascalarray_destroy( (char_parr*)parr );
#define bracketgather_loop_search_ERRREPORT( err ) \
		???; /* Do some reporting. */ \
		stack_ENDRETFRAME();
	LIB4_RESULT_BODYMATCH( res, LIB4_NULL_MACRO, bracketgather_loop_search_ERRREPORT );
	
	if( !found_entry )
	{
		/* It's just a token, so we'll push it. */
		
		static retframe_parr justtok =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* ( tokengroup* token* -- tokengroup* ) */
					(retframe){ &vm_pushto_tokengroup, (void*)0 },
					(retframe){ &token_queue_fetch, (void*)0 },
					(retframe){ &bracketgather_loop, (void*)0 }
				}
			};
			justtok.body[ 2 ].data = v_;
		return( (retframe){ &enqueue_returns, &justtok } );
		
	} else if( found_entry->reftype == GENNAMETYPE_RETFRAMEFUNC )
	{
		if( !( found_entry->ref ) )
		{
			BADNULL( bracketgather_loop, ptr );
			stack_ENDRETFRAME();
		}
		
		static retframe_parr activematch =
			(retframe_parr)
			{
				4, /* Number of retframes  */
				{
					(retframe){ &token_queue_fetch, (void*)0 },
						/* This is meant to be overwritten with the retframe at *( ->ref ). */
						/* ( tokengroup* token* token* -- tokengroup* ) */
					(retframe){ &bracketgather_invalidcall, (void*)0 },
					(retframe){ &token_queue_fetch, (void*)0 },
					(retframe){ &bracketgather_loop, (void*)0 }
				}
			};
				/* Patch in the reference that was found. */
			activematch.body[ 1 ].handler = ( (retframe*)( found_entry->ref ) )->handler;
			activematch.body[ 1 ].data = ( (retframe*)( found_entry->ref ) )->data;
			activematch.body[ 3 ].data = v_;
		return( (retframe){ &enqueue_returns, &activematch } );

	} else {

		TRESPASSPATH( bracketgather_loop, "Error! Unknown found_entry->reftype value: " );
			DECARG( ( found_entry->reftype ) );
		stack_ENDRETFRAME();
	}
	
	??? /* We should never reach here. */
}
	/* ( tokengroup* tokengroup* token* -- ??? ) */
retframe bracketgather_loop( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  bracketgather_loop );
	
	context_specials ctx = (context_specials*)v_;
	
	uintptr_t tok, arggrp;
	STACKPEEK_UINT( &( stkp->data ), 0, tok,  bracketgather_loop, scratch );
	
	lib4_intresult res = not_contextspecials( (token_head*)tok, ctx );
	
#define bracketgather_loop_ERRREPORT( err ) \
		???; /* Do some reporting. */ \
		stack_ENDRETFRAME();
	int a;
	LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETa, bracketgather_loop_ERRREPORT );
	
	switch( a )
	{
		case -1:
			/* Ending token encountered. */
			
			static const retframe_parr ending =
				(retframe_parr)
				{
					2, /* Number of retframes  */
					{
								/* ( token_head* --  ) */
						(retframe){ &invoke_dealloctoken, (void*)0 },
							/* ( tokengroup* tokengroup* -- tokengroup* ) */
						(retframe){ &vm_pushto_tokengroup, (void*)0 }
					}
				};
			return( (retframe){ &enqueue_returns, &ending } );
		
		case 0:
			/* Break character (e.g. comma) encountered. */
			
			static retframe_parr comma =
				(retframe_parr)
				{
					5, /* Number of retframes  */
					{
								/* ( token_head* --  ) */
						(retframe){ &invoke_dealloctoken, (void*)0 },
							/* ( tokengroup* tokengroup* -- tokengroup* ) */
						(retframe){ &vm_pushto_tokengroup, (void*)0 },
						(retframe){ &vm_buildempty_tokengroup, (void*)0 },
						(retframe){ &token_queue_fetch, (void*)0 },
						(retframe){ &bracketgather_loop, (void*)0 }
					}
				};
				comma.body[ 4 ].data = v_;
			return( (retframe){ &enqueue_returns, &comma } );
		
		case 1:
			/* Other character encountered: needs additional decoding. */
			
			static const retframe badtok = (retframe){ bracketgather_badtoken&, (void*)0 };
			static retframe_parr other =
				(retframe_parr)
				{
					2, /* Number of retframes  */
					{
							/* ( token* -- token* char_parr* ) */
							/* v_ must point to a retframe{} to handle "unrecognized token type" errors. */
						(retframe){ &token2char_parr, (void*)( &badtok ) },
						(retframe){ &bracketgather_loop_search, (void*)0 }	
					}
				};
				comma.body[ 1 ].data = v_;
			return( (retframe){ &enqueue_returns, &other } );
		
		default:
			??? /* Erroneous result, report and fail. */
	}
}
	/* ( tokengroup* token* -- ... ) */
retframe bracketgather_check( stackpair *stkp, void *v_ )
{
	???
		/* I think this might force an IMMEDIATE exit of the bracketgather */
		/*  subsystem- if so, then behavior needs to become more nuanced. */
	
	STACKCHECK2( stkp, v_,  bracketgather_check );
	
	context_specials ctx = (context_specials*)v_;
	
	int scratch;
	uintptr_t tok, tmp;
	FETCH_SIMPLETYPE( *( ctx->start ), tok,  bracketgather_check, scratch );
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), tmp,  bracketgather_check, scratch );
#define bracketgather_check_ONYES( ... ) \
		tokengroup *dest = build_tokengroup( 0 ); \
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)dest,  bracketgather_check, scratch ); \
		static retframe_parr seq = \
			(retframe_parr){ \
				5, /* Number of retframes  */ { \
					(retframe){ &swap2nd, (void*)0 }, \
							/* ( tokengroup* token_head* -- tokengroup* token_head* ) */ \
					(retframe){ &vm_setsubtype_tokengroup, (void*)0 }, \
					(retframe){ &invoke_dealloctoken, (void*)0 }, \
					(retframe){ &token_queue_fetch, (void*)0 }, \
					(retframe){ &bracketgather_loop, (void*)0 } } }; \
				/* And this is why seq{} isn't a const: we need to patch it at runtime. */ \
			seq.body[ 4 ].data = v_; \
		return( (retframe){ &enqueue_returns, &seq } );
#define bracketgather_check_ONNO( ... ) \
		??? ; \
		stack_ENDRETFRAME();
	CHECK_SIMPLETYPE(
		*( (token_head*)tmp ), tok, bracketgather_check_ONYES, bracketgather_check_ONNO,
		bracketgather_check, scratch
	);
}
	/* ( token_head* -- ??? ) */
retframe bracketgather_enter( stackpair *stkp, void *v_ )
{
	int scratch;
	
	STACKCHECK2( stkp, v_,  bracketgather_enter );
	
	if( ( (token_head*)v_ )->toktype != TOKTYPE_CONTEXTSPECIALS )
	{
		/* Error, bad type! */
		???
	}
	
	tokengroup *dest = build_tokengroup( 0 );
	if( !dest )
	{
		???;
	}
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)dest,  bracketgather_enter, scratch );
	
	static retframe_parr seq =
		(retframe_parr)
		{
			6, /* Number of retframes  */
			{
				(retframe){ &swap2nd, (void*)0 },
					/* ( tokengroup* token_head* -- tokengroup* token_head* ) */
				(retframe){ &vm_setsubtype_tokengroup, (void*)0 },
				(retframe){ &invoke_dealloctoken, (void*)0 },
				
				(retframe){ &token_queue_fetch, (void*)0 },
				(retframe){ &bracketgather_check, (void*)0 },
				(retframe){ &bracketgather_exit, (void*)0 }
			}
		};
		seq.body[ 4 ].data = v_;
		seq.body[ 5 ].data = v_;
	return( (retframe){ &enqueue_returns, &seq } );
}


	/* ( token* -- token* | tokengroup* ) */
retframe bracketgather_dispatcher( stackpair *stkp, void *v )
{
	static const context_specials
		curly =
			{
				(tokenhead){  },
				(tokhdptr_parr*){  }, /* breaks */
				(token_head*), /* start */
				(token_head*) /* end */
			},
		square =
			{
				(tokenhead){  },
				(tokhdptr_parr*){  }, /* breaks */
				(token_head*), /* start */
				(token_head*) /* end */
			},
		paren =
			{
				(tokenhead){  },
				(tokhdptr_parr*){  }, /* breaks */
				(token_head*), /* start */
				(token_head*) /* end */
			};
	int scratch;
	
	STACKCHECK( stkp,  bracketgather_dispatcher );
	
	uintptr_t tmp;
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), tmp,  bracketgather_dispatcher, scratch );
	FETCH_SIMPLETYPE( *( (token_head*)tmp ), tmp,  bracketgather_dispatcher, scratch );
	switch( tmp )
	{
		case TOKTYPE_OPCRLUP: /* {^ */
			return( (retframe){ &bracketgather_enter, (void*)&curly } );
		case TOKTYPE_OPSQRUP: /* [^ */
			return( (retframe){ &bracketgather_enter, (void*)&square } );
		case TOKTYPE_OPPARUP: /* (^ */
			return( (retframe){ &bracketgather_enter, (void*)&paren } );
			
		default:
			???;
	}
	
	???
}





/* Below is the new stuff: it should probably COMPLETELY */
/*  replace the stuff above, BUT CHECK. */



static uintptr_t callerinfo_typeid;
	/* This is not an accident, this IS meant to be named twice, C just */
	/*  doesn't differentiate between global variable declarations & */
	/*  definitions... */
uintptr_t callerinfo_typeid = (uintptr_t)&callerinfo_typeid;


static divertthread_callerinfo escapeinfo =
	{
		(uintptr_t)&callerinfo_typeid,
		{
			&divertthread_earlyexit_ptr_placeholder,
			(divertthread_info*)0
		},
		(retframe){ &vm_placeholder, (void*)0 }
	};
#define VALUE_subordinateinfo \
	(divertthread_info){ \
		0,\
		(retframe){ &vm_placeholder, (void*)0 }, (retframe){ &vm_placeholder, (void*)0 }, \
		&escapeinfo \
	}
static divertthread_info subordinateinfo = VALUE_subordinateinfo;

typedef struct bracketcontext_specials
{
	token_head th;
	
	tokhdptr_parr *breaks, *bads, *starts;
	token_head *start, *end;
	
	divertthread_info *jumpinfo;
	
} bracketcontext_specials;
#define INIT__SPECIALS( tokhead,  brkparr, badparr, strtparr,  strtthead, endthead ) \
	(bracketcontext_specials){ (tokhead),  &(brkparr), &(badparr), &(strtparr),  &(strtthead), &(endthead),  &subordinateinfo }


static token
	parenplain = INIT_TOKEN_GENERIC( TOKTYPE_OPPAR, "(" ), /* ( */
	parenup = INIT_TOKEN_GENERIC( TOKTYPE_OPPARUP, "(^" ), /* (^ */
	curlup = INIT_TOKEN_GENERIC( TOKTYPE_OPCRLUP, "{^" ), /* {^ */
	sqarup = INIT_TOKEN_GENERIC( TOKTYPE_OPSQRUP, "[^" ), /* [^ */
	
	plainparen = INIT_TOKEN_GENERIC( TOKTYPE_SYM_PARENCLOSE, ")" ), /* ) */
	upparen = INIT_TOKEN_GENERIC( TOKTYPE_SYM_PREPARCL, "^)" ), /* ^) */
	upcurl = INIT_TOKEN_GENERIC( TOKTYPE_SYM_PRECRLCL, "^}" ), /* ^} */
	upsqar = INIT_TOKEN_GENERIC( TOKTYPE_SYM_PRESQRCL, "^]" ), /* ^] */
	
	comma = INIT_TOKEN_GENERIC( TOKTYPE_SYM_COMMA, "," ), /* , */
	
		/* These three are used to configure tokengroup{}s. */
	argset = INIT_TOKEN_GENERIC( TOKTYPE_TOKENGROUP_ARGSET, "??? (a set of arguments)" ),
	argcontainer = INIT_TOKEN_GENERIC( TOKTYPE_TOKENGROUP_ARG, "??? (a single argument)" ),
	arggeneric = INIT_TOKEN_GENERIC( TOKTYPE_GENERICGROUP, "??? (a filler toktype for single arguments)" ),
	
		/* This one is used to mark errors. */
	errored = INIT_TOKEN_GENERIC( TOKTYPE_TOKENGROUP_ERROREDSET, "??? (a tokengroup that errored mid-build)" );


#define DEF_tokhdptr_parr( ... ) \
		LIB4_DEFINE_PASCALARRAY_LITERAL2( tokenheadptr_, token_head*, __VA_ARGS__ )
static tokhdptr_parr
	nobreaks = DEF_tokhdptr_parr(  ),
	commabreaks = DEF_tokhdptr_parr( &( comma.header ) ),
	
	plainbads = DEF_tokhdptr_parr( ( upparen.header ), ( upcurl.header ), ( upsqar.header ) ),
	parenbads = DEF_tokhdptr_parr( ( plainparen.header ), ( upcurl.header ), ( upsqar.header ) ),
	curlbads = DEF_tokhdptr_parr( ( plainparen.header ), ( upparen.header ), ( upsqar.header ) ),
	sqarbads = DEF_tokhdptr_parr( ( plainparen.header ), ( upparen.header ), ( upcurl.header ) ),
	
	starters = DEF_tokhdptr_parr( ( parenplain.header ), ( parenup.header ), ( curlup.header ), ( sqarup.header ) );

static bracketcontext_specials
	plainspecs =
		INIT__SPECIALS(
			INIT_TOKENHEAD( TOKTYPE_CONTEXTSPECIALS, 0,  0,  0, 0, 0 ),
			nobreaks,
			plainbads,
			starters,
			( parenplain.header ),
			( plainparen.header )
		),
	parenspecs =
		INIT__SPECIALS(
			INIT_TOKENHEAD( TOKTYPE_CONTEXTSPECIALS, 0,  0,  0, 0, 0 ),
			commabreaks,
			parenbads,
			starters,
			( parenup.header ),
			( upparen.header )
		),
	curlspecs =
		INIT__SPECIALS(
			INIT_TOKENHEAD( TOKTYPE_CONTEXTSPECIALS, 0,  0,  0, 0, 0 ),
			commabreaks,
			curlbads,
			starters,
			( curlup.header ),
			( upcurl.header )
		),
	sqarspecs =
		INIT__SPECIALS(
			INIT_TOKENHEAD( TOKTYPE_CONTEXTSPECIALS, 0,  0,  0, 0, 0 ),
			commabreaks,
			sqarbads,
			starters,
			( sqarup.header ),
			( upsqar.header )
		);

static retframe_parr
	build_argcontainer =
		(retframe_parr)
		{
			7, /* Number of retframes  */
			{
				(retframe){ &vm_buildempty_tokengroup, (void*)0 },
					(retframe){ &vm_pushdata, (void*)&argcontainer },
						(retframe){ &vm_tokenhead_settoktype, (void*)0 },
					(retframe){ &drop, (void*)0 },
					(retframe){ &vm_pushdata, (void*)&arggeneric },
						(retframe){ &vm_setsubtype_tokengroup, (void*)0 },
					(retframe){ &drop, (void*)0 }
			}
		},
			/* ( tokengroup*args tokengroup*arg token* -- tokengroup*args ) */
	complete_args =
		(retframe_parr)
		{
			2, /* Number of retframes  */
			{
					/* We just needed to DETECT the closing bracket, not keep it. */
					/* ( token* -- ) */
				(retframe){ &invoke_dealloctoken, (void*)0 },
					/* ( tokengroup*args tokengroup*arg -- tokengroup*args ) */
				(retframe){ &vm_pushto_tokengroup, (void*)0 }
			}
		};

retframe bracketgather_exit( stackpair *stkp, void *v )
{
	int scratch;
	
	uintptr_t tok;
	
	STACKCHECK2( stkp, v_,  bracketgather_exit );
	
		/* Are we looking at a valid stack state? Let's see if our */
		/*  identifier is at the right spot on the stack. */
		/* Note that this LOOKS PAST the top of the stack. */
	scratch = peek_divertthread_callerinfo( &( stkp->data ),  sizeof( uintptr_t ),  (divertthread_callerinfo*)0, &tok );
	if( !scratch )
	{
		FAILEDINTFUNC( "peek_divertthread_callerinfo", bracketgather_exit, scratch );
		
		stack_ENDRETFRAME();
	}
	if( tok != (uintptr_t)&callerinfo_typeid )
	{
		/* Fatal error! Invalid stack! Not recoverable! */
		
		BADINT( bracketgather_exit, &tok, (uintptr_t)&callerinfo_typeid );
			NOTELINE(); STRARG( "Call to peek_divertthread_callerinfo() returned wrong typeid!" );
		
		stack_ENDRETFRAME();
	}
	
	
		/* This is in the way, move it. */
	STACKPOP_UINT( &( stkp->data ), tok,  bracketgather_exit, scratch );
		
			/* First things first, restore the divertthread info! */
		scratch = pop_divertthread_callerinfo( &( stkp->data ),  &escapeinfo );
		if( !scratch )
		{
			FAILEDINTFUNC( "pop_divertthread_callerinfo", bracketgather_exit, scratch ):
			
			stack_ENDRETFRAME();
		}
		scratch = pop_divertthread_info( &( stkp->data ),  &subordinateinfo );
		if( !scratch )
		{
			FAILEDINTFUNC( "pop_divertthread_info", bracketgather_exit, scratch ):
			
			stack_ENDRETFRAME();
		}
		
		/* And now move the "token" back onto the stack. */
	STACKPUSH_UINT( &( stkp->data ), tok,  bracketgather_exit, scratch );
	
	
	RETFRAMEFUNC( bracketgather_exit, scratch );
}
		/* Handles commas. */
	retframe queue_argrecep( stkp, v_ );
		/* Gathers tokens to fill out the current argument, dispatching */
		/*  otherwise if appropriate for the token. */
		/* v_ must be non-null. */
		/* ( tokengroup* token* -- ... ) */
	retframe accumulate_argrecep( stackpair *stkp, void *v )
	{
#define accumulate_argrecep_SIMPLYBREAK( ... ) /* Success. */ break;
#define accumulate_argrecep_ISINFUNC_NULLARG( arga, argb ) \
		/* One or both arguments were provided as null. */ \
	BADNULL2( accumulate_argrecep, &( arga ), &( argb ) ); \
	stack_ENDRETFRAME();
#define accumulate_argrecep_ISINFUNC_NULLELEM( arga, argb, offset ) \
	TRESPASSPATH( accumulate_argrecep, "ERROR! Null element encountered by isin_tokhdptr_parr() in accumulate_argrecep()." ); \
		NOTELINE(); DATAPTR( argb ); \
		NOTESPACE(); DECARG( offset );
		
		int scratch;
		
		STACKCHECK2( stkp, v_,  accumulate_argrecep );
		
		bracketcontext_specials *spec = (bracketcontext_specials*)v_;
		uintptr_t tok;
		size_t matchloc;
		
		STACKPEEK_UINT( &( stkp->data ), 0, tok,  accumulate_argrecep, scratch );
		
		static retframe_parr
			badtoken_exit =
				(retframe_parr)
				{
					2, /* Number of retframes  */
					{
						(retframe){ &enqueue_returns, (void*)&complete_args },
						(retframe){ &vm_placeholder, (void*)0 }
					}
				};
			/* Have we found an illegal token? */
#define accumulate_argrecep_ONILLEGAL( arga, argb, offset ) \
	NOTELINE(); \
	STRARG( "ERROR! Invalid token in bracketcontext_specials{ " ); \
		DATAPTR( spec ); \
			STRARG( " }->start:  " ); \
			DECARG( spec->start->toktype ); \
			NOTESPACE(); \
			STRARG( ( (token*)( spec->start ) )->text ); \
			STRARG( "; bracket context: " ); \
		DECARG( ( (token_head*)tok )->toktype ); \
			NOTESPACE(); \
			STRARG( ( (token*)tok )->text ); \
		/* Patch over the placeholder. */ \
	badtoken_exit.body[ 1 ] = spec->jumpinfo->recepdata->longjump; \
	return( (retframe){ &enqueue_returns, (void*)&badtoken_exit } );
		ISIN_TOKHDPTR_PARR_RUN(
			tok, spec->bads, &matchloc,
			
			accumulate_argrecep_ISINFUNC_NULLARG, accumulate_argrecep_ISINFUNC_NULLELEM,
			accumulate_argrecep_ONILLEGAL,
			accumulate_argrecep_SIMPLYBREAK,
			
			&errs, accumulate_argrecep, scratch, stack_ENDRETFRAME
		);
		
			/* Have we found an argument separator? */
#define accumulate_argrecep_ONSEPARATOR( arga, argb, offset ) \
	return( (retframe){ &queue_argrecep, v_ } );
		ISIN_TOKHDPTR_PARR_RUN(
			tok, spec->bads, &matchloc,
			
			accumulate_argrecep_ISINFUNC_NULLARG, accumulate_argrecep_ISINFUNC_NULLELEM,
			accumulate_argrecep_ONSEPARATOR,
			accumulate_argrecep_SIMPLYBREAK,
			
			&errs, accumulate_argrecep, scratch, stack_ENDRETFRAME
		);
		
		static retframe_parr
			on_bracketstart =
				(retframe_parr)
				{
					2, /* Number of retframes  */
					{
						(retframe){ &on_execable, (void*)0 },
						(retframe){ &accumulate_argrecep, (void*)0 }
					}
				};
			/* Have we found a new bracket entry? */
#define accumulate_argrecep_ONBRACKETSTARTER( arga, argb, offset ) \
	on_bracketstart.body[ 1 ].data = v_; \
	return( (retframe){ &enqueue_returns, (void*)&on_bracketstart } );
		ISIN_TOKHDPTR_PARR_RUN(
			tok, spec->bads, &matchloc,
			
			accumulate_argrecep_ISINFUNC_NULLARG, accumulate_argrecep_ISINFUNC_NULLELEM,
			accumulate_argrecep_ONBRACKETSTARTER,
			accumulate_argrecep_SIMPLYBREAK,
			
			&errs, accumulate_argrecep, scratch, stack_ENDRETFRAME
		);
		
			/* Have we found the ending bracket? */
#define accumulate_argrecep_BADARGS( err ) \
	/* One or both arguments were provided as null. */ \
	FAILEDINTFUNC( "equal_tokens", accumulate_argrecep, ( err ) ); \
	STDMSG_BADNULL2_WRAPPER( accumulate_argrecep, &tok, &( spec->end ) );
#define accumulate_argrecep_ONEND( thead_a, thead_b, int_res ) \
	return( (retframe){ &enqueue_returns, (void*)&complete_args } );
		TOKEN_EQUALS_SIMPLECOMP_BODY(
			tok, spec->end,
			
			accumulate_argrecep_BADARGS,
			
			accumulate_argrecep_SIMPLYBREAK, accumulate_argrecep_SIMPLYBREAK, accumulate_argrecep_SIMPLYBREAK,
			accumulate_argrecep_ONEND
		);
		
			/* The token wasn't one of the types that causes specials-specific */
			/*  behavior, so let's search the executable table. */
		retframe_parr *retcalls = (retframe_parr*)0;
		static retframe dynamic_recursor;
		dynamic_recursor = (retframe){ &accumulate_argrecep, v_ };
			/* Search for a macro or directive entrance. */
		generic_named *found;
		scratch = is_execable( (token*)tok,  &found );
		switch( scratch )
		{
			case 0:
				/* Not exec-capable. We just use the token as a... token. */
				
				static retframe_parr
					not_execable_inner =
						(retframe_parr)
						{
							2, /* Number of retframes  */
							{
									/* ( tokengroup* token_head* -- tokengroup* ) */
								(retframe){ &vm_pushto_tokengroup, (void*)0 },
									/* ( -- token* ) */
								(retframe){ &token_queue_fetch, (void*)0 }
								
									/* ( tokengroup* token* -- ... ) */
								/* We will mystically, magically, proceed into */
								/*  accumulate_argrecep(), but it's probably best */
								/*  not to think about how too horribly much. */
							}
						},
					not_execable_outer =
						(retframe_parr)
						{
							3, /* Number of retframes  */
							{
								(retframe){ &vm_pushretframe, (void*)&dynamic_recursor }
									/* ( retframe -- ret: retframe ) */
								(retframe){ &swap_retframe2ret, (void*)0 },
								(retframe){ &enqueue_returns, (void*)&not_execable_inner }
							}
						};
				
				retcalls = &not_execable_outer;
				
				break;
			case 1:
				/* Exec-capable. Dispatch to "retframe on_execable( stkp, v_ )". */
				
				static retframe_parr
					execable =
						(retframe_parr)
						{
							4, /* Number of retframes  */
							{
									/* ( token* -- token* ) */
								(retframe){ &on_execable, (void*)0 },
								(retframe){ &vm_pushretframe, (void*)&dynamic_recursor }
									/* ( retframe -- ret: retframe ) */
								(retframe){ &swap_retframe2ret, (void*)0 },
								(retframe){ &enqueue_returns, (void*)&not_execable_inner }
							}
						};
				
				retcalls = &execable;
				break;
			case -3: /* Unrecognized generic_named->reftype */
			case -2: /* Uninitialized generic_named->ref */
			default: /* Unknown error. */
				
				FAILEDINTFUNC( "is_execable()", accumulate_argrecep, scratch );
					STDMSG_NOTELINE_WRAPPER( &errs ); STDMSG_DATAPTRARG_WRAPPER( &errs, (token*)tok );
					STDMSG_NOTESPACE_WRAPPER( &errs ); STDMSG_DATAPTRARG_WRAPPER( &errs, found );
				
				stack_ENDRETFRAME();
		}
		return( (retframe){ &enqueue_returns, (void*)retcalls } );
	}
		/* Handles commas. */
		/* Condense the current tokengroup{}, move into the parent */
		/*  tokengroup{}, and push a new tokengroup{} onto the stack. */
		/* v_ must be non-null. */
		/* ( tokengroup* tokengroup* token* -- ... ) */
	retframe queue_argrecep( stackpair *stkp, void *v )
	{
		int scratch;
		
		STACKCHECK2( stkp, v_,  queue_argrecep );
		
		bracketcontext_specials *spec = (bracketcontext_specials*)v_;
		uintptr_t tok;
		size_t matchloc;
		
		STACKPEEK_UINT( &( stkp->data ), 0, tok,  queue_argrecep, scratch );
		
			/* Confirm we have an argument seperator. */
#define queue_argrecep_SIMPLYBREAK( ... ) /* Success. */ break;
			/* These three are all errors. */
#define queue_argrecep_ISINFUNC_NULLARG( arga, argb ) \
		/* One or both arguments were provided as null. */ \
	BADNULL2( queue_argrecep, &( arga ), &( argb ) ); \
	stack_ENDRETFRAME();
#define queue_argrecep_ISINFUNC_NULLELEM( arga, argb, offset ) \
	TRESPASSPATH( queue_argrecep, "ERROR! Null element encountered by isin_tokhdptr_parr() in accumulate_argrecep()." ); \
		NOTELINE(); DATAPTR( argb ); \
		NOTESPACE(); DECARG( offset );
#define queue_argrecep_NOSEPARATOR( arga, argb ) \
	TRESPASSPATH( queue_argrecep, "ERROR! queue_argrecep() invoked without a separator token! " ); \
	DATAPTR( arga ); \
	NOTESPACE(); DATAPTR( argb );
		ISIN_TOKHDPTR_PARR_RUN(
			tok, spec->breaks, &matchloc,
			
			queue_argrecep_ISINFUNC_NULLARG, queue_argrecep_ISINFUNC_NULLELEM,
			queue_argrecep_SIMPLYBREAK,
			queue_argrecep_NOSEPARATOR,
			
			&errs, accumulate_argrecep, scratch, stack_ENDRETFRAME
		);
		
		static retframe_parr
			seq =
				(retframe_parr)
				{
					8, /* Number of retframes  */
					{
							/* We actually don't care about the seperator anymore. */
						(retframe){ &invoke_dealloctoken, (void*)0 },
						
						/* Shove the newest argument group into the deeper tokengroup{}. */
							/* ( tokengroup* tokengroup* -- tokengroup* ) */
						(retframe){ &vm_pushto_tokengroup, (void*)0 },
						
							/* ... and then replace it. */
						(retframe){ &enqueue_returns, (void*)&build_argcontainer },
						
						/* Recurse back to accumulate_argrecep() */
							/* ( -- token* ) */
						(retframe){ &token_queue_fetch, (void*)0 },
							/* The void* for accumulate_argrecep gets dynamically patched */
							/*  below! */
						(retframe){ &accumulate_argrecep, (void*)0 }
					}
				};
		seq.body[ 7 ].data = v_;
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
		/* ( token* -- ( token* 0 ) | ( tokengroup* 1 ) | ( tokengroup* 2 ) ) */
reframe bracketgather_entry( stackpair *stkp, void *v )
{
		/* This provides common behavior for the *script retframe_parr{}s. */
	static retframe_parr
			/* Push two tokengroup{}s onto the stack for use as things go along. */
			/* ( token* -- tokengroup* tokengroup* ) */
		preprep_arglist =
			(retframe_parr)
			{
				8, /* Number of retframes  */
				{
					/* WARNING: if you make any changes, then adjust the */
					/*  "subordinate function" sigs, and thus details! */
					
					(retframe){ &vm_buildempty_tokengroup, (void*)0 },
							/* v_ = &uintptr_t; uintptr_t = data; (  -- data ) */
						(retframe){ &vm_pushdata, (void*) &argset },
								/* ( dest-token_head* src-token_head* -- dest-token_head* src-token_head* ) */
							(retframe){ &vm_tokenhead_settoktype, (void*)0 },
								/* The token* from vm_pushdata() will be a static */
								/*  allocation, so deallocation would likely segfault... */
						(retframe){ &drop, (void*)0 },
							/* ( token* tokengroup* -- tokengroup* token* ) */
						(retframe){ &swap2nd, (void*)0 },
								/* ( tokengroup* token_head* -- tokengroup* token_head* ) */
							(retframe){ &vm_setsubtype_tokengroup, (void*)0 },
						(retframe){ &invoke_dealloctoken, (void*)0 },
					
					(retframe){ &enqueue_returns, (void*)&build_argcontainer }
				}
			};
	
		/* All have the same sig: ( token* -- tokengroup* tokengroup* token* -- ... ) */
	static retframe_parr
		plainscript =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* (  -- tokengroup* tokengroup* ) */
					(retframe){ &enqueue_returns, (void*)&preprep_arglist },
						/* ( -- token* ) */
					(retframe){ &token_queue_fetch, (void*)0 },
						/* ( tokengroup* token* -- ... ) */
					(retframe){ &accumulate_argrecep, (void*)&plainspecs }
				}
			},
		parenscript =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* (  -- tokengroup* tokengroup* ) */
					(retframe){ &enqueue_returns, (void*)&preprep_arglist },
						/* ( -- token* ) */
					(retframe){ &token_queue_fetch, (void*)0 },
						/* ( tokengroup* token* -- ... ) */
					(retframe){ &accumulate_argrecep, (void*)&parenspecs }
				}
			},
		curlscript =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* (  -- tokengroup* tokengroup* ) */
					(retframe){ &enqueue_returns, (void*)&preprep_arglist },
						/* ( -- token* ) */
					(retframe){ &token_queue_fetch, (void*)0 },
						/* ( tokengroup* token* -- ... ) */
					(retframe){ &accumulate_argrecep, (void*)&curlspecs }
				}
			},
		sqarscripts =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* (  -- tokengroup* tokengroup* ) */
					(retframe){ &enqueue_returns, (void*)&preprep_arglist },
						/* ( -- token* ) */
					(retframe){ &token_queue_fetch, (void*)0 },
						/* ( tokengroup* token* -- ... ) */
					(retframe){ &accumulate_argrecep, (void*)&sqarspecs }
				}
			},
		
			/* ( tokengroup*args bookmark -- tokengroup*args bookmark ) */
			/* We just need the hook right now, not any actual functionality. */
		errhandler =
			(retframe_parr)
			{
				1, /* Number of retframes  */
				{
					(retframe){ &noop, void*)0 }
				}
			};
	
	
	
	int scratch;
	
	retframe ret;
	
	STACKCHECK2( stkp, v_,  bracketgather_entry );
	
	uintptr_t tok;
	STACKPOP_UINT( &( stkp->data ), tok,  bracketgather_entry, scratch );
	
		/* Save the old divertthread info- we'll be needing it if this was nested. */
	scratch = push_divertthread_info( &( stkp->data ), subordinateinfo );
	if( !scratch )
	{
		FAILEDINTFUNC( "push_divertthread_info", bracketgather_entry, scratch );
		
		stack_ENDRETFRAME();
	}
		/* Yes, this MUST come last, because it'll ensure that *_callerinfo type id */
		/*  is on TOP of the stack. */
	scratch = push_divertthread_callerinfo( &( stkp->data ), escapeinfo );
	if( !scratch )
	{
		FAILEDINTFUNC( "push_divertthread_callerinfo", bracketgather_entry, scratch );
		
		stack_ENDRETFRAME();
	}
	
		/* Restored! And it's on top of the stuff that we won't need till exit! */
	STACKPUSH_UINT( &( stkp->data ), tok,  bracketgather_entry, scratch );
	
	
	
		/* Reset the divertthread_info to a nice, safe default. */
	subordinateinfo = VALUE_subordinateinfo;
	
	static retframe_parr
		seq =
			(retframe_parr)
			{
				2, /* Number of retframes  */
				{
						/* v_ must be a pointer to a divertthread_info{}. The */
						/*  values of the elements in the provided instance */
						/*  will be swapped with "foreign" values during the */
						/*  longevity of the provided setfunc and jumpfunc */
						/*  framefunc{}s, DON'T SCREW WITH THOSE VALUES. The */
						/*  CONTENTS of ->recepdata won't be touched, with */
						/*  the exception of ->earlyexit, which (IF */
						/*  ->recepdata is non-null) will be initialized so */
						/*  that user code can actually use it. */
						/* Named in reference Unix's fork() function, but it */
						/*  just diverts execution of the current thread */
						/*  while providing a special exit mechanism to */
						/*  preemptively get back out of that diversion. */
						/* subordinateinfo is an instance of */
						/*  divertthread_info{}. I don't remember why I named */
						/*  it "subordinate". */
						/* ( uintptr_t -- ??? ) : The uintptr_t is an arg to */
						/*  ->setfunc, but divertthread() assumes it's */
						/*  presence. */
					(retframe){ &divertthread, (void*)&subordinateinfo },
					
						/* ( tokengroup* token* -- ... ) */
					(retframe){ &bracketgather_exit, (void*)0 }
				}
			};
#define entry_PATCHSEQ( exit_v_ ) \
		seq.body[ 1 ].data = (void*)( exit_v_ );
	scratch = simplify_toktype( (token_head*)tok,  &tok );
	if( !scratch )
	{
		FAILEDINTFUNC( "simplify_toktype", bracketgather_entry, scratch );
			NOTELINE(); DATAPTR( tok );
		
		stack_ENDRETFRAME();
	}
	switch( tok )
	{
		case TOKTYPE_OPPAR:
				/* Both of these functions MUST comply with the following function */
				/*  signature: */
					/* ( bookmark -- bookmark ) */
				/*  What is under "bookmark" DOES NOT matter, but "bookmark" WILL */
				/*  BE on top upon entry, and MUST BE on top AND UNALTERED upon */
				/*  exit, lest the entire system break. This is NOT a small thing, */
				/*  it can completely screw up the stack. */
			subordinateinfo.setfunc = (retframe){ &enqueue_returns, (void*)&plainscript };
			subordinateinfo.jumpfunc = (retframe){ &enqueue_returns, (void*)&errhandler };
			entry_PATCHSEQ( &plainspecs );
			break;
		case TOKTYPE_OPPARUP:
			subordinateinfo.setfunc = (retframe){ &enqueue_returns, (void*)&parenscript };
			subordinateinfo.jumpfunc = (retframe){ &enqueue_returns, (void*)&errhandler };
			entry_PATCHSEQ( &parenspecs );
			break;
		case TOKTYPE_OPCRLUP:
			subordinateinfo.setfunc = (retframe){ &enqueue_returns, (void*)&curlscript };
			subordinateinfo.jumpfunc = (retframe){ &enqueue_returns, (void*)&errhandler };
			entry_PATCHSEQ( &curlspecs );
			break;
		case TOKTYPE_OPSQRUP:
			subordinateinfo.setfunc = (retframe){ &enqueue_returns, (void*)&sqarscripts };
			subordinateinfo.jumpfunc = (retframe){ &enqueue_returns, (void*)&errhandler };
			entry_PATCHSEQ( &sqarspecs );
			break;
		default:
			TRESPASSPATH( entry, "ERROR! Unfamiliar token type from simplify_toktype(): " );
				DECARG( tok );
			
			stack_ENDRETFRAME();
	}
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}



int is_execable( token *tok,  generic_named **found )
{
	generic_named *found_ = (generic_named*)0;
	if( !found )
	{
		found = &found_;
	}
	
		/* Some of the directives CAN'T be inlined, so we need to */
		/*  differentiate on that. */
	genname_parr *searchtable = ( was_freshline( (token_head*)tok ) ? freshline : inline );
	
	*found =  = bsearch1_gennamearr( searchbatch, tok );
	
	if( !( *found ) )
	{
		/* Just a token, simple exit. */
		
		return( 0 );
		
	} else if( ( *found )->reftype == GENNAMETYPE_RETFRAMEFUNC )
	{
		if( !( ( *found )->ref ) )
		{
				/* Error, no reference, fatal error! */
			TRESPASSPATH( is_execable, "Error! Null ( *found )->ref value!" );
			
			return( -2 );
		}
		
		return( 1 );
		
	} else {
		
		TRESPASSPATH( is_execable, "Error! Unknown ( *found )->reftype value: " );
			DECARG( ( ( *found )->reftype ) );
		
		return( -3 );
	}
	
	/* We should never reach here. */
}
	/* Note: does not necessarily return the SAME token pointer. In fact, */
	/*  generally SHOULDN'T. */
	/* ( token* -- ... -- token* ) */
retframe on_execable( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  on_execable );
	
	uintptr_t tok;
	STACKPEEK_UINT( &( stkp->data ), tok,  on_execable, scratch );
	
	generic_named *found;
	scratch = is_execable( (token*)tok,  &found );
	switch( scratch )
	{
		case 1:
			/* Exec-capable. */
			break;
		case 0:
			/* Not exec-capable: should never have gotten here. */
		case -2:
			/* generic_named->ref not initialized: also should never have gotten here. */
		case -3:
			/* generic_named->reftype unknown: still should never have gotten here. */
		default:
			/* Unknown error. */
			
			FAILEDINTFUNC( "is_execable", on_execable, scratch );
				NOTELINE(); DATAPTR( tok );
				NOTESPACE(); DATAPTR( &found );
			
			stack_ENDRETFRAME();
	}
	
	if( found->reftype != GENNAMETYPE_RETFRAMEFUNC )
	{
			/* Error! */
		BADUINT( on_execable, &( found->reftype ), GENNAMETYPE_RETFRAMEFUNC );
			NOTELINE(); DATAPTR( &found );
		
		stack_ENDRETFRAME();
	}
	if( !( found->ref ) )
	{
			/* Error, no reference, fatal error! */
		TRESPASSPATH( on_execable, "Error! Null ( *found )->ref value!" );
			NOTELINE(); DATAPTR( &found );
		
		stack_ENDRETFRAME();
	}
	
	return( *( (retframe*)( found->ref ) ) );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
