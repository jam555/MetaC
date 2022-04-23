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
	
	
	
	tokengroup* build_tokengroup( size_t elems );
	int regrow_tokengroup
	(
		tokengroup *tgrp,
		size_t newlen
	);
	int pushto_tokengroup
	(
		tokengroup *tgrp,
		token_head *thd
	);
	int place_tokenhead( token_head **dest, token_head *tok );
	retframe dealloc_tokengroup
	(
		stackpair *stkp, void *v,
		
		tokengroup *tgrp
	);
	
	
	tokenbranch* build_tokenbranch( size_t elems );
	int set_lead_tokenbranch( tokenbranch *tb, token_head *tok );
	int push_body_tokenbranch( tokenbranch *tb, token_head *tok );
	int set_tail_tokenbranch( tokenbranch *tb, token_head *tok );
	retframe dealloc_tokenbranch
	(
		stackpair *stkp, void *v,
		
		tokenbranch *tb
	);
	
	
		/* Expects a token of type TOKTYPE_SYM_COMMENTOP or */
		/*  TOKTYPE_SYM_COMMENTLINE to be pointed to by the top of the */
		/*  stack, and will consume tokens from the token source until */
		/*  either TOKTYPE_NEWLINE (for "commentline") or */
		/*  TOKTYPE_SYM_COMMENTCL (for "commentop") are encountered. Will */
		/*  leave a pointer to a tokenbranch on top of the stack, of subtype */
		/*  TOKTYPE_TOKENGROUP_COMNTMERGE, with all of the tokens (including */
		/*  whitespace and newlines) encountered between the openning and */
		/*  closing newlines contained within the body element. */
	retframe comment_entry( stackpair *stkp, void *v );
	
	
		/* Old comment, check against the next comment, AND the source file, */
		/*  AND the the relevant devdocs file. */
	/*
		The top of the stack must be a pointer to a _SPACE, _NEWLINE, or _OTHER.
		The next elemet must be either a null pointer, or a pointer to NOT a
		_SPACE, _NEWLINE, _OTHER, or TOKTYPE_TOKENGROUP_WHITESPACE. Upon returning,
		it'll have a pointer to a TOKTYPE_TOKENGROUP_WHITESPACE on the top of the
		stack. Not actually sure if it'll work right.
	retframe accumulate_whitespace( stackpair *stkp, void *v );
	
	retframe accumulate_token( stackpair *stkp, void *v );
	
	*/
		/* You must enter with a pointer to a token_head (and, honestly, */
		/*  probably something more complex) on the top of the data stack. When */
		/*  this returns, it'll have a pointer to a non-whitespace token on the */
		/*  top of the stack, and right below might have a pointer to some sort */
		/*  of token_head-lead whitespace-related object. Accumulating that */
		/*  lower object is the point of the function, but not necessarily */
		/*  possible, depending on what tokens are actually available. */
		/* Note that the top token should NOT be a tokengroup or tokenbranch. */
		/* Addendum: tokenbranches are now allowed for comments. */
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
