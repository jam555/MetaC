#ifndef ???
 #define ???
	
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
	
	/* These are used during macro execution to encode WHAT will be */
	/*  emitted (or emitting) to the next stage of processing when the */
	/*  individual structure instance is reached. */
		/* macro_link is a reference to an argument to the macro's */
		/*  current invocation. TOKTYPE_TOKENGROUP_MACROLINK. */
	typedef struct macro_link macro_link;
		/* macro_token is a hardwired token. */
		/*  TOKTYPE_TOKENGROUP_MACROTOKEN or */
		/*  TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION. */
	typedef struct macro_token macro_token;
		/* macro_run is an invocation of another macro. */
		/*  TOKTYPE_TOKENGROUP_MACRORUN. */
	typedef struct macro_run macro_run;
		/* macro_directive is an embedded invocation of a preprocessor */
		/*  directive. TOKTYPE_TOKENGROUP_MACRODIRECTIVE. */
	typedef struct macro_directive macro_directive;
	
		/* This encodes the actual macro. It should really be renamed. */
		/*  TOKTYPE_TOKENGROUP_MACROCALL. */
	typedef struct macro_call macro_call;
	
	
	
		/* Returns positive for the four structures (technically the five token */
		/*  types) that are valid inside macro_call{}.body. */
	int isvalid_macropart( token_head *th );
	
		/* ( len -- char_parr* len token* ) */
		/* The token will not be deleted- do it yourself. */
	retframe accumulate_macroargname( stackpair *stkp, void *v,  token_head *part );
		/* ( char_parr*[len] len -- macrocllaval_parr* ) */
	retframe finalize_macroargname( stackpair *stkp, void *v );
		/* ( macrocllaval_parr* --  ) */
	retframe destroy_macroargname( stackpair *stkp, void *v );
	
		/* ( len -- token* len ) */
	retframe accumulate_macropart( stackpair *stkp, void *v,  token_head *part );
		/* The first token in the array will be the deepest token on the stack. */
		/* ( token*[len] len -- tokhdptr_parr* ) */
	retframe finalize_macropart( stackpair *stkp, void *v );
		/* ( tokhdptr_parr* -- token*[len] len ) */
		/* If you want to get rid of those tokens, just use */
		/*  bulk_dealloc_token() from token.h. They will pop in reverse order- */
		/*  the last element will be nearest the top of the stack. */
	retframe unpack_macroparts( stackpair *stkp, void *v );
	
		/* ( macrocllaval_parr* tokhdptr_parr* -- macro_call* ) */
	retframe assemble_macrocall( stackpair *stkp, void *v );
		/* ( macro_call* -- macrocllaval_parr* tokhdptr_parr* ) */
	retframe disassemble_macrocall( stackpair *stkp, void *v );
	
	
	
		/* Fetches upparens with (presumably) a string inside them, and then */
		/*  uses that to find or build search table entries for it. Note the */
		/*  use of v_, below: this can sometimes change the return stack.*/
		/* (  -- ... -- genericnamed*freshline genericnamed*inline ( 0 | 1 ) ) */
		/* v_ must point to an array of retframes: */
			/* [0]: ( char_parr* -- char_parr* flag ), a syntax checker: */
				/* if flag==0 that [1] will be called with ( char_parr* -- ... ) */
				/* if flag==1 then find_defname_conclude() will run normally. */
			/* [1]: ( char_parr* -- ... ), an error handler: */
				/* Honestly, it just does whatever. The char_parr* is the only */
				/*  data it needs to clean up (hopefully). */
	retframe find_defname( stackpair *stkp, void *v_ );
	
	
	
		/* ( char_parr* -- char_parr* flag ) */
	retframe vm_charparr_isname( stackpair *stkp, void *v_ );
		/* This one is ultimately meant for use with find_defname(). It's the */
		/*  "canonical" v_[0] implementation. */
		/* ( char_parr* -- char_parr* flag ) */
	retframe vm_charparr_ismacrosearchable( stackpair *stkp, void *v_ );
	
	
		/* ( token* -- token* ) */
	retframe accumulate_string( stackpair *stkp, void *v_ )
	
#endif
