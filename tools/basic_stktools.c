#include "headers.h"
#include "stack.h"

#include "basic_stktools.h"



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



#define STACKCHECK( stack,  caller ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), lexalike_ENDRETFRAME )

#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )

#define RETFRAMEFUNC( stkpair,  caller ) \
	RET_FRAMEFUNC( ( stkpair ),  &errs, ( caller ), res, stack_ENDRETFRAME )



retframe enqueue_returns( stackpair *stkp, void *v )
{
	retframe_parr *rets;
	uintptr_t a, b;
	int scratch;
	size_t iter;
	
	STACKCHECK( stkp,  enqueue_returns );
	if( !v )
	{
		???
	}
	
	rets = (retframe_parr*)v;
	iter = rets->len;
	while( iter )
	{
		--iter;
		
		scratch = push_retframe( stkp->ret, rets->body[ iter ] );
		if( !scratch )
		{
			STDMSG_FAILEDINTFUNC_WRAPPER( &err, "push_retframe", enqueue_returns, scratch );
			( endfunc )();
		}
	}
	
	RETFRAMEFUNC( stkp,  enqueue_returns )
}



retframe dup( stackpair *stkp, void *v )
{
	uintptr_t a;
	int scratch;
	
	STACKCHECK( stkp,  dup );
	
	
	STACKPEEK_UINT( stkp->data, 0, a,  dup, scratch );
	STACKPUSH_UINT( stkp->data, a,  dup, scratch );
	
	
	RETFRAMEFUNC( stkp,  dup );
}



retframe swap2nd( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  swap2nd );
	
	
	STACKPOP_UINT( stkp->data, a,  swap2nd, scratch );
	STACKPOP_UINT( stkp->data, b,  swap2nd, scratch );
	
	STACKPUSH_UINT( stkp->data, a,  swap2nd, scratch );
	STACKPUSH_UINT( stkp->data, b,  swap2nd, scratch );
	
	
	RETFRAMEFUNC( stkp,  swap2nd );
}
retframe swap3rd( stackpair *stkp, void *v )
{
	uintptr_t a, b, c;
	int scratch;
	
	STACKCHECK( stkp,  swap3rd );
	
	
	STACKPOP_UINT( stkp->data, a,  swap3rd, scratch );
	STACKPOP_UINT( stkp->data, b,  swap3rd, scratch );
	STACKPOP_UINT( stkp->data, c,  swap3rd, scratch );
	
	STACKPUSH_UINT( stkp->data, a,  swap3rd, scratch );
	STACKPUSH_UINT( stkp->data, b,  swap3rd, scratch );
	STACKPUSH_UINT( stkp->data, c,  swap3rd, scratch );
	
	
	RETFRAMEFUNC( stkp,  swap3rd );
}
retframe swap4th( stackpair *stkp, void *v )
{
	uintptr_t a, b, c, d;
	int scratch;
	
	STACKCHECK( stkp,  swap4th );
	
	
	STACKPOP_UINT( stkp->data, a,  swap4th, scratch );
	STACKPOP_UINT( stkp->data, b,  swap4th, scratch );
	STACKPOP_UINT( stkp->data, c,  swap4th, scratch );
	STACKPOP_UINT( stkp->data, d,  swap4th, scratch );
	
	STACKPUSH_UINT( stkp->data, a,  swap4th, scratch );
		/* Yes, intended: these two don't need to get swapped. */
	STACKPUSH_UINT( stkp->data, c,  swap4th, scratch );
	STACKPUSH_UINT( stkp->data, b,  swap4th, scratch );
	STACKPUSH_UINT( stkp->data, d,  swap4th, scratch );
	
	
	RETFRAMEFUNC( stkp,  swap4th );
}



retframe and2( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  and2 );
	
	
	STACKPOP_UINT( stkp->data, a,  and2, scratch );
	STACKPOP_UINT( stkp->data, b,  and2, scratch );
	
	STACKPUSH_UINT( stkp->data, a & b,  and2, scratch );
	
	RETFRAMEFUNC( stkp,  and2 );
}
retframe and3( stackpair *stkp, void *v )
{
	uintptr_t a, b, c;
	int scratch;
	
	STACKCHECK( stkp,  and3 );
	
	
	STACKPOP_UINT( stkp->data, a,  and3, scratch );
	STACKPOP_UINT( stkp->data, b,  and3, scratch );
	STACKPOP_UINT( stkp->data, c,  and3, scratch );
	
	STACKPUSH_UINT( stkp->data, a & b & c,  and3, scratch );
	
	RETFRAMEFUNC( stkp,  and3 );
}
retframe and4( stackpair *stkp, void *v )
{
	uintptr_t a, b, c, d;
	int scratch;
	
	STACKCHECK( stkp,  and4 );
	
	
	STACKPOP_UINT( stkp->data, a,  and4, scratch );
	STACKPOP_UINT( stkp->data, b,  and4, scratch );
	STACKPOP_UINT( stkp->data, c,  and4, scratch );
	STACKPOP_UINT( stkp->data, d,  and4, scratch );
	
	STACKPUSH_UINT( stkp->data, a & b & c & d,  and4, scratch );
	
	RETFRAMEFUNC( stkp,  and4 );
}

retframe ior2( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  ior2 );
	
	
	STACKPOP_UINT( stkp->data, a,  ior2, scratch );
	STACKPOP_UINT( stkp->data, b,  ior2, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b,  ior2, scratch );
	
	RETFRAMEFUNC( stkp,  ior2 );
}
retframe ior3( stackpair *stkp, void *v )
{
	uintptr_t a, b, c;
	int scratch;
	
	STACKCHECK( stkp,  ior3 );
	
	
	STACKPOP_UINT( stkp->data, a,  ior3, scratch );
	STACKPOP_UINT( stkp->data, b,  ior3, scratch );
	STACKPOP_UINT( stkp->data, c,  ior3, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b | c,  ior3, scratch );
	
	RETFRAMEFUNC( stkp,  ior3 );
}
retframe ior4( stackpair *stkp, void *v )
{
	uintptr_t a, b, c, d;
	int scratch;
	
	STACKCHECK( stkp,  ior4 );
	
	
	STACKPOP_UINT( stkp->data, a,  ior4, scratch );
	STACKPOP_UINT( stkp->data, b,  ior4, scratch );
	STACKPOP_UINT( stkp->data, c,  ior4, scratch );
	STACKPOP_UINT( stkp->data, d,  ior4, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b | c | d,  ior4, scratch );
	
	RETFRAMEFUNC( stkp,  ior4 );
}

retframe xor2( stackpair *stkp, void *v )
{
	uintptr_t a, b;
	int scratch;
	
	STACKCHECK( stkp,  xor2 );
	
	
	STACKPOP_UINT( stkp->data, a,  xor2, scratch );
	STACKPOP_UINT( stkp->data, b,  xor2, scratch );
	
	STACKPUSH_UINT( stkp->data, a | b,  xor2, scratch );
	
	RETFRAMEFUNC( stkp,  xor2 );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
