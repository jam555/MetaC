#ifndef METAC_METACPREPROC_COMPONENTS_H
 #define METAC_METACPREPROC_COMPONENTS_H
	
	/* metaCpreproc/components.h */
	/* This file exists for "component parses" that get built up into more full */
	/*  preprocessor parsing runs. */
	
	#include "../headers.h"
	
	
	
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
	
	typedef struct gennameparr_stdpushentry_entry gennameparr_stdpushentry_entry;
	struct gennameparr_stdpushentry_entry
	{
		uintptr_t toktype;
		genname_parr *table;
			/* "followup" is invoked after a searchtable has been entered, while */
			/*  "conclude" is invoked after a searchtable has been removed. Both */
			/*  need to AT LEAST ensure that the caller of the function that */
			/*  entered/removed the searchtable is returned to. */
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
	
	
	
	int searchstack_init();
	int searchstack_deinit();
	
	/* All four of these expect a pointer to a populated copy of */
	/*  gennameparr_stdpushentry_entry to be provided in v_. */
		/* This shouldn't appear directly in any search tables, but instead */
		/*  should be returned in a retframe by another wrapper function, which */
		/*  should also provide a non-null value for the void pointer in the */
		/*  process. */
	retframe components_stdsearchinit( stackpair *stkp, void *v_ );
	retframe components_sequencedsearchproceed( stackpair *stkp, void *v_ );
		/* Use this if you care about the token that caused the function to be */
		/*  called. */
	/* This one stores the pointer to the token that caused it to be called */
	/*  in the .tail element of tokenbranch that it finalizes: the */
	/*  tokenbranch itself will be pointed to by the last element of the */
	/*  tokengroup that will be pointed to by the top element of the stack */
	/*  when this function returns. v_->conclude will be run after this */
	/*  function returns, but BEFORE the caller of this function is returned */
	/*  TO. */
	retframe components_stdsearchdeinit( stackpair *stkp, void *v_ );
		/* Use this if you DON'T care about the token that caused this function */
		/*  to be called. */
	/* This function works just like *_stdsearchdeinit(), except that it */
	/*  DOESN'T store the token that caused it to be run. So, it still */
	/*  returns with a tokengroup pointer on the top of the stack, the last */
	/*  element in that tokengroup is a pointer to a tokenbranch, */
	/*  v_->conclude gets run after this function (and ALSO after a */
	/*  deallocator accessed via invoke_dealloctoken(), to dealloocate the */
	/*  no-longer useful token), but before the return to this function's */
	/*  caller, etc. Just without .tail getting populated. Use this function */
	/*  for when you want to drop a searchtable, but DON'T consider the */
	/*  triggering token (or whatever else) to be valuable. */
	retframe components_skipendersearchdeinit( stackpair *stkp, void *v_ );
	
#endif
