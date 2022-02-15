#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include "lib4/pascalarray.h"


	/* Defines char_pascalarray, char_pascalarray_result, and various */
	/*  functions. */
LIB4_DEFINE_PASCALARRAY_STDDEFINE( char_, char );
#define BUILD_CHARPARR( string ) \
	(char_pascalarray){ sizeof( (char[]){ string } ) - 1, (char[]){ string } }


typedef struct stackframe stackframe;
struct stackframe
{
	char_pascalarray *stack;
	size_t used;
	
};

typedef struct stackpair stackpair;
struct stackpair
{
	stackframe ret, data;
	
};
extern stackpair std_stacks;


typedef struct retframe retframe;
typedef retframe (*framefunc)( stackpair*, void* );
struct retframe
{
	framefunc handler;
	void *data;
};


extern void err_interface( uintptr_t source, lib4_failure_result err, ... );


int init();
void deinit();
int reinit();

source* build_source( char_pascalarray *name, uintmax_t inclusionpoint );
int discard_source( source *src );



#define CALL_FRAMEFUNC( rethand, retval, fhand, fval ) \
	if( push_retframe( (retframe){ (rethand), (retval) } ) ) { \
		if( push_frame() ) { \
			return( (retframe){ (fhand), (fval) } ); } \
		else { return( (retframe){ &end_run, (void*)0 } ); } }\
	else { return( (retframe){ &end_run, (void*)0 } ); }
#define RET_FRAMEFUNC( refname, errnum, ... ) \
	if( 1 ) { \
		stackframe *csf = stack; stack = stack->prev; \
		retframe ret;  int res = pop_retframe( &ret ); \
		if( !res ) { \
			err_interface( &( refname ), (lib4_failure_result){ (errnum) }, __VA_ARGS__ ); \
			return( (retframe){ &end_run, (void*)0 } ); } \
		stack = csf; return( ret ); }
#define RET_FRAMEFUNC2( refname, subrefnum, ... ) \
	if( 1 ) { \
		stackframe *csf = stack; stack = stack->prev; \
		retframe ret;  int res = pop_retframe( &ret ); \
		if( !res ) { \
			err_interface( &( refname ), ( subrefnum ), __VA_ARGS__ ); \
			return( (retframe){ &end_run, (void*)0 } ); } \
		stack = csf; return( ret ); }



typedef struct extrachar
{
	char c;
	int was_freshline;
	int is_delimited;
};
	/* Defined in lexalike.c */
int popas_extrachar( stackpair *stkp, void *v,  extrachar *ec );

extrachar_result get_extrachar();
extrachar_result peek_extrachar();
int unget_extrachar( extrachar ec );

LIB4_MONAD_EITHER_BUILDTYPE_DEFINITION(
	extrachar_result,
	
	extrachar,
	lib4_failure_result
);

#define EXTRACHAR_BUILDSUCCESS( val ) \
	LIB4_MONAD_EITHER_BUILDLEFT( extrachar_result, extrachar, (val) )
#define EXTRACHAR_BUILDFAILURE( val ) \
	LIB4_MONAD_EITHER_BUILDRIGHT( extrachar_result, lib4_failure_result, (val) )

	/* The *BODY* version takes statements, *EXPR* takes expressions. */
	/*  The matches must be function-style, though function macros are */
	/*  allowed. */
#define EXTRACHAR_BODYMATCH( var, succ, fail ) \
	LIB4_MONAD_EITHER_BODYMATCH( var, succ, fail )
#define EXTRACHAR_EXPRMATCH( var, succ, fail ) \
	LIB4_MONAD_EITHER_EXPRMATCH( var, succ, fail )

	/* Convenience wrappers. See monads.h for more details. */
#define EXTRACHAR_RETURNSUCCESS( val ) \
	LIB4_MONAD_EITHER_RETURNLEFT( extrachar_result, extrachar, val )
#define EXTRACHAR_RETURNFAILURE( val ) \
	LIB4_MONAD_EITHER_RETURNRIGHT( extrachar_result, lib4_failure_result, val )


typedef struct token_head
{
	uintptr_t toktype;
	int length;
	int was_freshline;
	int is_delimited;
	
	/* For a full token, this would then be followed by "length" - 1 */
	/*  (because null) characters. */
	
} token_head;

#define TOKTYPE_INVALID ( 0 )

#define TOKTYPE_SPACE ( 1 )
#define TOKTYPE_NEWLINE ( 2 )

#define TOKTYPE_NAME ( 3 )
#define TOKTYPE_NUMBER ( 4 )

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
	#define TOKTYPE_SYM_PLACEDBINARYAND ( 0x72E )
	#define TOKTYPE_SYM_BINARYAND ( 0x72F )
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
	
	#define TOKTYPE_TOKENGROUP_MACRO ??? (0xB0 ??? )
	
	#define TOKTYPE_TOKENGROUP_MACROCALL (0xB0 ??? )



	/* Defined in simplelex.c */
	/* If either the last token was a newline, or only whitespace came after */
	/*  the most recent newline, then this returns 1, else returns 0. */
