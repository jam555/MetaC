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


#define STACK_BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )

#define STACK_MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
		
		#define STACK_NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
		
		#define STACK_SIGNEDARG( integer ) STDMSG_SIGNEDARG_WRAPPER( &errs, integer )
		#define STACK_HEXARG( hex ) STDMSG_HEXARG_WRAPPER( &errs, hex )

#define STACK_FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define STACK_TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )



retframe head_lex( stackpair *stkp, void *v )
{
	retframe ret = (retframe){ (framefunc)&end_run, (void*)0 };
		/* The first char read obviously has to go into the token, so we */
		/*  start with a default length of 1. */
	token_head th = (token_head){ /* type */TOKTYPE_INVALID, 1, 0, 0 };
	extrachar a;
	int res2;
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define head_lex_GETFAIL( err ) \
	STACK_MONADICFAILURE( head_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
	
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, head_lex_GETFAIL )
	}
	
	if( isspace( a.c ) )
	{
		if( isblank( a.c ) || a.is_delimited )
		{
			/* space, tab, etc. */
			
			th.toktype = TOKTYPE_SPACE;
			ret.handler = &space_lex;
			
		} else {
			
			/* newline, vtab, etc. */
			
			th.toktype = TOKTYPE_NEWLINE;
			ret.handler = &newline_lex;
		}
		
	} else if( a.c == '_' || isalpha( a.c ) )
	{
		/* Name. */
		
		th.toktype = TOKTYPE_NAME;
		ret.handler = &name_lex;
		
	} else if( isdigit( a.c ) )
	{
		/* Number. */
		
		th.toktype = TOKTYPE_NUMBER;
		ret.handler = &numberentry_lex;
		
	} else if( a.c == '(' || a.c == '[' || a.c == '{' )
	{
		/* Bracket start. */
		
		switch( a.c )
		{
			case '{': th.toktype = TOKTYPE_OPCRL; break;
			case '[': th.toktype = TOKTYPE_OPSQR; break;
			case '(': th.toktype = TOKTYPE_OPPAR; break;
		}
		ret.handler = &brackop_lex;
		
	} else if( a.c == '\"' || a.c == '\'' )
	{
		/* Text start. */
		
		th.toktype = ( a.c == '\'' ? TOKTYPE_SQSTR : TOKTYPE_DQSTR );
		ret.handler = &str_lex;
		
	} else if
	(
		a.c == '!' || a.c == '}' || a.c == '%' || a.c == '&' ||
		a.c == '*' || a.c == '+' || a.c == '-' || a.c == '.' ||
		a.c == '/' || a.c == ':' || a.c == ';' || a.c == '<' ||
		a.c == '=' || a.c == '>' || a.c == '?' || a.c == '^' ||
		a.c == '~' || a.c == '|' || a.c == ',' || a.c == ')' ||
		a.c == ']' || a.c == '\\'
	)
	{
		/* Disambiguate. */
		
		th.toktype = TOKTYPE_SYM;
		ret.handler = &syms_lex;
		
	} else if( a.c == '$' || a.c == '`' )
	{
		/* Unused. */
		
		th.toktype = ( a.c == '$' ? TOKTYPE_DOLLAR : TOKTYPE_GRAVE );
		
		res2 = push_char( &( stkp->data ),  a.c );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_char", head_lex, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		res2 = push_tokenhead( &( stkp->data ),  th );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_tokenhead", head_lex, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RET_FRAMEFUNC( head_lex_refid, -6, 0 );
		
	} else if( a.c == '#' )
	{
		/* Preprocessor. */
		
		th.toktype = TOKTYPE_OCTO;
		
		res2 = push_char( &( stkp->data ),  a.c );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_char", head_lex, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		res2 = push_tokenhead( &( stkp->data ),  th );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_token_head", head_lex, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RET_FRAMEFUNC( head_lex_refid, -7, 0 );
		
	} else if( a.c == '@' )
	{
		/* Meta-C. */
		
		th.toktype = TOKTYPE_AT;
		ret.handler = &at_lex;
		
	} else {
		
		/* Unrecognized. */
		
		th.toktype = TOKTYPE_OTHER;
		
		res2 = push_char( &( stkp->data ),  a.c );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_char", head_lex, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		res2 = push_tokenhead( &( stkp->data ),  th );
		if( !res2 )
		{
			STACK_FAILEDINTFUNC( "push_token_head", head_lex, res2 );
			return( (retframe){ &end_run, (void*)0 } );
		}
		
		RET_FRAMEFUNC( head_lex_refid, -10, 0 );
	}
	
	/* TODO: Are these two REALLY supposed to exit like this, instead of with errors? */
	
	res2 = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_TRESPASSPATH( head_lex, "WARNING: head_lex() exited via a suspect route." );
		RET_FRAMEFUNC( head_lex_refid, -11, &th, &a.c );
	}
	
	res2 = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_TRESPASSPATH( head_lex, "WARNING: head_lex() exited via a suspect route." );
		RET_FRAMEFUNC( head_lex_refid, -12, &th );
	}
	
	return( ret );
}

