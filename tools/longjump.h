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
/* tools/longjump.h */

#ifndef METAC_TOOLS_LONGJUMP_H
 #define METAC_TOOLS_LONGJUMP_H



/* Beware all ye who enter here, for here be little dragons, and paths to greater. */



/* This file provides declarations for two facilities, longjump and */
/*  divertthread. They're passingly similar, and divertthread uses longjump, but */
/*  they're not identical. */
	/* The longjump facility provides a VM equivalent to the C longjmp() facility, */
	/*  though it'll leave values on the stack which MUST be preserved. */
	/* The divertthread facility is more involved, and among other things will */
	/*  actually create a new retframe{} for your code to use as a longjump from */
	/*  within it. Divert thread is meant to be more resilient in the case of */
	/*  nested calls to it (so try to only use longjump with stuff that doesn't */
	/*  call anything complex...), but has been reviewed less and thus is more */
	/*  likely to be buggy. Also, it's only used by the more recent bracket-gather */
	/*  code, and needs better documentation. */



/* This is the setjump/longjump facility. It's usage can be approximated as the following: */
/*
	retframe example_function( stackpair *stkp, void *v_ )
	{
		uintptr_t bookmarkname;
		static const retframe_parr
			onset_ =
				(retframe_parr)
				{
					element count,
					{
						some instructions,
						
						(retframe){ &longjump_callstack, &bookmarkname }
					}
				},
			onjump_ = (retframe_parr){ element count, { some other instructions } );
		static const retframe
			onset = (retframe){ &enqueue_returns, (void*)&onset_ },
			onjump = (retframe){ &enqueue_returns, (void*)&onjump_ };
		
		LOCALIZE_SETJUMP( bookmarkname, someprefix, enqueable,  &onset, &onjump );
		
		return( (retframe){ &enqueue_returns, (void*)&enqueable } );
	}
*/
/* It's worth noting that neither onset{} nor onjump{} will be exposed to the */
/*  bookmark values used. Instead, LOCALIZE_SETJUMP() keeps those values nicely */
/*  contained inside the scripts that it generates, which is much nicer than what */
/*  divertthread currently does. The greatest complication comes in that there is no */
/*  good way to handle things like destructors, deinitializers, nor other such */
/*  constructs within this system. */

	/* v_ MUST point to a uintptr_t in both funcs: it will be treated as a bookmark. */
	/* Note that setjump_*() DOES NOT pop the return retframe{} from the */
	/*  stack, but instead peeks it: use just_run() (or a custom function) */
	/*  for that retframe, so that you can properly handle that. */
		/* ( -- 0 ); the next function will be the next retframe on stkp->ret, but */
		/*  as an exception to the norm it WILL NOT be popped, so that it can later */
		/*  be executed by longjump(). */
retframe setjump_callstack( stackpair *stkp, void *v_ );
		/* (  -- 1 ); additionally, stkp->ret will be rewound to the location */
		/*  indicated by *( (uintptr_t*)v_ ), before the usual */
		/*  pop-retstack-for-next-instruction behavior (which is why setjump() */
		/*  doesn't pop: longjump() will execute the same code as the corresponding */
		/*  setjump() did, but with a 1 on top of the data stack). */
retframe longjump_callstack( stackpair *stkp, void *v_ );
	/* bookmark needs to be the name for a uintptr_t, localname will be the name for */
	/*  a retframe_parr that can be used with enqueue_returns(). onset_ptr and */
	/*  onjump_ptr need to be pointers to retframe instances: you should be able to */
	/*  figure out the purpose yourself. Several names will be constructed with */
	/*  prefix at the start, and upper-case text continuing afterwards. */
	/* Note that the user-provided onset and onjump retframes will have to contend */
	/*  with a uintptr_t on top of the data stack: THEY MUST NOT SCREW AROUND WITH */
	/*  THIS (other than to VERY CAREFULLY move stuff underneath it), lest the */
	/*  entire system break, IT MUST BE ON TOP OF THE STACK UPON BOTH onset{} AND */
	/*  onjump{}'s RETURNS. Seriously, here there be dragons. */
	/* Effectively: */
		/* (retframe){ ???, (void*)&localname } : */
		/*  (  -- uintptr_t-bookmark ) -> onset ->  ( uintptr_t-bookmark --  ) */
