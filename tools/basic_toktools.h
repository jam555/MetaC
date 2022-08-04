	/* ( token* -- token* ( 0 | 1 ) ) */
	/* This function MUST be given a token pointer via it's */
	/*  void argument: that's used to comunicatre WHAT is */
	/*  required. */
retframe require_match( stackpair *stkp, void *v );
	/* ( token* -- token* ( 0 | 1 ) ) */
	/* If the token pointer points to a preprocessor opening */
	/*  bracket, then 1, else 0. */
retframe require_preprocopener( stackpair *stkp, void *v );
	/* As with require_preprocopener(). */
retframe require_octothorp( stackpair *stkp, void *v );
	/* As with require_preprocopener(). The name DOES have */
	/*  to have a minimum length of 1. */
retframe require_anyname( stackpair *stkp, void *v );
