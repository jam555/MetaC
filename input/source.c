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

#include "source.h"
#include "../err/inner_err.h"



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
	STDMSG_BADNULL_WRAPPER( &errs, ( funcname ), ( ptr ) )
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, ( funcname ), ( ptr ) )

#define MONADICFAILURE( funcname, calltext, err ) \
	STDMSG_MONADICFAILURE_WRAPPER( &errs, ( funcname ), ( calltext ), ( err ) )

	#define NOTELINE() \
		STDMSG_NOTELINE_WRAPPER( &errs )
	#define NOTESPACE() \
		STDMSG_NOTESPACE_WRAPPER( &errs )

	#define DECARG( val ) \
		STDMSG_DECARG_WRAPPER( &errs, ( val ) )
	$define CHARARG( val ) \
		STDMSG_CHARARG_WRAPPER( &errs, ( val ) )
	#define DATAPTRARG( val ) \
		STDMSG_DATAPTRARG_WRAPPER( &errs, ( val ) )
	#define STRARG( strptr ) \
		STDMSG_STRARG_WRAPPER( &errs, ( strptr ) )


#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), stack_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )


#define RETFRAMEFUNC( caller, scratch ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), ( callername ), ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, ( funcname ), ( msgstr ) )



int refed_pstr_incrrefs( refed_pstr *pstr )
{
	if( pstr )
	{
		if( pstr->refs )
		{
			++( pstr->refs );
			return( 1 );
			
		} else {
			
			TRESPASSPATH( refed_pstr_incrrefs, "ERROR: refed_pstr_incrrefs() was given a refed_pstr with a null ref count." );
			return( -2 );
		}
	}
	
	TRESPASSPATH( refed_pstr_incrrefs, "ERROR: refed_pstr_incrrefs() was given a null arg." );
	return( -1 );
}
int refed_pstr_decrrefs( refed_pstr *pstr )
{
	if( pstr )
	{
		if( pstr->refs )
		{
			--( pstr->refs );
			
			if( pstr->refs )
			{
				return( 1 );
			}
			
				/* Default to success. */
			int ret = 2;
			
#define refed_pstr_decrrefs_BADDESTROY( err ) \
	MONADICFAILURE( init, "char_pascalarray_destroy()", ( err ).val ); \
	NOTESPACE(); DATAPTRARG( pstr->text ); \
	ret = -2;
			lib4_result res = char_pascalarray_destroy( pstr->text );
			LIB4_RESULT_BODYMATCH( res, LIB4_NULL_MACRO, refed_pstr_decrrefs_BADDESTROY );
			
#define refed_pstr_decrrefs_BADDEALLOC( err ) \
	MONADICFAILURE( init, "lib4_stdmemfuncs.dealloc()", ( err ).val ); \
	NOTESPACE(); DATAPTRARG( pstr->text ); \
	ret = -3;
			res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, a );
			LIB4_RESULT_BODYMATCH( res, LIB4_NULL_MACRO, refed_pstr_decrrefs_BADDEALLOC );
			
			return( ret );
			
		} else {
			
			TRESPASSPATH( refed_pstr_decrrefs, "ERROR: refed_pstr_decrrefs() was given a refed_pstr with a null ref count." );
			return( -4 );
		}
	}
	
	TRESPASSPATH( refed_pstr_decrrefs, "ERROR: refed_pstr_decrrefs() was given a null arg." );
	return( -1 );
}



struct
{
	char_pascalarray *pushedback;
	size_t used;
	
} inbuf = { (char_pascalarray*)0, 0 };


struct source
{
	source *prev;
	
	FILE *file;
	refed_pstr *name;
	
		/* Max val is UINTMAX_MAX */
	uintmax_t base, progress;
	
} *sources = (source*)0;

#define COMMONINCLUDE "common_include.incl"


static root_name =
	LIB4_DEFINE_PASCALARRAY_LITERAL2( char_, char, COMMONINCLUDE );
