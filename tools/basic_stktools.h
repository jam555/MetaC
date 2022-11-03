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
	/* Pops a retframe from the data stack, and returns it. */
retframe vm_datacall( stackpair *stkp, void *v );


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
	/*  THIS (ther than to VERY CAREFULLY move stuff underneath it), lest the entire */
	/*  system break, IT MUST BE ON TOP OF THE STACK UPON BOTH onset{} AND */
	/*  onjump{}'s RETURNS. Seriously, here there be dragons. */
	/* Effectively: */
		/* (retframe){ ???, (void*)&localname } : */
		/*  (  -- uintptr_t-bookmark ) -> onset ->  ( uintptr_t-bookmark --  ) */
#define LOCALIZE_SETJUMP( bookmark, prefix, localname,  onset_ptr, onjump_ptr ) \
	static const retframe_parr \
		prefix##_ONJUMP = \
			(retframe_parr){ 3, { \
				(retframe){ &drop, (void*)0 }, \
				(retframe){ &just_run, (void*)( onjump_ptr ) } \
				(retframe){ &vm_push1, (void*)0 } \
			} }, \
		prefix##_ONSET = \
			(retframe_parr){ 3, { \
				(retframe){ &drop, (void*)0 }, \
				(retframe){ &just_run, (void*)( onset_ptr ) }, \
				(retframe){ &vm_push0, (void*)0 } \
			} }, \
		prefix##_DISPATCH = \
			(retframe_parr){ 3, { \
					/* This odd order is to preserve the normal if-else order. */ \
				(retframe){ &run_if, (void*)&( prefix##_ONJUMP ) }, \
				(retframe){ &run_else, (void*)&( prefix##_ONSET ) }, \
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
