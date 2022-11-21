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

	/* Copies an array of retframe instances onto the return */
	/*  stack, and immediately begins executing them in order, */
	/*  starting with the one at the start of the array. */
	/* Note that v must be a pointer to a pascal array of */
	/*  retframes: these are the retframes that will be pushed */
	/*  onto the stack, and ->body[ 0 ] will be the first one */
	/*  executed. */
	/* This function was directly inspired by (but not used */
	/*  in) require_preprocopener() from basic_toktools.c, */
	/*  where it would have been useful. Think of it as a */
	/*  variation on direct threaded code. */
retframe enqueue_returns( stackpair *stkp, void *v );

	/* ( uintptr_t*a -- uintptr_t*a uintptr_t*a ) */
retframe dup( stackpair *stkp, void *v );
	/* ( uintptr_t -- ) */
retframe drop( stackpair *stkp, void *v );

	/* ( uintptr_t*a uintptr_t*b -- uintptr_t*b uintptr_t*a ) */
retframe swap2nd( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t uintptr_t*b -- uintptr_t*b uintptr_t uintptr_t*a ) */
retframe swap3rd( stackpair *stkp, void *v );
	/* ( uintptr_t*a uintptr_t uintptr_t uintptr_t*b -- uintptr_t*b uintptr_t uintptr_t uintptr_t*a ) */
retframe swap4th( stackpair *stkp, void *v );


	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe and2( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe and3( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe and4( stackpair *stkp, void *v );

	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe ior2( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe ior3( stackpair *stkp, void *v );
	/* ( uintptr_t uintptr_t uintptr_t uintptr_t -- uintptr_t ) */
retframe ior4( stackpair *stkp, void *v );

	/* ( uintptr_t uintptr_t -- uintptr_t ) */
retframe xor2( stackpair *stkp, void *v );


retframe vm_push0( stackpair *stkp, void *v );
retframe vm_push1( stackpair *stkp, void *v );
retframe vm_push2( stackpair *stkp, void *v );
retframe vm_pushmax( stackpair *stkp, void *v );

	/* These require v_ to point to a uintptr_t, which will be used as the data */
	/*  in question. */
retframe vm_pushdata( stackpair *stkp, void *v_ );
retframe vm_popdata( stackpair *stkp, void *v_ );
	/* retframe*, not uintptr_t pointer. */
retframe vm_pushretframe( stackpair *stkp, void *v_ );


	/* These all require a pointer to a retframe as v. */
retframe just_run( stackpair *stkp, void *v );
	/* ( uintptr_t -- uintptr_t ) */
retframe run_if( stackpair *stkp, void *v );
retframe run_else( stackpair *stkp, void *v );
retframe run_on0( stackpair *stkp, void *v );
retframe run_on1( stackpair *stkp, void *v );
retframe run_on2( stackpair *stkp, void *v );
retframe run_on3( stackpair *stkp, void *v );
	/* Pops a retframe from the data stack, and returns it. */
retframe vm_datacall( stackpair *stkp, void *v );



typedef struct divertthread_info divertthread_info;
typedef retframe (*divertthread_earlyexit_ptr)( stackpair*, divertthread_info*, unsigned );

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
	framefunc setfunc, jumpfunc;
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


/* All of the setjump()/longjump() stuff below is used to implement the */
/*  divertthread() stuff. */
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
				(retframe){ &just_run, (void*)( onset_ptr ) }, \
				(retframe){ &vm_push2, (void*)0 }, \
					/* We've exited onset_ptr successfully, so we'll be seeing a */ \
					/*  SECOND pass through prefix##_DISPATCH, thus we'll need */ \
					/*  TWO "surpress" values/flags instead of just one. */ \
					/*  prefix##_ONJUMP doesn't deal with this, because it */ \
					/*  forcefully drops back through the first pass and only */ \
					/*  deals with the second pass. */ \
				(retframe){ &vm_push2, (void*)0 } \
			} }, \
		prefix##_ONJUMP = \
			(retframe_parr){ 3, { \
				(retframe){ &drop, (void*)0 }, \
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
					/* setjump() peeks just_run(), it DOESN'T POP IT! */ \
				(retframe){ &just_run, (void*)&( prefix##_DISPATCH ) }, \
				(retframe){ &vm_popdata, &( bookmark ) } \
			} };
