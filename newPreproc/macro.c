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

#include "macro.h"
#include "inner_macro.h"

#include "err/inner_err.h"
#include "token.h"
#include "stack.h"



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

#define STACKPEEK_UINT( stk, offset, destvar,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPOP_UINT( stk, destvar,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )

#define RETFRAMEFUNC( caller, scratch ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )


#define FETCH_SIMPLETYPE( tokhead, destuintp,  caller, scratch ) \
	TOKEN_FETCH_SIMPLETYPE( ( tokhead ), ( destuintp ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define CHECK_SIMPLETYPE( tokhead, testval, on_yes, on_no,  caller, scratch ) \
	TOKEN_CHECK_SIMPLETYPE( ( tokhead ), ( testval ), ( on_yes ), ( on_no ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )



int isvalid_macropart( token_head *th )
{
	if( !th )
	{
		return( -1 );
	}
	
	int scratch;
	uintptr_t type;
	
#define isvalid_macropart_ONERR() \
		return( -2 );
	TOKEN_FETCH_SIMPLETYPE( *th, type,  &errs, isvalid_macropart, scratch, isvalid_macropart_ONERR );
	
	switch( type )
	{
		case TOKTYPE_TOKENGROUP_MACROLINK:
		case TOKTYPE_TOKENGROUP_MACROTOKEN:
		case TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION:
		case TOKTYPE_TOKENGROUP_MACRORUN:
		case TOKTYPE_TOKENGROUP_MACRODIRECTIVE:
			return( 1 );
		
		default:
			break;
	}
	
	return( 0 );
}

	/* DOES NOT delete the token. */
	/* ( len -- char_parr* len token* ) */
retframe accumulate_macroargname( stackpair *stkp, void *v,  token_head *part )
{
	STACKCHECK( stkp,  accumulate_macroargname );
	
	int scratch = ??? ( part );
	if( scratch ??? )
	{
		???
	}
	
	uintptr_t len;
	
	STACKPOP_UINT( &( stkp->data ), len,  accumulate_macroargname, scratch );
	
	STACKPUSH_UINT( &( stkp->data ), len + 1,  accumulate_macroargname, scratch );
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)part,  accumulate_macroargname, scratch );
	
	static retframe_parr seq =
		(retframe_parr)
		{
			3, /* Number of retframes  */
			{
					/* ( token* -- token* char_parr* ) */
				(retframe){ &token2char_parr, (void*)0 },
					/* ( len token* char_parr* -- char_parr* token* len ) */
				(retframe){ &swap3rd, (void*)0 },
					/* ( char_parr* token* len -- char_parr* len token* ) */
				(retframe){ &swap2nd, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, &seq } );
}
	/* ( char_parr*[len] len -- macrocllaval_parr* ) */
retframe finalize_macroargname( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  finalize_macroargname );
	
	uintptr_t len, tmp;
	int scratch;
	
	STACKPOP_UINT( &( stkp->data ), len,  finalize_macroargname, scratch );
	
#define finalize_macroargname_ONERR( err ) \
		???
	macrocallargval_pascalarray_result res = macrocallargval_pascalarray_build( len );
	macrocallargval_pascalarray *a;
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, finalize_macroargname_ONERR );
	if( !a )
	{
		???
	}
	
	uintptr_t mark = a->len;
	while( len )
	{
		STACKPOP_UINT( &( stkp->data ), tmp,  finalize_macroargname, scratch );
		--len;
		
		if( !tmp && mark != a->len )
		{
			mark = len;
		}
		
		a->body[ len ] = (macro_call_argval){ 0, (char_pascalarray*)tmp };
	}
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)a,  finalize_macroargname, scratch );
	if( mark != a->len )
	{
		/* Report error. */
	}
	
	
	RETFRAMEFUNC( finalize_macroargname, scratch );
}
	/* ( macrocllaval_parr* --  ) */
