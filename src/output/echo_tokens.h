#ifndef METAC_OUTPUT_ECHO_TOKENS_H
 #define METAC_OUTPUT_ECHO_TOKENS_H
	
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
	
	/* output/echo_tokens.h */
	
	
	
	int echo_tokenhead( stackpair *stkp, void *v,  token_head **th );
	
		/* This dispatches to other functions (eg. echo_tokengroup) to do the */
		/*  work, and just exists for the sake of that very dynamic dispatch. */
	retframe echo_tokens_entrypoint( stackpair *stkp, void *v );
	
	/* These all expect a pointer to a token on the top of the stack (as */
	/*  does echo_tokens_entrypoint() ). They eventually terminate with the */
	/*  effect of an ordinary system return. */
	retframe echo_token( stackpair *stkp, void *v );
	retframe echo_tokennum( stackpair *stkp, void *v );
	retframe echo_tokengroup( stackpair *stkp, void *v );
	retframe echo_tokenbranch( stackpair *stkp, void *v );
	
	retframe echo_tokengroup_extension( stackpair *stkp, void *v );
	
	retframe echo_tokenbranch_conclude( stackpair *stkp, void *v );
	retframe echo_tokenbranch_tail( stackpair *stkp, void *v );
	retframe echo_tokenbranch_body( stackpair *stkp, void *v );
	
	
	
	retframe emit_C_stringifier( stackpair *stkp, void *v );
	retframe emit_C_concatenizer( stackpair *stkp, void *v );
	
#endif