static refed_pstr root_pname = (refed_pstr){ 1, &root_name };
static source root_src = (source){ (source*)0,  0, &root_pname,  0, 0 };
int init()
{
	int ret = -1;
	
	if( !sources )
	{
		root_src.file = fopen( root_src.name->text->body, "rb" );
		if( !root_src.file )
		{
			BADNULL( init, &( root_src.file ) );
			return( -2 );
		}
		
		sources = &root_src;
		
		ret = 1;
	}
	
	if( !( inbuf.pushedback ) )
	{
		char_pascalarray_result res =
			char_pascalarray_build( size_t len );
		int res2;
#define init_SUCCESS( arr ) inbuf.pushedback = ( arr );
#define init_FAILURE( err ) \
	MONADICFAILURE( init, "char_pascalarray_build()", ( err ) ); \
	sources = (source*)0; \
	res2 = fclose( root_src.file ); \
	if( res2 != 0 ) { \
		FAILEDINTFUNC( "fclose", init, res2 ); } \
	root_pname = (refed_pstr){ 1, &root_name };
	root_src = (source){ (source*)0,  0, &root_pname,  0, 0 }; \
	return( err );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, init_SUCCESS, init_FAILURE )
	}
	
	if( !ret )
	{
		TRESPASSPATH( init, "ERROR: init() in source.c failed to exit before reaching it's end." );
		BADNONULL( init, &sources );
	}
	return( ret );
}
void deinit()
{
	if( inbuf.pushedback )
	{
		lib4_result res = char_pascalarray_destroy( inbuf.pushedback );
#define deinit_SUCCESS( succ ) \
	inbuf = { (char_pascalarray*)0, 0 };
#define deinit_FAILURE( err ) \
	MONADICFAILURE( deinit, "char_pascalarray_destroy()", ( err ).val ); \
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, deinit_SUCCESS, deinit_FAILURE )
	}
	
	if( sources == &root_src )
	{
		sources = (source*)0;
		
		if( root_src.file )
		{
			int res = fclose( root_src.file );
			if( res != 0 )
			{
				FAILEDINTFUNC( "fclose", deinit, res );
			}
		}
		
		root_pname = (refed_pstr){ 1, &root_name };
		root_src = (source){ (source*)0,  0, &root_pname,  0, 0 };
	}
}
int reinit()
{
	deinit();
	return( init() );
}