retframe destroy_macroargname( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  destroy_macroargname );
	
	uintptr_t tmp;
	int scratch;
	
	STACKPOP_UINT( &( stkp->data ), tmp,  destroy_macroargname, scratch );
	macrocallargval_pascalarray *parr = (macrocallargval_pascalarray*)tmp;
	if( !parr )
	{
		???
	}
	tmp = 0;
	
	lib4_result res;
	
	while( parr->len > tmp )
	{
		res = char_pascalarray_destroy( parr->body[ tmp ].text );
#define destroy_macroargname_ONERR1( err ) \
	???
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_NULL_MACRO, destroy_macroargname_ONERR1 );
		++tmp;
	}
	
	res = macrocallargval_pascalarray_destroy( parr );
#define destroy_macroargname_ONERR2( err ) \
		???
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_NULL_MACRO, destroy_macroargname_ONERR2 );
	
	
	RETFRAMEFUNC( destroy_macroargname, scratch );
}

	/* ( len -- token* len ) */
retframe accumulate_macropart( stackpair *stkp, void *v,  token_head *part )
{
	STACKCHECK( stkp,  accumulate_macropart );
	
	int scratch = isvalid_macropart( part );
	if( scratch <= 0 )
	{
		???
	}
	
	uintptr_t len;
	
	STACKPOP_UINT( &( stkp->data ), len,  accumulate_macropart, scratch );
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)part,  accumulate_macropart, scratch );
	STACKPUSH_UINT( &( stkp->data ), len + 1,  accumulate_macropart, scratch );
	
	RETFRAMEFUNC( accumulate_macropart, scratch );
}
	/* ( token*[len] len -- tokhdptr_parr* ) */
retframe finalize_macropart( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  finalize_macropart );
	
	uintptr_t len, tmp;
	int scratch;
	
	STACKPOP_UINT( &( stkp->data ), len,  finalize_macropart, scratch );
	
#define finalize_macropart_ONERR( err ) \
		???
	tokenheadptr_pascalarray_result res = tokenheadptr_pascalarray_build( len );
	tokenheadptr_pascalarray *a;
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, finalize_macropart_ONERR );
	if( !a )
	{
		???
	}
	
	uintptr_t mark = a->len;
	while( len )
	{
		STACKPOP_UINT( &( stkp->data ), tmp,  finalize_macropart, scratch );
		--len;
		
		if( !tmp && mark != a->len )
		{
			mark = len;
		}
		
		a->body[ len ] = (token_head*)tmp;
	}
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)a,  finalize_macropart, scratch );
	if( mark != a->len )
	{
		/* Report error. */
	}
	
	
	RETFRAMEFUNC( finalize_macropart, scratch );
}
	/* ( tokhdptr_parr* -- token*[len] len ) */
retframe unpack_macroparts( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  unpack_macroparts );
	
	uintptr_t tmp;
	int scratch;
	
	STACKPOP_UINT( &( stkp->data ), tmp,  unpack_macroparts, scratch );
	tokenheadptr_pascalarray *parr = (tokenheadptr_pascalarray*)tmp;
	if( !parr )
	{
		???
	}
	tmp = 0;
	
	lib4_result res;
	
	while( parr->len > tmp )
	{
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)( parr->body[ tmp ] ),  unpack_macroparts, scratch );
		++tmp;
	}
	
	res = tokenheadptr_pascalarray_destroy( parr );
#define unpack_macroparts_ONERR( err ) \
		???
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, unpack_macroparts_ONERR );
	
	
	RETFRAMEFUNC( unpack_macroparts, scratch );
}

	/* ( macrocllaval_parr* tokhdptr_parr* -- macro_call* ) */
retframe assemble_macrocall( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  assemble_macrocall );
	
	lib4_ptrresult res = lib4_stdmemfuncs.alloc( lib4_stdmemfuncs.data, sizeof( macro_call ) );
	macro_call *a;
