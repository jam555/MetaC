#ifndef METAC_OUTPUT_ECHO_TOKENS_H
 #define METAC_OUTPUT_ECHO_TOKENS_H
	
	/* output/echo_tokens.h */
	
	
	
	int echo_tokenhead( stackpair *stkp, void *v,  token_head **th );
	
		/* This dispatches to other functions (eg. echo_tokengroup) to do the */
		/*  work, and just exists for the sake of that very dynamic dispatch. */
	retframe echo_tokens_entrypoint( stackpair *stkp, void *v );
	
	/* These all expect a pointer to a token on the top of the stack (as */
	/*  does echo_tokens_entrypoint() ). They eventually terminate with the */
	/*  effect of an ordinary system return. */
	retframe echo_token( stackpair *stkp, void *v );
	retframe echo_tokengroup( stackpair *stkp, void *v );
	retframe echo_tokenbranch( stackpair *stkp, void *v );
	
	retframe echo_tokengroup_extension( stackpair *stkp, void *v );
	
	retframe echo_tokenbranch_conclude( stackpair *stkp, void *v );
	retframe echo_tokenbranch_tail( stackpair *stkp, void *v );
	retframe echo_tokenbranch_body( stackpair *stkp, void *v );
	
#endif