int is_freshline();

	/* Provided by lexalike.c. Pops a token_head from stkp->data, tests the */
	/*  length (and possibly errors out), peeks at a char on the same stack, */
	/*  pushes the token_head back on top, and returns the output of the */
	/*  provided function when given the char value. */
lib4_result stack_testchar( stackpair *stkp, void *v,  int (*testfunc)( int ),  int fail_on_multichar );


size_t token_queue_init();

	/* These implement an "unget" capability for tokens: use */
	/*  token_queue_fetch() if you want a token, token_queue_push() if you */
	/*  want to give it back. */
size_t token_queue_used();
int token_queue_push( token *tok );
int token_queue_pop( token **tok );
retframe token_queue_fetch( stackpair *stkp, void *v );

	/* Provides a way to temporarily store tokens that WILL be going onto */
	/*  the token's "unget" stack, for those cases when you need to reverse */
	/*  the order that they'll be pulled back off. */
size_t token_queue_shuffleused();
int token_queue_shufflepush( token *tok );
int token_queue_shufflepop( token **tok );

	/* Moves one token pointer from the "shuffle" stack to the "unget" */
	/*  stack. */
int token_queue_shuffle2queue();

	/* *_deinit() makes NO attempt to release any tokens. If you care, then */
	/*  just keep popping & deallocating until *_used() returns 0. */
int token_queue_deinit();



/* All of these functions are found in lexalike.c */
/* Note that set_lexentry() is the only one that you should probably use, as */
/*  token_queue_fetch() uses them while also supporting an "unget" facility. */
	/* We'll certainly want these three functions declared. */
retframe head_lex( stackpair*, void* );
	/* Technically you might want a lexer other than head_lex(), so this is */
	/*  how you set that override. Returns 0 for "null argument", otherwise */
	/*  returns the PREVIOUS lexer function pointer. Note that the "output" */
	/*  of your lexer needs to follow the pattern of head_lex(), so look at */
	/*  either that family of functions, or */
	/*  assemble_token()/getANDassemble_token() to understand the */
	/*  communication route and format. */
intptr_t set_lexentry( lexentry lentry );
retframe getANDassemble_token( stackpair*, void* );
	/* This functions as a deallocator for only the simplest of tokens- the */
	/*  leaf tokens which don't have pointers. */
retframe dealloc_token( stackpair *stkp, void *v );
	/* These all expect a token_head on top of a char on the stkp->data */
	/*  stack: these values will be restored to that stack, and an integer */
	/*  representing the associated C function's return placed on top. */
retframe stack_isalnum( stackpair *stkp, void *v );
retframe stack_isalpha( stackpair *stkp, void *v );
retframe stack_isblank( stackpair *stkp, void *v );
retframe stack_iscntrl( stackpair *stkp, void *v );
retframe stack_isdigit( stackpair *stkp, void *v );
retframe stack_isgraph( stackpair *stkp, void *v );
retframe stack_islower( stackpair *stkp, void *v );
retframe stack_isprint( stackpair *stkp, void *v );
retframe stack_ispunct( stackpair *stkp, void *v );
retframe stack_isspace( stackpair *stkp, void *v );
retframe stack_isupper( stackpair *stkp, void *v );
retframe stack_isxdigit( stackpair *stkp, void *v );
	/* These function as "enriched" equivalents of the normal C IO */
	/8  functions. They specifically handle characters that are output by an */
	/*  EARLY lexing stage, which currently handles delimiting, but in the */
	/*  future will probably be expanded to handle trigraphs as well. */
extrachar_result get_extrachar( stackpair *stkp, void *v );
extrachar_result peek_extrachar( stackpair *stkp, void *v );
int unget_extrachar( extrachar ec );

retframe accumulate_whitespace( stackpair*, void* );
	/* This function expects an already ASSEMBLED token to be pointed to by */
	/*  a pointer on the top of the data stack. It will then either directly */
	/*  defer to accumulate_whitespace() (if that token is a SPACE, NEWLINE, */
	/*  or OTHER), or else shove it into a tokenbranch, push a return to */
	/*  conclude_accumulate_token(), then call getANDassemble_token() with a */
	/*  return to accumulate_whitespace(). accumulate_whitespace() will then */
	/*  loop (accumulating whitespace as it goes) until it finds something */
	/*  that isn't whitespace, at which point it will return via the */
	/*  conclude_accumulate_token() reference that was pushed onto the */
	/*  return stack. */
