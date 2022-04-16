#include "headers.h"

#include "../err/inner_err.h"
#include "../tokenization/token.h"
#include ".../tokenization/complexlex.h"



	/* This file actually shoves something inside "errs", so for the sake of */
	/*  reducing the code in preprocessor ifs, we require C++2020 support at */
	/*  the minimum. */
#if defined( __cplusplus ) && __cplusplus >= 202002L
	namespace
	{
		static msg_styleset errs;
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs;
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif


#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &errs, ( caller ), ( endfunc ) )

#define POPUINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define PUSHUINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), ( endfunc ) )

#define CALL_FRAMEFUNC( stkpair, rethand, retval, callhand, callval,  caller, scratch, endfunc ) \
	CALL_FRAMEFUNC( \
		( stkpair ), \
			( rethand ), ( retval ), ( callhand ), ( callval ), \
		&errs, ( caller ), ( scratch ), ( endfunc ) \
	)



	/* This is a simple example of an addition function, because it provides */
	/*  a simple context to show how to implement a custom error report. */
msg_style example_adduint_wrongtype =
	{
			/* As you might suspect, yes, this is a printf() style format */
			/*  string. There is a trick: this version ALSO supports a "%Lp" */
			/*  specifier, which in turn expects both a void pointer and a */
			/*  "customprint_signature" value (a function pointer defined in */
			/*  inner_errs.h), IN THAT ORDER. The function pointer will be */
			/*  used to print the accompanying void pointer via a callback */
			/*  structure provided by the function that called the */
			/*  customprint_signature function. */
		"Error in example_adduint( %p, ignored ), file line %x: encountered type %ju when expecting type %ju."
	};
#define CHECK_TOKTYPE( stkp, tokheaderptr, goodtype,  caller, endfunc ) \
	if( ( tokheaderptr )->toktype != ( goodtype ) ) { \
		msg_interface( \
			&errs, 1, \
			(void*)( stkp ), (unsigned)( __LINE__ ), \
			(uintmax_t)( ( tokheaderptr )->toktype ), (uintmax_t)( goodtype ) \
		); \
		( endfunc )(); }
retframe example_adduint( stackpair *stkp, void *v_ )
{
	STACKCHECK( stkp,  example_adduint, stack_ENDRETFRAME );
	
	uintptr_t tmp;
	token_uint *a, *b;
	int scratch;
	
	POPUINT( stkp->data, &tmp,  example_adduint, scratch, stack_ENDRETFRAME );
	b = (token_uint*)tmp;
	CHECK_TOKTYPE( stkp, &( b->header ), TOKTYPE_NUMBER_UINT,  example_adduint, stack_ENDRETFRAME );
	
	POPUINT( stkp->data, &tmp,  example_adduint, scratch, stack_ENDRETFRAME );
	a = (token_uint*)tmp;
	CHECK_TOKTYPE( stkp, &( a->header ), TOKTYPE_NUMBER_UINT,  example_adduint, stack_ENDRETFRAME );
	
	
		/* The core of the function: add and push the result. */
	a->val += b->val;
	PUSHUINT( stkp->data, (uintptr_t)a,  example_adduint, scratch, stack_ENDRETFRAME );
	
	
		/* We don't need "b" anymore, so delegate it's destruction to the */
		/*  standardized deallocator. */
	PUSHUINT( stkp->data, (uintptr_t)b,  example_adduint, scratch, stack_ENDRETFRAME );
	return(
		(retframe)
		{
			&invoke_dealloctoken,
			(void*)0
		}
	);
}



	/* For "native" compatibility in both C and C++, DO NOT use compound */
	/*  literals. Designated initializers with this specific syntax are C as */
	/*  of C99, and C++ as of C++20; thus, it's the best syntax for this. */
	/* Note that this is an array, not JUST a single instance. And yes, it */
	/*  SHOULD be an array, because that's what "msg_styleset" is SUPPOSED */
	/*  to support, hence the sizeof() muckery in both versions. */
static msg_piece msgs[] =
	{
		{ .style = &example_adduint_wrongtype }, 0
	};

/* A note about "msg_piece": the first member of msg_piece is a union of */
/*  anonymous type that contains a pointer to a "msg_style" element- one of */
/*  these is provided & implemented in this file. HOWEVER, it also contains */
/*  a pointer to a "msg_styleset" element, thereby allowing style sets to */
/*  nest (if you choose to do this, then every msg_piece that contains a */
/*  style set pointer must have it's non-union member set to "1" instead of */
/*  "0"). This nesting is possible so that error reporting can be */
/*  sub-divided where appropriate, and is made use of by just adding more */
/*  ERR_KEYTYPE values to your msg_interface invocation, to whatever depth */
/*  is needed to reach the desired error format. */

#if defined( __cplusplus ) && __cplusplus >= 202002L
	namespace
	{
		msg_styleset errs =
			{
				msgs,
				sizeof( msgs ) / sizeof( msgs[ 0 ] )
			};
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs =
		(msg_styleset)
		{
			msgs,
			sizeof( msgs ) / sizeof( msgs[ 0 ] )
		};
#endif
