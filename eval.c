#include <stdlib.h>

#include "headers.h"

#include "err/inner_err.h"
#include "command_parse.h"
#include "lexalike.h"
#include "stack.h"
#include "basic_toktools.h"
#include "basic_stktools.h"

#include "eval.h"



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

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )


#define CALL_FFUNC( stkpair,  rethand, retdat,  callhand, calldat,  caller, scratch ) \
	CALL_FRAMEFUNC( stkpair,  rethand, retdat,  callhand, calldat,  &errs, caller, scratch, lexalike_ENDRETFRAME )





retframe common_macrodispatch( stackpair *stkp, void *v )
{
	STACKCHECK( stack,  caller, endfunc );
	
	???
	
	genericnamed *gn = bsearch1_gennamearr( genname_parr *parr, token *tok );
	if( gn )
	{
		switch( gn->reftype )
		{
			case GENNAMETYPE_RETFRAMEFUNC:
				/* ->ref points to a retframe to be called/run. */

					/* Maybe we need to setup additional returns as well? */
				CALL_FFUNC(
					stkp,
					
					rethand, (void*)0,
					( (retframe*)( gn->ref ) )->handler, ( (retframe*)( gn->ref ) )->data,
					
					ommon_macrodispatch, scratch
				);
			case GENNAMETYPE_TABLEENTRY:
				/* ->ref points to a token array. */
				
				???;
			
			case GENNAMETYPE_INVALID:
				/* The default state, doesn't actually represent anything, invalid here. */
			case GENNAMETYPE_NAMEONLY:
				/* Used for tracking includes, invalid here. */
			default:
				/* Unknown ref type, invalid here. */
		}
		
		???
		
		struct genericnamed
		{
			char_pascalarray *name;
			void *ref;
			uintptr_t reftype;
		};
		
	} else {
		
		retframe echo_token( stackpair *stkp, void *v )
	}
	
	???
}
{
	STACKCHECK( stack,  caller, endfunc );
	
	???
	
	retframe getANDassemble_token( stackpair *stkp, void *v );
	
	if( ->toktype == TOKTYPE_NAME )
	{
			/* tok->text must point to an actual string of text. */
		genericnamed* bsearch1_gennamearr( genname_parr *parr, token *tok );
		
	} else {
	}
	
	???
}
retframe common_opdispatch( stackpair *stkp, void *v )
{
	STACKCHECK( stack,  caller, endfunc );
	
	token *tok;
	uintptr_t a;
	int scratch;
	
	STACKPEEK_UINT( stkp->data, 0, a,  common_opdispatch, scratch );
	*t = (token*)a;
	
	if( tok->toktype != TOKTYPE_NAME )
	{
		generictyped *gt = gentyped_bsearch( stkp->ops, tok );
		if( gt )
		{
			switch( gt->reftype )
			{
				case GENNAMETYPE_RETFRAMEFUNC:
					/* ->ref points to a retframe to be called/run. */
					
						/* Maybe we need to setup additional returns as well? */
					CALL_FFUNC(
						stkp,
						
							/* The return route. */
						rethand, retdat,
							/* The immediate jump. */
						( (retframe*)( gt->ref ) )->handler, ( (retframe*)( gt->ref ) )->data,
						
						common_opdispatch, scratch
					);
				case GENNAMETYPE_INVALID:
					/* The default state, doesn't actually represent anything, invalid here. */
				case GENNAMETYPE_NAMEONLY:
					/* Used for tracking includes, invalid here. */
				case GENNAMETYPE_TABLEENTRY:
					/* ->ref points to a lookup table to be used for */
					/*  further searches. Usually it gets pushed onto */
					/*  a stack, but probably invalid here? */
				default:
					/* Unknown ref type, invalid here. */
			}
			
			???
			
		} else {
			
				/* No match found, check for a macro. */
			return( (retframe){ &common_macrodispatch, (void*)0 } );
		}
		
	} else {
		
			/* It's a name, check for a macro. */
		return( (retframe){ &common_macrodispatch, (void*)0 } );
	}
}


	/* Gets a token onto the stack, often as a tokenbranch */
	/*  with whitespace shoved wherever in it: only the */
	/*  body member really matters. */
