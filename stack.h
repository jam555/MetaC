#ifndef METAC_STACK_H
 #define METAC_STACK_H
	
	#include "headers.h"
	
	#include "err/inner_err.h"
	
	
	
	typedef struct stackframe stackframe;
	struct stackframe
	{
		char_pascalarray *stack;
		size_t used;
		
	};
	
	typedef struct stackpair stackpair;
	struct stackpair
	{
		stackframe ret, data;
		int run;
		
	};
	extern stackpair std_stacks;
	
	
	typedef struct retframe retframe;
	typedef retframe (*framefunc)( stackpair*, void* );
	struct retframe
	{
		framefunc handler;
		void *data;
	};
	
	
	int enable_loop( stackpair* );
	int run_loop( retframe*,  stackpair* );
	retframe end_run( stackpair*, void* );
	
	
	
	int init_stack( stackframe *stk );
	int resize_stack( stackframe *stk,  int deltaChars );
	int clear_stack( stackframe *stk );
	
	
	int push_char( stackframe *stk,  char val );
	int pop_char( stackframe *stk,  char *dest );
	int peek_char( stackframe *stk,  size_t off,  char *dest );
	
	
	int push_int( stackframe *stk,  int val );
	int pop_int( stackframe *stk,  int *dest );
	int peek_int( stackframe *stk,  size_t off,  int *dest );
	
	int push_uintptr( stackframe *stk,  uintptr_t val );
	int pop_uintptr( stackframe *stk,  uintptr_t *dest );
	int peek_uintptr( stackframe *stk,  size_t off,  uintptr_t *dest );
	
	int push_retframe( stackframe *stk,  retframe rf );
	int pop_retframe( stackframe *stk,  retframe *rf );
	int peek_retframe( stackframe *stk,  size_t off,  retframe *rf );
	
	int push_tokenhead( stackframe *stk,  token_head val );
	int pop_tokenhead( stackframe *stk,  token_head *dest );
	int peek_tokenhead( stackframe *stk,  size_t off,  token_head *dest );
	
	
	int push_block( stackframe *stk,  const char *src, size_t len );
	int pop_block( stackframe *stk,  char *dest, size_t len );
	
	int push_token( stackframe *stk,  token_head *src );
	int pop_token( stackframe *stk,  token_head **dest );
	
	
	
	#define stack_ENDRETFRAME( ... ) return( (retframe){ &end_run, (void*)0 } )
	
		/* Old version: STACK_CHECK( stack,  errfunc, errnum,  ... ) */
	#define STACK_CHECK( stack,  stylesetptr, caller, endfunc ) \
		if( !( stack ) ) { \
			STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( stack ) ); \
			( endfunc )(); }
		/* Old version: STACK_CHECK2( stack, v,  errfunc, errnum,  ... ) */
	#define STACK_CHECK2( stack, v,  stylesetptr, caller, endfunc ) \
		if( !( stack ) || !( v ) ) { \
			STDMSG_BADNULL2_WRAPPER( ( stylesetptr ), ( caller ), &( stack ), &( v ) ); \
			( endfunc )(); }
	
	
		/* Old version: STACK_PEEK_UINT( stk, offset, var,  errfunc, err1, err2,  ... ) */
	#define STACK_PEEK_UINT( stk, offset, dest,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = peek_uintptr( ( stk ),  ( offset ),  &( dest ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "peek_uintptr", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( dest ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( dest ) ); \
				( endfunc )(); } }
		/* Old version: STACK_POP_UINT( stk, var,  errfunc, err1, err2,  ... ) */
	#define STACK_POP_UINT( stk, dest,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = pop_uintptr( ( stk ),  &( dest ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "pop_uintptr", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( dest ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( dest ) ); \
				( endfunc )(); } }
		/* Old version: STACK_PUSH_UINT( stk, val,  errfunc, err,  ... ) */
	#define STACK_PUSH_UINT( stk, val,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = push_uintptr( ( stk ),  ( val ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "push_uintptr", ( caller ), ( scratch ) ); \
				( endfunc )(); } }
	
	
		/* Old version: CALL_FRAMEFUNC( rethand, retval, fhand, fval ) */
	#define CALL_FRAMEFUNC( stkpair, rethand, retval, callhand, callval,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = push_retframe( ( stkpair )->ret, (retframe){ (rethand), (retval) } ); \
			if( ( scratch ) ) { \
				return( (retframe){ (callhand), (callval) } ); } \
			else { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "push_retframe", ( caller ), ( scratch ) ); \
				( endfunc )(); } }
		/* Old version: RET_FRAMEFUNC( refname, errnum, ... ) */
		/* Old version: RET_FRAMEFUNC2( refname, subrefnum, ... ) */
	#define RET_FRAMEFUNC( stkpair,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			retframe RET_FRAMEFUNC_ret; \
			( scratch ) = pop_retframe( ( stkpair )->ret, &RET_FRAMEFUNC_ret ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "pop_retframe", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			return( RET_FRAMEFUNC_ret ); }
	
#endif
