#include "headers.h"



	/* Negative are errors, 0 is non-match, 1 is full match, 2 is length-limited match. */
int match_token( token *tok,  unsigned char *str, size_t len );



	/* For use with qsort & bsearch. */
int stdcompare_str( char *a_, char *b_, size_t len );
int stdcompare_token( const void *key_, const void *elem_ );
int stdcompare_genericnamed( const void *a_, const void *b_ );



genname_parr* build_gennamearr( size_t count, uintptr_t *errkey );
genname_parr* resize_gennamearr( genname_parr *parr, size_t newlen );
void destroy_gennamearr( genname_parr *parr,  void (*release_ref)( genericnamed* ) );

	/* These apply qsort/bsearch to genname_parr. */
void qsort_gennamearr( genname_parr *parr );
	/* tok->text must point to an actual string of text. */
genericnamed* bsearch1_gennamearr( genname_parr *parr, token *tok );


	/* Pushes pushable onto the end of keys. Negative is error, positive is success. */
int lexparse1_pushsearchtable( uintptr_t *refid, genname_parrparr **keys, size_t *keys_used, int err_subsource, int errsubsub, genname_parr *pushable );
	/* Obvious purpose, same returns as pushing. */
int lexparse1_popsearchtable( uintptr_t *refid, genname_parrparr **keys, size_t *keys_used, int err_subsource, int errsubsub );
	/* A token that can be given to bsearch1_gennamearr() must be on the top */
	/*  of the stack. Bsearch will search through keys[][], and the function */
	/*  then either returns the result (if the genericnamed->reftype says */
	/*  that the result is a retframe), pushes the result onto the end of */
	/*  keys[][] and returns the "return retframe" from the stack (if */
	/*  ->reftype says that the result is a table), or returns seekother in */
	/*  other cases... */
	/*  Unless in case of error, in which case a standard end_run() retframe */
	/*  gets returned. */
/****************************************************************************/
/****************************************************************************/
/** !!! WARNING !!! The handler functions must remove the tokens they use, **/
/**  *_tokensearch() DOES NOT do so! ****************************************/
/****************************************************************************/
/****************************************************************************/
retframe lexparse1_tokensearch
(
		/* Was a stackframe*, but the revised retrn macro needs the pair. */
	stackpair *stkp,
	
	uintptr_t *refid,
	
	genname_parrparr **keys,
	size_t *keys_used,
	
	int err_subsource,
	
	retframe seekother
);
