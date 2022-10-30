#ifndef METAC_TOKENIZATION_SIMPLELEX_H
 #define METAC_TOKENIZATION_SIMPLELEX_H
	
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
