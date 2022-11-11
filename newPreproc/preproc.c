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



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
