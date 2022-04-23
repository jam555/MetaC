#include "headers.h"

#include "err/inner_err.h"



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


#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )

#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )

#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), ( endfunc ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )

#define CALLFRAMEFUNC( rethand, retval, callhand, callval,  caller ) \
	CALL_FRAMEFUNC( stkp, rethand, retval, callhand, callval,  &errs, ( caller ), res, stack_ENDRETFRAME )
#define RETFRAMEFUNC( caller ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), res, stack_ENDRETFRAME )



retframe comment_cstyle( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tok, tb;
	
	STACKCHECK( stkp,  comment_cstyle, stack_ENDRETFRAME );
	
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), tb,  comment_cstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tb )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH(
			comment_cstyle,
			"ERROR: comment_cstyle() encountered a non-tokenbranch pointer beneath the top of the stack."
		);
		stack_ENDRETFRAME();
	}
	
	STACKPOP_UINT( stkp->data, tok,  comment_cstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tok )->toktype != TOKTYPE_SYM_COMMENTCL )
	{
		/* Not the end of the comment, so just store and proceed. */
		
		scratch = push_body_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "push_body_tokenbranch", comment_cstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* Fetch & loop. */
		CALLFRAMEFUNC(
			&comment_cstyle, (void*)0,
			&getANDassemble_token, (void*)0,
			
			comment_cstyle
		);
		
	} else {
		
		scratch = set_tail_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "set_tail_tokenbranch", comment_cstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* We're done. */
		RETFRAMEFUNC( comment_cstyle );
	}
}
retframe comment_cppstyle( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tok, tb;
	
	STACKCHECK( stkp,  comment_cppstyle, stack_ENDRETFRAME );
	
	STACKPEEK_UINT( stkp->data, sizeof( uintptr_t ), tb,  comment_cppstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tb )->toktype != TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		TRESPASSPATH(
			comment_cppstyle,
			"ERROR: comment_cppstyle() encountered a non-tokenbranch pointer beneath the top of the stack."
		);
		stack_ENDRETFRAME();
	}
	
	STACKPOP_UINT( stkp->data, tok,  comment_cppstyle, scratch, stack_ENDRETFRAME );
	if( ( (token_head*)tok )->toktype != TOKTYPE_NEWLINE )
	{
		/* Not the end of the comment, so just store and proceed. */
		
		scratch = push_body_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "push_body_tokenbranch", comment_cppstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* Fetch & loop. */
		CALLFRAMEFUNC(
			&comment_cstyle, (void*)0,
			&getANDassemble_token, (void*)0,
			
			comment_cppstyle
		);
		
	} else {
		
		scratch = set_tail_tokenbranch( (tokenbranch*)tb, (token_head*)tok );
		if( !scratch )
		{
			FAILEDINTFUNC( "set_tail_tokenbranch", comment_cppstyle, scratch );
			stack_ENDRETFRAME();
		}
		
			/* We're done. */
		RETFRAMEFUNC( comment_cppstyle );
	}
}

retframe comment_entry( stackpair *stkp, void *v )
{
	int scratch;
	uintptr_t tmp, type;
	
	STACKCHECK( stkp,  comment_entry, stack_ENDRETFRAME );
	
		/* So, DO we have an acceptable token pointed to by the top of the */
		/*  stack? */
	STACKPEEK_UINT( stkp->data, 0, tmp,  comment_entry, scratch, stack_ENDRETFRAME );
	type = ( (token_head*)tmp )->toktype;
		/* Note: "commentop" is C style, "commentline" is C++ style. */
	if ( type != TOKTYPE_SYM_COMMENTOP && type != TOKTYPE_SYM_COMMENTLINE )
	{
		TRESPASSPATH(
			comment_entry,
			"ERROR: comment_entry() encountered a non-comment-opening token pointer on top of the stack."
		);
		stack_ENDRETFRAME();
	}
	
		/* The argument doesn't get used, but the function signature */
		/*  currently requires it. */
	tokenbranch *tb = build_tokenbranch( 0 );
	if( !tb )
	{
		BADNULL( comment_entry, &tb );
		stack_ENDRETFRAME();
	}
	
		/* Start accumulating the comment. */
	scratch = set_lead_tokenbranch( tb, (token_head*)tmp );
	if( !scratch )
	{
		FAILEDINTFUNC( "set_lead_tokenbranch", comment_entry, scratch );
		stack_ENDRETFRAME();
	}
	tb->subtype = TOKTYPE_TOKENGROUP_COMNTMERGE;
	
		/* Swap to tracking the wrapper. */
	STACKPOP_UINT( stkp->data, tmp,  comment_entry, scratch, stack_ENDRETFRAME );
	STACKPUSH_UINT( stkp->data, &( tb->header ),  comment_entry, scratch, stack_ENDRETFRAME );
	
		/* Need to pick the correct accumulator function... */
		/* Just to repeat, "commentop" is C style comments, "commentline" is */
		/*  C++ style comments. */
	framefunc accumulator =
		(
			type == TOKTYPE_SYM_COMMENTOP ?
				&comment_cstyle :
				&comment_cppstyle
		);
	
		/* Time to start accumulating the comment. */
	CALLFRAMEFUNC(
		accumulator, (void*)0,
		&getANDassemble_token, (void*)0,
		
		comment_entry
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
