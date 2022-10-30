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
	
	
	
	struct macro_link
	{
			/* TOKTYPE_TOKENGROUP_MACROLINK. */
		token_head header;
		
			/* This identifies which element in the argument list should be */
			/*  used. */
		size_t args_offset;
	};
	struct macro_token
	{
			/*  TOKTYPE_TOKENGROUP_MACROTOKEN or */
			/*  TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION. */
		token_head header;
		
		token_head *tok;
	};
	struct macro_run
	{
			/* TOKTYPE_TOKENGROUP_MACRORUN. */
		token_head header;
		
		macro_call *mac;
			/* The tokens should all be macro_link, macro_token, macro_run, or */
			/*  macro_directive instances. Each one represents a single */
			/*  argument. For the macro_run and macro_directive arguments, their */
			/*  own arguments are obtained from the same context as the */
			/*  macro_run or macro_directive they're being used as arguments */
			/*  FOR. */
		tokhdptr_parr *args;
	};
	struct macro_directive
	{
			/* TOKTYPE_TOKENGROUP_MACRODIRECTIVE. */
		token_head header;
		
			/* The handler will find it's arguments in the tokhdptr_parr* on the */
			/*  top of the stkp->data stack, but by the time of it's return MUST */
			/*  HAVE MOVED that pointer to the top of the macro_args stack- any */
			/*  actual deallocation will be done elsewhere. It also MUST push a */
			/*  uintptr_t to the stkp->data stack saying how many token_head* */
			/*  instances it pushed onto the shuffle stack (whether it pushes */
			/*  anything to the shuffle stack is optional, but the top of the */
			/*  data stack MUST say how many token header pointers were pushed, */
			/*  regardless of the numbers): the directive should otherwise leave */
			/*  the stacks alone unless that is EXPLICITLY it's purpose. It's */
			/*  stack signature should thusly be as follows:*/
				/*
						tokenheadptr_pascalarray*
					--
						shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
				*/
			/* By doing this set of actions, the handler will have fully */
			/*  duplicated the required set of actions from the */
			/*  shufflequeue_entry_macro_call() sequence of functions. */
		retframe handler;
			/* Same as macro_run. */
		tokhdptr_parr *args;
	};
	
	
	
	/* These hold the actual definition of a macro, and are also used */
	/*  during initial macro parsing. */
		/* This encodes individual arguments. Probably should be replaced */
		/*  with something that's easier for searching. */
	typedef struct macro_call_argval macro_call_argval;
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( macrocallargval_, macro_call_argval );
	typedef macrocallargval_pascalarray macrocllaval_parr;
	
	struct macro_call_argval
	{
			/* One and ONLY one of these should be non-null. */
			/* ... */
			/* Actually, does args_offset even belong here? */
		size_t args_offset;
		char_pascalarray *text;
	};
	struct macro_call
	{
			/* TOKTYPE_TOKENGROUP_MACROCALL. */
		token_head header;
		
			/* This gets used while a macro is first getting parsed to figure */
			/*  out if a particular token corresponds to a macro arg or not, and */
			/*  furthermore WHICH arg. */
		macrocllaval_parr *args;
		
			/* This gets used to hold the tokens that represent the macro's */
			/*  body. */
		tokhdptr_parr *body;
		
		???;
	};
	
	
	
#endif
