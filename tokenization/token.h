#ifndef METAC_TOKENIZATION_TOKEN_H
 #define METAC_TOKENIZATION_TOKEN_H
	
	/* tokenization/token.h */
	
	
	
	typedef struct token_head
	{
		uintptr_t toktype;
		int length;
		
		/* "was_freshline" was previously here. Apply "!" to column, and you */
		/*  get the old "was_freshline". */
		int is_delimited;
			/* These three vars (src, line, and column) are semi-late */
			/*  changes, so there will be a lot of code not expecting them, */
			/*  while expecting "was_freshline" instead. */
		refed_pstr *src;
		uintmax_t line, column;
		
		/* For a full token, this would then be followed by "length" - 1 */
		/*  (because null) characters. */
		
	} token_head;
	
	typedef struct token token;
	struct token
	{
		token_head header;
		char text[];
	};
	
		/* ->header.toktype should equal TOKTYPE_NUMBER_UINT */
	typedef struct token_uint
	{
		token_head header;
		uintptr_t val;
		char text[];
		
	} token_uint;
	
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( tokenheadptr_, token_head* );
	typedef tokenheadptr_pascalarray tokhdptr_parr;
	
	
	#define token_ENDRETFRAME( ... ) return( (retframe){ &end_run, (void*)0 } )
	
	
	#define TOKTYPE_INVALID ( 0 )
	
	#define TOKTYPE_SPACE ( 1 )
	#define TOKTYPE_NEWLINE ( 2 )
	
	#define TOKTYPE_NAME ( 3 )
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
		#define TOKTYPE_SYM_DECREMENT ( 0X700 )
		#define TOKTYPE_SYM_PLACEDSUB ( 0X701 )
		#define TOKTYPE_SYM_ELEMEDDEREF ( 0X702 )
		#define TOKTYPE_SYM_SUBTRACT ( 0x703 )
		#define TOKTYPE_SYM_COMMENTCL ( 0x704 )
		#define TOKTYPE_SYM_DECLCASTCLOSE ( 0x705 )
		#define TOKTYPE_SYM_ARRCASTCLOSE ( 0x706 )
		#define TOKTYPE_SYM_CASTCLOSE ( 0x707 )
		#define TOKTYPE_SYM_PLACEDMULT ( 0x708 )
		#define TOKTYPE_SYM_MULTIPLY ( 0x709 )
		#define TOKTYPE_SYM_COMMENTOP ( 0x70A )
		#define TOKTYPE_SYM_COMMENTLINE ( 0x70B )
		#define TOKTYPE_SYM_PLACEDDIV ( 0x70C )
		#define TOKTYPE_SYM_DIVIDE ( 0x70D )
		#define TOKTYPE_SYM_ADDROF ( 0x70E )
		#define TOKTYPE_SYM_DEREFERENCE ( 0x70F )
		#define TOKTYPE_SYM_PRECRLCL ( 0x710 )
		#define TOKTYPE_SYM_PRESQRCL ( 0x711 )
		#define TOKTYPE_SYM_PREPARCL ( 0x712 )
		#define TOKTYPE_SYM_PLACEDXOR ( 0x713 )
		#define TOKTYPE_SYM_XOR ( 0x714 )
		#define TOKTYPE_SYM_COMPTCRLCL ( 0x715 )
		#define TOKTYPE_SYM_COMPTSQRCL ( 0x716 )
		#define TOKTYPE_SYM_COMPTPARCL ( 0x717 )
		#define TOKTYPE_SYM_GREATEQUAL ( 0x718 )
		#define TOKTYPE_SYM_GREATERTHAN ( 0x719 )
		#define TOKTYPE_SYM_DECLCRLCL ( 0x71A )
		#define TOKTYPE_SYM_DECLSQRCL ( 0x71B )
		#define TOKTYPE_SYM_DECLPARCL ( 0x71C )
		#define TOKTYPE_SYM_SCOPE ( 0x71D )
		#define TOKTYPE_SYM_COLON ( 0x71E )
		#define TOKTYPE_SYM_INCREMENT ( 0x71F )
		#define TOKTYPE_SYM_PLACEDADD ( 0x720 )
		#define TOKTYPE_SYM_ADDITION ( 0x721 )
		#define TOKTYPE_SYM_PLACEDMODULO ( 0x722 )
		#define TOKTYPE_SYM_MODULO ( 0x723 )
		#define TOKTYPE_SYM_EQUALITY ( 0x724 )
		#define TOKTYPE_SYM_SET ( 0x725 )
		#define TOKTYPE_SYM_NOTEQUAL ( 0x726 )
		#define TOKTYPE_SYM_NOT ( 0x727 )
		#define TOKTYPE_SYM_LESSEREQUAL ( 0x728 )
		#define TOKTYPE_SYM_LESSERTHAN ( 0x729 )
		#define TOKTYPE_SYM_LOGICAND ( 0x72A )
		#define TOKTYPE_SYM_PLACEDBINARYAND ( 0x72B )
		#define TOKTYPE_SYM_BINARYAND ( 0x72C )
		#define TOKTYPE_SYM_LOGICIOR ( 0x72D )
		#define TOKTYPE_SYM_PLACEDBINARYIOR ( 0x72E )
		#define TOKTYPE_SYM_BINARYIOR ( 0x72F )
		#define TOKTYPE_SYM_QUERY ( 0x730 )
		#define TOKTYPE_SYM_DIVISIVSHIFT ( 0x731 )
		#define TOKTYPE_SYM_PLACEDDIVISIVSHIFT ( 0x732 )
		#define TOKTYPE_SYM_MULTIPYSHIFT ( 0x733 )
		#define TOKTYPE_SYM_PLACEDMULTIPLYSHIFT ( 0x734 )
		#define TOKTYPE_SYM_CONFUSION ( 0x735 )
		#define TOKTYPE_SYM_UNKNOWN ( 0x736 )
		#define TOKTYPE_SYM_CURLYCLOSE ( 0x737 )
		#define TOKTYPE_SYM_SQUARECLOSE ( 0x738 )
		#define TOKTYPE_SYM_PARENCLOSE ( 0x739 )
		#define TOKTYPE_SYM_TILDE ( 0x73A )
		#define TOKTYPE_SYM_DOT ( 0x73B )
		#define TOKTYPE_SYM_COMMA ( 0x73C )
		#define TOKTYPE_SYM_BSLASH ( 0x73D )
	
	/* Unused: "8" block. */
		#define TOKTYPE_DOLLAR ( 0x800 )
		#define TOKTYPE_GRAVE ( 0x801 )
	
	#define TOKTYPE_OCTO ( 9 )
	
	#define TOKTYPE_AT ( 10 )
	#define TOKTYPE_OTHER ( 11 )
	
	
	/* tokengroup: "12"/"B" block. */
		#define TOKTYPE_TOKENGROUP_SAMEMERGE ( 0xB00 )
		
			/* The "body" member of the merge is "real", the rest is */
			/*  "imaginary". */
		#define TOKTYPE_TOKENGROUP_STRMERGE ( 0xB01 )
		#define TOKTYPE_TOKENGROUP_COMNTMERGE ( 0xB02 )
		#define TOKTYPE_TOKENGROUP_EQUIVMERGE ( 0xB03 )
		
		#define TOKTYPE_TOKENGROUP_WHITESPACE ( 0xB04 )
		
			/* The "body" contains characters that should be treated differently */
			/*  than normal. The details will have to be defined elsewhere. */
		#define TOKTYPE_TOKENGROUP_DELIMITED ( 0xB05 )
		
		
		#define TOKTYPE_TOKENGROUP_MACROLINK (0xB06 )
		#define TOKTYPE_TOKENGROUP_MACROTOKEN (0xB07 )
				/* Same data structure as *_MACROTOKEN, but specifically */
				/*  establishes a firewall against deallocation: it will only be */
				/*  deallocated when it's parent is deallocated, and the things */
				/*  it points at will be left untouched as well. */
			#define TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION (0xB08 )
		#define TOKTYPE_TOKENGROUP_MACRORUN (0xB09 )
		#define TOKTYPE_TOKENGROUP_MACRODIRECTIVE (0xB0A )
		
		#define TOKTYPE_TOKENGROUP_MACROCALL (0xB0B )
	
	
#endif
