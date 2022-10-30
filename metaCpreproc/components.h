#ifndef METAC_METACPREPROC_COMPONENTS_H
 #define METAC_METACPREPROC_COMPONENTS_H
	
/*
MetaC Preprocessor - A macro processor for preprocessing usage.
Copyright (C) 2022 Jared A. Maddox

This program is free software; you can redistribute it and/or modify it
under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the:
	Free Software Foundation, Inc.,
	59 Temple Place, Suite 330,
	Boston, MA 02111-1307 USA
*/
	
	/* metaCpreproc/components.h */
	/* This file exists for "component parses" that get built up into more full */
	/*  preprocessor parsing runs. */
	
	#include "../headers.h"
	
	
	
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
