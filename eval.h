
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
