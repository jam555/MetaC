	/* This function MUST be given a token pointer via it's */
	/*  void argument: that's used to comunicatre WHAT is */
	/*  required. */
retframe require_match( stackpair *stkp, void *v );
	/* ( token* -- 0 | 1 ) */
	/* If the token pointer points to a preprocessor opening */
	/*  bracket, then 1, else 0. */
retframe require_preprocopener( stackpair *stkp, void *v );
