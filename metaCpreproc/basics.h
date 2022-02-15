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
	
	
	
	
	
	/* These macroargs_*() functions, plus some stuff in the .c file, define */
	/*  an entire additional stack system. It likely belongs in it's own */
	/*  file pair. */
	
	int macroargs_init();
	int macroargs_resize( int deltaMacros );
	int macroargs_clear();
	
	int macroargs_pushset( tokhdptr_parr *val );
	int macroargs_popset( tokhdptr_parr **dest );
	int macroargs_peekset( size_t off,  tokhdptr_parr **dest );
	
	#define PEEK_MACROARGS( offset, var,  errfunc, err1, err2,  ... ) \
		if( !macroargs_peekset( ( offset ),  &( var ) ) ) { errfunc( ( err1 ),  __VA_ARGS__, &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__, &( var ) ); }
	#define POP_MACROARGS( destptr,  errfunc, err1, err2,  ... ) \
		if( !macroargs_popset( destptr ) ) { errfunc( ( err1 ),  __VA_ARGS__, &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__, &( var ) ); }
	#define PUSH_MACROARGS( val,  errfunc, err,  ... ) \
		if( !macroargs_pushset( val ) ) { errfunc( ( err ),  __VA_ARGS__, ( val ) ); }
	
	
		/*
				shufflecount shuffle:token*[shufflecount]
			--
				arglist*
		*/
	int pack_arglist( stackpair *stkp,  uintptr_t *refid, int errgate );
	retframe vm_pop_macroarg( stackpair *stkp, void *v );

#endif
