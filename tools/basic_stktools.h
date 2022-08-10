
	/* Copies an array of retframe instances onto the return */
	/*  stack, and immediately begins executing them in order, */
	/*  starting with the one at the start of the array. */
	/* Note that v must be a pointer to a pascal array of */
	/*  retframes: these are the retframes that will be pushed */
	/*  onto the stack, and ->body[ 0 ] will be the first one */
	/*  executed. */
	/* This function was directly inspired by (but not used */
	/*  in) require_preprocopener() from basic_toktools.c, */
	/*  where it would have been useful. Think of it as a */
	/*  variation on direct threaded code. */
retframe enqueue_returns( stackpair *stkp, void *v );

	/* ( uintptr_t*a -- uintptr_t*a uintptr_t*a ) */
retframe dup( stackpair *stkp, void *v );
	/* ( uintptr_t -- ) */
retframe drop( stackpair *stkp, void *v );

	/* ( uintptr_t*a uintptr_t*b -- uintptr_t*b uintptr_t*a ) */
retframe swap2nd( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t uintptr_t*b -- uintptr_t*b uintptr_t uintptr_t*a ) */
retframe swap3rd( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t uintptr_t uintptr_t*b -- uintptr_t*b uintptr_t uintptr_t uintptr_t*a ) */
retframe swap4th( stackpair *stkp, void *v );


	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe and2( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe and3( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe and4( stackpair *stkp, void *v );

	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe ior2( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe ior3( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe ior4( stackpair *stkp, void *v );

	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe xor2( stackpair *stkp, void *v );

	/* ( uintptr_t -- uintptr_t ) */
	/* Requires a pointer to a retframe as v. */
retframe run_if( stackpair *stkp, void *v );
retframe run_else( stackpair *stkp, void *v );