source* build_source2( char_pascalarray *name, uintmax_t inclusionpoint, const char *opentype )
{
	source *a = (source*)0;
	
#define build_source2_ERR1( err ) \
	MONADICFAILURE( build_source2, "lib4_stdmemfuncs.alloc()", ( err ) )
#define build_source2_ERR2( err ) \
	MONADICFAILURE( build_source2, "char_pascalarray_build()", ( err ) )
	
	if( name )
	{
		size_t len = 0;
		while
		(
			len < name->len &&
			( isspace( name->body[ len ] ) || isgraph( name->body[ len ] ) )
		)
		{
			++len;
		}
		if( name->body[ len ] != '\0' )
		{
			++len;
		}
		
		lib4_ptrresult ptrres =
			lib4_stdmemfuncs.alloc
				( lib4_stdmemfuncs.data, sizeof( source ) );
		LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETa, build_source2_ERR1 )
		if( a )
		{
			*a = (source){ (source*)0,  (FILE*)0, (refed_pstr*)0,  inclusionpoint, 0 };
		}
		
		refed_pstr *b = (refed_pstr*)0;
		lib4_ptrresult ptrres =
			lib4_stdmemfuncs.alloc
				( lib4_stdmemfuncs.data, sizeof( refed_pstr ) );
		LIB4_PTRRESULT_BODYMATCH( ptrres, LIB4_OP_SETb, build_source2_ERR1 )
		if( !b )
		{
			if( lib4_stdmemfuncs.dealloc )
			{
				lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, a );
			}
			a = (source*)0;
			
			TRESPASSPATH( build_source2, "ERROR: build_source2() failed to allocate a refed_pstr." );
			
		} else {
			
			*b = (refed_pstr){ 0, (char_pascalarray*)0 };
			
			char_pascalarray *c = (char_pascalarray*)0;
			char_pascalarray_result res =
				char_pascalarray_build( len );
			LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETc, build_source2_ERR2 )
			if( c )
			{
				--len;
				c->body[ len ] = '\0';
				
				while( len )
				{
					--len;
					c->body[ len ] = name->body[ len ];
				}
				
					/* Very important! Both increment & decrement will error */
					/*  out if this isn't positive! */
				b->refs = 1;
				b->text = c;
				
				a->name = b;
				a->file = fopen( a->name->text->body, opentype );
				
			} else {
				
				if( lib4_stdmemfuncs.dealloc )
				{
					lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, b );
					lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, a );
				}
				b = (refed_pstr*)0;
				a = (source*)0;
				
				TRESPASSPATH( build_source2, "ERROR: build_source2() failed to allocate a char_pascalarray of length " );
					DECARG( len );
			}
		}
	}
	
	return( a );
}
source* build_source( char_pascalarray *name, uintmax_t inclusionpoint )
{
	return( build_source2( name, inclusionpoint, "rb" ) );
}
int discard_source( source *src )
{
	if( src == &root_src )
	{
		TRESPASSPATH( discard_source, "ERROR: discard_source() was told to deallocate static source instance \"root_src\"." );
		return( -2 );
	}
	
	if( src )
	{
		src->prev = (source*)0;
		
			/* We'll probably have other references floating around, so */
			/*  don't DIRECTLY delete... */
		int res = refed_pstr_decrrefs( src->name );
		if( res < 0 )
		{
			FAILEDINTFUNC( "refed_pstr_decrrefs", discard_source, res );
				NOTESPACE();
				DATAPTRARG( src->name );
			
			/* Just continue on. */
		}
		src->name = (refed_pstr*)0;
		
		res = fclose( src->file );
		if( res != 0 )
		{
			FAILEDINTFUNC( "fclose", discard_source, res );
				NOTESPACE();
				DATAPTRARG( src->file );
			
			/* Just continue on. */
		}
		
		if( lib4_stdmemfuncs.dealloc )
		{
			return( lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, src ) );
		}
	}
	
	TRESPASSPATH( discard_source, "ERROR: discard_source() failed to exit before reaching it's end." );
	return( -1 );
}




retframe metaC_stdinclude_body( stackpair *stkp, void *v );
	??? /* Will we have a token* on the stack? Will that need to be deallocated? */
