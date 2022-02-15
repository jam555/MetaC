#ifndef METAC_INPUT_LEXALIKE_H
 #define METAC_INPUT_LEXALIKE_H
	
	/* This file started as lexalike.c. It's been modified some, but some of */
	/*  these functions might not belong here, and other definitely DON'T */
	/*  belong here. */
	
	
	
	#include "stdmonads.h"
	#include "headers.h"
	
	
	
	/* The purpose of lexalike.c is to provide wrappers for charin(), */
	/*  isspace(), etc., but which convert stuff to tokens so that there's */
	/*  an easier way to support character delimiting, by moving it before */
	/*  token lexing and assembly. */
	/* Note that support for trigraphs belongs in this file. */
	/* There's no deep reason for the still_freshline, lexentry, and */
	/*  assemble_token stuff to be here, but it makes about as much sense as */
	/*  sticking it elsewhere. */
	
	
	int is_freshline();
	
	
	
	intptr_t set_lexentry( lexentry lentry );
	retframe assemble_token( stackpair *stkp, void *v );
	retframe dealloc_token( stackpair *stkp, void *v );
	retframe getANDassemble_token( stackpair *stkp, void *v );
	
	int token_queue_init();
	
	size_t token_queue_used();
	int token_queue_push( token *tok );
	int token_queue_pop( token **tok );
	retframe token_queue_fetch( stackpair *stkp, void *v );
	
	size_t token_queue_shuffleused();
	int token_queue_shufflepush( token *tok );
	int token_queue_shufflepop( token **tok );
	
	int token_queue_shuffle2queue();
	
		/* Note: this DOES NOT attempt to deallocate any tokens, just the */
		/*  file. If you actually care, then empty out the file first. */
	int token_queue_deinit();
	
	
	
	lib4_intresult bin2num( char c );
	lib4_intresult oct2num( char c );
	lib4_intresult dec2num( char c );
	lib4_intresult hexa2num( char c );
		/* Roughly the system of the ILLIAC 1, the first private */
		/*  (particularly college) owned computer. Yes, it seems odd, but it */
		/*  was very early, and the letters may have started as */
		/*  abbreviations. */
		/* Probably worth noting that this isn't actually used: I just put */
		/*  it in because it came to mind (blame Dave's Garage on Youtube). */
	lib4_intresult sexa2num( char c );
	
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
	lib4_result stack_testchar( stackpair *stkp, void *v,  int (*testfunc)( int ),  int fail_on_multichar );
	retframe stack_testchar2( stackpair *stkp, void *v,  int (*testfunc)( int ), char *funcname );
	
	int popas_extrachar( stackpair *stkp, void *v,  extrachar *ec );
	
	extrachar_result get_extrachar( stackpair *stkp, void *v );
	extrachar_result peek_extrachar( stackpair *stkp, void *v );
	int unget_extrachar( extrachar ec );
	
	
	/* None of these pop anything (technicaly they do, but then put it */
	/*  back). */
	
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
