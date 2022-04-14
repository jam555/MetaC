#ifndef METAC_METACPREPROC_BASICS_H
 #define METAC_METACPREPROC_BASICS_H
	
	/* metaCpreproc/basics.h */
	
	#include "../headers.h"
	
	#include "err/inner_err.h"
	
	
	retframe preproc_search1( stackpair *stkp, void *v );
	retframe preproc_search2( stackpair *stkp, void *table_ );
	
	retframe gentyped_search( stackpair *stkp, gentyped_parr *table, retframe (*tablehand)( stackpair*, void* ) );

#endif