#define LOCALIZE_SETJUMP( bookmark, prefix, localname,  onset_ptr, onjump_ptr ) \
	static const retframe_parr \
		prefix##_ONSET = \
			(retframe_parr){ 4, { \
				(retframe){ &drop, (void*)0 }, \
					/* This requires a pointer to a retframe as v. */ \
				(retframe){ &just_run, (void*)( onset_ptr ) }, \
					/* We've exited onset_ptr successfully, so we're going to */ \
					/*  pass through prefix##_DISPATCH, but we want NOTHING to */ \
					/*  run this time (because we've already done the actual */ \
					/*  work associated with it), so here's a value that will */ \
					/*  do exactly that. */ \
				(retframe){ &vm_push2, (void*)0 }, \
					/* Regretfully, the behavior of the entire system means */ \
					/*  we'll be seeing a SECOND pass through */ \
					/*  prefix##_DISPATCH, and thus a second encounter with */ \
					/*  it's tests, thus we'll need TWO "surpress" flags */ \
					/*  instead of just one. prefix##_ONJUMP doesn't deal with */ \
					/*  this, because it forcefully pops the first pass and */ \
					/*  only deals with the second pass. */ \
				(retframe){ &vm_push2, (void*)0 } \
			} }, \
		prefix##_ONJUMP = \
			(retframe_parr){ 3, { \
				(retframe){ &drop, (void*)0 }, \
					/* This requires a pointer to a retframe as v. */ \
				(retframe){ &just_run, (void*)( onjump_ptr ) }, \
					/* This push is just here because the drop at the end of */ \
					/*  prefix##_DISPATCH is gonna delete something, so we need a */ \
					/*  sacrificial value. */ \
				(retframe){ &vm_push1, (void*)0 } \
			} }, \
		prefix##_DISPATCH = \
			(retframe_parr){ 3, { \
				(retframe){ &run_on0, (void*)&( prefix##_ONSET ) }, \
				(retframe){ &run_on1, (void*)&( prefix##_ONJUMP ) }, \
					/* If setjump() was just run, then on0() will run. If */ \
					/*  longjump() kicked us back to here, then on1() will run. */ \
					/*  If onset_ptr returned normally, then the two instances */ \
					/*  of "2" on the stack will allow the current run and the */ \
					/*  non-removed run of this script to both be surpressed. */ \
				(retframe){ &drop, (void*)0 } \
			} }, \
		localname = \
			(retframe_parr){ 4, { \
				(retframe){ &vm_pushdata, &( bookmark ) }, \
				(retframe){ &setjump_callstack, (void*)&( bookmark ) }, \
					/* setjump() peeks enqueue_returns(), it DOESN'T POP IT! */ \
				(retframe){ &enqueue_returns, (void*)&( prefix##_DISPATCH ) }, \
				(retframe){ &vm_popdata, &( bookmark ) } \
			} };



/* All of the divertthread stuff below is essentially a fancy wrapper for */
/*  setjump/longjump to give it richer facilities and maybe make it */
/*  easier to use. Though it probably needs a few more reviews & revisions */
/*  before it's actually AS EASY to use... */

typedef struct divertthread_info divertthread_info;
typedef retframe (*divertthread_earlyexit_ptr)( stackpair*, divertthread_info*, unsigned );
retframe divertthread_earlyexit_ptr_placeholder( stackpair*, divertthread_info*, unsigned );

typedef struct divertthread_callerinfo
{
		/* The user deals with this, the system doesn't actually care. */
	uintptr_t user_typeid;
	
		/* Acts as divertthread_exit(), but for when using longjump() to */
		/*  jump past LATER setjump() instances. This is for user code */
		/*  to call if that user code needs to jump COMPLETELY OUT of */
		/*  the current divertthread() context instead of just exiting */
		/*  it early. In particular, this MUST be called before the */
		/*  "outer scope" longjump() is called, AND before divertthread() */
		/*  is called again, AND before the divertthread_info{} submitted */
		/*  in the current call context is released, WHICHEVER HAPPENS */
		/*  FIRST: the best bet is to just call it immediately before the */
		/*  related longjump(). It specifically runs cleanup code that is */
		/*  otherwise automatically inlined by divertthread() itself. If */
		/*  the outer longjump() is itself part of a divertthread() */
		/*  context, then that longjump() WON'T WORK CORRECTLY until */
		/*  AFTER this (and any other preceding equivalents) are called. */
		/* Note: You DON'T need to call this when nesting another layer */
		/*  of divertthread(), just when you're trying to bypass the exit */
		/*  code of the associated enclosing layer. */
		/* Also note: This DOES NOT execute the jumpfunc() associated */
		/*  with this divertthread() context, as the assumptions of that */
		/*  function are expected to no longer be valid in any situation */
		/*  where this is needed- if that's wrong, then the user should */
		/*  be able to compensate themselves. */
	struct
	{
		divertthread_earlyexit_ptr handle;
		divertthread_info *data;
		
	} earlyexit;
	
		/* If you want to exit out of the current divertthread() context */
		/*  using the normal route, then just execute this. It'll be */
		/*  initialized with a correctly formatted longjump() by */
		/*  divertthread() itself. */
	retframe longjump;
	
} divertthread_callerinfo;
int push_divertthread_callerinfo( stackframe *stk, divertthread_callerinfo val );
int peek_divertthread_callerinfo( stackframe *stk,  size_t off,  divertthread_callerinfo *val, uintptr_t *user_typeid );
int pop_divertthread_callerinfo( stackframe *stk,  divertthread_callerinfo *val );

struct divertthread_info
{
	uintptr_t bookmark;
		/* Both of these functions MUST comply with the following function */
		/*  signature: */
			/* ( ???1 bookmark -- ???2 bookmark ) */
		/*  What is under "bookmark" DOES NOT matter, but "bookmark" WILL */
		/*  BE on top upon entry, and MUST BE on top AND UNALTERED upon */
		/*  exit, lest the entire system break. This is NOT a small thing, */
		/*  it can completely screw up the stack. */
	retframe setfunc, jumpfunc;
		/* Optional, but valuable. */
	divertthread_callerinfo *recepdata;
};
int push_divertthread_info( stackframe *stk, divertthread_info val );
int peek_divertthread_info( stackframe *stk,  size_t off,  divertthread_info *val );
int pop_divertthread_info( stackframe *stk,  divertthread_info *val );

typedef struct divertthread_pairedinfo
{
	divertthread_info info;
	divertthread_callerinfo callerinfo;
} divertthread_pairedinfo;
#define INIT_divertthread_pairedinfo( varptr ) \
	( varptr )->info.recepdata = &( ( varptr )->callerinfo );

	/* v_ must be a pointer to a divertthread_info{}. The values of the */
	/*  elements in the provided instance will be swapped with "foreign" */
	/*  values during the longevity of the provided setfunc and jumpfunc */
	/*  framefunc{}s, DON'T SCREW WITH THOSE VALUES. The CONTENTS of */
	/*  ->recepdata won't be touched, with the exception of ->earlyexit, */
	/*  which (IF ->recepdata is non-null) will be initialized so that */
	/*  user code can actually use it. */
	/* Note that this is intentionally named in reference to */
	/*  multi-threading (and e.g. ITC threaded code), and Unix's fork() */
	/*  function- I considered naming it branch() or similar, but */
	/*  decided that sounded too much like fork- it doesn't produce a */
	/*  new process, thread, fiber, or anything else, it just diverts */
	/*  execution of the current thread while providing a special exit */
	/*  mechanism to preemptively get back out of that diversion. */
	/* ( uintptr_t -- ??? ) */
		??? /* Note: I need to verify the func sig. */
retframe divertthread( stackpair *stkp, void *v_ );

#endif
/* End tools/longjump.h */