retframe accumulate_token( stackpair *stkp, void *v )


























		This type:
			struct gennameparr_stdpushentry_entry
			{
				uintptr_t toktype;
				genname_parr *table;
					/* "followup" is invoked after a searchtable has been entered, while */
					/*  "conclude" is invoked after a searchtable has been removed. Both */
					/*  need to AT LEAST ensure that the caller of the function that */
					/*  entered/removed the searchtable is returned to. */
				retframe followup, conclude;
			};
	... is ALSO used with the functions below, with a pointer to a
	gennameparr_stdpushentry_entry used as the v_ argument to all four.
	
		These functions:
			retframe components_stdsearchinit( stackpair *stkp, void *v_ );
			retframe components_sequencedsearchproceed( stackpair *stkp, void *v_ );
			retframe components_stdsearchdeinit( stackpair *stkp, void *v_ );
			retframe components_skipendersearchdeinit( stackpair *stkp, void *v_ );
	... work in conjunction with the searchstack. All of them require a pointer
	to a gennameparr_stdpushentry_entry as their "v_" argument (so in general
	use wrapper functions for all of them). In essence, then handle situations
	like comments or argument lists where you have an "opening token" to
	indicate a new context is being entered, thus being deserving of a change
	in treatment. gennameparr_stdpushentry_entry->followup is used after a new
	context has been entered, whereas conclude is used after a context has been
	left.
		components_stdsearchinit() is used to setup a search context. It
	expects a pointer to a token on top of the data stack, and a pointer to a
	tokengroup right under it. The token will be popped, placed in the "lead"
	element of a new tokenbranch, a pointer to that tokebnbranch will be pushed
	onto the stack, then a pointer to a !NEW! tokenGROUP,
	lexparse1_pushsearchtable() will be called to push v_->table onto the
	searchstack, and finally v_->followup will be returned as the next retframe
	to execute.
		components_sequencedsearchproceed() is used to replace one context with
	another. It expects a token on the top of the stack (which it does NOT
	remove), pops the top of the searchstack, pushes a new context back on top,
	and uses a CALLFRAME invocation to tripper a call both to v_->followup, and
	v_->conclude.
		components_stdsearchdeinit() and components_skipendersearchdeinit() are
	roughly equivalent. Both expect a pointer to a token_head derived struct on
	top of the stack with either a ->toktype == v_->toktype, or a toktype of
	EQUIVMERGE (a tokenbranch I think?) with the subtype equal to v_->toktype,
	which both will pop. Under that, both expect a token_head derivation with a
	->toktype of SAMEMERGE (a tokengroup), which again they both pop. Then,
	they both expect a pointer to a tokenbranch, which yes, they both pop.
	Stuff then gets stored into that final tokenbranch (producing the result),
	with both storing the tokengroup into ->body, but only
	components_stdsearchdeinit() storing the triggering token, which goes into
	->tail. They then peek (not pop!) another tokengroup, push the result
	tokenbranch into it, and pop the search table that has been getting used.
	At this point behavior once again diverges, as components_stdsearchdeinit()
	just proceeds straight to v_->conclude, while
	components_skipendersearchdeinit() first pushes the triggering token's
	pointer back onto the stack, before registering v_->conclude for execution,
	and passing execution to invoke_dealloctoken() so that the triggering token
	that it re-pushed will be properly deallocated.
	
		So, a decent framework to handle context-specific search exists here,
	but there doesn't seem to actually be a way to search INSIDE it- that will
	need to be provided elsewhere, or added to this.





			struct macro_directive
			{
				token_head header;
				
				retframe handler;
				tokhdptr_parr *args;
			};
	... represent code invocations of two types: macro invocations, and
	directive invocations. macro_run obviously involves the execution of a
	user-defined macro, while macro_directive corresponds to the execution of a
	built-in directive. Both of them use "args" for the same purpose, that
	being a per-argument set of instructions for how to actually obtain the
	arguments- the allowable set of token_head derived types is limited to
	macro_run and macro_directive themselves, as well as macro_link (used when
	the argument is to be obtained from an argument to the ENCLOSING macro_run)
	and macro_token (which directly holds a pointer to the argument value).
		macro_run's "mac" holds a pointer to the macro_call instances that
	actually DEFINES the behavior of the macro, while macro_directive's
	"handler" is a retframe that implements that particular directive.



	
		This function:
			retframe shufflequeue_macro_run( stackpair *stkp, void *v );
	... is the "caller side" of a macro call. It builds an argument list for
	the designated macro from it's own context, then calls the same function
	that shufflequeue_entry_macro_wrapper() most immediately delegates to. It's
	a good target of study if you want to execute macros, and may even be
	usable directly... but it's honestly intended for internal usage.
		It expects a pointer to a macro_run on top of the data stack, and a
	pointer to a tokenheadptr_pascalarray that holds the CALLER'S argument
	values on top of the macro_args stack. On the "final" return, it will
	leave a uintptr_t on the data stack, a set of token pointers on the shuffle
	stack equal in number to the value of the uintptr_t, and the argument list
	on macro_args will have been popped... but I'm not certain that it SHOULD
	be popped.
	
		This function:
			retframe shufflequeue_macro_directive( stackpair *stkp, void *v );
	... is the directive equivalent of shufflequeue_macro_run(). It executes
	macro_directive->handler with the arguments of :
		( tokenheadptr_pascalarray* macro_args: )
	(... except I think macro_asrgs: DOES have an argument list?), which in
	turn MUST return
		( shuffledlength shufflestack:token*[shuffledlength] macro_args: tokenheadptr_pascalarray* ),
	because a function will be enqueued that pops the tokenheadptr_pascalarray
	pointer. The shufflelength MUST be generated by the handler, and the
	handler MUST put a number of token pointers onto the shuffle stack that
	equals shufflelength.




			int pack_arglist( stackpair *stkp );
	... takes a number of token pointers, and packages it up into an array for
	convenience. It expects a uintptr_t on the data stack, and a set of token
	pointers on the shuffle stack of number equal to the value of the
	uintptr_t. The pointers on the shuffle stack get moved into a
	tokenheadptr_pascalarray, with the pointer on the top of the stack being
	placed at the end of the tokenheadptr_pascalarray, and a pointer to the
	tokenheadptr_pascalarray then gets pushed onto the data stack.




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
retframe shufflequeue_macro_run( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_run *mr;
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_macro_run, macroargs_ENDRETFRAME );
	
	STACKPEEK_UINT( &( stkp->data ), 0, a,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	mr = (macro_run*)a;
	if( mr->header.toktype != TOKTYPE_TOKENGROUP_MACRORUN || !( mr->mac ) || !( mr->args ) )
	{
		TRESPASSPATH( shufflequeue_macro_run, "ERROR: shufflequeue_macro_run found an incorrectly configured macro_run on top of the data stack" );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Prep for the call to shufflequeue_step_macro_call(). */
	{
		size_t used = token_queue_shuffleused();
		
		STACKPUSH_UINT( &( stkp->data ), used,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	}
	STACKPUSH_UINT( &( stkp->data ), mr->args,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	STACKPUSH_UINT( &( stkp->data ), 0,  shufflequeue_macro_run, res, macroargs_ENDRETFRAME );
	
	
	CALLFRAMEFUNC(
		stkp,
		
		&shufflequeue_macro_run_continue,
		(void*)0,
		
		&shufflequeue_step_macro_call,
		(void*)0,
		
		shufflequeue_macro_run,
		res
	);
}
		/*
				(
						macro_run* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
					--
						macro_call* tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
					--
						shuffledlength shufflestack:token*[?] macro_args:
				)
		*/
	retframe shufflequeue_macro_run_continue( stackpair *stkp, void *v )
	{
		uintptr_t args, b;
		macro_run *mr;
		
		
		STACKCHECK( stkp,  shufflequeue_macro_run_continue, macroargs_ENDRETFRAME );
		
			/*
					shuffledlength shufflestack:token*[shuffledlength]
				--
					arglist*
			*/
		int res = pack_arglist( stkp );
		if( !res )
		{
			FAILEDINTFUNC( "pack_arglist", shufflequeue_macro_run_continue, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		STACKPOP_UINT( &( stkp->data ), args,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		
		STACKPOP_UINT( &( stkp->data ), b,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		mr = (macro_run*)b;
		if( mr->header.toktype != TOKTYPE_TOKENGROUP_MACRORUN || !( mr->mac ) )
		{
			TRESPASSPATH(
				shufflequeue_macro_run_continue,
				"ERROR: shufflequeue_macro_run_continue found an incorrectly configured macro_run on top of the data stack"
			);
			return( (retframe){ &end_run, (void*)0 } );
		}
		STACKPUSH_UINT( &( stkp->data ), mr->mac,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		
		STACKPUSH_UINT( &( stkp->data ), args,  shufflequeue_macro_run_continue, res, macroargs_ENDRETFRAME );
		
		
		CALLFRAMEFUNC(
			stkp,
			
			&vm_pop_macroarg,
			(void*)0,
			
			&shufflequeue_entry_macro_call,
			(void*)0,
			
			shufflequeue_macro_run_continue,
			res
		);
	}













#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif