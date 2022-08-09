
retframe common_macrodispatch( stackpair *stkp, void *v )
retframe common_opdispatch( stackpair *stkp, void *v )


	/* This one function will replace both of those above, */
	/*  by the magic of dispatch and recursion! */
retframe enter_try_directive( stackpair *stkp, void *v );
	/* ( token* -- ... ) -> ( ... -- tokenbranch* ) */
	/* This requires the appropriate token to ALREADY have been */
	/*  confirmed, it doesn't do ANY such thing itself. */
	/* Specifically handles "(^" and it's matching "^)". */
retframe enter_try_upparopen( stackpair *stkp, void *v );
