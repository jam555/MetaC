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

???
/* token2char_parr() was formerly declared here, but it's been */
/*  moved to token.h/.c, and reworked: the second argument must */
/*  now point to a retframe(), with a slightly changed stack */
/*  format. If SETJUMP() and longjump() are to be used, then */
/*  they must be used via that retframe{}. Also, it no longer */
/*  removes the token pointer, nor deletes the token. */

	/* Repacks the text form of the tokens in the body of a */
	/*  tokenbranch into a macrocllaval_parr. This is used when */
	/*  declaring the argument name4s for a macro. The token at */
	/*  the very start of the tokenbranch should be on the stack */
	/*  before entry to this function. */
	/* ( tokenbranch* token* -- stuff happens... -- tokenbranch* macrocllaval_parr* ) */
retframe tokens2macrocllaval_parr( stackpair *stkp, void *v );







retframe common_macrodispatch( stackpair *stkp, void *v )
retframe common_opdispatch( stackpair *stkp, void *v )






	/* This one function will replace both of those above, */
	/*  by the magic of dispatch and recursion! */
retframe enter_try_directive( stackpair *stkp, void *v );
	/* ( token* -- ... ) -> ( ... -- tokenbranch* ) */
	/* This requires the appropriate token to ALREADY have been */
	/*  confirmed, it doesn't do ANY such thing itself. */
	/* Specifically handles "(^" and it's matching "^)". */
retframe enter_try_pardefopen( stackpair *stkp, void *v );
