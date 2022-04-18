#ifndef METAC_TOKENMATCHING_H
 #define METAC_TOKENMATCHING_H
	
	/* tokenmatching.h */
	
	#include "headers.h"
	
	
	
	typedef struct genericnamed genericnamed;
	struct genericnamed
	{
		char_pascalarray *name;
		void *ref;
		uintptr_t reftype;
	};
		/* The default state, doesn't actually represent anything. */
	#define GENNAMETYPE_INVALID ( 0 )
		/* Used for tracking includes. */
	#define GENNAMETYPE_NAMEONLY ( 1 )
		/* ->ref points to a lookup table to be used for further searches. */
		/*  Usually it gets pushed onto a stack. */
	#include GENNAMETYPE_TABLEENTRY ( 2 )
		/* ->ref points to a retframe to be called/run. */
	#include GENNAMETYPE_RETFRAMEFUNC ( 3 )
	
	
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( genericnamed_, genericnamed );
	typedef genericnamed_pascalarray genname_parr;
	
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( genericnamedparr_, genname_parr* );
	typedef genericnamedparr_pascalarray genname_parrparr;
	
	typedef struct genericnamed_retframe genericnamed_retframe;
	struct genericnamed_retframe
	{
		genericnamed name;
		retframe frame;
	};
		/* Note: obj must be a pointer to the genericnamed_retframe that the */
		/*  resulting value will be stored inside of. Predefine to match. */
	#define BUILD_genericnamed_retframe( obj,  name_cparr, func, vdata ) \
		(genericnamed_retframe){ \
			(genericnamed){ \
				(name_cparr), \
				(void*)&( ( obj )->frame ), \
				(uintptr_t)&genericnamed_retframe_refid }, \
			(retframe){ (func), (vdata) } }
	
	typedef struct gennameparr_stdpushentry_entry
		gennameparr_stdpushentry_entry;
	struct gennameparr_stdpushentry_entry
	{
		uintptr_t toktype;
		genname_parr *table;
			/* "followup" is invoked after a searchtable has been entered, */
			/*  while "conclude" is invoked after a searchtable has been */
			/*  removed. Both need to AT LEAST ensure that the caller of the */
			/*  function that entered/removed the searchtable is returned */
			/*  to. */
		retframe followup, conclude;
	};
	
	
	
	typedef struct generictyped generictyped;
	struct generictyped
	{
		uintptr_t toktype;
		void *ref;
			/* ->reftype uses the same values as in struct genericnamed. */
		uintptr_t reftype;
	};
	
	typedef struct generictyped_retframe generictyped_retframe;
	struct generictyped_retframe
	{
		generictyped name;
		retframe frame;
	};
	
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( generictyped_, generictyped );
	typedef generictyped_pascalarray gentyped_parr;
	
	
	
		/* Negative are errors, 0 is non-match, 1 is full match, 2 is */
		/*  length-limited match. */
	int match_token( token *tok,  unsigned char *str, size_t len );
	
	
	
		/* For use with qsort & bsearch. */
	int stdcompare_str( char *a_, char *b_, size_t len );
	int stdcompare_token( const void *key_, const void *elem_ );
	int stdcompare_genericnamed( const void *a_, const void *b_ );
	
	
	
	genname_parr* build_gennamearr( size_t count );
	genname_parr* resize_gennamearr( genname_parr *parr, size_t newlen );
	void destroy_gennamearr
	(
		genname_parr *parr,
		
		void (*release_ref)( genericnamed* )
	);
	
		/* These apply qsort/bsearch to genname_parr. */
	void qsort_gennamearr( genname_parr *parr );
		/* tok->text must point to an actual string of text. */
	genericnamed* bsearch1_gennamearr( genname_parr *parr, token *tok );
	
	
	
		/* Both of these are wrappers for standard C library functions. */
	void gentyped_qsort( gentyped_parr *parr );
	generictyped* gentyped_bsearch( gentyped_parr *parr, token *tok );
	
	
	
		/* Pushes pushable onto the end of keys. Negative is error, positive */
		/*  is success. */
	int lexparse1_pushsearchtable
	(
		genname_parrparr **keys,
		size_t *keys_used,
		
		genname_parr *pushable
	);
		/* Obvious purpose, same returns as pushing. */
	int lexparse1_popsearchtable( genname_parrparr **keys, size_t *keys_used );
		/* A token that can be given to bsearch1_gennamearr() must be on the */
		/*  top of the stack. Bsearch will search through keys[][], and the */
		/*  function then either returns the result (if the */
		/*  genericnamed->reftype says that the result is a retframe), */
		/*  pushes the result onto the end of keys[][] and returns the */
		/*  "return retframe" from the stack (if ->reftype says that the */
		/*  result is a table), or returns seekother in other cases... */
		/*  Unless in case of error, in which case a standard end_run() */
		/*  retframe gets returned. */
	/*************************************************************************/
	/*************************************************************************/
	/** !!! WARNING !!! The handler functions must remove the tokens they ****/
	/**  use, *_tokensearch() DOES NOT do so! ********************************/
	/*************************************************************************/
	/*************************************************************************/
	retframe lexparse1_tokensearch
	(
			/* Was a stackframe*, but the revised retrn macro needs the */
			/*  pair. */
		stackpair *stkp,
		
		genname_parrparr **keys,
		size_t *keys_used,
		
		retframe seekother
	);
	
#endif
