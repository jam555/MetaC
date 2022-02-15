#ifndef METAC_METACPREPROC_BASICS_H
 #define METAC_METACPREPROC_BASICS_H
	
	/* metaCpreproc/basics.h */
	
	#include "../headers.h"
	
	
	int gentyped_tokencomp( const void *key_, const void *elem_ );
	int gentyped_gencomp( const void *key1_, const void *key2_ );
	
	
		/* Both of these are wrappers for standard C library functions. */
	void gentyped_qsort( gentyped_parr *parr );
	generictyped* gentyped_bsearch( gentyped_parr *parr, token *tok );
	
	
	
	retframe preproc_search1( stackpair *stkp, void *v );
	retframe preproc_search2( stackpair *stkp, void *table_ );
	
	retframe gentyped_search( stackpair *stkp, gentyped_parr *table, retframe (*tablehand)( stackpair*, void* ) );

#endif
