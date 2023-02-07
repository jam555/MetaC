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

	/* For token_queue_seekFreshline(), as used by */
	/*  metaC_stdinclude_entry(). */
#include "../input/lexalike.h"



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

	#define SIGNEDARG( val ) \
		STDMSG_SIGNEDARG_WRAPPER( &errs, ( val ) )
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
					/* We're done, everything below this is */
					/*  dedicated to deallocation. */
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
			res = lib4_stdmemfuncs.dealloc( lib4_stdmemfuncs.data, pstr );
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
		/* I'm not sure what base is for. Maybe it was in case of a */
		/*  later inclusion version that allows starting part way */
		/*  through a fire? */
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




static uintptr_t metaC_stdinclude_jumpref = 0;

retframe metaC_stdinclude_stringify( stackpair *stkp, void *v );
retframe metaC_stdinclude_gatherhandler( stackpair *stkp, void *v );
retframe metaC_stdinclude_body( stackpair *stkp, void *v );
	/* ( token*directiveName -- token* ) */
retframe metaC_stdinclude_entry( stackpair *stkp, void *v );
	/* (  --  ) */
retframe metaC_stdinclude_conclude( stackpair *stkp, void *v );
	/* ( token*directiveName --  ) */
retframe metaC_stdinclude( stackpair *stkp, void *v )
{
	static const retframe_parr
		onjump_ =
			(retframe_parr)
			{
				2,
				{
					/*
						(
							( ( depth==0: token*0 ) | ( depth==1: string*1 ) )
							tokengroup* ( x==0: | x==2: char_parr* token* ) !!depth x
						)
					*/
					
					??? /* We HAVE TO do more than just drop this. */
						/* ( depth-flag -- ) */
					(retframe){ &drop, (void*)0 },
						/* ( tokengroup* -- ) */
					(retframe){ &invoke_dealloctoken, (void*)0 }
				}
			};
		/* It's worth noting that neither onset{} nor onjump{} will be */
		/*  exposed to the bookmark values used. Instead, */
		/*  LOCALIZE_SETJUMP() keeps those values nicely contained inside */
		/*  the scripts that it generates, which is much nicer than what */
		/*  divertthread currently does. The greatest complication comes */
		/*  in that there is no good way to handle things like destructors, */
		/*  deinitializers, nor other such constructs within this system. */
	static const retframe
		onset = (retframe){ &metaC_stdinclude_entry, (void*)0 },
		onjump = (retframe){ &enqueue_returns, (void*)&onjump_ };
	
	LOCALIZE_SETJUMP( metaC_stdinclude_jumpref, metaC_stdinclude_internal, seq1,  &onset, &onjump );
	
	static const retframe_parr
		seq2 =
			(retframe_parr)
			{
				element count,
				{
					(retframe){ &enqueue_returns, (void*)&seq1 },
					(retframe){ &metaC_stdinclude_conclude, (void*)0 }
				}
			}
	
	if( metaC_stdinclude_jumpref == 0 )
	{
		/* We've seemingly recursed, so error out. */
		
		BADNONULL( metaC_stdinclude, &metaC_stdinclude_jumpref );
			NOTELINE();
			STRARG( "Non-null metaC_stdinclude_jumpref, recursion suspected." );
		
		stack_ENDRETFRAME();
	}
	
	return( (retframe){ &enqueue_returns, (void*)&seq2 } );
}
		/* ( token*directiveName -- ->metaC_stdinclude_gatherhandler() ) */
	retframe metaC_stdinclude_entry( stackpair *stkp, void *v )
	{
		int scratch;

		uintptr_t tok_;

		STACKCHECK( stkp,  metaC_stdinclude_entry );

		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, tok_,  metaC_stdinclude_entry, scratch );
		token *tok = (token*)tok_;
		if( tok->header.toktype != TOKTYPE_NAME )
		{
			TRESPASSPATH( metaC_stdinclude_entry, " ERROR: toktype wasn't TOKTYPE_NAME : " );
				DECARG( tok->header.toktype );

			stack_ENDRETFRAME();
		}
		static char includetext[] = "_include";
			/* sizeof( char ) should hopefully always == 1. */
		static int includetextsize = sizeof( includetext ) / sizeof( char );
		if( tok->header.length < includetextsize )
		{
			TRESPASSPATH( metaC_stdinclude_entry, " ERROR: token name too short: " );
				DECARG( tok->header.length );
				STRARG( " < " );
				DECARG( includetextsize );

			stack_ENDRETFRAME();
		}
		if
		(
			memcmp
			(
				(const void*)includetext,
				(const void*)tok->text,
				tok->header.length > includetextsize ?
					includetextsize :
					tok->header.length
			) != 0
		)
		{
			TRESPASSPATH( metaC_stdinclude_entry, " ERROR: wrong token name, should be: " );
				STRARG( includetext );
				STRARG( "  was: " );
				STRARG( tok->text );

			stack_ENDRETFRAME();
		}

		static retframe_parr
			seq =
				(retframe_parr)
				{
					5, /* Number of retframes  */
					{
							/* ( token*directiveName -- ) */
						(retframe){ &invoke_dealloctoken, (void*)0 },
							/* ( -- token* ) */
						(retframe){ &token_queue_fetch, (void*)0 },
							/* ( token* -- ( token* 0 ) | ( tokengroup* 1 ) | ( tokengroup* 2 ) ) */
						(retframe){ &bracketgather_entry, (void*)0 },
							/* Fetches and discards tokens with token_queue_fetch() and */
							/*  invoke_dealloctoken() until it finds a fresh line, then returns that */
							/*  final token to be returned by token_queue_fetch() once again. */
							/* (  --  ) */
							/* This is the earliest point where we can go looking for the freshline, */
							/*  because anywhere earlier will either obliterate any correctly formed */
							/*  brackets, or result in tokens to be discarded instead being */
							/*  perpetuated. */
						(retframe){ &token_queue_seekFreshline, (void*)0 },
						(retframe){ &metaC_stdinclude_gatherhandler, (void*)0 }
					}
				};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
		/* ( ( token* 0 ) | ( tokengroup* 1 ) | ( tokengroup* 2 ) -- ??? ) */
	retframe metaC_stdinclude_gatherhandler( stackpair *stkp, void *v )
	{
		int scratch;

		uintptr_t tok;

		STACKCHECK( stkp,  metaC_stdinclude_gatherhandler );

		STACKPOP_UINT( &( stkp->data ), tok,  metaC_stdinclude_gatherhandler, scratch );
		switch( tok )
		{
			case 0:
				{
					STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, tok,  metaC_stdinclude_gatherhandler, scratch );

	#define metaC_stdinclude_gatherhandler_BADRET( err ) \
		MONADICFAILURE( metaC_stdinclude_gatherhandler, "get_deeptoktype", ( err ) ); \
		stack_ENDRETFRAME();
					deep_toktype a;
					deeptoktype_result deepres = get_deeptoktype( &( tok->header ) );
					DEEPTOKTYPE_RESULT_BODYMATCH( deepres, LIB4_OP_SETa, metaC_stdinclude_gatherhandler_BADRET );
					TRESPASSPATH( metaC_stdinclude_gatherhandler, " ERROR: expected tokengroup, shallow type: " );
						DECARG( a.shallow_type );
						STRARG( "; virtual type: " );
						DECARG( a.virtual_type );
					
						??? /* This should probably be replaced with a return */
						/*  straight to invoke_dealloctoken(). */
					RETFRAMEFUNC( metaC_stdinclude_gatherhandler, scratch );
				}
			case 1:
				/* 1 if correctly formed, or */
				break;
			case 2:
				/* 2 if explicitly bad syntax ( e.g. pairing an opening parenthese */
				/*  with a closing square bracket). */
				TRESPASSPATH( metaC_stdinclude_gatherhandler, " ERROR: Bad syntax: " );

					/* We should reprint the entire tokengroup here. */
				???
					/* And then we should discard it. */

				RETFRAMEFUNC( metaC_stdinclude_gatherhandler, scratch );
			default:
				TRESPASSPATH( metaC_stdinclude_gatherhandler, " ERROR: unexpected result value: " );
					DECARG( tok );

				stack_ENDRETFRAME();
		}

		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, tok,  metaC_stdinclude_gatherhandler, scratch );
		tokengroup *tg = (tokengroup*)tok;

		scratch = validate_tokengroup( tg );
		switch( scratch )
		{
			case 1:
				/* Good. */
				break;
			case -2:
				/* Wrong type. */
				TRESPASSPATH( metaC_stdinclude_gatherhandler, " ERROR: expected tokengroup, type: " );
					DECARG( tg->header.toktype );

				stack_ENDRETFRAME();
			case -1:
				/* Fatal error. */
				TRESPASSPATH(
					metaC_stdinclude_gatherhandler,
					" ERROR: validate_tokengroup() given null pointer, this should be impossible."
				);
				
				stack_ENDRETFRAME();
			default:
				/* Unknown. */
				TRESPASSPATH( metaC_stdinclude_gatherhandler, " ERROR: validate_tokengroup() general error: " );
					SIGNEDARG( scratch );

				stack_ENDRETFRAME();
		}
		if( tg->arr->len != 2 )
		{
			TRESPASSPATH( metaC_stdinclude_gatherhandler, " ERROR: length != 2 in tokengroup( " );
				DATAPTRARG( tg );
				STRARG( " )->len == " );
				DECARG( tg->arr->len );

			stack_ENDRETFRAME();
		}

		token_head *a = (token_head*)0, *b = (token_head*)0;
		tokenheadptr_result thpres = popfrom_tokengroup( tg );
	#define metaC_stdinclude_gatherhandler_TGPOP_FAIL( err ) \
			TRESPASSPATH( metaC_stdinclude_gatherhandler, \
				" ERROR: failure in a call to popfrom_tokengroup(), group: " ); \
				DATAPTRARG( tg );
		TOKENHEADPTR_RESULT_BODYMATCH( thpres, LIB4_OP_SETa, metaC_stdinclude_gatherhandler_TGPOP_FAIL );

		thpres = popfrom_tokengroup( tg );
		TOKENHEADPTR_RESULT_BODYMATCH( thpres, LIB4_OP_SETb, metaC_stdinclude_gatherhandler_TGPOP_FAIL );

		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)a,  metaC_stdinclude_gatherhandler, scratch );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)b,  metaC_stdinclude_gatherhandler, scratch );

		static retframe_parr
			seq =
				(retframe_parr)
				{
					6, /* Number of retframes  */
					{
							/* ( tokengroup*orig token*0 token*1 -- token*0 token*1 tokengroup*orig ) */
						(retframe){ &raise3rd, (void*)0 },
							/* ( tokengroup*orig -- ) */
						(retframe){ &invoke_dealloctoken, (void*)0 },
							/* ( token*1 -- string*1 ) */
						(retframe){ &metaC_stdinclude_stringify, (void*)0 },
							/* ( token*0 string*1 -- string*1 token*0 ) */
						(retframe){ &swap2nd, (void*)0 },
							/* ( token*0 -- string*0 ) */
						(retframe){ &metaC_stdinclude_stringify, (void*)1 },
						(retframe){ &metaC_stdinclude_body, (void*)0 }
					}
				};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
		/* ( string*1 string*0 -- ... --  ) */
	retframe metaC_stdinclude_body( stackpair *stkp, void *v )
	{
		int scratch;

		uintptr_t tok;

		STACKCHECK( stkp,  metaC_stdinclude_body );

		uintptr_t rootname, relapath;

		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, rootname,  metaC_stdinclude_body, scratch );
		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 1, relapath,  metaC_stdinclude_body, scratch );

		/* We should be "canonicalizing" the path referenced by */
		/*  str1, and then confirming that it's relative (which is */
		/*  mandatory). */

		/* We actually don't USE string 0, because it's a reference */
		/*  into a list of reference directories, and that list */
		/*  doesn't exist yet. We would do the lookup here. */

		/* This is the right place to merge the strings. */

			/* We currently are using the relative path directly, */
			/*  just because we don't currently have alternatives. */
		source *inclusion = build_source( (char_pascalarray*)relapath, 0 );
		inclusion->prev = sources;
		sources = inclusion;

			/* Will need another string deallocation once this thing */
			/*  is fleshed out, but the merged string doesn't yet exist. */
		static retframe_parr
			seq =
				(retframe_parr)
				{
					2, /* Number of retframes  */
					{
							/* ( relapath-string rootname-string -- rootname-string ) */
						(retframe){ &dealloc_cparr, (void*)0 },
							/* ( relapath-string --  ) */
						(retframe){ &dealloc_cparr, (void*)0 }
					}
				};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
		/* ( tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) -- ) */
	retframe metaC_stdinclude_stringify_conclude( stackpair *stkp, void *v );
		/* ( ( token*0 token*1 v_==0 ) | ( string*1 token*0 v_==1 ) -> metaC_stdinclude_body() ) */
	retframe metaC_stdinclude_stringify( stackpair *stkp, void *v_ )
	{
		int scratch;

		uintptr_t tok;
		
		STACKCHECK( stkp,  metaC_stdinclude_stringify );
		
		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, tok,  metaC_stdinclude_stringify, scratch );
		switch( ( (token_head*)tok )->toktype )
		{
			case ???:
		}

		???
		
		/* This is the complex route. */
		
		static retframe_parr
			seq =
				(retframe_parr)
				{
					2, /* Number of retframes  */
					{
							/* ( tokengroup* -- tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) ) ) */
						(retframe){ &convert_tokengroup2string, (void*)0 },
							/* ( tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) ) -- ) */
						(retframe){ &metaC_stdinclude_stringify_conclude, (void*)0 }
					}
				};
		seq.arr[ 1 ].data = v_;
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
		/*
			(
					( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
					tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) )
				--
					( ( ( v_==0: token*0 ) | ( v_==1: string*1 ) ) char_parr* ) | error path execution
			)
		*/
	retframe metaC_stdinclude_stringify_conclude( stackpair *stkp, void *v_ )
	{
		int scratch;

		uintptr_t res, depth;

		STACKCHECK( stkp,  metaC_stdinclude_stringify_conclude );
		depth = (uintptr_t)v_;

		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, res,  metaC_stdinclude_stringify_conclude, scratch );
			/* ( tokengroup* ( 0 | char_parr* ( 1 | token* 2 ) -- tokengroup* ( char_parr* ( token* ) ) */
		/* The result code on top of the stack will be: */
			/* 0 if nothing was done, */
			/* 1 on plain success, or */
			/* 2 on mid-process error. */
		switch( res )
		{
			case 1:
				/* Good case. */
				break;

			case 2:
				/* Mid-work error. */
				TRESPASSPATH( metaC_stdinclude_stringify_conclude, " ERROR: mid-work error in  convert_tokengroup2string(). " );
					STRARG( "Top type: " );
						/* Discard the result, we'll manually restore it later. */
					STACKPOP_UINT( &( stkp->data ), res,  metaC_stdinclude_stringify_conclude, scratch );
					STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 1, res,  metaC_stdinclude_stringify_conclude, scratch );
					DECARG( ( (token_head*)res )->toktype );
					STRARG( "; Bottom type: " );
					STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 4, res,  metaC_stdinclude_stringify_conclude, scratch );
					DECARG( ( (token_head*)res )->toktype );
				static retframe_parr
					seq0 =
						(retframe_parr)
						{
							3, /* Number of retframes  */
							{
								/*
									(
										( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
										tokengroup* char_parr* token*
									)
								*/
								
									/* This will get replaced. */
								(retframe){ &vm_push_placeholder, (void*)0 },
									/* ( -- 2 ) */
								(retframe){ &vm_push2, (void*)0 },
									/* Requires a pointer to a retframe as v. */
								(retframe){ &longjump_callstack, (void*)&metaC_stdinclude_jumpref }
									/*
										(
											( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
											tokengroup* char_parr* token* !!v_ 2
										)
									*/
							}
						};
				seq0.arr[ 0 ].hand = ( depth ? &vm_push1 : &vm_push0 );
				return( (retframe){ &enqueue_returns, (void*)&seq0 } );
			case 0:
				/* No work done. */
				TRESPASSPATH( metaC_stdinclude_stringify_conclude, " ERROR: convert_tokengroup2string(), did no work, Type: " );
						/* We'll restore this later. */
					STACKPOP_UINT( &( stkp->data ), res,  metaC_stdinclude_stringify_conclude, scratch );
					STACKPEEK_UINT( &( stkp->data ), 0, res,  metaC_stdinclude_stringify_conclude, scratch );
					DECARG( ( (token_head*)res )->toktype );
				static retframe_parr
					seq0 =
						(retframe_parr)
						{
							3, /* Number of retframes  */
							{
								/*
									(
										( ( v_==0: token*0 ) | ( v_==1: string*1 ) ) tokengroup*
									)
								*/
								
									/* This will get replaced. */
								(retframe){ &vm_push_placeholder, (void*)0 },
									/* ( -- 0 ) */
								(retframe){ &vm_push0, (void*)0 },
									/* Requires a pointer to a retframe as v. */
								(retframe){ &longjump_callstack, (void*)&metaC_stdinclude_jumpref }
									/*
										(
											( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
											tokengroup* !!v_ 0
										)
									*/
							}
						};
				seq0.arr[ 0 ].hand = ( depth ? &vm_push1 : &vm_push0 );
				return( (retframe){ &enqueue_returns, (void*)&seq0 } );
			default:
				/* Unknown error. */
				TRESPASSPATH( metaC_stdinclude_stringify_conclude, " ERROR: unknown result from convert_tokengroup2string(): " );
					DECARG( res );
					STRARG( " Type: " );
					STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 1, res,  metaC_stdinclude_stringify_conclude, scratch );
					DECARG( ( (token_head*)res )->toktype );
				stack_ENDRETFRAME();
		}
		
		
		static retframe_parr
			seq =
					/*
						(
								( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
								tokengroup* char_parr* 1
							--
								char_parr*
						)
					*/
				(retframe_parr)
				{
					3, /* Number of retframes  */
					{
						/*
							(
								( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
								tokengroup* char_parr* 1
							)
						*/
						
							/* ( 1 -- ) */
						(retframe){ &drop, (void*)0 },
							/* ( tokengroup* char_parr* -- char_parr* tokengroup* ) */
						(retframe){ &swap2nd, (void*)0 },
							/* ( char_parr* tokengroup* -- char_parr* ) */
						(retframe){ &invoke_dealloctoken, (void*)0 }
							/* ( ( ( v_==0: token*0 ) | ( v_==1: string*1 ) ) char_parr* ) */
					}
				};
		return( (retframe){ &enqueue_returns, (void*)&seq } );
	}
	/* (  --  ) */
retframe metaC_stdinclude_conclude( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  metaC_stdinclude_conclude );
	
		/* Even minutiae can be important. */
	metaC_stdinclude_jumpref = 0;
	
	/* By this point, the line we started reading should be fully cleared out. */
	
	RETFRAMEFUNC( metaC_stdinclude_conclude, scratch );
}



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