retframe accumulate_token( stackpair *stkp, void *v );

	/* These all expect a pointer to a token on the top of the stack. They */
	/*  eventually terminate with the effect of an ordinary system return. */
retframe echo_tokens_entrypoint( stackpair *stkp, void *v );
retframe echo_token( stackpair *stkp, void *v );
retframe echo_tokengroup( stackpair *stkp, void *v );
retframe echo_tokenbranch( stackpair *stkp, void *v );

	/* All four of these expect a pointer to a populated copy of */
	/*  gennameparr_stdpushentry_entry to be provided in v_. */
retframe components_stdsearchinit( stackpair *stkp, void *v_ );
retframe components_sequencedsearchproceed( stackpair *stkp, void *v_ );
	/* This one stores the pointer to the token that caused it to be called */
	/*  in the .tail element of tokenbranch that it finalizes: the */
	/*  tokenbranch itself will be pointed to by the last element of the */
	/*  tokengroup that will be pointed to by the top element of the stack */
	/*  when this function returns. v_->conclude will be run after this */
	/*  function returns, but BEFORE the caller of this function is returned */
	/*  TO. */
retframe components_stdsearchdeinit( stackpair *stkp, void *v_ );
	/* This function works just like *_stdsearchdeinit(), except that it */
	/*  DOESN'T store the token that caused it to be run. So, it still */
	/*  returns with a tokengroup pointer on the top of the stack, the last */
	/*  element in that tokengroup is a pointer to a tokenbranch, */
	/*  v_->conclude gets run after this function (and ALSO after a */
	/*  deallocator accessed via invoke_dealloctoken(), to dealloocate the */
	/*  no-longer useful token), but before the return to this function's */
	/*  caller, etc. Just without .tail getting populated. Use this function */
	/*  for when you want to drop a searchtable, but DON'T consider the */
	/*  triggering token (or whatever else) to be valuable. */
retframe components_skipendersearchdeinit( stackpair *stkp, void *v_ );




/* Need to build a file that actually defines these uintptr_t instances, */
/*  instead of just declaring that they exist. */

extern uintptr_t lexalike_refid;
	#define REFID_SUBIDS_lexalike__tokenize_char 1, 0
		#define REFID_SUBIDS_lexalike__tokenize_char__accumulate 1, 1
	#define REFID_SUBIDS_lexalike__stack_testchar 2, 0
		#define REFID_SUBIDS_lexalike__stack_testchar2 2, 1
	#define REFID_SUBIDS_lexalike__get_extrachar 3

extern uintptr_t head_lex_refid;
extern uintptr_t space_lex_refid;
extern uintptr_t newline_lex_refid;
extern uintptr_t name_lex_refid;
extern uintptr_t number_lex_refid;
extern uintptr_t brackop_lex_refid;
extern uintptr_t str_lex_refid;
extern uintptr_t syms_lex_refid;
extern uintptr_t at_refid;

extern uintptr_t echo_tokens_refid;

extern uintptr_t complexlex_refid;
	#define REFID_SUBIDS_complexlex__set_dealloctoken (lib4_failure_result){ 1 }, 0
	#define REFID_SUBIDS_complexlex__invoke_dealloctoken (lib4_failure_result){ 2 }, 0
	#define REFID_SUBIDS_complexlex__smart_dealloc_token (lib4_failure_result){ 3 }, 0
		#define REFID_SUBIDS_complexlex__smart_dealloc_token__innerfunc1 (lib4_failure_result){ 3 }, 1
		#define REFID_SUBIDS_complexlex__smart_dealloc_token__innerfunc2 (lib4_failure_result){ 3 }, 2
extern uintptr_t tokengroup_refid;
extern uintptr_t tokenbranch_refid;
extern uintptr_t aggregatetokens_refid;

extern uintptr_t searchstack_refid;
	#define REFID_SUBIDS_searchstack__init 1, 0
	#define REFID_SUBIDS_searchstack__components_stdsearchinit 2, 0
		#define REFID_SUBIDS_searchstack__components_stdsearchinit__pushfunc 2, 1
	#define REFID_SUBIDS_searchstack__components_sequencedsearchproceed 3, 0
		#define REFID_SUBIDS_searchstack__components_sequencedsearchproceed__popfunc 3, 1
		#define REFID_SUBIDS_searchstack__components_sequencedsearchproceed__pushfunc 4
	#define REFID_SUBIDS_searchstack__components_stdsearchdeinit 5, 0
		#define REFID_SUBIDS_searchstack__components_stdsearchdeinit__popfunc 5, 1
		#define REFID_SUBIDS_searchstack__components_stdsearchdeinit__pushfunc 6
	#define REFID_SUBIDS_searchstack__components_skipendersearchdeinit 7, 0
		#define REFID_SUBIDS_searchstack__components_skipendersearchdeinit__popfunc 7, 1
		#define REFID_SUBIDS_searchstack__components_skipendersearchdeinit__pushfunc 8
	#define REFID_SUBIDS_searchstack__components_shufflequeue_entry_macro_call 9
	#define REFID_SUBIDS_searchstack__components_shufflequeue_step_macro_call 10
	#define REFID_SUBIDS_searchstack__components_shufflequeue_macro_link 11
	#define REFID_SUBIDS_searchstack__components_shufflequeue_macro_token 12
	#define REFID_SUBIDS_searchstack__components_shufflequeue_macro_run 13
	
