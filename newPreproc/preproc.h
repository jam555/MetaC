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

	/* .th.toktype must == TOKTYPE_CONTEXTSPECIALS */
typedef struct context_specials
{
	token_head th;
	
	tokhdptr_parr *breaks;
	token_head *start, *end;
	
} context_specials;
lib4_intresult not_contextspecials( token_head *tok_, context_specials *ctx );
	
	/* This REQUIRES a context_specials{}* to be provided to v_. It will */
	/*  return a sequence of tokens, themselves additional tokengroup{}s, */
	/*  each representing a separate argument or similar. These may have */
	/*  macro or directive invocation tokens mixed in, as most macros & */
	/*  tokens are expected to NOT be invoked at this early point in the */
	/*  parsing process. */
	/* (  -- tokengroup* ) */
retframe bracketgather_enter( stackpair *stkp, void *v_ );

	/* This calls bracketgather_enter(), use it instead. */
	/* ( token* -- token* | tokengroup* ) */
retframe bracketgather_dispatcher( stackpair *stkp, void *v )

