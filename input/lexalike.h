#ifndef METAC_INPUT_LEXALIKE_H
 #define METAC_INPUT_LEXALIKE_H
	
	/* This file started as lexalike.c. It's been modified some, but some of */
	/*  these functions might not belong here, and other definitely DON'T */
	/*  belong here. */
	
	
	
	#include "stdmonads.h"
	#include "headers.h"
	
	#include "err/inner_err.h"
	
	
	
	/* The purpose of lexalike.c is to provide wrappers for charin(), */
	/*  isspace(), etc., but which convert stuff to tokens so that there's */
	/*  an easier way to support character delimiting, by moving it before */
	/*  token lexing and assembly. */
	/* Note that support for trigraphs belongs in this file. */
	/* There's no deep reason for the still_freshline, lexentry, and */
	/*  assemble_token stuff to be here, but it makes about as much sense as */
	/*  sticking it elsewhere. */
	
	
	/* Defined in simplelex.c */
	/* If either the last token was a newline, or only whitespace came after */
	/*  the most recent newline, then this returns 1, else returns 0. */
	int is_freshline();
	
	
	
	/* Note that set_lexentry() is the only one that you should probably use, as */
	/*  token_queue_fetch() uses them while also supporting an "unget" facility. */
	
	/* Technically you might want a lexer other than head_lex(), so this is */
	/*  how you set that override. Returns 0 for "null argument", otherwise */
	/*  returns the PREVIOUS lexer function pointer. Note that the "output" */
	/*  of your lexer needs to follow the pattern of head_lex(), so look at */
	/*  either that family of functions, or */
	/*  assemble_token()/getANDassemble_token() to understand the */
	/*  communication route and format. */
	intptr_t set_lexentry( lexentry lentry );
	
	retframe assemble_token( stackpair *stkp, void *v );
	
	/* This functions as a deallocator for only the simplest of tokens- the */
	/*  leaf tokens which don't have pointers. */
	retframe dealloc_token( stackpair *stkp, void *v );
	
	retframe getANDassemble_token( stackpair *stkp, void *v );
	
		/* Converts a ->toktype == NUMBER token into a token_uint, with the */
		/*  token text on the back. */
	retframe numbertoken_touint( stackpair *stkp, void *v );
	
	
	
	#define lexalike_ENDRETFRAME() return( (retframe){ &end_run, (void*)0 } )
	
	
	
	int token_queue_init();
	
		/* These implement an "unget" capability for tokens: use */
		/*  token_queue_fetch() if you want a token, token_queue_push() if you */
		/*  want to give it back. */
	size_t token_queue_used();
	intmax_t token_queue_fetchlock( intmax_t loc );
	int token_queue_push( token *tok );
	int token_queue_pop( token **tok );
	retframe token_queue_fetch( stackpair *stkp, void *v );
	
		/* Provides a way to temporarily store tokens that WILL be going onto */
		/*  the token's "unget" stack, for those cases when you need to reverse */
		/*  the order that they'll be pulled back off. */
	size_t token_queue_shuffleused();
	int token_queue_shufflepop( token **tok );
	int token_queue_shufflepush( token *tok );
	
	#define POP_SHUFFLE( scratchint, destptrptr,  errfunc, err,  ... ) \
		( scratchint ) = token_queue_shufflepop( destptrptr ); \
		if( !( scratchint ) ) { errfunc( ( err ),  __VA_ARGS__, &( scratchint ) ); }
		/* Old version: PUSH_SHUFFLE( scratchint, token,  errfunc, err,  ... ) */
	#define PUSH_SHUFFLE( tokptr,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = token_queue_shufflepush( (token*)( tokptr ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "token_queue_shufflepush", ( caller ), ( scratch ) ); \
				( endfunc )(); } } \
	
		/* Moves one token pointer from the "shuffle" stack to the "unget" */
		/*  stack. */
	int token_queue_shuffle2queue();
	
		/* Note: this DOES NOT attempt to deallocate any tokens, just the */
		/*  file. If you actually care, then empty out the file first. */
	int token_queue_deinit();
	
	
	
	int is_bslash( int c );
	int tokenize_char__accumulate( stackpair *stkp, void *v,  token_head *th, char *a_, char *b_ );
		/* TODO: This comment was pulled from headers.h. and reflects the */
		/*  function before it was modified to be called directly instead of */
		/*  via the ITC loop. It needs to be updated. */
		/* Another that's in lexalike.c, and roughly the whole reason for */
		/*  the file: this handles backslashes, including allowing them to */
		/*  "hide" newlines, and encode for number-encoded characters. The */
		/*  character that it "detects" gets put onto the stackpair data */
		/*  stack, currently as a char. */
	int tokenize_char( stackpair *stkp, void *v );
	
		/* A helper function to handle most of the conversion between normal */
		/*  character operations & stackpair operations. Unlike some stuff */
		/*  in this file, this should pretty much be complete. */
	/* Provided by lexalike.c. Pops a token_head from stkp->data, tests the */
	/*  length (and possibly errors out), peeks at a char on the same stack, */
	/*  pushes the token_head back on top, and returns the output of the */
	/*  provided function when given the char value. */
	lib4_result stack_testchar( stackpair *stkp, void *v,  int (*testfunc)( int ),  int fail_on_multichar );
	retframe stack_testchar2( stackpair *stkp, void *v,  int (*testfunc)( int ), char *funcname );
	
	
	
	/* None of these pop anything (technicaly they do, but then put it */
	/*  back). */
	
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
	
#endif
