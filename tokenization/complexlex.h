#ifndef METAC_TOKENIZATION_COMPLEXLEX_H
 #define METAC_TOKENIZATION_COMPLEXLEX_H
	
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
	
	
	
		/* ( -- ) */
		/* Returns 1 on validity. Errors: */
			/* -1: Null argument. */
			/* -2: Unrecognized header token type. */
			/* -3: Uninitialized ->arr element. */
			/* -4: ->used higher than ->arr->len. */
	int validate_tokengroup( tokengroup *tg );
		/* The return will have the header toktype set to samemerge, */
		/*  header length set to the length of the non-header part, */
		/*  subtype set to invalid, arr set to an array of the */
		/*  specified element count, and used set to 0. */
	tokengroup* build_tokengroup( size_t elems );
	int regrow_tokengroup
	(
		tokengroup *tgrp,
		size_t newlen
	);
		/* Pushes thd to the end of tgrp. */
		/* Returns 1 on success. Errors: */
			/* -1: One of the arguments was null. */
			/* -2: Couldn't grow the group's array when needed. */
			/* -3: The group's array supposedly grew, but still reports too small a size. */
	int pushto_tokengroup
	(
		tokengroup *tgrp,
		token_head *thd
	);
		/* popfront_*() is slow, TRY to use popfrom_*() (which */
		/*  removes from the back) instead. */
	tokenheadptr_result popfront_tokengroup( tokengroup *tgrp );
	tokenheadptr_result popfrom_tokengroup( tokengroup *tgrp );
	int place_tokenhead( token_head **dest, token_head *tok );
	retframe dealloc_tokengroup
	(
		stackpair *stkp, void *v,
		
		tokengroup *tgrp
	);
	int lengthof_tokengroup( tokengroup *tg );
	
		/* ( -- tokengroup* ) */
		/* Note: the STANDARD deallocator will release */
		/*  tokengroup{}, no need for anything special. */
		/* VM wrapper for build_tokengroup() with element count of 0. */
	retframe vm_buildempty_tokengroup( stackpair *stkp, void *v );
		/* ( tokengroup* token_head* -- tokengroup* token_head* ) */
		/* Sets just tokengroup->subtype from token_head->toktype. */
	retframe vm_setsubtype_tokengroup( stackpair *stkp, void *v );
			/* ( tokengroup* token_head* -- tokengroup* ) */
			/* VM wrapper for pushto_tokengroup(). */
	retframe vm_pushto_tokengroup( stackpair *stkp, void *v );
		/* ( tokengroup* -- tokengroup* token* ) */
		/* VM wrapper for popfront_tokengroup(). */
	retframe vm_popfront_tokengroup( stackpair *stkp, void *v );
		/* ( tokengroup* -- tokengroup* token* ) */
		/* VM wrapper for popfrom_tokengroup(). */
	retframe vm_popfrom_tokengroup( stackpair *stkp, void *v );
		/* ( tokengroup* -- tokengroup* length ) */
		/* VM wrapper for lengthof_tokengroup(). */
	retframe vm_lengthof_tokengroup( stackpair *stkp, void *v );
	typedef struct vm_flexbuild_tokengroup_args
	{
		uintptr_t *toktype;
		refed_pstr **src;
		uintmax_t *line, *column;
		
		uintptr_t *subtype;
		
			/* The array will NOT be deallocated, just shallow-copied. */
		tokhdptr_parr *arr;
		size_t used;
		
	} vm_flexbuild_tokengroup_args;
	#define FORMAT_FLEXBUILD_TOKENGROUP_ARGS( toktype,  src, line, col,  subtype,  srcarr, srccount ) \
		(vm_flexbuild_tokengroup_args){ \
			(uintptr_t*)(toktype), \
			(refed_pstr**)(src), (uintmax_t*)(line), (uintmax_t*)(col), \
			(uintptr_t*)(subtype), \
			(tokhdptr_parr*)(srcarr), (size_t)(srccount) }
		/* (  -- tokengroup* ) */
		/* Note: v_ MUST BE a pointer to a vm_flexbuild_tokengroup_args{}. */
		/*  For reasons of code cleanliness, this WILL NOT load the */
		/*  elements from v_->arr into the tokengroup{}*. */
	retframe vm_flexbuild_tokengroup( stackpair *stkp, void *v_ );
		/* ( tokengroup* -- tokengroup* ) */
		/* Note: v_ MUST BE a pointer to a vm_flexbuild_tokengroup_args{}. */
		/* As a complement to vm_flexbuild_tokengroup(), this will ONLY load */
		/*  the elements from v_->arr into the tokengroup{}* */
	retframe vm_pack_tokengroup( stackpair *stkp, void *v_ )
	
	
	tokenbranch* build_tokenbranch( size_t elems );
	int set_lead_tokenbranch( tokenbranch *tb, token_head *tok );
	int push_body_tokenbranch( tokenbranch *tb, token_head *tok );
	tokenheadptr_result popfront_body_tokenbranch( tokenbranch *tb );
	tokenheadptr_result pop_body_tokenbranch( tokenbranch *tb );
	int set_tail_tokenbranch( tokenbranch *tb, token_head *tok );
	retframe dealloc_tokenbranch
	(
		stackpair *stkp, void *v,
		
		tokenbranch *tb
	);
	int lengthof_body_tokenbranch( tokenbranch *tb );
	
	retframe vm_push_body_tokenbranch( stackpair *stkp, void *v );
	retframe vm_popfront_body_tokenbranch( stackpair *stkp, void *v );
	retframe vm_pop_body_tokenbranch( stackpair *stkp, void *v );
	retframe vm_lengthof_body_tokenbranch( stackpair *stkp, void *v );
	
	
		/* Expects a token of type TOKTYPE_SYM_COMMENTOP or */
		/*  TOKTYPE_SYM_COMMENTLINE to be pointed to by the top of the */
		/*  stack, and will consume tokens from the token source until */
		/*  either TOKTYPE_NEWLINE (for "commentline") or */
		/*  TOKTYPE_SYM_COMMENTCL (for "commentop") are encountered. Will */
		/*  leave a pointer to a tokenbranch on top of the stack, of subtype */
		/*  TOKTYPE_TOKENGROUP_COMNTMERGE, with all of the tokens (including */
		/*  whitespace and newlines) encountered between the openning and */
		/*  closing newlines contained within the body element. */
		/* The calls to this, and eventually the code itself, will eventually */
		/*  be removed: it belongs in the MetaC compiler, but not the */
		/*  preprocessor. */
	retframe comment_entry( stackpair *stkp, void *v );
	
	
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