retframe space_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &space_lex;
	extrachar a;
	
	/* Validation stuff. */
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", space_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_SPACE )
	{
		STACK_TRESPASSPATH( space_lex, "ERROR: space_lex encountered a non-_SPACE token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define space_lex_GETFAIL( err ) \
	STACK_MONADICFAILURE( space_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, space_lex_GETFAIL )
	}
	
	if( isblank( a.c ) || ( isspace( a.c ) && a.is_delimited ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else {
		
		res = unget_extrachar( a );
		if( res )
		{
			/* Success. */
			
			res = push_tokenhead( &( stkp->data ),  th );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_tokenhead", space_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( space_lex_refid, -7, 0 );
			
		} else {
			
			/* Fail, but don't immediately end. */
			
			STACK_FAILEDINTFUNC( "unget_extrachar", space_lex, res );
			hand = &end_run;
		}
	}
	
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", space_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", space_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}

retframe newline_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &newline_lex;
	extrachar a;
	
	/* Validation stuff. */
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", newline_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NEWLINE )
	{
		STACK_TRESPASSPATH( newline_lex, "ERROR: newline_lex encountered a non-_NEWLINE token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define newline_lex_GETFAIL( err ) \
	STACK_MONADICFAILURE( newline_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, newline_lex_GETFAIL )
	}
	
	if( isspace( a.c ) && ( !isblank( a.c ) == !a.is_delimited ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else {
		
		res = unget_extrachar( a );
		if( res )
		{
			/* Success. */
			
			res = push_tokenhead( &( stkp->data ),  th );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_tokenhead", newline_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( newline_lex_refid, -7, 0 );
			
		} else {
			
			/* Fail. */
			
			STACK_FAILEDINTFUNC( "unget_extrachar", newline_lex, res );
			hand = &end_run;
			
			/* Fall-through. */
		}
	}
	
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", newline_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", newline_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}

retframe name_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &nameline_lex;
	extrachar a;
	
	/* Validation stuff. */
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", name_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NAME )
	{
		STACK_TRESPASSPATH( name_lex, "ERROR: name_lex encountered a non-_NAME token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define name_lex_GETFAIL( err ) \
	STACK_MONADICFAILURE( name_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, name_lex_GETFAIL )
	}
	
	if( a.c == '_' && isalpha( a.c ) ) && isdigit( a.c ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else {
		
		res = unget_extrachar( a );
		if( res )
		{
			/* Success. */
			
			res = push_tokenhead( &( stkp->data ),  th );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_tokenhead", name_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( name_lex_refid, -7, 0 );
			
		} else {
			
			/* Fail. */
			
			STACK_FAILEDINTFUNC( "unget_extrachar", name_lex, res );
			hand = &end_run;
			
			/* ... */
		}
	}
	
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", name_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", name_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}


retframe numberentry_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", numberentry_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		STACK_TRESPASSPATH( numberentry_lex, "ERROR: numberentry_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = peek_extrachar( stkp, v );
#define numberentry_lex1_GETFAIL( err ) \
	STACK_MONADICFAILURE( numberentry_lex, "peek_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberentry_lex1_GETFAIL )
	}
	
	switch( a.c )
	{
		default:
				/* Don't even need to look at the next character, it's */
				/*  decimal. */
			hand = &numberdecimal_lex;
			
			break;
			
		case '0':
			{
					/* The extrachar that we peek'd isn't needed anymore, so */
					/*  we're just getting it out of the way. */
				extrachar_result ecr = get_extrachar( stkp, v );
				
#define numberentry_lex2_FETCHFAIL( err ) \
	if( 1 ) { \
		STACK_MONADICFAILURE( numberentry_lex, "get_extrachar", (err) ); \
		return( (retframe){ &end_run, (void*)0 } ); \
	}
				ecr = get_extrachar( stkp, v );
				EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberentry_lex2_FETCHFAIL )
			}
			
			switch( a.c )
			{
				case 'x':
					/* Hexadecimal. */
					
					hand = &numberhexadecimal_lex;
					
					break;
				
				case 'b':
					/* Binary. */
					
					hand = &numberbinary_lex;
					
					break;
				
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
					/* Octal. */
					
					hand = &numberoctal_lex;
					
					break;
				
				default:
					res = unget_extrachar( a );
					if( res )
					{
						/* Success. */
						
						res = push_tokenhead( &( stkp->data ),  th );
						if( !res )
						{
							STACK_FAILEDINTFUNC( "push_tokenhead", numberentry_lex, res );
							return( (retframe){ &end_run, (void*)0 } );
						}
						
						RET_FRAMEFUNC( number_lex_refid, -7, 0 );
						
					} else {
						
						/* Fail. */
						
						STACK_FAILEDINTFUNC( "unget_extrachar", numberentry_lex, res );
						return( (retframe){ &end_run, (void*)0 } );
					}
					
					break;
			}
			
			th.length += 1;
			res = push_char( &( stkp->data ),  a.c );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_char", numberentry_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			break;
	}
	
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", numberentry_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberdecimal_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberdecimal_lex;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", numberdecimal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		STACK_TRESPASSPATH( numberdecimal_lex, "ERROR: numberdecimal_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberdecimal_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( numberdecimal_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberdecimal_lex_FETCHFAIL )
	}
	
	if( !isdigit( a.c ) )
	{
			/* One way or another, we want to break the loop. */
		res = unget_extrachar( a );
		if( res )
		{
			/* Success. */
			
			res = push_tokenhead( &( stkp->data ),  th );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_tokenhead", numberdecimal_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -15, 0 );
			
		} else {
			
			/* Fail. */
			
			STACK_FAILEDINTFUNC( "unget_extrachar", numberdecimal_lex, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", numberdecimal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	th.length += 1;
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", name_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberhexadecimal_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberhexadecimal_lex;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", numberhexadecimal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		STACK_TRESPASSPATH( numberhexadecimal_lex, "ERROR: numberhexadecimal_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberhexadecimal_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( numberhexadecimal_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberhexadecimal_lex_FETCHFAIL )
	}
	
	if( !isxdigit( a.c ) )
	{
			/* One way or another, we want to break the loop. */
		res = unget_extrachar( a );
		if( res )
		{
			/* Success. */
			
			res = push_tokenhead( &( stkp->data ),  th );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_tokenhead", numberhexadecimal_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -23, 0 );
			
		} else {
			
			/* Fail. */
			
			STACK_FAILEDINTFUNC( "unget_extrachar", numberhexadecimal_lex, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", numberhexadecimal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", numberhexadecimal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberoctal_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberoctal_lex;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", numberoctal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		STACK_TRESPASSPATH( numberoctal_lex, "ERROR: numberoctal_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberoctal_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( numberoctal_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberoctal_lex_GETFAIL )
	}
	
	if( !(
		a.c == '0' || a.c == '1' || a.c == '2' || a.c == '3' ||
		a.c == '4' || a.c == '5' || a.c == '6' || a.c == '7'
	) )
	{
			/* One way or another, we want to break the loop. */
		res = unget_extrachar( a );
		if( res )
		{
			/* Success. */
			
			res = push_tokenhead( &( stkp->data ),  th );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_tokenhead", numberoctal_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -31, 0 );
			
		} else {
			
			/* Fail. */
			
			STACK_FAILEDINTFUNC( "unget_extrachar", numberoctal_lex, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", numberoctal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", numberoctal_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberbinary_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberbinary_lex;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", numberbinary_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		STACK_TRESPASSPATH( numberbinary_lex, "ERROR: numberbinary_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberbinary_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( numberbinary_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberbinary_lex_GETFAIL )
	}
	
	if( !( a.c == '0' || a.c == '1' ) )
	{
			/* One way or another, we want to break the loop. */
		res = unget_extrachar( a );
		if( res )
		{
			/* Success. */
			
			res = push_tokenhead( &( stkp->data ),  th );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_tokenhead", numberbinary_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -39, 0 );
			
		} else {
			
			/* Fail. */
			
			STACK_FAILEDINTFUNC( "unget_extrachar", numberbinary_lex, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", numberbinary_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", numberbinary_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}


retframe brackop_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &end_run;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", brackop_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !(
		th.toktype == TOKTYPE_OPCRL ||
		th.toktype == TOKTYPE_OPSQR ||
		th.toktype == TOKTYPE_OPPAR
	) )
	{
		STACK_TRESPASSPATH( brackop_lex, "ERROR: brackop_lex encountered a non- opening-bracket token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define brackop_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( brackop_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, brackop_lex_GETFAIL )
	}
	
	switch( a.c )
	{
		case '^':
			th.toktype += 3;
			break;
		case '<':
			th.toktype += 6;
			break;
		case ':':
			th.toktype += 9;
			break;
		case '*':
			th.toktype += 12;
			break;
		default:
				/* One way or another, we want to break the loop. */
			res = unget_extrachar( a );
			if( res )
			{
				/* Success. */
				
				res = push_tokenhead( &( stkp->data ),  th );
				if( !res )
				{
					STACK_FAILEDINTFUNC( "push_tokenhead", brackop_lex, res );
					return( (retframe){ &end_run, (void*)0 } );
				}
				
				RET_FRAMEFUNC( number_lex_refid, -6, 0 );
				
			} else {
				
				/* Fail. */
				
				STACK_FAILEDINTFUNC( "unget_extrachar", brackop_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
	}
	
	/* Wouldn't be bad to double-check the value of th.toktype here, but not */
	/*  important. */
	
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", brackop_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", brackop_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RET_FRAMEFUNC( number_lex_refid, -10, 0 );
}


retframe str_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &str_lex;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", str_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !(
		th.toktype == TOKTYPE_SQSTR ||
		th.toktype == TOKTYPE_DQSTR
	) )
	{
		STACK_TRESPASSPATH( str_lex, "ERROR: str_lex encountered a non-quote token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define str_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( str_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, str_lex_GETFAIL )
	}
	
	if( th.toktype == TOKTYPE_SQSTR && a.c == '\"' )
	{
		STACK_TRESPASSPATH( str_lex, "ERROR: str_lex encountered character / token type mismatch." );
		
	} else if( th.toktype == TOKTYPE_DQSTR && a.c == '\'' )
	{
		STACK_TRESPASSPATH( str_lex, "ERROR: str_lex encountered character / token type mismatch." );
		
	} else if( a.c == '\\' )
	{
		hand = &escstr_lex;
	}
	
	th.length += 1;
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", str_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", str_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if
	(
		( th.toktype == TOKTYPE_SQSTR && a.c == '\'' ) ||
		( th.toktype == TOKTYPE_DQSTR && a.c == '\"' )
	)
	{
		RET_FRAMEFUNC( str_lex_refid, -9, 0 );
	}
	return( (retframe){ hand, (void*)0 } );
}
retframe escstr_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &str_lex;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", escstr_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !(
		th.toktype == TOKTYPE_SQSTR ||
		th.toktype == TOKTYPE_DQSTR
	) )
	{
		STACK_TRESPASSPATH( escstr_lex, "ERROR: escstr_lex encountered a non-quote token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define escstr_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( escstr_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, escstr_lex_GETFAIL )
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	res = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_char", escstr_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", escstr_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	if
	(
		( th.toktype == TOKTYPE_SQSTR && a.c == '\'' ) ||
		( th.toktype == TOKTYPE_DQSTR && a.c == '\"' )
	)
	{
		RET_FRAMEFUNC( str_lex_refid, -15, 0 );
	}
	return( (retframe){ hand, (void*)0 } );
}


retframe syms_lex( stackpair *stkp, v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ),  &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", syms_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_SYM )
	{
		STACK_TRESPASSPATH( syms_lex, "ERROR: syms_lex encountered a non-_SYM token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define name_lex_GETFAIL( err ) \
	STACK_MONADICFAILURE( syms_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, name_lex_GETFAIL )
	}
	
	switch( a.c )
	{
		case '}':
			th.toktype = TOKTYPE_SYM_CURLYCLOSE;
			
			break;
		case ']':
			th.toktype = TOKTYPE_SYM_SQUARECLOSE;
			
			break;
		case ')':
			th.toktype = TOKTYPE_SYM_PARENCLOSE;
			
			break;
		case '~':
			th.toktype = TOKTYPE_SYM_TILDE;
			
			break;
		case '.':
			th.toktype = TOKTYPE_SYM_DOT;
			
			break;
		case ',':
			th.toktype = TOKTYPE_SYM_COMMA;
			
			break;
		case '\\':
			th.toktype = TOKTYPE_SYM_BSLASH;
			
			break;
			
		default:
			extrachar b;
			
			{
				extrachar_result ecr = get_extrachar( stkp, v );
				
				EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETb, syms_lex_GETFAIL )
			}
			
			/* Note that while "a" is already on the stack, "b" has only */
			/*  just now been obtained from the character source, so they */
			/*  naturally need to be treated differently. */
			
			switch( a.c )
			{
				case '-':
					switch( b.c )
					{
						case '-': th.toktype = TOKTYPE_SYM_DECREMENT; break;
						case '=': th.toktype = TOKTYPE_SYM_PLACEDSUB; break:
						case '>': th.toktype = TOKTYPE_SYM_ELEMEDDEREF; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '*':
					switch( b.c )
					{
						case '/': th.toktype = TOKTYPE_SYM_COMMENTCL; break;
						case '}': th.toktype = TOKTYPE_SYM_DECLCASTCLOSE; break;
						case ']': th.toktype = TOKTYPE_SYM_ARRCASTCLOSE; break;
						case ')': th.toktype = TOKTYPE_SYM_CASTCLOSE; break;
						case '=': th.toktype = TOKTYPE_SYM_PLACEDMULT; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '/':
					switch( b.c )
					{
						case '*': th.toktype = TOKTYPE_SYM_COMMENTOP; break;
						case '/': th.toktype = TOKTYPE_SYM_COMMENTLINE; break;
						case '=': th.toktype = TOKTYPE_SYM_PLACEDDIV; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '^':
					switch( b.c )
					{
						case '}': th.toktype = TOKTYPE_SYM_PRECRLCL; break;
						case ']': th.toktype = TOKTYPE_SYM_PRESQRCL; break;
						case ')': th.toktype = TOKTYPE_SYM_PREPARCL; break;
						case '=': th.toktype = TOKTYPE_SYM_PLACEDXOR; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '>':
					switch( b.c )
					{
						case '}': th.toktype = TOKTYPE_SYM_COMPTCRLCL; break;
						case ']': th.toktype = TOKTYPE_SYM_COMPTSQRCL; break;
						case ')': th.toktype = TOKTYPE_SYM_COMPTPARCL; break;
						case '=': th.toktype = TOKTYPE_SYM_GREATEQUAL; break;
						case '>': return( symsext_lex( &( stkp->data ),  (void*)0, &th, a, b ) );
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case ':':
					switch( b.c )
					{
						case '}': th.toktype = TOKTYPE_SYM_DECLCRLCL; break;
						case ']': th.toktype = TOKTYPE_SYM_DECLSQRCL; break;
						case ')': th.toktype = TOKTYPE_SYM_DECLPARCL; break;
						case ':': th.toktype = TOKTYPE_SYM_SCOPE; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '+':
					switch( b.c )
					{
						case '+': th.toktype = TOKTYPE_SYM_INCREMENT; break;
						case '=': th.toktype = TOKTYPE_SYM_PLACEDADD; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '%':
					switch( b.c )
					{
						case '=': th.toktype = TOKTYPE_SYM_PLACEDMODULO; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '=':
					switch( b.c )
					{
						case '=': th.toktype = TOKTYPE_SYM_EQUALITY; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '!':
					switch( b.c )
					{
						case '=': th.toktype = TOKTYPE_SYM_NOTEQUAL; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '<':
					switch( b.c )
					{
						case '=': th.toktype = TOKTYPE_SYM_LESSEREQUAL; break;
						case '<': return( symsext_lex( &( stkp->data ),  (void*)0, &th, a, b ) );
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '&':
					switch( b.c )
					{
						case '&': th.toktype = TOKTYPE_SYM_LOGICAND; break;
						case '=': th.toktype = TOKTYPE_SYM_PLACEDBINARYAND; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '|':
					switch( b.c )
					{
						case '|': th.toktype = TOKTYPE_SYM_LOGICIOR; break;
						case '=': th.toktype = TOKTYPE_SYM_PLACEDBINARYIOR; break;
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
					break;
				case '?':
					switch( b.c )
					{
						case '?': return( symsext_lex( &( stkp->data ),  (void*)0, &th, a, b ) );
						default: return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
					}
				default:
					return( symssinglet_lex( &( stkp->data ),  (void*)0, &th, a.c, b ) );
			}
			
			th.length += 1;
			res = push_char( &( stkp->data ),  b.c );
			if( !res )
			{
				STACK_FAILEDINTFUNC( "push_char", syms_lex, res );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			break;
	}
	
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", syms_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RET_FRAMEFUNC( number_lex_refid, -8, 0 );
}
retframe symssinglet_lex( stackpair *stkp, void *arg, token_head *th, char a, extrachar b )
{
	if( !th )
	{
		STACK_BADNULL( symssinglet_lex, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	int res = unget_extrachar( a );
	if( res )
	{
		STACK_FAILEDINTFUNC( "unget_extrachar", symssinglet_lex_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	switch( a )
	{
		case '-':
			th.toktype = TOKTYPE_SYM_SUBTRACT;
			
			break;
		case '*':
			th.toktype = TOKTYPE_SYM_MULTIPLY;
			
			break;
		case '/':
			th.toktype = TOKTYPE_SYM_DIVIDE;
			
			break;
		case '^':
			th.toktype = TOKTYPE_SYM_XOR;
			
			break;
		case '>':
			th.toktype = TOKTYPE_SYM_GREATERTHAN;
			
			break;
		case ':':
			th.toktype = TOKTYPE_SYM_COLON;
			
			break;
		case '+':
			th.toktype = TOKTYPE_SYM_ADDITION;
			
			break;
		case '%':
			th.toktype = TOKTYPE_SYM_MODULO;
			
			break;
		case '=':
			th.toktype = TOKTYPE_SYM_SET;
			
			break;
		case '!':
			th.toktype = TOKTYPE_SYM_NOT;
			
			break;
		case '<':
			th.toktype = TOKTYPE_SYM_LESSERTHAN;
			
			break;
		case '&':
			th.toktype = TOKTYPE_SYM_BINARYAND;
			
			break;
		case '|':
			th.toktype = TOKTYPE_SYM_BINARYAND;
			
			break;
		case '?':
			th.toktype = TOKTYPE_SYM_QUERY;
			
			break;
		default:
			th.toktype = TOKTYPE_SYM_UNKNOWN;
			
			break;
	}
	
	
	res = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", symssinglet_lex_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RET_FRAMEFUNC( syms_lex_refid, -12, 0 );
}
retframe symsext_lex( stackpair *stkp, void *arg, token_head *th, extrachar a, extrachar b )
{
	extrachar c;
	int res;
	
	if( !th )
	{
		STACK_BADNULL( symsext_lex, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define symsext_lex_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( symsext_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETc, symsext_lex_FETCHFAIL )
	}
	
#define symsetx_lex_DITOK( erra, errb ) \
		if( 1 ) { \
			res = unget_extrachar( c ); if( !res ) { \
				STACK_FAILEDINTFUNC( "unget_extrachar", symsext_lex, res ); \
				STACK_NOTELINE(); STACK_SIGNEDARG( (erra) ); \
				return( (retframe){ &end_run, (void*)0 } ); } \
			res = push_char( &( stkp->data ),  b.c ); if( !res ) { \
				STACK_FAILEDINTFUNC( "push_char", symsext_lex, res ); \
				STACK_NOTELINE(); STACK_SIGNEDARG( (errb) ); \
				return( (retframe){ &end_run, (void*)0 } ); } }
#define symsext_lex_TRITOK( erra, errb ) \
		if( 1 ) { \
			res = push_char( &( stkp->data ),  b.c ); if( !res ) { \
				STACK_FAILEDINTFUNC( "push_char", symsext_lex, res ); \
				STACK_NOTELINE(); STACK_SIGNEDARG( (erra) ); \
				return( (retframe){ &end_run, (void*)0 } ); } \
			res = push_char( &( stkp->data ),  c.c ); if( !res ) { \
				STACK_FAILEDINTFUNC( "push_char", symsext_lex, res ); \
				STACK_NOTELINE(); STACK_SIGNEDARG( (errb) ); \
				return( (retframe){ &end_run, (void*)0 } ); } \
			th.length += 1; }
	switch( a.c )
	{
		case '>':
			if( b.c == '>' )
			{
				if( c.c == '=' )
				{
					symsext_lex_TRITOK( -15, -16 );
					th.toktype = TOKTYPE_SYM_DIVISIVSHIFT;
					
				} else {
					
					symsetx_lex_DITOK( -17, -18 );
					th.toktype = TOKTYPE_SYM_PLACEDDIVISIVSHIFT;
				}
				
			} else {
				
				STACK_TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered dual contiguous > characters." );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			th.length += 1;
			break;
			
		case '<':
			if( b.c == '<' )
			{
				if( c.c == '=' )
				{
					symsext_lex_TRITOK( -20, -21 );
					th.toktype = TOKTYPE_SYM_MULTIPLYSHIFT;
					
				} else {
					
					symsetx_lex_DITOK( -22, -23 );
					th.toktype = TOKTYPE_SYM_PLACEDMULTIPLYSHIFT;
				}
				
			} else {
				
				STACK_TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered dual contiguous < characters." );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			th.length += 1;
			break;
			
		case '?':
			if( b.c == '?' )
			{
				if( c.c == '?' )
				{
					symsext_lex_TRITOK( -25, -26 );
					th.toktype = TOKTYPE_SYM_CONFUSION;
					
				} else {
					
					res = unget_extrachar( c );
					if( !res )
					{
						STACK_FAILEDINTFUNC( "unget_extrachar", symsext_lex, res );
						return( (retframe){ &end_run, (void*)0 } );
					}
					res = unget_extrachar( b );
					if( !res )
					{
						STACK_FAILEDINTFUNC( "unget_extrachar", symsext_lex, res );
						return( (retframe){ &end_run, (void*)0 } );
					}
					
					th.toktype = TOKTYPE_SYM_QUERY;
				}
				
			} else {
				
				STACK_TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered an isolated ? character." );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			th.length += 1;
			break;
			
		default:
			STACK_TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered an unforeseen character: 0x" );
			STACK_HEXARG( a.c );
			return( (retframe){ &end_run, (void*)0 } );
	}
	
	/* Success. */
	
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", symsext_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RET_FRAMEFUNC( syms_lex_refid, -32, 0 );
}


retframe at_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	int res = pop_tokenhead( &( stkp->data ), &th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "pop_tokenhead", at_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_AT )
	{
		STACK_TRESPASSPATH( at_lex, "ERROR: at_lex encountered a non-_AT token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define at_FETCHFAIL( err ) \
	STACK_MONADICFAILURE( at_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, at_FETCHFAIL )
	}
	
	if( a.c == '<' )
	{
		res = push_char( &( stkp->data ), a.c );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "push_char", at_lex, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		th.toktype = TOKTYPE_SYM_ADDROF;
		th.length += 1;
		
	} else {
		
		res = unget_extrachar( a );
		if( !res )
		{
			STACK_FAILEDINTFUNC( "unget_extrachar", at_lex, res );
			return( (retframe){ &end_run, (void*)0 } );
		}
		th.toktype = TOKTYPE_SYM_DEREFERENCE;
	}
	
	/* Success. */
	
	res = push_tokenhead( &( stkp->data ), th );
	if( !res )
	{
		STACK_FAILEDINTFUNC( "push_tokenhead", at_lex, res );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RET_FRAMEFUNC( at_refid, -8, 0 );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
