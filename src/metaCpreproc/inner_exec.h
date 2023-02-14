#ifndef METAC_METACPREPROC_INNER_EXEC_H
 #define METAC_METACPREPROC_INNER_EXEC_H
	
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
	
	/* metaCpreproc/inner_exec.h */
	
	/* Everything in this file probably belongs in this file, HOWEVER, */
	/*  nothing has actually been updated to reflect it's movement to */
	/*  exec.h. Beware the comments! */
	
	
	
	/* Every once in a while you'll see a comment like this: */
		/* ( x y -- z y h ) */
	/*  everywhere it appears, it's a Forth-style notation of what the stack */
	/*  meant to be on entry and exit of the relevant function. If you see */
	/*  more than one set of double-dashes, then it's indicating the result */
	/*  of some final exit from a directly triggered call as well, maybe */
	/*  with additional steps in between. */
	
	
	
	/* These are the handlers for individual macro_* derivatives of */
	/*  token_head. It's important to note that directives require THEIR OWN */
	/*  versions of these for shufflequeue_macro_directive() to invoke. This */
	/*  is because they perform "special magic", such as building the lists */
	/*  of these macro_* instances in the first place. */
		/*
				*macro_link macro_args:tokenheadptr_pascalarray* shuffle:token*[]
			--
				macro_args:tokenheadptr_pascalarray* shuffle:token*[]+(arglist[subcall->link])
		*/
	retframe shufflequeue_macro_link( stackpair *stkp, void *v );
		/*
				macro_token* args:*arglist shuffle:token*[]
			--
				args:*arglist shuffle:token*[]+(subcall->link)
		*/
	retframe shufflequeue_macro_token( stackpair *stkp, void *v );
	
	
			/*
					(
							macro_run* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
						--
							macro_call* tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
						--
							shuffledlength shufflestack:token*[?] macro_args:
					)
			*/
		retframe shufflequeue_macro_run_continue( stackpair *stkp, void *v );
	
			/*
					(
							macro_directive* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
						--
							tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
						--
							shuffledlength shufflestack:token*[?] macro_args:
					)
			*/
		retframe shufflequeue_macro_directive_continue( stackpair *stkp, void *v );
	
	
	/* These functions form the "callee side" of running a macro. They see */
	/*  to it that all of the handler functions above all get called in */
	/*  order with the correct stack structure, and process the final result */
	/*  for whichever consumer to use later, whether the base-level of the */
	/*  preprocessor, or (in the case of macros and directives) later stages */
	/*  of the handlers themselves: directives need to partially implement */
	/*  this themselves, though the *_tool() functions should hopefully make */
	/*  that task easier. Note that the shufflequeue_exit_macro_wrapper() */
	/*  wrapper function is responsible for moving the generated tokens to */
	/*  the token source- it isn't desirable until the parent-most macro has */
	/*  been fully executed. Note that this DOES NOT calculate the arguments */
	/*  themselves: that is the job of the "caller side", not the callee... */
	/*
		The signature of the whole call sequence:
		(
				*callbody args:*arglist
			--
				shufflecount shuffle:token*[shufflecount]
		)
	*/
		/*
			Forth stack notation:
				(
						macro_call* tokenheadptr_pascalarray*
					--
						shufflebookmark tokhdptr_parr* '0' macro_args:tokenheadptr_pascalarray*
				)
			Note that the tokenheadptr_pascalarray* is specifically a pointer to
			the pascal array that holds the actual arguments provided TO the macro
			in the macro call (as opposed to the names that those arguments are
			associated with within the body of the macro).
		*/
	retframe shufflequeue_entry_macro_call( stackpair *stkp, void *v );
		/* Just a wrapper. Will need an imitator for directives. */
	retframe shufflequeue_step_macro_call( stackpair *stkp, void *v );
			/*
				Forth stack notation:
					(
							shufflebookmark tokhdptr_parr* x macro_args:tokenheadptr_pascalarray* shuffle:token*[?]
						--
							shufflebookmark tokhdptr_parr* x+1 ( token_header* |  ) macro_args:tokenheadptr_pascalarray* shuffle:token*[?]
					)
			*/
		retframe shufflequeue_step_macro_calltool( stackpair *stkp, void *v,  retframe loop, retframe ret );
		/*
			Forth stack notation:
				(
						shufflebookmark tokhdptr_parr* x macro_args:tokenheadptr_pascalarray* shufflestack:token*[shuffledlength]
					--
						shuffledlength shufflestack:token*[shuffledlength]
				)
			Yes, upon exit, all of that stuff will be gone.
		*/
	retframe shufflequeue_exit_macro_call( stackpair *stkp, void *v );
				/*
						shufflebookmark shuffle:token*[shufflecount]
					--
						shufflecount shuffle:token*[shufflecount]
				*/
		retframe shufflequeue_exit_macro_calltool( stackpair *stkp, void *v );
	
#endif