#define assemble_macrocall_ONERR( err ) \
		???
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, assemble_macrocall_ONERR );
	
	int scratch;
	uintptr_t toks, args;
	STACKPOP_UINT( &( stkp->data ), toks,  assemble_macrocall, scratch );
	STACKPOP_UINT( &( stkp->data ), args,  assemble_macrocall, scratch );
	
	a->header = INIT_TOKENHEAD( TOKTYPE_TOKENGROUP_MACROCALL, 0,  0,  0, 0, 0 );
	a->args = (macrocllaval_parr*)args;
	a->body = (tokhdptr_parr*)toks;
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)a,  assemble_macrocall, scratch );
	
	RETFRAMEFUNC( assemble_macrocall, scratch );
}
	/* ( macro_call* -- macrocllaval_parr* tokhdptr_parr* ) */
retframe disassemble_macrocall( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  disassemble_macrocall );
	
	int scratch;
	uintptr_t tmp;
	STACKPOP_UINT( &( stkp->data ), tmp,  disassemble_macrocall, scratch );
	
	macro_call *mc = (macro_call*)tmp;
	lib4_result = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, mc );
	lib4_success_result a;
#define disassemble_macrocall_ONERR( err ) \
		???
	LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, disassemble_macrocall_ONERR );
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)( a->args ),  disassemble_macrocall, scratch );
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)( a->body ),  disassemble_macrocall, scratch );
	
	RETFRAMEFUNC( disassemble_macrocall, scratch );
}





	/* The actually available stuff. Maybe belongs here, maybe */
	/*  doesn't, worry about that later. */
???
genname_parr *freshline, *inline;
???

	/*
		(
				char_parr*
			--
				( genericnamed*freshline genericnamed*inline ( 1 | 2 ) )
		)
	*/
	/* If the top value is 0 then the string wasn't found and new */
	/*  entries were created, if it's 1 then entries were found- */
	/*  in either case,  */
	/* Pointers to both of the instances will be on the stack. */
	/*  If the number is 1, then they were found already existing, */
	/*  if 0 then they had to be built from scratch and have only */
	/*  been initialized with the name (and null/invalid...). */
retframe find_defname_conclude( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  find_defname_conclude );
	
	int scratch;
	uintptr_t parr, flag;
	
	STACKPOP_UINT( &( stk->data ), flag,  find_defname_conclude, scratch );
	STACKPEEK_UINT( &( stk->data ), 0, parr,  find_defname_conclude, scratch );
	/* The opening and closing quotes will have been removed */
	/*  from the string already. */
	
	if( flag )
	{
			/* Success. */
		STACKPOP_UINT( &( stk->data ), parr,  find_defname_conclude, scratch );
		
	} else {
		
			/* Failure */
		return( ( (retframe)v_ )[ 1 ] );
	}
	
		/* To search for a pre-existing macro of this name. */
	genericnamed *found_entry = bsearch1_gennamearr( freshline, (char_parr*)parr );
	if( found_entry )
	{
		lib4_result res = char_pascalarray_destroy( (char_parr*)parr );
		lib4_success_result a;
#define find_defname_conclude_ONERR2( err ) \
	???
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, find_defname_conclude_ONERR2 );
		
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)found_entry,  find_defname_conclude, scratch );
		
		found_entry = bsearch1_gennamearr( inline, (char_parr*)parr );
		if( !found_entry )
		{
			???
		}
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)found_entry,  find_defname_conclude, scratch );
		
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)1,  find_defname_conclude, scratch );
		
		RETFRAMEFUNC( find_defname_conclude, scratch );
		
	} else {
		
		size_t oldlen = freshline->len;
		
		genname_parr *tmp = resize_gennamearr( freshline, oldlen + 1 );
		if( !tmp )
		{
			???
		}
		freshline = tmp;
		freshline->body[ oldlen ] = (genericnamed){ (char_parr*)parr, (void*)0, GENNAMETYPE_INVALID };
		qsort_gennamearr( freshline );
		
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)&( freshline->body[ oldlen ] ),  find_defname_conclude, scratch );
		
		
		oldlen = inline->len;
		
		tmp = resize_gennamearr( inline, oldlen + 1 );
		if( !tmp )
		{
			???
		}
		inline = tmp;
		char_pascalarray_result res = char_pascalarray_build( ( (char_parr*)parr )->len );
		char_parr *a;
