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

#include "includedirective.h"

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



static uintptr_t metaC_stdinclude_jumpref = 0;

retframe metaC_stdinclude_stringify( stackpair *stkp, void *v );
retframe metaC_stdinclude_gatherhandler( stackpair *stkp, void *v );
retframe metaC_stdinclude_body( stackpair *stkp, void *v );
	/* ( token*directiveName -- token* ) */
retframe metaC_stdinclude_entry( stackpair *stkp, void *v );
	/* (  --  ) */
retframe metaC_stdinclude_conclude( stackpair *stkp, void *v );
	/*
		(
			( ( depth==0: token*0 ) | ( depth==1: string*1 ) )
			( src!=1: tokengroup* | src==1: token* )
			( src==0: | src==1: &stringtoken2char_parr | src==2: char_parr* token* )
			!!depth src
		)
	*/
	/* The value of src should always match the number of elements that it */
	/*  indicates the presence of. */
retframe metaC_stdinclude_onerr( stackpair *stkp, void *v )
{
	int scratch;
	
	STACKCHECK( stkp,  metaC_stdinclude_entry );
	
	uintptr_t src, booldepth, tmp;
	
	STACKPOP_UINT( &( stkp->data ), src,  metaC_stdinclude_entry, scratch );
	STACKPOP_UINT( &( stkp->data ), booldepth,  metaC_stdinclude_entry, scratch );
	
	static const retframe_parr
		on_srcEQ2 =
				/*
					(
						... ( src==2: char_parr* token* )
					)
				*/
			(retframe_parr)
			{
				2,
				{
						/* ( token* -- ) */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* ( char_parr* --  ) */
					(retframe){ &dealloc_cparr, (void*)0 }
				}
			};
	
	#define metaC_stdinclude_onerr_SRCGATE( val ) \
		seq.arr[ 0 ].handler = ( ( (val) == 2 ) ? &enqueue_returns : ( ( (val) == 1 ) ? &drop : &noop ) )
	#define metaC_stdinclude_onerr_DEPTHGATE( val ) \
		seq.arr[ 2 ].handler = ( ( (val) == 0 ) ? &invoke_dealloctoken : &dealloc_cparr )
				/*
					(
						( ( booldepth==0: token*0 ) | ( booldepth==1: string*1 ) )
						( src!=1: tokengroup* | src==1: token* )
						( src==0: | src==1: &stringtoken2char_parr | src==2: char_parr* token* )
					)
				*/
	static const retframe_parr
		seq =
			(retframe_parr)
			{
				3,
				{
						/* ( src==0: | src==1: &stringtoken2char_parr | src==2: char_parr* token* ) */
					(retframe){ &vm_placeholder, (void*)&on_srcEQ2 },
						/* ( src!=1: tokengroup* | src==1: token* ) */
					(retframe){ &invoke_dealloctoken, (void*)0 },
						/* ( ( booldepth==0: token*0 ) | ( booldepth==1: string*1 ) ) */
					(retframe){ &vm_placeholder, (void*)0 }
				}
			};
	
	
	
	TRESPASSPATH( metaC_stdinclude_onerr, " ERROR: metaC_stdinclude() failure." );
	
	if( src == 2 )
	{
		/*
			(
				( ( booldepth==0: token*0 ) | ( booldepth==1: string*1 ) )
				tokengroup*
		->		src==2: char_parr* token*		<-
			)
		*/
		
		NOTELINE();
			STRARG( " Partial string: " );
			STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 1, tmp,  metaC_stdinclude_entry, scratch );
			STRARG( ( (char_parr*)tmp )->body );
		
		NOTELINE();
			STRARG( " Unconverted token type: " );
			STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, tmp,  metaC_stdinclude_entry, scratch );
			DECARG( ( (token_head*)tmp )->toktype );
			STRARG( " Address: 0x" );
			HEXARG( ( (token_head*)tmp ) );
		
			/* Through a nice accident, src always matches the number of elements */
			/*  that it accompanies, so we can use it arithmatically, instead of */
			/*  just logically. Not planned, but I'm not above using it. */
		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * src, tmp,  metaC_stdinclude_entry, scratch );
		
	} else if( src == 1 )
	{
		/*
			(
				( ( booldepth==0: token*0 ) | ( booldepth==1: string*1 ) )
				token*
		->		src==1: &stringtoken2char_parr		<-
			)
		*/
		
		NOTELINE();
			STRARG( " stringtoken2char_parr() failure. Token type: " );
			STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 1, tmp,  metaC_stdinclude_entry, scratch );
			DECARG( ( (token_head*)tmp )->toktype );
		
	} else {
		/*
			(
				( ( booldepth==0: token*0 ) | ( booldepth==1: string*1 ) )
				tokengroup*
		->		src==0:		<-
			)
		*/
		
		NOTELINE();
			STRARG( " No action taken." );
		
		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * src, tmp,  metaC_stdinclude_entry, scratch );
	}
	
	
	/*
		(
			( ( booldepth==0: token*0 ) | ( booldepth==1: string*1 ) )
	->		( src!=1: tokengroup* | src==1: token* )													<-
			( src==0: | src==1: &stringtoken2char_parr | src==2: char_parr* token* )
		)
	*/
	
	NOTELINE();
		STRARG( " Token pointer: " );
		HEXARG( ( (token_head*)tmp ) );
	
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * ( 1 + src ), tmp,  metaC_stdinclude_entry, scratch );
	if( booldepth )
	{
		NOTELINE();
			STRARG( " Untouched token type: " );
			DECARG( ( (token_head*)tmp )->toktype );
			STRARG( " Address: 0x" );
			HEXARG( ( (token_head*)tmp ) );
		
	} else {
		
		NOTELINE();
			STRARG( " Converted string: " );
			STRARG( ( (char_parr*)tmp )->body );
	}
	
	
	
	metaC_stdinclude_onerr_SRCGATE( src );
	metaC_stdinclude_onerr_DEPTHGATE( booldepth );
	
	return( (retframe){ &enqueue_returns, (void*)&seq } );
}
	/* ( token*directiveName --  ) */
