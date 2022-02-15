#ifndef METAC_STACK_H
 #define METAC_STACK_H
	
	#include "headers.h"
	
	
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
	
	
	#define STACK_CHECK( stack,  errfunc, errnum,  ... ) \
		if( !( stack ) ){ errfunc( ( errnum ),  __VA_ARGS__  ); }
	#define STACK_CHECK2( stack, v,  errfunc, errnum,  ... ) \
		if( !( stack ) || !( v ) ){ errfunc( ( errnum ),  __VA_ARGS__  ); }
	
	#define STACK_PEEK_UINT( stk, offset, var,  errfunc, err1, err2,  ... ) \
		if( !peek_uintptr( ( stk ),  ( offset ),  &( var ) ) ) { errfunc( ( err1 ),  __VA_ARGS__,  &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__,  &( var ) ); }
	#define STACK_POP_UINT( stk, var,  errfunc, err1, err2,  ... ) \
		if( !pop_uintptr( ( stk ),  &( var ) ) ) { errfunc( ( err1 ),  __VA_ARGS__,  &( var ) ); } \
		if( !( var ) ) { errfunc( ( err2 ),  __VA_ARGS__,  &( var ) ); }
	#define STACK_PUSH_UINT( stk, val,  errfunc, err,  ... ) \
		if( !push_uintptr( ( stk ),  ( val ) ) ) { errfunc( ( err ),  __VA_ARGS__, ( val ) ); }
	
#endif
