#ifndef METAC_TOKENIZATION_TOKEN_H
 #define METAC_TOKENIZATION_TOKEN_H
	
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
	
	/* tokenization/token.h */
	
	
	
	typedef struct token_head
	{
		uintptr_t toktype;
		int length;
		
		/* "was_freshline" was previously here. Apply "!" to column, and you */
		/*  get the old "was_freshline". */
		/* Actually, this should be replaced with a flag container, with */
		/*  delimited & freshline both being provided. */
		int is_delimited;
			/* These three vars (src, line, and column) are semi-late */
			/*  changes, so there will be a lot of code not expecting them, */
			/*  while expecting "was_freshline" instead. */
		refed_pstr *src;
		uintmax_t line, column;
		
		/* For a full token, this would then be followed by "length" - 1 */
		/*  (because null) characters. */
		
	} token_head;
	/* Note: look for equal_tokenheads() below token{}. */
		/* -1: th was null; otherwise 0 for "fancy token" or 1 for standard token */
	int is_stdtoken( token_head *th );
	int is_delimited( token_head *th );
	int was_freshline( token_head *th );
		/* ( dest-token_head* src-token_head* -- dest-token_head* src-token_head* ) */
	retframe vm_tokenhead_setflags( stackpair *stkp, void *v_ );
		/* ( dest-token_head* src-token_head* -- dest-token_head* src-token_head* ) */
	retframe vm_tokenhead_setsource( stackpair *stkp, void *v_ );
		typedef struct deep_toktype
		{
			uintptr_t shallow_toktype, virtual_toktype;

		} deep_toktype;
		LIB4_MONAD_EITHER_BUILDTYPE_DEFINITION(
			deeptoktype_result,

			deep_toktype,
			uintptr_t
		);
		#define DEEPTOKTYPE_RESULT_BUILDSUCCESS( val ) \
			LIB4_MONAD_EITHER_BUILDLEFT( \
				deeptoktype_result, deep_toktype, (val) \
			)
		#define DEEPTOKTYPE_RESULT_BUILDFAILURE( val ) \
			LIB4_MONAD_EITHER_BUILDRIGHT( \
				deeptoktype_result, uintptr_t, (val) \
			)
		#define DEEPTOKTYPE_RESULT_BODYMATCH( var, matcha, matchb ) \
			LIB4_MONAD_EITHER_BODYMATCH( var, matcha, matchb )
		#define DEEPTOKTYPE_RESULT_EXPRMATCH( var, matcha, matchb ) \
			LIB4_MONAD_EITHER_EXPRMATCH( var, matcha, matchb )
		#define DEEPTOKTYPE_RESULT_RETURNSUCCESS( val ) \
			LIB4_MONAD_EITHER_RETURNLEFT( \
				deeptoktype_result, deep_toktype, val )
		#define DEEPTOKTYPE_RESULT_RETURNFAILURE( val ) \
			LIB4_MONAD_EITHER_RETURNRIGHT( \
				deeptoktype_result, uintptr_t, val )
		/* Special values: */
			/* shallow = TOKTYPE_TOKENGROUP_SAMEMERGE: tokengroup */
			/* shallow = TOKTYPE_TOKENBRANCH: tokenbranch */
			/* shallow = TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION: macro_token */
			/* other shallow: ignore virtual, shallow says it all. */
			/* err = TOKTYPE_INVALID: arg was null */
			/* err = TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION: as shallow, but ->tok was null */
			/* err = TOKTYPE_SPACE: somehow the switch didn't return, even though it always should. */
	deeptoktype_result get_deeptoktype( token_head *th );
	int simplify_toktype( token_head *tok,  uintptr_t *dest );
		/* ( token* -- token* char_parr* ) */
		/* v_ must point to a retframe{} to handle "unrecognized token type" */
		/*  errors. The stack will be ( token* char_parr* ). */
		/* Various things (e.g. delimited handling) need to be reexamined. */
	retframe token2char_parr( stackpair *stkp, void *v_ );
		/* ( string-token* -- token* char_parr* ) */
		/* v_ must point to a retframe{} to handle "unrecognized token type" */
		/*  errors. The stack will be ( token* &stringtoken2char_parr ). */
		/* This ultimately just exists to discard the wrapping quotes for */
		/*  cases where those are in the way. token2char_parr() is the normal */
		/*  function. */
	retframe stringtoken2char_parr( stackpair *stkp, void *v_ )
	
		/* toktype */
			/* Choose any single TOKTYPE_* value macro from below: note that some */
			/*  are actually NOT currently used/handled. */
		/* len */
			/* This is used for "plain" tokens that are just text, but might be */
			/*  needed for some other stuff too. */
		/* delim */
			/* Flags. Only "delimited" is currently accomodated, but */
			/*  "was freshline" should be distinguished in the future. */
		/* sf_name */
			/* The name of the source file. */
		/* line & column */
			/* The location within the source file where the token started. */
	#define INIT_TOKENHEAD( toktype, len,  delim,  sf_name, line, column ) \
		(token_head){ \
			(uintptr_t)( toktype ), (int)( len ), \
			(int)( delim ), \
			(refed_pstr*)( sf_name ), (uintmax_t)( line ), (uintmax_t)( column ) \
		}
	
	retframe set_dealloctoken( retframe dealc_ );
	retframe invoke_dealloctoken( stackpair *stkp, void *v );
	retframe smart_dealloc_token( stackpair *stkp, void *v );
		/* ( token*[len] len -- ) */
	retframe bulk_dealloc_token( stackpair *stkp, void *v );
	
	
	
		/* Note: neither src_tokhead nor destvar_uintp are pointers */
		/*  in either of these two macros. The *_uintp is to be a */
		/*  uintptr_t. */
	#defint TOKEN_FETCH_SIMPLETYPE( src_tokhead, destvar_uintp,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = simplify_toktype( &( src_tokhead ),  &( destvar_uintp ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "peek_uintptr", ( caller ), ( scratch ) ); \
				( endfunc )(); } }
	#defint TOKEN_CHECK_SIMPLETYPE( src_tokhead, desired_val, on_yes, on_no,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			uintptr_t TOKEN_CHECK_SIMPLETYPE_type; \
			TOKEN_FETCH_SIMPLETYPE( \
				( src_tokhead ), TOKEN_CHECK_SIMPLETYPE_type, \
				( stylesetptr ), ( caller ), ( scratch ), ( endfunc ) ); \
			if( TOKEN_CHECK_SIMPLETYPE_type == ( desired_val ) ) { \
				( on_yes )( TOKEN_CHECK_SIMPLETYPE_type ); } \
			else { \
				( on_no )( TOKEN_CHECK_SIMPLETYPE_type ); } }
	
	
	
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( tokenheadptr_, token_head* );
	typedef tokenheadptr_pascalarray tokhdptr_parr;
	
	LIB4_MONAD_EITHER_BUILDTYPE_DEFINITION(
		tokenheadptr_result,
		
		token_head*,
		uintptr_t
	);
	#define TOKENHEADPTR_RESULT_BUILDSUCCESS( val ) \
		LIB4_MONAD_EITHER_BUILDLEFT( \
			tokenheadptr_result, token_head*, (val) \
		)
	#define TOKENHEADPTR_RESULT_BUILDFAILURE( val ) \
		LIB4_MONAD_EITHER_BUILDRIGHT( \
			tokenheadptr_result, uintptr_t, (val) \
		)
	#define TOKENHEADPTR_RESULT_BODYMATCH( var, matcha, matchb ) \
		LIB4_MONAD_EITHER_BODYMATCH( var, matcha, matchb )
	#define TOKENHEADPTR_RESULT_EXPRMATCH( var, matcha, matchb ) \
		LIB4_MONAD_EITHER_EXPRMATCH( var, matcha, matchb )
	#define TOKENHEADPTR_RESULT_RETURNSUCCESS( val ) \
		LIB4_MONAD_EITHER_RETURNLEFT( \
			tokenheadptr_result, token_head*, val )
	#define TOKENHEADPTR_RESULT_RETURNFAILURE( val ) \
		LIB4_MONAD_EITHER_RETURNRIGHT( \
			tokenheadptr_result, uintptr_t, val )
	
	
	typedef struct token token;
	struct token
	{
		token_head header;
		char text[];
	};
	#define INIT_TOKEN_GENERIC( toktype, text ) \
		(token){ \
			INIT_TOKENHEAD( \
				( toktype ), \
				sizeof( (char[]){ ( text ) } ) / sizeof( char ),  \
				0,  0, 0, 0 ), \
			(char[]){ ( text ) } \
		}
	
		/* If the result is the failure case, then one or both arguments were provided as null. */
		/* If the result is the success case: */
			/* Negatives: a < b */
			/* Positives: a > b */
			/* 0: Match (ignores stuff like line and source file). */
			/* 1: Ended on toktype. */
			/* 2: Ended on length. */
	lib4_result equal_tokenheads( token_head *a, token_head *b );
		/* This inherits the results of equal_tokenheads(), and adds... */
			/* 3+: Mismatch at character[ abs( result ) - 3 ]. */
	lib4_result equal_tokens( token_head *a_, token_head *b_ );
	
		/* Returns: */
			/* -2: Null in *parr, if loc is provided it will hold the offset of the null. */
			/* -1: tok or parr is null. */
			/* 0: tok not found in parr. */
			/* 1: tok found, if loc is provided it will hold the offset of the find. */
	int isin_tokhdptr_parr( token *tok, tokhdptr_par *parr,  size_t *loc );
	
		/* ->header.toktype should equal TOKTYPE_NUMBER_UINT */
	typedef struct token_uint
	{
		token_head header;
		uintptr_t val;
		char text[];
		
	} token_uint;
	
		/* ->header.toktype should equal TOKTYPE_DEFERED_RETFRAME */
	typedef struct token_retframe
	{
		token_head header;
		retframe closure;
		
	} token_retframe;
	
	
	#define token_ENDRETFRAME( ... ) return( (retframe){ &end_run, (void*)0 } )
	
	
	#define TOKTYPE_INVALID ( 0 )
	
	#define TOKTYPE_SPACE ( 1 )
	#define TOKTYPE_NEWLINE ( 2 )
			/* Break the current parse layer. Used for 1st-stage */
			/*  directive/macro/whatever parsing, specifically */
			/*  in the process of finishing up bracket */
			/*  parse-tree building. */
		#define TOKTYPE_PARSEBREAK ( 0x200 )
	
	#define TOKTYPE_NAME ( 3 )
		#define TOKTYPE_ARGVAL ( 0x300 )
	#define TOKTYPE_NUMBER ( 4 )
		#define TOKTYPE_NUMBER_UINT ( 0x400 )
	
	/* Bracket openers: "5" block. */
		#define TOKTYPE_OPCRL ( 0x500 ) /* { */
		#define TOKTYPE_OPSQR ( 0x501 ) /* [ */
		#define TOKTYPE_OPPAR ( 0x502 ) /* ( */
		#define TOKTYPE_OPCRLUP ( 0x503 ) /* {^ */
		#define TOKTYPE_OPSQRUP ( 0x504 ) /* [^ */
		#define TOKTYPE_OPPARUP ( 0x505 ) /* (^ */
		#define TOKTYPE_OPCRLANG ( 0x506 ) /* {< */
		#define TOKTYPE_OPSQRANG ( 0x507 ) /* [< */
		#define TOKTYPE_OPPARANG ( 0x508 ) /* (< */
		#define TOKTYPE_OPCRLCOL ( 0x509 ) /* {: */
		#define TOKTYPE_OPSQRCOL ( 0x50A ) /* [: */
		#define TOKTYPE_OPPARCOL ( 0x50B ) /* (: */
		#define TOKTYPE_OPCRLAST ( 0x50C ) /* {* */
		#define TOKTYPE_OPSQRAST ( 0x50D ) /* [* */
		#define TOKTYPE_OPPARAST ( 0x50E ) /* (* */
	
	/* Strings: "6" block. */
		#define TOKTYPE_SQSTR ( 0x600 )
		#define TOKTYPE_DQSTR ( 0x601 )
	
	/* Symbols: "7" block. */
		#define TOKTYPE_SYM_DECREMENT ( 0X700 ) /* -- */
		#define TOKTYPE_SYM_PLACEDSUB ( 0X701 ) /* -= */
		#define TOKTYPE_SYM_ELEMEDDEREF ( 0X702 ) /* -> */
		#define TOKTYPE_SYM_SUBTRACT ( 0x703 ) /* - */
		#define TOKTYPE_SYM_COMMENTCL ( 0x704 ) /* It's * / but without the space. */
		#define TOKTYPE_SYM_DECLCASTCLOSE ( 0x705 )
		#define TOKTYPE_SYM_ARRCASTCLOSE ( 0x706 )
		#define TOKTYPE_SYM_CASTCLOSE ( 0x707 )
		#define TOKTYPE_SYM_PLACEDMULT ( 0x708 ) /* *= */
		#define TOKTYPE_SYM_MULTIPLY ( 0x709 ) /* * */
		#define TOKTYPE_SYM_COMMENTOP ( 0x70A ) /* It's / * but without the space. */
		#define TOKTYPE_SYM_COMMENTLINE ( 0x70B ) /* It's / / but without the space. */
		#define TOKTYPE_SYM_PLACEDDIV ( 0x70C ) /* /= */
		#define TOKTYPE_SYM_DIVIDE ( 0x70D ) /* / */
		#define TOKTYPE_SYM_ADDROF ( 0x70E )
		#define TOKTYPE_SYM_DEREFERENCE ( 0x70F )
		#define TOKTYPE_SYM_PRECRLCL ( 0x710 ) /* ^} */
		#define TOKTYPE_SYM_PRESQRCL ( 0x711 ) /* ^] */
		#define TOKTYPE_SYM_PREPARCL ( 0x712 ) /* ^) */
		#define TOKTYPE_SYM_PLACEDXOR ( 0x713 ) /* ^= */
		#define TOKTYPE_SYM_XOR ( 0x714 ) /* ^ */
		#define TOKTYPE_SYM_COMPTCRLCL ( 0x715 )
		#define TOKTYPE_SYM_COMPTSQRCL ( 0x716 )
		#define TOKTYPE_SYM_COMPTPARCL ( 0x717 )
		#define TOKTYPE_SYM_GREATEQUAL ( 0x718 ) /* >= */
		#define TOKTYPE_SYM_GREATERTHAN ( 0x719 ) /* > */
		#define TOKTYPE_SYM_DECLCRLCL ( 0x71A )
		#define TOKTYPE_SYM_DECLSQRCL ( 0x71B )
		#define TOKTYPE_SYM_DECLPARCL ( 0x71C )
		#define TOKTYPE_SYM_SCOPE ( 0x71D ) /* :: */
		#define TOKTYPE_SYM_COLON ( 0x71E ) /* : */
		#define TOKTYPE_SYM_INCREMENT ( 0x71F ) /* ++ */
		#define TOKTYPE_SYM_PLACEDADD ( 0x720 ) /* += */
		#define TOKTYPE_SYM_ADDITION ( 0x721 ) /* + */
		#define TOKTYPE_SYM_PLACEDMODULO ( 0x722 ) /* %= */
		#define TOKTYPE_SYM_MODULO ( 0x723 ) /* % */
		#define TOKTYPE_SYM_EQUALITY ( 0x724 ) /* == */
		#define TOKTYPE_SYM_SET ( 0x725 ) /* = */
		#define TOKTYPE_SYM_NOTEQUAL ( 0x726 ) /* != */
		#define TOKTYPE_SYM_NOT ( 0x727 ) /* ! */
		#define TOKTYPE_SYM_LESSEREQUAL ( 0x728 ) /* <= */
		#define TOKTYPE_SYM_LESSERTHAN ( 0x729 ) /* < */
		#define TOKTYPE_SYM_LOGICAND ( 0x72A ) /* && */
		#define TOKTYPE_SYM_PLACEDBINARYAND ( 0x72B ) /* &= */
		#define TOKTYPE_SYM_BINARYAND ( 0x72C ) /* & */
		#define TOKTYPE_SYM_LOGICIOR ( 0x72D ) /* || */
		#define TOKTYPE_SYM_PLACEDBINARYIOR ( 0x72E ) /* |= */
		#define TOKTYPE_SYM_BINARYIOR ( 0x72F ) /* | */
		#define TOKTYPE_SYM_QUERY ( 0x730 ) /* ? */
		#define TOKTYPE_SYM_DIVISIVSHIFT ( 0x731 )
		#define TOKTYPE_SYM_PLACEDDIVISIVSHIFT ( 0x732 )
		#define TOKTYPE_SYM_MULTIPYSHIFT ( 0x733 )
		#define TOKTYPE_SYM_PLACEDMULTIPLYSHIFT ( 0x734 )
		#define TOKTYPE_SYM_CONFUSION ( 0x735 ) /* ??? */
		#define TOKTYPE_SYM_UNKNOWN ( 0x736 ) /* I think this is supposed to NOT correspond to a token. */
		#define TOKTYPE_SYM_CURLYCLOSE ( 0x737 ) /* } */
		#define TOKTYPE_SYM_SQUARECLOSE ( 0x738 ) /* ] */
		#define TOKTYPE_SYM_PARENCLOSE ( 0x739 ) /* ) */
		#define TOKTYPE_SYM_TILDE ( 0x73A ) /* ~ */
		#define TOKTYPE_SYM_DOT ( 0x73B )
		#define TOKTYPE_SYM_COMMA ( 0x73C ) /* , */
		#define TOKTYPE_SYM_BSLASH ( 0x73D )
	
	/* Unused: "8" block. */
		#define TOKTYPE_DOLLAR ( 0x800 )
		#define TOKTYPE_GRAVE ( 0x801 )
	
	#define TOKTYPE_OCTO ( 9 )
		#define TOKTYPE_DOUBLEOCTO ( 0x900 )
	
	#define TOKTYPE_AT ( 10 )
	#define TOKTYPE_OTHER ( 11 )
	
	
	/* tokengroup: "12"/"B" block. */
		#define TOKTYPE_TOKENGROUP_SAMEMERGE ( 0xB00 )
		#define TOKTYPE_TOKENBRANCH ( 0xB01 )
			/* The "body" member of the merge is "real", the rest is */
			/*  "imaginary". */
		#define TOKTYPE_TOKENGROUP_STRMERGE ( 0xB02 )
		#define TOKTYPE_TOKENGROUP_COMNTMERGE ( 0xB03 )
		#define TOKTYPE_TOKENGROUP_EQUIVMERGE ( 0xB04 )
		
		#define TOKTYPE_TOKENGROUP_WHITESPACE ( 0xB05 )
		
			/* The "body" contains characters that should be treated differently */
			/*  than normal. The details will have to be defined elsewhere. */
		#define TOKTYPE_TOKENGROUP_DELIMITED ( 0xB06 )
		
		
		#define TOKTYPE_TOKENGROUP_MACROLINK ( 0xB07 )
		#define TOKTYPE_TOKENGROUP_MACROTOKEN ( 0xB08 )
				/* Same data structure as *_MACROTOKEN, but specifically */
				/*  establishes a firewall against deallocation: it will only be */
				/*  deallocated when it's parent is deallocated, and the things */
				/*  it points at will be left untouched as well. */
			#define TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION ( 0xB09 )
		#define TOKTYPE_TOKENGROUP_MACRORUN ( 0xB0A )
		#define TOKTYPE_TOKENGROUP_MACRODIRECTIVE ( 0xB0B )
		
		#define TOKTYPE_TOKENGROUP_MACROCALL ( 0xB0C )
		
		#define TOKTYPE_DEFERED_RETFRAME ( 0xB0D )
		
		#define TOKTYPE_CONTEXTSPECIALS ( 0xB0E )
			/* This is used mid-parse to mark sets of tokens that are arguments. */
		#define TOKTYPE_GENERICGROUP ( 0xB0F )
	
	
#endif