retframe metaC_stdinclude( stackpair *stkp, void *v )
{
	int scratch;
	
	uintptr_t tok;
	
	STACKCHECK( stkp,  metaC_stdinclude );
	
	static retframe_parr
		seq =
			(retframe_parr)
			{
				3, /* Number of retframes  */
				{
						/* ( -- token* ) */
					(retframe){ &token_queue_fetch, (void*)0 },
					(retframe){ &bracketgather_entry, (void*)0 },
					(retframe){ &metaC_stdinclude_body, (void*)0 }
				}
			};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	/* ( tokengroup* retcategory -- ... ) */
retframe metaC_stdinclude_body( stackpair *stkp, void *v )
{
	int scratch;
	
	uintptr_t tok;
	
	STACKCHECK( stkp,  metaC_stdinclude_body );
	
	uintptr_t retcategory, tok;
		/* ( ... -- tokengroup* ) */
	STACKPOP_UINT( &( stkp->data ), retcategory,  metaC_stdinclude_body, scratch );
	STACKPEEK_UINT( &( stkp->data ), 0, tok,  metaC_stdinclude_body, scratch );
		/* Note: retcategory will be the stack-topper provided by bracketgather_entry()! */
	switch( retcategory )
	{
		case 1:
			/* Success! */
			
			break;
		case 0:
			/* Bad token: not a bracket opener. */
			
			TRESPASSPATH( metaC_stdinclude_body, "ERROR! Bad token, was not a bracket-starter!" );
				NOTELINE(); DATAPTRARG( tok );
				NOTESPACE(); DECARG( ( ( (token_head*)tok )->toktype ) );
			
			return( (retframe){ &token_queue_seekFreshline, (void*)0 } );
		case 2:
			/* Bad token: failed bracket accumulation. */
			
			TRESPASSPATH( metaC_stdinclude_body, "ERROR! Bad token, bracket accumulation encountered a forbidden token!" );
				NOTELINE(); DATAPTRARG( tok );
				NOTESPACE(); DECARG( ( ( (token_head*)tok )->toktype ) );
			
			return( (retframe){ &token_queue_seekFreshline, (void*)0 } );
		default:
			/* Unknown retcategory from bracketgather_entry(): this shouldn't happen. */
			
			TRESPASSPATH( metaC_stdinclude_body, "ERROR! Unfamiliar result category from bracketgather_entry()!" );
				NOTELINE(); DECARG( retcategory );
				NOTESPACE(); DATAPTRARG( tok );
				NOTESPACE(); DECARG( ( ( (token_head*)tok )->toktype ) );
			
			stack_ENDRETFRAME();
	}
	
	if( ( (token_head*)tok )->toktype != ??? )
	{
		???
	}
	tokengroup *tg = (tokengroup*)tok;
	scratch = validate_tokengroup( tg );
	if( !scratch )
	{
		???
	}
	switch( tg->used )
	{
		case 0:
			???
		case 1:
			???
		case 2:
			/* Success! */
			
			break;
		default:
			???
	}
	
	
	/* ( token* -- token* boolean ) */
	/* boolean == 1 is token is of TOKTYPE_SQSTR or TOKTYPE_DQSTR, else 0. */
retframe require_anystring( stackpair *stkp, void *v );
	/* ( char_parr* token* -- char_parr* ( 1 )|( token* 0 ) ) */
retframe grow_string( stackpair *stkp, void *v );
	/* ( string-token* -- token* char_parr* ) */
(retframe){ &stringtoken2char_parr, (void*)0 },
	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe vm_lesser( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe vm_equal( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe vm_greater( stackpair *stkp, void *v );
	/* ( uintptr_t -- uintptr_t ) */
retframe vm_not( stackpair *stkp, void *v );
retframe vm_push0( stackpair *stkp, void *v );
retframe vm_push1( stackpair *stkp, void *v );
retframe vm_push2( stackpair *stkp, void *v );
retframe vm_pushmax( stackpair *stkp, void *v );
	/* These all require a pointer to a retframe as v. */
retframe just_run( stackpair *stkp, void *v );
	/* ( uintptr_t -- uintptr_t ) */
retframe run_if( stackpair *stkp, void *v );
retframe run_else( stackpair *stkp, void *v );
retframe run_on0( stackpair *stkp, void *v );
retframe run_on1( stackpair *stkp, void *v );
retframe run_on2( stackpair *stkp, void *v );
retframe run_on3( stackpair *stkp, void *v );
	/* ( tokengroup* -- tokengroup* token* ) */
retframe vm_popfront_tokengroup( stackpair *stkp, void *v );
	/* ( tokengroup* -- tokengroup* token* ) */
retframe vm_popfrom_tokengroup( stackpair *stkp, void *v );
	/* ( tokengroup* -- tokengroup* length ) */
retframe vm_lengthof_tokengroup( stackpair *stkp, void *v );
retframe invoke_dealloctoken( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t*b -- uintptr_t*b uintptr_t*a ) */
retframe swap2nd( stackpair *stkp, void *v );
	/* These require v_ to point to a uintptr_t, which will be used as the data */
	/*  in question. */
retframe vm_pushdata( stackpair *stkp, void *v_ );
retframe vm_popdata( stackpair *stkp, void *v_ );
	/* retframe*, not uintptr_t pointer. */
retframe vm_pushretframe( stackpair *stkp, void *v_ );
	/* Pops a retframe from the data stack, and returns it. */
retframe vm_datacall( stackpair *stkp, void *v );
retframe vm_push_noop( stackpair *stkp, void *v_ );
	/* Runs enqueue_returns() with v_ upon condition match. */
	/* ( uintptr_t -- uintptr_t ) */
retframe enqueue_if( stackpair *stkp, void *v_ );
retframe enqueue_else( stackpair *stkp, void *v_ );
	/* Runs vm_pushretframe() with v_ upon condition match. */
	/* ( uintptr_t -- uintptr_t ) */
retframe vm_pushretframe_if( stackpair *stkp, void *v_ );
	/* ( uintptr_t -- uintptr_t ) */
retframe vm_pushretframe_else( stackpair *stkp, void *v_ );
	
	
	static retframe push_noop = (retframe){ &vm_push_noop, (void*)0 };
	static retframe_parr
			/* ( retframe tokengroup* cparr* token* 0 -- retframe tokengroup* cparr* 0 ) */
		canonical_growstring_from_tg_catcherr =
			{
				??? , /* Number of retframes  */
				{
						/* ( flag -- ... ) */
					(retframe){ &drop, (void*)0 },
					
					test token
						if breaking type
							report error
							deallocate token
							call swap3rd_retframe to bring retframe to top
							use vm_datacall() to run provided on-error retframe
								that SHOULD act like a longjump()
					
					
						/* ( token* --  ) */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* Ding, dong, the token's dead, the token's dead, ... */
					
						/* ( ... -- flag ) */
					(retframe){ &vm_push0, (void*)0 }
				}
			},
			/* ( cparr* tokengroup* retframe -- cparr* tokengroup* ) */
		canonical_growstring_from_tg =
			{
				11, /* Number of retframes  */
				{
						/* ( cparr* tokengroup* retframe -- retframe cparr* tokengroup* ) */
					(retframe){ &swap3rd_retframe, (void*)0 },
					
						/* ( cparr* tokengroup* -- cparr* tokengroup* token* ) */
					(retframe){ &vm_popfront_tokengroup, (void*)0 },
						/* ( ... -- tg* tok* cparr* ) */
					(retframe){ &raise3rd, (void*)0 },
						/* ( ... -- tg* cparr* tok* ) */
					(retframe){ &swap2nd, (void*)0 },
						/* ( char_parr* token* -- char_parr* ( 1 )|( token* 0 ) ) */
					(retframe){ &grow_string, (void*)0 },
							/* Catch the syntax error. */
						(retframe){ &enqueue_else, (void*)&canonical_growstring_from_tg_catcherr },
						/* Don't care about the flag anymore. */
					(retframe){ &drop, (void*)0 },
					
						/* ( retframe cparr* tg* -- 1/2retframe tg* cparr* 1/2retframe ) */
					(retframe){ &swap3rd, (void*)0 },
					(retframe){ &drop, (void*)0 },
						/* ( 1/2retframe tg* cparr* -- cparr* tg* 1/2retframe ) */
					(retframe){ &swap3rd, (void*)0 },
					(retframe){ &drop, (void*)0 }
				}
			};
	static retframe
		not_retframe = (retframe){ &vm_not, (void*)0 },
		push0_retframe = (retframe){ &vm_push0, (void*)0 },
		push1_retframe = (retframe){ &vm_push1, (void*)0 },
		push_firsthook_onelems_loop,
		firsthook_catcherr = (retframe){ &enqueue_returns, (void*)&firsthook_catcherr_ };
	static retframe_parr
			/* ( tokengroup* cparr* tokengroup* flag -- tokengroup* cparr* tokengroup* flag ) */
		firsthook_onelems_loop_ =
			{
			9, /* Number of retframes  */
				{
						/* ( flag -- ... ) */
					(retframe){ &drop, (void*)0 },
					
					/* Accumulate string. */
						/* The provided retframe should approximately be longjump(). */
					(retframe){ &vm_pushretframe, (void*)(retframe*)& ??? }, },
						/* ( cparr* tokengroup* retframe -- cparr* tokengroup* ) */
					(retframe){ &enqueue_returns, (void*)&canonical_growstring_from_tg },
					
					/* Test and dispatch for looping. We want to be a */
					/*  little fussier than normal with timing. */
						(retframe){ &vm_lengthof_tokengroup, (void*)0 },
						(retframe){ &vm_not, (void*)0 },
							/* Will restore the value that got us into */
							/*  this loop, and exit. */
						(retframe){ &vm_pushretframe_if, (void*)&not_retframe },
							/* Continue looping. */
						(retframe){ &vm_pushretframe_else, (void*)&push_firsthook_onelems_loop },
						/* Pops a retframe from the data stack, and returns it. */
					(retframe){ &vm_datacall, (void*)0 },
					
						/* ( ... -- flag ) */
					(retframe){ &vm_push0, (void*)0 }
				}
			},
			/* ( tokengroup* tokengroup* flag -- tokengroup* cparr* flag ) */
		firsthook_onelems_ =
			{
				13, /* Number of retframes  */
				{
						/* ( flag -- ... ) */
					(retframe){ &drop, (void*)0 },
					
						/* ( tokengroup* -- tokengroup* token* ) */
					(retframe){ &vm_popfront_tokengroup, (void*)0 },
						/* ( string-token* -- token* char_parr* ) */
					(retframe){ &stringtoken2char_parr, (void*)0 },
						/* ( tg* tok* cparr* -- tg* cparr* tok* ) */
					(retframe){ &swap2nd, (void*)0 },
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* ( tg* cparr* -- cparr* tg* ) */
					(retframe){ &swap2nd, (void*)0 },
					
						(retframe){ &vm_lengthof_tokengroup, (void*)0 },
						(retframe){ &vm_not, (void*)0 },
						(retframe){ &run_if, (void*)&noop_retframe },
							/* ( tokengroup* cparr* tokengroup* flag -- ... ) */
						(retframe){ &enqueue_else, (void*)&firsthook_onelems_loop_ },
							/* ( flag --  ) */
						(retframe){ &drop, (void*)0 },
					
						/* ( cparr* tg* -- cparr* ) */
					(retframe){ &invoke_dealloctoken, (void*)0 },
					
						/* ( ... -- flag ) */
					(retframe){ &vm_push0, (void*)0 }
				}
			},
			/* ( tokengroup* tokengroup* flag -- ... flag ) */
		firsthook_onelemless_ =
			{
				??? , /* Number of retframes  */
				{
						/* ( flag -- ... ) */
					(retframe){ &drop, (void*)0 },
					
						/* ( tokengroup* -- tokengroup* token* ) */
					(retframe){ &vm_popfront_tokengroup, (void*)0 },
					(retframe){ &swap2nd, (void*)0 },
						/* ( ... -- tok* tg* ) */
					
						/* ( ... -- flag ) */
					(retframe){ &vm_push1, (void*)0 }
				}
			},;
	static retframe
		firsthook_onelems = (retframe){ &enqueue_returns, (void*)&firsthook_onelems_ },
		firsthook_onelemless = (retframe){ &enqueue_returns, (void*)&firsthook_onelemless_ };
	static retframe_parr
		firsthook =
			(retframe_parr)
			{
				??? , /* Number of retframes  */
				{
					(retframe){ &vm_lengthof_tokengroup, (void*)0 },
					(retframe){ &vm_not, (void*)0 },
					(retframe){ &run_if, (void*)&firsthook_onelemless },
					(retframe){ &run_else, (void*)&firsthook_onelems },
					(retframe){ &drop, (void*)0 }
				}
			};
	
	
	/* Horay, we have a proper bracket set, and have clear to the end */
	/*  of the line! Now we check the number of arguments (should be */
	/*  2), confirm that both are strings (which may involve merging */
	/*  them), and do some search+setup to include the designated */
	/*  file. */
	/* The arguments to this directive: */
		/* 1) Root name: the program-argument assigned name of the */
		/*  point in the directory that the next directive-argument */
		/*  will be defined in relation to: this will currently be */
		/*  ignored, but it'll matter later. */
		/* 2) Inclusion path: the directory path and file name to */
		/*  include. This MUST be converted to "cannonical form" (so */
		/*  no ".." or "." components in it) BEFORE being combined */
		/*  with the root. The named-root directory + the cannonical */
		/*  form of this path specify the file to be included. */
	
	???
	
	static int hasrun = 0;
	if( !hasrun )
	{
		push_firsthook_onelems_loop = (retframe){ &enqueue_returns, (void*)&firsthook_onelems_loop_ };
		
		hasrun = 1;
	}
	
	static retframe_parr
		seq =
			(retframe_parr)
			{
				??? , /* Number of retframes  */
				{
						/* ( tokengroup* -- tokengroup* token* ) */
					(retframe){ &vm_popfrom_tokengroup, (void*)0 },
					(retframe){ &swap2nd, (void*)0 },
						/* ( ... -- tok* tg* ) */
						/* Need to verify that tok* is a tokengroup? */
					
						/* ( tokengroup* -- tokengroup* token* ) */
					(retframe){ &vm_popfrom_tokengroup, (void*)0 },
					(retframe){ &swap2nd, (void*)0 },
						/* ( ... -- tok* tok* tg* ) */
						/* Need to verify that tok* is a tokengroup? */
					
						/* ( tokengroup* -- ) */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* ( ... -- tg*2nd tg*1st ) */
					
					???
				}
			};
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	NOTELINE()
	NOTESPACE()

	DECARG( val )
	CHARARG( val )
	DATAPTRARG( val )
	STRARG( strptr )
	
	RETFRAMEFUNC( caller, scratch );



char_result charin( refed_pstr **refresh_srcname, uintmax_t *prog )
{
	char val;
	
	if( refresh_srcname && !prog )
	{
		LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_DOMAIN );
	}
	
	if( !inbuf.used )
	{
		int res = fgetc( sources->file );
		while( res == EOF && sources )
		{
				/* Delink. */
			source *tmp = sources;
			sources = sources->prev;
			
			tmp->prev = (source*)0;
			res = discard_source( tmp );
			if( !res )
			{
				FAILEDINTFUNC( "discard_source", charin, res );
					NOTESPACE();
					DATAPTRARG( tmp );
				
				if( refresh_srcname && *refresh_srcname )
				{
					res = refed_pstr_incrrefs( *refresh_srcname );
					if( res < 0 )
					{
						FAILEDINTFUNC( "refed_pstr_incrrefs", charin, res );
							NOTESPACE();
							DATAPTRARG( *refresh_srcname );
					}
				}
				
				LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
			}
			
			if( refresh_srcname )
			{
				*refresh_srcname = sources->name;
				prog = sources->base + sources->progress;
			}
			res = fgetc( sources->file );
		}
		if( res == EOF )
		{
				/* Failure. */
			FAILEDINTFUNC( "fgetc", charin, res );
				NOTESPACE();
					/* This should be accurate REGARDLESS of which fgetc() */
					/*  call we're dealing with. */
				DATAPTRARG( sources->file );
			
			if( refresh_srcname && *refresh_srcname )
			{
				res = refed_pstr_incrrefs( *refresh_srcname );
				if( res < 0 )
				{
					FAILEDINTFUNC( "refed_pstr_incrrefs", charin, res );
						NOTESPACE();
						DATAPTRARG( *refresh_srcname );
				}
			}
			
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
		++( sources->progress );
		val = res;
		
	} else {
		
		--( inbuf.used );
		val = inbuf.pushedback->body[ inbuf.used ];
	}
	
	if( refresh_srcname && *refresh_srcname )
	{
		res = refed_pstr_incrrefs( *refresh_srcname );
		if( res < 0 )
		{
			FAILEDINTFUNC( "refed_pstr_incrrefs", charin, res );
				NOTESPACE();
				DATAPTRARG( *refresh_srcname );
			
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_??? );
		}
	}
	
		/* Success. */
	LIB4_CHARRESULT_RETURNSUCCESS( val );
}
char_result charpeek( int *isEOF )
{
	char val;
	
	if( isEOF )
	{
		*isEOF = 0;
	}
	
	if( !inbuf.used )
	{
		int res = fgetc( sources->file );
		if( res == EOF )
		{
			if( isEOF )
			{
				*isEOF = 1;
			}
			
				/* Failure. */
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_EOF );
		}
		
		++( sources->progress );
		val = res;
		
		res = charback( val );
		if( res != 1 )
		{
			FAILEDINTFUNC( "charback", charpeek, res );
				NOTESPACE();
				CHARARG( val );
			
			LIB4_CHARRESULT_RETURNFAILURE( LIB4_RESULT_FAILURE_UNDIFFERENTIATED );
		}
		
	} else {
		
		val = inbuf.pushedback->body[ inbuf.used - 1 ];
	}
	
		/* Success. */
	LIB4_CHARRESULT_RETURNSUCCESS( val );
}
int char_dropeof( refed_pstr **refresh_srcname, uintmax_t *prog )
{
	if( refresh_srcname && !prog )
	{
		return( -2 );
	}
	if( inbuf.used )
	{
		return( -3 );
	}
	
	char_result res = charpeek( (int*)0 );
	if( res == LIB4_CHARRESULT_BUILDFAILURE( LIB4_RESULT_FAILURE_EOF ) )
	{
		/* Eof found! Rembember to only drop 1 level. */
		
		source *tmp = sources;
		sources = sources->prev;
		
		tmp->prev = (source*)0;
		int res2 = discard_source( tmp );
		if( !res2 )
		{
			FAILEDINTFUNC( "discard_source", char_dropeof, res2 );
				NOTESPACE();
				DATAPTRARG( tmp );
			
			if( refresh_srcname && *refresh_srcname )
			{
				res2 = refed_pstr_incrrefs( *refresh_srcname );
				if( res2 < 0 )
				{
					FAILEDINTFUNC( "refed_pstr_incrrefs", char_dropeof, res2 );
						NOTESPACE();
						DATAPTRARG( *refresh_srcname );
				}
			}
			
			return( -4 );
		}
	}
	
	if( refresh_srcname && *refresh_srcname )
	{
			/* Don't forget: we need to track name reference counts! */
		*refresh_srcname = sources->name;
		prog = sources->base + sources->progress;
		res = refed_pstr_incrrefs( *refresh_srcname );
		if( res < 0 )
		{
			FAILEDINTFUNC( "refed_pstr_incrrefs", char_dropeof, res );
				NOTESPACE();
				DATAPTRARG( *refresh_srcname );
			
			return( -5 );
		}
	}
	
	return( 1 );
}
int charback( char val )
{
	if( inbuf.used >= inbuf.pushedback->len )
	{
		char_pascalarray_result res =
			char_pascalarray_rebuild( inbuf.pushedback, inbuf.used * 2 );
#define charback_SUCCESS( arr ) inbuf.pushedback = ( arr );
#define charback_FAILURE( err ) \
	MONADICFAILURE( charback, "char_pascalarray_rebuild()", ( err ) ); \
	return( err );
		LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, charback_SUCCESS, charback_FAILURE )
	}
	
	inbuf.pushedback->body[ inbuf.used ] = val;
	++inbuf.used;
	
	return( 1 );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