???
	#define REFID_SUBIDS_searchstack__components_shufflequeue_exit_macro_call ???
???






	/* So, turns out that this is a semi-duplicate. I'll need to fix that */
	/*  later. */
	/* Note: this is the CORRECT form. */
typedef struct token token;
struct token
{
	token_head header;
	char text[];
};


uintptr_t assemble_token_refid;

LIB4_DEFINE_PASCALARRAY_STDDEFINE( tokenheadptr_, token_head* );
typedef tokenheadptr_pascalarray tokhdptr_parr;
typedef struct tokengroup tokengroup;
struct tokengroup
{
	token_head header;
		/* Takes the same values as token_head.toktype */
	uintptr_t subtype;
	
	tokhdptr_parr *arr;
	size_t used;
};
uintptr_t aggregate_tokens_refid;


typedef struct tokenbranch tokenbranch;
struct tokenbranch
{
	token_head header;
	uintptr_t subtype;
	
	token_head *lead, *body, *tail;
};
uintptr_t tokenbranch_refid;



LIB4_DEFINE_PASCALARRAY_STDDEFINE( charparrptr_, char_pascalarray* );
typedef charparrptr_pascalarray charparrptr_parr;



typedef struct genericnamed genericnamed;
struct genericnamed
{
	char_pascalarray *name;
	void *ref;
	uintptr_t reftype;
};
	/* The default state, doesn't actually represent anything. */
#define GENNAMETYPE_INVALID ( 0 )
	/* Used for tracking includes. */
#define GENNAMETYPE_NAMEONLY ( 1 )
	/* ->ref points to a lookup table to be used for further searches. */
	/*  Usually it gets pushed onto a stack. */
#include GENNAMETYPE_TABLEENTRY ( 2 )
	/* ->ref points to a retframe to be called/run. */
#include GENNAMETYPE_RETFRAMEFUNC ( 3 )


LIB4_DEFINE_PASCALARRAY_STDDEFINE( genericnamed_, genericnamed );
typedef genericnamed_pascalarray genname_parr;

typedef struct gennameparr_stdpushentry_entry gennameparr_stdpushentry_entry;
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

uintptr_t gennamearr_refid;


LIB4_DEFINE_PASCALARRAY_STDDEFINE( genericnamedparr_, genname_parr* );
typedef genericnamedparr_pascalarray genname_parrparr;

uintptr_t lexparse1_refid;

typedef struct genericnamed_retframe genericnamed_retframe;
struct genericnamed_retframe
{
	genericnamed name;
	retframe frame;
	
} genericnamed_retframe_refid;
	/* Note: obj must be a pointer to the genericnamed_retframe that the */
	/*  resulting value will be stored inside of. Predefine to match. */
#define BUILD_genericnamed_retframe( obj,  name_cparr, func, vdata ) \
	(genericnamed_retframe){ \
		(genericnamed){ \
			(name_cparr), \
			(void*)&( ( obj )->frame ), \
			(uintptr_t)&genericnamed_retframe_refid }, \
		(retframe){ (func), (vdata) } }



typedef struct generictyped generictyped;
struct generictyped
{
	uintptr_t toktype;
	void *ref;
		/* ->reftype uses the same values as in struct genericnamed. */
	uintptr_t reftype;
};

typedef struct generictyped_retframe generictyped_retframe;
struct generictyped_retframe
{
	generictyped name;
	retframe frame;
	
} generictyped_retframe_refid;

uintptr_t gentypedarr_refid;

LIB4_DEFINE_PASCALARRAY_STDDEFINE( generictyped_, generictyped );
typedef generictyped_pascalarray gentyped_parr;





genname_parrparr *std_entry_keys;
size_t std_entry_keys_used;
uintptr_t std_entry_refid;



void err_interface( uintptr_t source, lib4_failure_result err, ... );





#include "stack.h"
#include "tokenmatching.h"

#include "input/source.h"
#include "input/lexalike.h"

#include "metaCpreproc/basics.h"
#include "metaCpreproc/components.h"

#include "output/echo_tokens.h"

#include "tokenization/simplelex.h"
#include "tokenization/complexlex.h"





