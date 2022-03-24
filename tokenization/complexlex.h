#ifndef METAC_TOKENIZATION_COMPLEXLEX_H
 #define METAC_TOKENIZATION_COMPLEXLEX_H
	
	
	
	#include "headers.h"
	
	
	
	typedef struct tokengroup tokengroup;
	typedef struct tokenbranch tokenbranch;
	
	
	
	struct tokengroup
	{
		token_head header;
			/* Takes the same values as token_head.toktype */
		uintptr_t subtype;
		
		tokhdptr_parr *arr;
		size_t used;
	};
	struct tokenbranch
	{
		token_head header;
		uintptr_t subtype;
		
		token_head *lead, *body, *tail;
	};
	
	
	
	retframe set_dealloctoken( retframe dealc_ );
	retframe invoke_dealloctoken( stackpair *stkp, void *v );
	retframe smart_dealloc_token( stackpair *stkp, void *v );
	
	
	
	tokengroup* build_tokengroup
	(
		uintptr_t *refid,
		
		size_t elems
	);
	int regrow_tokengroup
	(
		uintptr_t *refid,
		
		tokengroup *tgrp,
		size_t newlen
	);
	int pushto_tokengroup
	(
		uintptr_t *refid,
		int err_subsource,
		
		tokengroup *tgrp,
		token_head *thd
	);
	int place_tokenhead( uintptr_t *refid, token_head **dest, token_head *tok );
	retframe dealloc_tokengroup
	(
		stackpair *stkp, void *v,
		
		uintptr_t *refid,
		int err_source,
		int err_subsource,
		
		tokengroup *tgrp
	);
	
	
	tokenbranch* build_tokenbranch
	(
		uintptr_t *refid,
		
		size_t elems
	);
	int set_lead_tokenbranch( uintptr_t *refid,  tokenbranch *tb, token_head *tok );
	int push_body_tokenbranch( uintptr_t *refid,  tokenbranch *tb, token_head *tok );
	int set_tail_tokenbranch( uintptr_t *refid,  tokenbranch *tb, token_head *tok );
	retframe dealloc_tokenbranch
	(
		stackpair *stkp, void *v,
		
		uintptr_t *refid,
		int err_source,
		int err_subsource,
		
		tokenbranch *tb
	);
	
	
	/* Note: build some functions to properly accumulate a token. That should */
	/*  include te whitespace AFTER the token, which we now have the function */
	/*  for. */
	
		/* You must enter with a pointer to a token_head (and, honestly, */
		/*  probably something more complex) on the top of the data stack. When */
		/*  this returns, it'll have a pointer to a non-whitespace token on the */
		/*  top of the stack, and right below might have a pointer to some sort */
		/*  of token_head-lead whitespace-related object. Accumulating that */
		/*  lower object is the point of the function, but not necessarily */
		/*  possible, depending on what tokens are actually available. */
		/* Note that the top token should NOT be a tokengroup or tokenbranch. */
	retframe accumulate_whitespace( stackpair *stkp, void *v );
		/* This function expects an already ASSEMBLED token to be pointed to by */
		/*  a pointer on the top of the data stack. It will then either directly */
		/*  defer to accumulate_whitespace() (if that token is a SPACE, NEWLINE, */
		/*  or OTHER), or else shove it into a tokenbranch, push a return to */
		/*  conclude_accumulate_token(), then call getANDassemble_token() with a */
		/*  return to accumulate_whitespace(). accumulate_whitespace() will then */
		/*  loop (accumulating whitespace as it goes) until it finds something */
		/*  that isn't whitespace, at which point it will return via the */
		/*  conclude_accumulate_token() reference that was pushed onto the */
		/*  return stack. */
	retframe accumulate_token( stackpair *stkp, void *v );
	retframe conclude_accumulate_token( stackpair *stkp, void *v );
	
#endif