retframe metaC_stdinclude( stackpair *stkp, void *v )
{
		/* It's worth noting that neither onset{} nor onjump{} will be */
		/*  exposed to the bookmark values used. Instead, */
		/*  LOCALIZE_SETJUMP() keeps those values nicely contained inside */
		/*  the scripts that it generates, which is much nicer than what */
		/*  divertthread currently does. The greatest complication comes */
		/*  in that there is no good way to handle things like destructors, */
		/*  deinitializers, nor other such constructs within this system. */
	static const retframe
		onset = (retframe){ &metaC_stdinclude_entry, (void*)0 },
		onjump = (retframe){ &metaC_stdinclude_onerr, (void*)0 };
	
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
				
					/* And then we should discard the tokengroup, which we */
					/*  currently don't even attempt. */
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
		source *inclusion = push_source( (char_pascalarray*)relapath, 0 );
		
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
		/*
			(
					( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
					tokengroup*
					( 0 | char_parr* ( 1 | token* 2 ) )
				--
						( ( ( v_==0: token*0 ) | ( v_==1: string*1 ) ) char_parr* )
					|
						error path execution
			)
		*/
	retframe metaC_stdinclude_stringify_conclude( stackpair *stkp, void *v );
		/* ( ( v_==0: token*0 token*1 ) | ( v_==1: string*1 token*0 ) -> metaC_stdinclude_body() ) */
	retframe metaC_stdinclude_stringify( stackpair *stkp, void *v_ )
	{
		int scratch;

		uintptr_t tok;
		
		STACKCHECK( stkp,  metaC_stdinclude_stringify );
		
		STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ) * 0, tok,  metaC_stdinclude_stringify, scratch );
		switch( ( (token_head*)tok )->toktype )
		{
			case TOKTYPE_SQSTR:
			case TOKTYPE_DQSTR:
				static unsigned booldepth;
				static retframe_parr
					stringerr_ =
						(retframe_parr)
						{
							3, /* Number of retframes  */
							{
								/*
									(
										( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
										token*
										&stringtoken2char_parr
									)
								*/
								
								
									/*
										(
											--
												( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
												token*
												&stringtoken2char_parr !!depth
										)
									*/
								(retframe){ &vm_pushdata, (void*)&booldepth },
									/* Src==1 wasn't used yet, and is also compliant with */
									/*  that function pointer on the stack (when considering */
									/*  the handler function's requirements for src). */
								(retframe){ &vm_push1, (void*)0 },
								
								(retframe){ &longjump_callstack, (void*)&metaC_stdinclude_jumpref }
								/*
									(
										( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
										token*
										( src==0: | src==2: char_parr* token* ) !!v_ src
									)
								*/
								/* The value of src should always match the number of elements that it */
								/*  indicates the presence of. */
							}
						};
				static retframe stringerr = (retframe){ &enqueue_returns, (void*)&stringerr_ };
				static retframe_parr
					isstring =
						(retframe_parr)
						{
							3, /* Number of retframes  */
							{
									/*
										(
												( ( v_==0: token*0 ) | ( v_==1: string*1 ) ) string-token*
											--
												( ( v_==0: token*0 ) | ( v_==1: string*1 ) ) token* char_parr*
										)
									*/
									/* v_ must point to a retframe{} to handle "unrecognized token type" */
									/*  errors. The stack will be: */
										/*
											(
												( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
												token*
												&stringtoken2char_parr
											)
										*/
								??? (retframe){ &stringtoken2char_parr, (void*)&stringerr },
								(retframe){ &vm_push1, (void*)0 },
									/*
										(
												( ( v_==0: token*0 ) | ( v_==1: string*1 ) )
												strtoken*
												( 0 | char_parr* ( 1 | token* 2 ) )
											--
													( ( ( v_==0: token*0 ) | ( v_==1: string*1 ) ) char_parr* )
												|
													error path execution
										)
									*/
								(retframe){ &metaC_stdinclude_stringify_conclude, (void*)0 }
							}
						};
				booldepth = ( ( (uintptr_t)v_ ) ? 1 : 0 );
				return( (retframe){ &enqueue_returns, (void*)&isstring } );
				
			default:
				break;
		}

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
					tokengroup*
					( 0 | char_parr* ( 1 | token* 2 ) )
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
									??? /* Requires a pointer to a retframe as v. */
									/* Is the above note right? It seems meant for something else... */
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



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
