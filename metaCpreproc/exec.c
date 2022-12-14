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

#include "inner_exec.h"


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
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
	#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	
	#define DECARG( val ) STDMSG_DECARG_WRAPPER( &errs, ( val ) )
	#define STRARG( str ) STDMSG_STRARG_WRAPPER( &errs, ( str ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )

#define PEEKMACROARGS( offset, dest,  caller, scratch, endfunc ) \
	PEEK_MACROARGS( offset, dest,  &err, caller, scratch, endfunc )
#define POPMACROARGS( destptr,  caller, scratch, endfunc ) \
	POP_MACROARGS( destptr,  &err, caller, scratch, endfunc )
#define PUSHMACROARGS( val,  caller, scratch, endfunc ) \
	PUSH_MACROARGS( val,  &err, caller, scratch, endfunc )


#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &err, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &err, ( caller ), ( scratch ), ( endfunc ) )


#define PUSHSHUFFLE( tokptr,  caller, scratch, endfunc ) \
	PUSH_SHUFFLE( tokptr,  &err, caller, scratch, endfunc )


#define RETFRAMEFUNC( stkp,  caller, scratch ) \
	RET_FRAMEFUNC( ( stkp ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )



/* These functions essentially define the actual execution engine for macros */
/*  & directives. */



	/*
			*macro_link macro_args:tokenheadptr_pascalarray* shuffle:token*[]
		--
			macro_args:tokenheadptr_pascalarray* shuffle:token*[]+(arglist[subcall->link])
	*/
	/* A pointer to a macro_link should be on top of the stack, and a */
	/*  pointer to the tokhdptr_parr pascal array of actual arguments TO */
	/*  the macro should be on "macro_args". */
retframe shufflequeue_macro_link( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_link *mlink;
	tokhdptr_parr *thdptpr;
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_macro_link, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), a,  shufflequeue_macro_link, res, macroargs_ENDRETFRAME );
	mlink = (macro_link*)a;
	if( mlink->header.toktype != TOKTYPE_TOKENGROUP_MACROLINK )
	{
		TRESPASSPATH( shufflequeue_macro_link, "ERROR: shufflequeue_macro_link found a macro_link with a non-_MACROLINK token type on top of the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	PEEKMACROARGS( 0, thdptpr,  shufflequeue_macro_link, res, macroargs_ENDRETFRAME );
	if( thdptpr->len <= mlink->args_offset )
	{
		TRESPASSPATH( shufflequeue_macro_link, "ERROR: shufflequeue_macro_link was given a macro_link with too large an args_offset." );
		return( (retframe){ &end_run, (void*)0 } );
	}
		???
		/* Note that OUTWARDLY ONE token should be pushed here. This is a */
		/*  SINGLE arg that we're dealing with, NOT multiple. */
	PUSHSHUFFLE( thdptpr->body[ mlink->args_offset ],  shufflequeue_macro_link, res, macroargs_ENDRETFRAME );
	
	RETFRAMEFUNC( stkp,  shufflequeue_macro_link, res );
}
	/*
			macro_token* args:*arglist shuffle:token*[]
		--
			args:*arglist shuffle:token*[]+(subcall->link)
	*/
retframe shufflequeue_macro_token( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_token *mtok;
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_macro_token, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), a,  shufflequeue_macro_token, res, macroargs_ENDRETFRAME );
	mtok = (macro_token*)a;
	if( mtok->header.toktype != TOKTYPE_TOKENGROUP_MACROTOKEN )
	{
		TRESPASSPATH( shufflequeue_macro_token, "ERROR: shufflequeue_macro_token found a macro_link with a non-_MACROTOKEN token type on top of the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !( mtok->tok ) )
	{
		BADNULL( shufflequeue_macro_token, &( mtok->tok ) );
		return( (retframe){ &end_run, (void*)0 } );
	}
		???
		/* Is this actually enough? Might need to ensure that a */
		/*  tokengroup is present, and push it's elements individually, */
		/*  or something similar. */
	PUSHSHUFFLE( mtok->tok,  shufflequeue_macro_token, res, macroargs_ENDRETFRAME );
	
	RETFRAMEFUNC( stkp,  shufflequeue_macro_token, res );
}
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
			TRESPASSPATH( shufflequeue_macro_run_continue, "ERROR: shufflequeue_macro_run_continue found an incorrectly configured macro_run on top of the data stack" );
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
retframe shufflequeue_macro_directive( stackpair *stkp, void *v )
{
	uintptr_t a;
	macro_directive *mdir;
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_macro_directive, macroargs_ENDRETFRAME );
	
	STACKPEEK_UINT( &( stkp->data ), 0, a,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	mdir = (macro_directive*)a;
	if( mdir->header.toktype != TOKTYPE_TOKENGROUP_MACRODIRECTIVE || !( mdir->handler ) || !( mdir->args ) )
	{
		TRESPASSPATH( shufflequeue_macro_directive, "ERROR: shufflequeue_macro_directive found an incorrectly configured macro_run on top of the data stack" );
		return( (retframe){ &end_run, (void*)0 } )
	}
	
		/* Prep for the call to shufflequeue_step_macro_call(). */
	{
		size_t used = token_queue_shuffleused();
		
		STACKPUSH_UINT( &( stkp->data ), used,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	}
	STACKPUSH_UINT( &( stkp->data ), mdir->args,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	STACKPUSH_UINT( &( stkp->data ), 0,  shufflequeue_macro_directive, res, macroargs_ENDRETFRAME );
	
	
	CALLFRAMEFUNC(
		stkp,
		
		&shufflequeue_macro_directive_continue,
		(void*)0,
		
		&shufflequeue_step_macro_call,
		(void*)0,
		
		shufflequeue_macro_directive,
		res
	);
}
		/*
				(
						macro_directive* shuffledlength shufflestack:token*[shuffledlength] macro_args:tokenheadptr_pascalarray*
					--
						tokenheadptr_pascalarray* macro_args:tokenheadptr_pascalarray*
					--
						shuffledlength shufflestack:token*[?] macro_args:
				)
		*/
	retframe shufflequeue_macro_directive_continue( stackpair *stkp, void *v )
	{
		uintptr_t args, b;
		macro_directive *mdir;
		
		
		STACKCHECK( stkp,  shufflequeue_macro_directive_continue, macroargs_ENDRETFRAME );
		
			/*
					shuffledlength shufflestack:token*[shuffledlength]
				--
					arglist*
			*/
		int res = pack_arglist( stkp );
		if( !res )
		{
			FAILEDINTFUNC( "pack_arglist", shufflequeue_macro_directive_continue, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		STACKPOP_UINT( &( stkp->data ), args,  shufflequeue_macro_directive_continue, res, macroargs_ENDRETFRAME );
		
		STACKPOP_UINT( &( stkp->data ), b,  shufflequeue_macro_directive_continue, res, macroargs_ENDRETFRAME );
		mdir = (macro_directive*)b;
		if( mdir->header.toktype != TOKTYPE_TOKENGROUP_MACRODIRECTIVE || !( mdir->handler ) )
		{
			TRESPASSPATH( shufflequeue_macro_directive_continue, "ERROR: shufflequeue_macro_directive_continue popped an invalidly configured macro_directive from the data stack." );
			return( (retframe){ &end_run, (void*)0 } ); 
		}
		
		STACKPUSH_UINT( &( stkp->data ), args,  shufflequeue_macro_directive_continue, res, macroargs_ENDRETFRAME );
		
		
		CALLFRAMEFUNC(
			stkp,
			
			&vm_pop_macroarg,
			(void*)0,
			
			mdir->handler.handler,
			mdir->handler.data,
			
			shufflequeue_macro_directive_continue,
			res
		);
	}



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
retframe shufflequeue_entry_macro_call( stackpair *stkp, void *v )
{
	uintptr_t mcall;
	int res;
	
	
	STACKCHECK( stkp,  shufflequeue_entry_macro_call, macroargs_ENDRETFRAME );
	
		/* Move the arguments to the argument stack. */
	STACKPOP_UINT( &( stkp->data ), mcall,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	PUSHMACROARGS( mcall,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	
	STACKPOP_UINT( &( stkp->data ), mcall,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
		/* Just verification. */
	if( ( (token_head*)mcall )->toktype != TOKTYPE_TOKENGROUP_MACROCALL )
	{
		TRESPASSPATH( shufflequeue_entry_macro_call, "ERROR: shufflequeue_entry_macro_call found a non-_MACROCALL token on top of the data stack." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
		/* Push a bookmark for where the tokenqueue shuffle stack currently */
		/*  is: if we have macro calls embedded inside of other macrocalls, */
		/*  then this will make it MUCH easier to avoid causing buffer */
		/*  "overflow/underflow" errors. */
	{
		size_t used = token_queue_shuffleused();
		
		STACKPUSH_UINT( &( stkp->data ), used,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	}
	
		/* Push the body element from mcall instead of mcall itself, and */
		/*  then push a "0" progress indicator, since we haven't processed */
		/*  anything yet. */
	STACKPUSH_UINT( &( stkp->data ), mcall->body,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
		/* Our progress? None! Because we haven't really started. */
	STACKPUSH_UINT( &( stkp->data ), 0,  shufflequeue_entry_macro_call, res, macroargs_ENDRETFRAME );
	
	return( (retframe){ &shufflequeue_step_macro_call, (void*)0 } );
}

	/*
		Forth stack notation:
			(
					shufflebookmark tokhdptr_parr* x macro_args:tokenheadptr_pascalarray* shuffle:token*[?]
				--
					shufflebookmark tokhdptr_parr* x+1 ( token_header* |  ) macro_args:tokenheadptr_pascalarray* shuffle:token*[?]
			)
	*/
retframe shufflequeue_step_macro_calltool( stackpair *stkp, void *v,  retframe loop, retframe ret )
{
	uintptr_t prog, ops_;
	tokhdptr_parr *arg_parr;
	
	
	STACKCHECK( stkp,  shufflequeue_step_macro_calltool, macroargs_ENDRETFRAME );
	
		/* Pull these two from the stack, since we need to use and modify */
		/*  the "deeper" one, while otherwise preserving both. */
	int res;
	PEEKMACROARGS( 0, arg_parr,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME )
		/* "0" is actually valid, so we can't use the usual macro. */
	STACKPOP_UINT( &( stkp->data ), &prog,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME );
	
	STACKPEEK_UINT( &( stkp->data ), 0, ops_,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME );
	tokhdptr_parr *ops = (tokhdptr_parr*)ops_;
	
	++prog;
	STACKPUSH_UINT( &( stkp->data ), prog,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME );
	
	if( ops->len >= prog )
	{
		token_header *th = ( ops->body[ prog - 1 ] );
		
		STACKPUSH_UINT( &( stkp->data ), th,  shufflequeue_step_macro_calltool, res, macroargs_ENDRETFRAME );
		
		framefunc hand = &end_run;
		switch( th->toktype )
		{
			case TOKTYPE_TOKENGROUP_MACROLINK:
				hand = &shufflequeue_macro_link;
				break;
			case TOKTYPE_TOKENGROUP_MACROTOKEN:
				hand = &shufflequeue_macro_token;
				break;
			case TOKTYPE_TOKENGROUP_MACRORUN:
				hand = &shufflequeue_macro_run;
				break;
			case TOKTYPE_TOKENGROUP_MACRODIRECTIVE:
				hand = &shufflequeue_macro_directive;
				break;
			
			default:
				TRESPASSPATH( shufflequeue_step_macro_calltool, "ERROR: shufflequeue_step_macro_calltool encountered an unrecognized token type: " );
				DECARG( th->toktype );
					/* "hand" contains a reference to end_run, so no need to */
					/*  do anything other than break out of the switch. */
				break;
		}
		
		CALLFRAMEFUNC(
			stkpair,
			
			loop.handler,
			loop.data,
			
			hand,
			(void*)0,
			
			shufflequeue_step_macro_calltool,
			res
		);
		
	} else {
		
		return( ret );
	}
}
	/* Just a wrapper. Will need an imitator for directives. */
retframe shufflequeue_step_macro_call( stackpair *stkp, void *v )
{
	return
	(
		shufflequeue_step_macro_calltool
		(
			stkp, v,
			
			(retframe){ &shufflequeue_step_macro_call, (void*)0 },
			retframe ret /* Should be shufflequeue_exit_macro_call() or similar. */
		)
	);
}
	
		/*
				shufflebookmark shuffle:token*[shufflecount]
			--
				shufflecount shuffle:token*[shufflecount]
		*/
retframe shufflequeue_exit_macro_calltool( stackpair *stkp, void *v )
{
	uintptr_t shufflebookmark;
	
	
	STACKCHECK( stkp,  shufflequeue_exit_macro_calltool, macroargs_ENDRETFRAME );
	
		/* And now pop the shuffle stack/queue/whatever's bookmark, so that */
		/*  we'll know how many token pointers need to be moved from */
		/*  "shuffle" to the token get/unget stack. These will be */
		/*  deallocated elsewhere, so no need to do it here. */
	int res;
	STACKPOP_UINT( &( stkp->data ), &shufflebookmark,  shufflequeue_exit_macro_calltool, res, macroargs_ENDRETFRAME );
	??? /* Actually, where DO these get freed? That might be a source of */
		/*  errors. */
	
	size_t used = token_queue_shuffleused() - shufflebookmark;
		/* Push the number of tokens on the shuffle stack: some */
		/*  things need to know this for their own purposes. */
	STACKPUSH_UINT( &( stkp->data ), shufflebookmark,  shufflequeue_exit_macro_calltool, res, macroargs_ENDRETFRAME );
	
	RETFRAMEFUNC( stkp,  shufflequeue_exit_macro_calltool, res );
}
	/*
		Forth stack notation:
			(
					shufflebookmark tokhdptr_parr* x macro_args:tokenheadptr_pascalarray* shufflestack:token*[shuffledlength]
				--
					shuffledlength shufflestack:token*[shuffledlength]
			)
		Yes, upon exit, all of that stuff will be gone.
	*/
retframe shufflequeue_exit_macro_call( stackpair *stkp, void *v )
{
	uintptr_t a;
	tokhdptr_parr *args;
	
	
	STACKCHECK( stkp,  shufflequeue_exit_macro_call, macroargs_ENDRETFRAME );
	
		/* Pop the pascal array of arguments provided to the macro first: */
		/*  these will need to be deallocated at some point. */
	int res;
	POPMACROARGS( &args,  shufflequeue_exit_macro_call, res, macroargs_ENDRETFRAME );
		/* Do something to deallocate those tokens. */
	???
	
		/* Now pop the progress marker: we're done, so it's done. Just a */
		/*  number, so no need to deallocate. */
	STACKPOP_UINT( &( stkp->data ), &a,  shufflequeue_exit_macro_call, res, macroargs_ENDRETFRAME );
	
		/* Now the operations array. The array itself is */
		/*  owned elsewhere, so no need to deallocate. Also, deallocating */
		/*  now could break something later. */
	STACKPOP_UINT( &( stkp->data ), &a,  shufflequeue_exit_macro_call, res, macroargs_ENDRETFRAME );
	
	return( shufflequeue_exit_macro_calltool( stkp, v ) );
}



	/*
			macro_call* tokenheadptr_pascalarray*
		--
			shuffledlength shufflestack:token*[shuffledlength]
		--
			source:token*
	*/
retframe shufflequeue_entry_macro_wrapper( stackpair *stkp, void *v )
{
	int res;
	CALLFRAMEFUNC(
		stkp,
		
		&shufflequeue_entry_macro_call,
		(void*)0,
		
		&shufflequeue_exit_macro_wrapper,
		(void*)0,
		
		shufflequeue_entry_macro_wrapper,
		res
	);
}
		/*
				shufflecount shuffle:token*[shufflecount]
			--
				source:token*
		*/
	retframe shufflequeue_exit_macro_wrapper( stackpair *stkp, void *v )
	{
		uintptr_t count;
		int res;
		
		
		STACKCHECK( stkp,  shufflequeue_entry_macro_wrapper, macroargs_ENDRETFRAME );
		
		STACKPOP_UINT( &( stkp->data ), count,  shufflequeue_exit_macro_wrapper, res, macroargs_ENDRETFRAME );
		
		while( count )
		{
			--count;
			int res = token_queue_shuffle2queue();
			if( !res )
			{
				FAILEDINTFUNC( "token_queue_shuffle2queue", shufflequeue_exit_macro_wrapper, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
		}
		
		/* The various produced tokens should now be in the token stream, */
		/*  AND in the correct order. */
		
		
		RETFRAMEFUNC( stkp,  shufflequeue_exit_macro_wrapper, res );
	}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