#define find_defname_conclude_ONERR3( err ) \
	???
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, find_defname_conclude_ONERR3 );
		memcpy
		(
			(void*)( a->body ),
			(const void*)( ( (char_parr*)parr )->body ),
			
			( (char_parr*)parr )->len
		);
		inline->body[ oldlen ] = (genericnamed){ a, (void*)0, GENNAMETYPE_INVALID };
		qsort_gennamearr( inline );
		
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)&( inline->body[ oldlen ] ),  find_defname_conclude, scratch );
		
		
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)0,  find_defname_conclude, scratch );
		
		
		RETFRAMEFUNC( find_defname_conclude, scratch );
	}
}
	/* ( token* | tokengroup* -- ??? ) */
retframe find_defname_mid( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  find_defname_mid );
	
	int scratch;
	uintptr_t tmp, type;
	STACKPEEK_UINT( &( stk->data ), 0, tmp,  find_defname_mid, scratch );
	
	FETCH_SIMPLETYPE( (token_head*)tmp, type,  find_defname_mid, scratch );
	switch( type )
	{
		case TOKTYPE_OPPARUP:
			
			break;
		
		case TOKTYPE_SQSTR:
		case TOKTYPE_DQSTR:
			/* Special error message: wrap your args in preproc-parens! */
		default:
			/* Standard error reporting. */
	}
	tokengroup *tg = (tokengroup*)tmp;
	if( !( tg->arr ) )
	{
		/* Error, array is missing! */
		???
	}
	if( tg->used != 1 )
	{
		/* Error! Though we should really be resilient against comments... */
		???
	}

	token_head *th = tg->arr->body[ 0 ];
	FETCH_SIMPLETYPE( th, type,  find_defname_mid, scratch );
	if( type != TOKTYPE_SQSTR && type != TOKTYPE_DQSTR )
	{
		/* Error! But maybe shouldn't be??? */
		???
	}
	tg->used = 0;
	tg->arr->body[ 0 ] = (token_head*)0;

	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)th,  find_defname_mid, scratch );
	
	static retframe_parr seq =
		(retframe_parr)
		{
			7, /* Number of retframes  */
			{
					/* ( tokengroup* token_head* -- token_head* tokengroup* ) */
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ &invoke_dealloctoken, (void*)0 },
					/* ( string-token* -- token* char_parr* ) */
				(retframe){ &stringtoken2char_parr, (void*)??? },
				(retframe){ &swap2nd, (void*)0 },
				(retframe){ &invoke_dealloctoken, (void*)0 },
				(retframe){ &end_run, (void*)0 },
					/* ( char_parr* -- ... ) */
				(retframe){ &find_defname_conclude, (void*)0 }
			}
		};
	seq.body[ 5 ] = *( (retframe)v_ )[ 0 ];
	seq.body[ 6 ].data = v_;
	return( (retframe){ &enqueue_returns, &seq } );
}
	/* v_ must point to an array of retframes: */
		/* [0]: ( char_parr* -- char_parr* flag ), a syntax checker: */
			/* if flag==0 that [1] will be called with ( char_parr* -- ... ) */
			/* if flag==1 then find_defname_conclude() will run normally. */
		/* [1]: ( char_parr* -- ... ), an error handler: */
			/* Honestly, it just does whatever. The char_parr* is the only */
			/*  data it needs to clean up (hopefully). */
retframe find_defname( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  find_defname );
	
	static retframe_parr
		onsucc_ =
			(retframe_parr)
			{
				4, /* Number of retframes  */
				{
						/* Drop the flag. */
					(retframe){ &drop, (void*)0 },
					(retframe){ &bracketgather_dispatcher, (void*)0 },
	/* This define exists to adjust the data member of the */
	/*  find_defname_mid() retframe: keep it properly adjusted. */
#define find_defname_UPDATE( val ) onsucc_.body[ 2 ].data = ( val );
					(retframe){ &find_defname_mid, (void*)0 },
						/* We'll be followed by an else, so surpress it with a fresh flag. */
					(retframe){ &vm_push1, (void*)0 }
				}
			},
		onerr_ =
			(retframe_parr)
			{
				???, /* Number of retframes  */
				{
						/* Drop the flag. */
					(retframe){ &drop, (void*)0 },
						/* We have a syntax error, report it. */
					???,
					(retframe){ &end_run, (void*)0 }
				}
			};
	retframe
		onsucc = (retframe){ &enqueue_returns, &onsucc_ },
		onerr = (retframe){ &enqueue_returns, &onerr_ };
	
	find_defname_UPDATE( v_ );
	static const retframe_parr seq =
		(retframe_parr)
		{
			5, /* Number of retframes  */
			{
				(retframe){ &token_queue_fetch, (void*)0 },
					/* ( token* -- token* ( 0 | 1 ) ) */
				(retframe){ &require_preprocoppar, (void*)0 },
				(retframe){ &run_if, (void*)&onsucc },
				(retframe){ &run_else, (void*)&onerr },
				(retframe){ &drop, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, &seq } );
}



	/* ( char_parr* -- char_parr* flag ) */
retframe vm_charparr_isname( stackpair *stkp, void *v_ )
{
	STACKCHECK( stkp,  vm_charparr_isname );
	
	int scratch;
	uintptr_t cparr;
	STACKPEEK_UINT( &( stk->data ), 0, cparr,  vm_charparr_isname, scratch );
	
		/* Returns -1 on error, 0 on "no", 1 on "yes". */
	int res = charparr_isname( (char_parr*)cparr );
	if( res < 0 )
	{
		???
	}
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)res,  vm_charparr_isname, scratch );
	
	RETFRAMEFUNC( vm_charparr_isname, scratch );
}
	/* ( char_parr* -- char_parr* flag ) */
retframe vm_charparr_ismacrosearchable( stackpair *stkp, void *v_ )
{
	STACKCHECK( stkp,  vm_charparr_isname );
	
	int scratch, res;
	uintptr_t cparr_;
	STACKPEEK_UINT( &( stk->data ), 0, cparr_,  vm_charparr_isname, scratch );
	char_parr *cparr = (char_parr*)cparr_;
	
	if( !( cparr->len == 1 && cparr->body ) )
	{
		???
	}
	if( cparr->body[ 0 ] == '#' )
	{
		res = 1;
		
	} else {
		
			/* Returns -1 on error, 0 on "no", 1 on "yes". */
		res = charparr_isname( cparr );
		if( res < 0 )
		{
			???
		}
	}
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)res,  vm_charparr_isname, scratch );
	
	RETFRAMEFUNC( vm_charparr_isname, scratch );
}



	/* These need to be passed stack rewind data. They */
	/*  both receive ONLY syntax errors, so execution */
	/*  should continue! */
retframe accumulate_string_onmerge_err( stackpair *stkp, void *v_ )
{
	???
}
retframe accumulate_string_notmerge_err( stackpair *stkp, void *v_ )
{
	???
}
	/* ( token* -- token* ) */
