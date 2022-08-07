	/* ( token* -- token* ( 0 | 1 ) ) */
	/* This function MUST be given a token pointer via it's */
	/*  void argument: that's used to comunicatre WHAT is */
	/*  required. */
retframe require_match( stackpair *stkp, void *v );
	/* ( token* -- token* ( 0 | 1 ) ) */
	/* If the token pointer points to a preprocessor opening */
	/*  bracket, then 1, else 0. */
retframe require_preprocoppar( stackpair *stkp, void *v );
retframe require_preprocopsqr( stackpair *stkp, void *v );
retframe require_preprocopcrl( stackpair *stkp, void *v );
	/* Combines the results of all three above into one result. */
retframe require_preprocopener( stackpair *stkp, void *v );
	/* As with require_preprocopener(). */
retframe require_octothorp( stackpair *stkp, void *v );
	/* As with require_preprocopener(). The name DOES have */
	/*  to have a minimum length of 1. */
retframe require_anyname( stackpair *stkp, void *v );


	/* token*(a) token*(b) -- tokenbranch* */
	/* The two token pointers will be moved into the returned */
	/*  tokenbranch. "b" (the top of the stack) will be the */
	/*  body, and "a" will be the leading element. */
retframe tokenbranch_buildleadbody( stackpair *stkp, void *v );

	/* ( -- tokenbranch* ) */
retframe tokenbranch_build( stackpair *stkp, void *v );
	/* ( tokenbranch* token* -- tokenbranch* ) */
retframe tokenbranch_setlead( stackpair *stkp, void *v );
	/* ( tokenbranch* token* -- tokenbranch* ) */
retframe tokenbranch_pushbody( stackpair *stkp, void *v );
	/* ( tokenbranch* token* -- tokenbranch* token* ) */
	/* The ->subtype of the tokenbranch DOES need to be set, */
	/*  but I don't believe that it actually DOES get set */
	/*  anywhere, hence the need for this function. Note */
	/*  that the token* is NOT removed, since it is NOT */
	/*  itself stored in the tokenbranch instance. It's */
	/*  worth noting that normal tokens, tokengroups, and */
	/*  tokenbranches are all handled as the source token. */
retframe tokenbranch_setsubtype( stackpair *stkp, void *v );
	/* ( tokenbranch* token* -- tokenbranch* ) */
retframe tokenbranch_settail( stackpair *stkp, void *v );
	/* ( tokenbranch* -- ) */
retframe tokenbranch_settail( stackpair *stkp, void *v );
