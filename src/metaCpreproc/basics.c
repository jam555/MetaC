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

#include "../headers.h"

#include "err/inner_err.h"


#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		static msg_styleset errs;
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs;
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
#define NOTESPACE() STDMSG_NOTESPACE_WRAPPER( &errs )
#define HEXARG( hex ) STDMSG_HEXARG_WRAPPER( &errs, hex )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )
#define FAILEDPTRFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDPTRFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )



	/* This is the actual basal lookup table. Searching is based on */
	/*  token_head's toktype value. If an entry's reftype is *_RETFRAMEFUNC */
	/*  then it'll get used directly as the return value of the search */
	/*  function, if it's *_TABLEENTRY then it'll get paired with */
	/*  preproc_search2() as the return vector for a call to */
	/*  accumulate_token(), and any other value results in a call to a */
	/*  "seekother" retframe value: the seekother value for both *_search1() */
	/*  and *_search2() currently just end execution, but that should be */
	/*  changed in the future. */
	/* If an entry's reftype is *_RETFRAMEFUNC, then it's ref must be a */
	/*  pointer to a retframe. If the reftype is *_TABLEENTRY, then it's ref */
	/*  must be a pointer to a genname_parr, the entries of which follow the */
	/*  same rules as preproc_basic_gentyping's entries, excepting that */
	/*  search is done via text value instead of toktype. */
gentyped_parr *preproc_basic_gentyping =
&(
	(gentyped_parr)
	{
			/* The number of elements in the generictyped[] array below, */
			/*  minus one because we don't want to acknowledge the very last */
			/*  element, because it's just there to increase the chance of */
			/*  catching errors. */
		element_count,
		(generictyped[])
		{
				/* Now we should be preprocessing! Except that I need to be */
				/*  checking for "line freshness". */
			(generictyped){ TOKTYPE_OCTO, (void*)ref, reftype },
			
			/*
				GENNAMETYPE_NAMEONLY
				GENNAMETYPE_TABLEENTRY
				GENNAMETYPE_RETFRAMEFUNC
			*/
			
			(generictyped){ toktype, (void*)ref, reftype },
			(generictyped){ toktype, (void*)ref, reftype },
			
			/* New elements go here! */
			
			
				/* Do not count this one, leave it at the end as well. */
			(generictyped){ TOKTYPE_INVALID, (void*)0, GENNAMETYPE_INVALID }
		}
	}
);





retframe preproc_search1( stackpair *stkp, void *v )
{
	return( gentyped_search( stkp, preproc_basic_gentyping, &preproc_search2 ) );
}
retframe preproc_search2( stackpair *stkp, void *table_ )
{
		/* This is for the preprocessor, so if we encounter a character */
		/*  we're not looking for, then we'll just echo it. */
	static retframe seekother = (retframe){ &echo_tokens_entrypoint, (void*)0 };
	genname_parr *table = (genname_parr*)table_;
	uintptr_t a;
	
	if( !stkp || !table )
	{
		STACK_BADNULL2( preproc_search2, &stkp, &table_ );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( table->len < 1 )
	{
		BADNULL( preproc_search2, &( table_len ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	int res = peek_uintptr( stk,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "peek_uintptr", preproc_search2, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	genericnamed *found = bsearch1_gennamearr( table->body, (token*)a );
	if( !found )
	{
		FAILEDPTRFUNC( "bsearch1_gennamearr", preproc_search2, found );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	switch( found->reftype )
	{
		case GENNAMETYPE_RETFRAMEFUNC:
			/* Found a function, proceed to return it directly. */
			
			return( *( (retframe*)( found->ref ) ) );
			
		case GENNAMETYPE_TABLEENTRY:
			/* Found a lookup table; queue the string searcher as a return */
			/*  route with it as an argument, and call accumulate_token() to */
			/*  fetch a token for it to interpret. */
			
			CALL_FRAMEFUNC( &preproc_search2, (void*)( found->ref ), &accumulate_token, 0 );
		default:
			return( seekother );
	}
	
	TRESPASSPATH( preproc_search2, "preproc_search2() returned from end instead of via switch. Should have defaulted to calling:" );
		NOTELINE(); HEXARG( (uintptr_t)( seekother.func ) );
		NOTESPACE(); HEXARG( (uintptr_t)( seekother.data ) );
	return( (retframe){ &end_run, (void*)0 } );
}

retframe gentyped_search( stackpair *stkp, gentyped_parr *table, retframe (*tablehand)( stackpair*, void* ) )
{
		/* This is for the preprocessor, so if we encounter a character */
		/*  we're not looking for, then we'll just echo it. */
	static retframe seekother = (retframe){ &echo_tokens_entrypoint, (void*)0 };
	uintptr_t a;
	
	if( !stkp || !tablehand )
	{
		BADNULL2( gentyped_search, &stkp, &tablehand );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !table || table->len < 1 )
	{
		BADNULL2( gentyped_search, &table, &( table->len ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	int res = peek_uintptr( stk,  &a );
	if( !res )
	{
		FAILEDINTFUNC( "peek_uintptr", gentyped_search, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	generictyped *found = gentyped_bsearch( table, (token*)a );
	if( !found )
	{
		FAILEDPTRFUNC( "gentyped_bsearch", gentyped_search, found );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	switch( found->reftype )
	{
		case GENNAMETYPE_RETFRAMEFUNC:
			/* Found a function, proceed to return it directly. */
			
			return( *( (retframe*)( found->ref ) ) );
			
		case GENNAMETYPE_TABLEENTRY:
			/* Found a lookup table; queue the string searcher as a return */
			/*  route with it as an argument, and call accumulate_token() to */
			/*  fetch a token for it to interpret. */
			
			CALL_FRAMEFUNC( tablehand, (void*)( found->ref ), &accumulate_token, 0 );
			
		default:
			return( seekother );
	}
	
		/* Shouldn't ever get here, but whatever. */
	TRESPASSPATH( gentyped_search, "gentyped_search() returned from end instead of via switch. Should have defaulted to calling:" );
		NOTELINE(); HEXARG( (uintptr_t)( seekother.func ) );
		NOTESPACE(); HEXARG( (uintptr_t)( seekother.data ) );
	return( (retframe){ &end_run, (void*)0 } );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
