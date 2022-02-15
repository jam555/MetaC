#ifndef METAC_METACPREPROC_COMPONENTS_H
 #define METAC_METACPREPROC_COMPONENTS_H
	
	/* metaCpreproc/components.h */
	
	#include "../headers.h"
	
	/* This file exists for "component parses" that get built up into more full */
	/*  preprocessor parsing runs. */
	
	
	uintptr_t searchstack_refid;
	genname_parrparr *searchstack = (genname_parrparr*)0;
	size_t searchstack_used = 0;
	genname_parr default =
	{
		(size_t)???, /* Length. */
		(genericnamed[])
		{
			(genericnamed){ (char_pascalarray *)0, (void*)0, (uintptr_t)0 },
			(genericnamed){ (char_pascalarray *)0, (void*)0, (uintptr_t)0 },
			...
		}
	};
	
	int searchstack_init();
	int searchstack_deinit();
	
		/* This shouldn't appear directly in any search tables, but instead */
		/*  should be returned in a retframe by another wrapper function, which */
		/*  should also provide a non-null value for the void pointer in the */
		/*  process. */
	retframe components_stdsearchinit( stackpair *stkp, void *v_ );
	retframe components_sequencedsearchproceed( stackpair *stkp, void *v_ );
		/* Use this if you care about the token that caused the function to be */
		/*  called. */
	retframe components_stdsearchdeinit( stackpair *stkp, void *v_ );
		/* Use this if you DON'T care about the token that caused this function */
		/*  to be called. */
	retframe components_skipendersearchdeinit( stackpair *stkp, void *v_ );
	
	
	
	
	
	/* The stuff below may well belong in a completely different file! Also, */
	/*  it's sorta the core of preprocessing execution. Every once in a */
	/*  while you'll see a comment like this: ( x y -- z y h ); everywhere */
	/*  it appears, it's a Forth-style notation of what the stack meant to */
	/*  be on entry and exit of the relevant function. If you see more than */
	/*  one set of double-dashes, then it's indicating the resuklt of some */
	/*  final exit from a directly triggered call as well, maybe with */
	/*  additional steps in between. */
	
	
	
	typedef struct macro_link macro_link;
	struct macro_link
	{
		token_head header;
		
			/* This identifies which element in the argument list should be */
			/*  used. */
		size_t args_offset;
	};
	typedef struct macro_token macro_token;
	struct macro_token
	{
		token_head header;
		
		token_head *tok;
	};
	typedef struct macro_run macro_run;
	struct macro_run
	{
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
	typedef struct macro_directive macro_directive;
	struct macro_directive
	{
		token_head header;
		
			/* The handler will find it's arguments in the tokhdptr_parr* on the */
			/*  top of the stkp->data stack, but by the time of it's return MUST */
			/*  HAVE MOVED that pointer to the top of the macro_args stack- any */
			/*  actual deallocation will be donme elsewhere. It also MUST push a */
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
	
	typedef struct macro_call_argval macro_call_argval;
	struct macro_call_argval
	{
			/* One and ONLY one of these should be non-null. */
			/* ... */
			/* Actually, does args_offset even belong here? */
		size_t args_offset;
		char_pascalarray *text;
	};
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( macrocallargval_, macro_call_argval );
	typedef macrocallargval_pascalarray macrocllaval_parr;
	typedef struct macro_call macro_call;
	struct macro_call
	{
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
	
	
	
	/* These macroargs_*() functions, plus some stuff in the .c file, define */
	/*  an entire additional stack system. It likely belongs in it's own */
	/*  file pair. */
	
	int macroargs_init();
	int macroargs_resize( int deltaMacros );
	int macroargs_clear();
	
	int macroargs_pushset( tokhdptr_parr *val );
	int macroargs_popset( tokhdptr_parr **dest );
	int macroargs_peekset( size_t off,  tokhdptr_parr **dest );
	
	
		/*
				shufflecount shuffle:token*[shufflecount]
			--
				arglist*
		*/
	int pack_arglist( stackpair *stkp,  uintptr_t *refid, int errgate );
	retframe vm_pop_macroarg( stackpair *stkp, void *v );
	
	
	
	/* Where do these defines even belong? */
	#define PEEK_MACROARGS( offset, var,  errfunc, err1, err2,  ... ) \
		if( !macroargs_peekset( ( offset ),  &( var ) ) ) { errfunc( ( err1 ),  __VA_ARGS__, &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__, &( var ) ); }
	#define POP_MACROARGS( destptr,  errfunc, err1, err2,  ... ) \
		if( !macroargs_popset( destptr ) ) { errfunc( ( err1 ),  __VA_ARGS__, &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__, &( var ) ); }
	#define PUSH_MACROARGS( val,  errfunc, err,  ... ) \
		if( !macroargs_pushset( val ) ) { errfunc( ( err ),  __VA_ARGS__, ( val ) ); }
	
	#define PUSH_SHUFFLE( scratchint, token,  errfunc, err,  ... ) \
		( scratchint ) = token_queue_shufflepush( (token*)( token ) ); \
		if( !( scratchint ) ) { errfunc( ( err ),  __VA_ARGS__, &( scratchint ) ); }
	
	
	
	/* These functions essentially define the actual execution engine for */
	/*  macros & directives. */
	
	
	/* These two define a convenient entry-point for normal code: the others */
	/*  will only need to be dealt with by directive implementations (which */
	/*  will need to implement their own versions of */
	/*  shufflequeue_step_macro_call() and shufflequeue_exit_macro_call()... */
	/*  I think). Also, the arguments (described below) must be created */
	/*  OUTSIDE of this wrapper, before it gets called. Look at the */
	/*  shufflequeue_macro_run*() functions to get some idea of how this */
	/*  might be done. */
		/* The tokenheadptr_pascalarray* instance is the list of arguments */
		/*  TO the macro that's provided by macro_call*; each token_head* */
		/*  corresponds to ONE argument, and is expected to not be */
		/*  deallocated (and, indeed, this entire set of functions is */
		/*  basically expected to perform NO deallocations, lest it step on */
		/*  something else's toes). source: corresponds to the system in */
		/*  input/lexalike that we actually get tokens from: in essence, */
		/*  each macro works by executing, pushing generated tokens to the */
		/*  input/lexalike token shuffle stack in the process, and at the */
		/*  end of everything all of those tokens get shuffled to source:, a */
		/*  process which SHOULD result in them being arranged in the */
		/*  correct order as well. */
		/* TODO: Currently, nothing is done to restrict which macros can be */
		/*  caused to be executed BY those generated tokens: something for */
		/*  the future... */
		/*
				macro_call* tokenheadptr_pascalarray*
			--
				shuffledlength shufflestack:token*[shuffledlength]
			--
				source:token*
		*/
	retframe shufflequeue_entry_macro_wrapper( stackpair *stkp, void *v );
			/*
					shufflecount shuffle:token*[shufflecount]
				--
					source:token*
			*/
		retframe shufflequeue_exit_macro_wrapper( stackpair *stkp, void *v );
	
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
		/* This is the one for actual macros: note that it is a SET of */
		/*  functions instead of just one. These functions implement the */
		/*  "caller side" of a macro invocation: they essentially use */
		/*  shufflequeue_step_macro_call() to iterate over some token_header */
		/*  derivatives to generate the individual arguments (each of the */
		/*  instances that are iterated over corresponds to a single */
		/*  argument: this is a RECURSIVE process, and naturally produces */
		/*  each argument in the proper format); then do so again using */
		/*  shufflequeue_entry_macro_call() with the target macro, while */
		/*  providing the newly generated argument list as it's arguments, */
		/*  before finally getting rid of those arguments so that the */
		/*  "caller side"'s own caller can continue processing as normal. */
		/*
			(
					macro_run* macro_args:tokenheadptr_pascalarray*
				--
					macro_run* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
			)
		*/
		/*
			This entire sub-sequence of calls roughly produces this:
			(
					macro_run* macro_args:tokenheadptr_pascalarray*
				--
					macro_run* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
				--
					macro_run* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
				--
					macro_call* tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
				--
					shuffledlength shufflestack:token*[?] macro_args:
			)
		*/
	retframe shufflequeue_macro_run( stackpair *stkp, void *v );
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
			/* This performs similarly to shufflequeue_macro_run(), but */
			/*  instead of calling shufflequeue_entry_macro_call() at the */
			/*  end, it calls the function in macro_directive->handler with */
				/* ( tokenheadptr_pascalarray* macro_args: ) */
			/*  as it's effective arguments. It's true that there's no */
			/*  macro_call* equivalent argument, but that's not a problem */
			/*  because macro_directive->handler is a retframe instance, and */
			/*  thus the void* "data" member passed as part of it's */
			/*  execution does the job instead. */
			/* NOTE: The return state of the handler sequence MUST be */
			/*  equivalent to */
				/* ( shuffledlength shufflestack:token*[shuffledlength] macro_args: tokenheadptr_pascalarray* ), */
			/*  because the same call that enters the directive's handler */
			/*  ALSO queues another function that'll pop the */
			/*  tokenheadptr_pascalarray* after the directive's handler */
			/*  triggers a "proper" return instead of a call or tailcall. */
			/* As an additional note: The */
				/* ( shuffledlength shufflestack:token*[shuffledlength] ) */
			/*  portion of the directive handler's return is generated BY */
			/*  THE DIRECTIVE HANDLER ITSELF, it is NOT data already */
			/*  existing on the relevant stacks. This is the way that a */
			/*  directive is intended to inject actual tokens into the token */
			/*  stream. */
			/*
				(
						macro_directive* macro_args:tokenheadptr_pascalarray*
					--
						macro_directive* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
				)
			*/
			/*
				This entire sub-sequence of calls roughly produces this:
				(
						macro_directive* macro_args:tokenheadptr_pascalarray*
					--
						macro_directive* shufflebookmark tokhdptr_parr* 0 macro_args:tokenheadptr_pascalarray*
					--
						macro_directive* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
					--
						tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
					--
						shuffledlength shufflestack:token*[?] macro_args:
				)
			*/
	retframe shufflequeue_macro_directive( stackpair *stkp, void *v );
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
	/*  wrapper function is responsible for mkoving the generated tokens to */
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
