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
		
	};
	extern stackpair std_stacks;
	
	
	typedef struct retframe retframe;
	typedef retframe (*framefunc)( stackpair*, void* );
	struct retframe
	{
		framefunc handler;
		void *data;
	};
	
	
	
	stackpair std_stacks;
	
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
	
	
	
	#define stack_ENDRETFRAME() return( (retframe){ &end_run, (void*)0 } )
	
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
	
	
	#define STACK_PEEK_UINT( stk, offset, var,  errfunc, err1, err2,  ... ) \
		if( !peek_uintptr( ( stk ),  ( offset ),  &( var ) ) ) { errfunc( ( err1 ),  __VA_ARGS__,  &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__,  &( var ) ); }
	#define STACK_POP_UINT( stk, var,  errfunc, err1, err2,  ... ) \
		if( !pop_uintptr( ( stk ),  &( var ) ) ) { errfunc( ( err1 ),  __VA_ARGS__,  &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__,  &( var ) ); }
	#define STACK_PUSH_UINT( stk, val,  errfunc, err,  ... ) \
		if( !push_uintptr( ( stk ),  ( val ) ) ) { errfunc( ( err ),  __VA_ARGS__, ( val ) ); }
	
	
		/* I think these need to be MAJORLY reworked. Among other things, I */
		/*  think push_frame() was from a C-ish "single stack" era, whereas */
		/*  I'm currently using a Forth-style multiple-stack system (albeit */
		/*  with more than just two stacks). */
	#define CALL_FRAMEFUNC( rethand, retval, fhand, fval ) \
		if( push_retframe( (retframe){ (rethand), (retval) } ) ) { \
			if( push_frame() ) { \
				return( (retframe){ (fhand), (fval) } ); } \
			else { return( (retframe){ &end_run, (void*)0 } ); } }\
		else { return( (retframe){ &end_run, (void*)0 } ); }
	#define RET_FRAMEFUNC( refname, errnum, ... ) \
		if( 1 ) { \
			stackframe *csf = stack; stack = stack->prev; \
			retframe ret;  int res = pop_retframe( &ret ); \
			if( !res ) { \
				err_interface( &( refname ), (lib4_failure_result){ (errnum) }, __VA_ARGS__ ); \
				return( (retframe){ &end_run, (void*)0 } ); } \
			stack = csf; return( ret ); }
	#define RET_FRAMEFUNC2( refname, subrefnum, ... ) \
		if( 1 ) { \
			stackframe *csf = stack; stack = stack->prev; \
			retframe ret;  int res = pop_retframe( &ret ); \
			if( !res ) { \
				err_interface( &( refname ), ( subrefnum ), __VA_ARGS__ ); \
				return( (retframe){ &end_run, (void*)0 } ); } \
			stack = csf; return( ret ); }
	
#endif