retframe accumulate_string( stackpair *stkp, void *v_ )
{
	STACKCHECK( stkp,  accumulate_string );
	
	int scratch, res;
	uintptr_t tmp;
	STACKPEEK_UINT( &( stk->data ), 0, tmp,  accumulate_string, scratch );
	token_head *th = (token_head*)tmp;
	
	FETCH_SIMPLETYPE( th, tmp,  accumulate_string, scratch );
	
	if
	(
		tmp != TOKTYPE_SQSTR &&
		tmp != TOKTYPE_DQSTR
	)
	{
		???
	}
	
	
	token_head mergetype =
		INIT_TOKENHEAD( TOKTYPE_TOKENGROUP_STRMERGE, 0,  0,  0, 0, 0 );
	uintptr_t refhead = (uintptr_t)&mergetype;
	static const retframe_parr
		build_group_ =
			(retframe_parr)
			{
				??? , /* Number of retframes  */
				{
					(retframe){ &drop, (void*)0 },
						(retframe){ &vm_buildempty_tokengroup, (void*)0 },
							/* ( -- refhead ) */
						(retframe){ &vm_pushdata, (void*)&refhead },
							/* ( tokengroup* token_head* -- tokengroup* token_head* ) */
						(retframe){ &vm_setsubtype_tokengroup, (void*)0 },
							/* ( token_head* --  ) */
						(retframe){ &drop, (void*)0 },
					(retframe){ &vm_push1, (void*)0 }
				}
			},
		push_member_ =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
					(retframe){ &drop, (void*)0 },
							/* ( tokengroup* token* -- tokengroup* ) */
						(retframe){ &vm_pushto_tokengroup, (void*)0 },
					(retframe){ &vm_push1, (void*)0 }
				}
			};
	retframe
		build_group = { &enqueue_returns, &build_group_ },
		push_member = { &enqueue_returns, &push_member_ },
		merge_err = { &accumulate_string_onmerge_err, (void*)0 },
		notmerge_err = { &accumulate_string_notmerge_err, (void*)0 };
	
		/* The function needs to be passed stack rewind data, since */
		/*  this is just a syntax error! */
	merge_err.data = ???;
	notmerge_err.data = ???;
	
	static const retframe_parr
		on_mergetype_ =
			(retframe_parr)
			{
				7, /* Number of retframes  */
				{
					(retframe){ &drop, (void*)0 },
						(retframe){ &token_queue_fetch, (void*)0 },
						(retframe){ &require_anystring, (void*)0 },
							(retframe){ &run_if, (void*)&push_member },
							(retframe){ &run_else, (void*)&merge_err },
						(retframe){ &drop, (void*)0 }
					(retframe){ &vm_push1, (void*)0 }
				}
			};
	static const retframe_parr
		not_mergetype_ =
			(retframe_parr)
			{
				11, /* Number of retframes  */
				{
					(retframe){ &drop, (void*)0 },
						(retframe){ &require_anystring, (void*)0 },
							(retframe){ &run_if, (void*)&build_group },
							(retframe){ &run_else, (void*)&notmerge_err },
						(retframe){ &drop, (void*)0 },
							/* ( token* tokengroup* -- tokengroup* token* ) */
						(retframe){ &swap2nd, (void*)0 },
						(retframe){ &vm_push0, (void*)0 },
								/* Total result: ( tokengroup* token* -- tokengroup* ) */
							(retframe){ &just_run, (void*)&push_member },
							(retframe){ &enqueue_returns, &on_mergetype_ },
						(retframe){ &drop, (void*)0 },
					(retframe){ &vm_push0, (void*)0 }
				}
			};
	retframe
		on_mergetype = { &enqueue_returns, &on_mergetype_ },
		not_mergetype = { &enqueue_returns, &not_mergetype_ };
	
	static const retframe_parr seq =
		(retframe_parr)
		{
			4, /* Number of retframes  */
			{
				(retframe){ &require_strmerge, (void*)0 },
					(retframe){ &run_if, (void*)&on_mergetype },
					(retframe){ &run_else, (void*)&not_mergetype },
				(retframe){ &drop, (void*)0 }
			}
		};
	return( (retframe){ &enqueue_returns, &seq } );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
