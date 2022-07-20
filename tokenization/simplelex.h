#ifndef METAC_TOKENIZATION_SIMPLELEX_H
 #define METAC_TOKENIZATION_SIMPLELEX_H
	
	#include "headers.h"
	
	
	
	retframe head_lex( stackpair *stkp, void *v );
	
	retframe space_lex( stackpair*, void* );
	retframe newline_lex( stackpair*, void* );
	
	retframe name_lex( stackpair*, void* );
	
	
	retframe numberentry_lex( stackpair *stkp, void *v );
	
	retframe numberbinary_lex( stackpair*, void* );
	retframe numberoctal_lex( stackpair *stkp, void *v );
	retframe numberdecimal_lex( stackpair *stkp, void *v );
	retframe numberhexadecimal_lex( stackpair *stkp, void *v );
	
	
	retframe brackop_lex( stackpair *stkp, void *v );
	
	
	retframe str_lex( stackpair *stkp, void *v );
	retframe escstr_lex( stackpair *stkp, void *v );
	
	
	retframe syms_lex( stackpair *stkp, void *v );
	retframe symssinglet_lex( stackpair *stkp, void *arg, token_head *th, char a, extrachar b );
	retframe symsext_lex( stackpair *stkp, void *arg, token_head *th, extrachar a, extrachar b );
	
	
	retframe at_lex( stackpair *stkp, void *v );
	
#endif
