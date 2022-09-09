

	/* This takes a token pointer on the stack, copies it's text */
	/*  value into a char_parr, pushes the pointer to that array */
	/*  under the token pointer, and then returns into the */
	/*  invoke_dealloctoken() vm function. */
	/* v_ must be a pointer to a uintptr_t that has been */
	/*  initialized according to the SETJUMP vm macro. This is */
	/*  used with the longjump vm function to report some */
	/*  errors. When this happens, underneath the setjump flag */
	/*  will be a pointer to token2char_parr, and beneath that */
	/*  the number 0. This is to make it easier for the */
	/*  user-programmer to make use of the same mechanism for */
	/*  their own purposes. */
		/* Error 0: The provided token was not a standard or */
		/*  simple type, and therefore has no simple text */
		/*  form. */
	/* ( token* -- char_parr* ) */
retframe token2char_parr( stackpair *stkp, void *v_ );

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
