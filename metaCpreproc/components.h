#ifndef METAC_METACPREPROC_COMPONENTS_H
 #define METAC_METACPREPROC_COMPONENTS_H
	
	/* metaCpreproc/components.h */
	
	#include "../headers.h"
	
	/* This file exists for "component parses" that get built up into more full */
	/*  preprocessor parsing runs. */
	
	
	uintptr_t searchstack_refid;
	genname_parrparr *searchstack = (genname_parrparr*)0;
	size_t searchstack_used = 0;
	genname_parr default =
	{
		(size_t)???, /* Length. */
		(genericnamed[])
		{
			(genericnamed){ (char_pascalarray *)0, (void*)0, (uintptr_t)0 },
			(genericnamed){ (char_pascalarray *)0, (void*)0, (uintptr_t)0 },
			...
		}
	};
	
	int searchstack_init();
	int searchstack_deinit();
	
		/* This shouldn't appear directly in any search tables, but instead */
		/*  should be returned in a retframe by another wrapper function, which */
		/*  should also provide a non-null value for the void pointer in the */
		/*  process. */
	retframe components_stdsearchinit( stackpair *stkp, void *v_ );
	retframe components_sequencedsearchproceed( stackpair *stkp, void *v_ );
		/* Use this if you care about the token that caused the function to be */
		/*  called. */
	retframe components_stdsearchdeinit( stackpair *stkp, void *v_ );
		/* Use this if you DON'T care about the token that caused this function */
		/*  to be called. */
	retframe components_skipendersearchdeinit( stackpair *stkp, void *v_ );
	
#endif
