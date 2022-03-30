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


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )

#define MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
		
		#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
		
		#define SIGNEDARG( integer ) STDMSG_SIGNEDARG_WRAPPER( &errs, integer )
		#define HEXARG( hex ) STDMSG_HEXARG_WRAPPER( &errs, hex )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )



#define RETFRAMEFUNC( caller ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), res, stack_ENDRETFRAME )



#define PUSHCHAR( stk, val,  caller, scratch ) \
	( scratch ) = push_char( ( stk ),  ( val ) ); \
	if( !( scratch ) ) { \
		FAILEDINTFUNC( "push_char", ( caller ), ( scratch ) ); \
		return( (retframe){ &end_run, (void*)0 } ); }

#define POPTOKENHEAD( stk, varptr,  caller, scratch ) \
	( scratch ) = pop_tokenhead( ( stk ),  ( varptr ) ); \
	if( !( scratch ) ) { \
		FAILEDINTFUNC( "push_tokenhead", ( caller ), ( scratch ) ); \
		return( (retframe){ &end_run, (void*)0 } ); }
#define PUSHTOKENHEAD( stk, val,  caller, scratch ) \
	( scratch ) = push_tokenhead( ( stk ),  ( val ) ); \
	if( !( scratch ) ) { \
		FAILEDINTFUNC( "push_tokenhead", ( caller ), ( scratch ) ); \
		return( (retframe){ &end_run, (void*)0 } ); }

#define UNGETEXTRACHAR_1( val,  caller, scratch ) \
	if( 1 ) { \
		( scratch ) = unget_extrachar( ( val ) ); \
		if( !( scratch ) ) { \
			FAILEDINTFUNC( "unget_extrachar", ( caller ), ( scratch ) ); \
			return( (retframe){ &end_run, (void*)0 } ); } }
#define SETHAND_ENDRUN() hand = &end_run;
#define RETURN_ENDRUN() return( (retframe){ &end_run, (void*)0 } );
#define UNGETEXTRACHAR_2( stk, val, tokhead,  caller, scratch, onerr ) \
	if( 1 ) 
		( scratch ) = unget_extrachar( ( val ) ); \
		if( ( scratch ) ) { /* Success. */ \
			PUSHTOKENHEAD( &( ( stk )->data ), ( tokhead ),  ( caller ), ( scratch ) ); \
			RETFRAMEFUNC( ( caller ) ); \
		} else { /* Failure. */ \
			FAILEDINTFUNC( "unget_extrachar", ( caller ), ( scratch ) ); \
			( onerr )(); } }



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
	MONADICFAILURE( head_lex, "get_extrachar", (err) ); \
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
		
		PUSHCHAR( &( stkp->data ), a.c,   head_lex, res2 );
		PUSHTOKENHEAD( &( stkp->data ), th,  head_lex, res2 );
		
		RETFRAMEFUNC( head_lex );
		
	} else if( a.c == '#' )
	{
		/* Preprocessor. */
		
		th.toktype = TOKTYPE_OCTO;
		
		PUSHCHAR( &( stkp->data ), a.c,   head_lex, res2 );
		PUSHTOKENHEAD( &( stkp->data ), th,  head_lex, res2 );
		
		RETFRAMEFUNC( head_lex );
		
	} else if( a.c == '@' )
	{
		/* Meta-C. */
		
		th.toktype = TOKTYPE_AT;
		ret.handler = &at_lex;
		
	} else {
		
		/* Unrecognized. */
		
		th.toktype = TOKTYPE_OTHER;
		
		PUSHCHAR( &( stkp->data ), a.c,   head_lex, res2 );
		PUSHTOKENHEAD( &( stkp->data ), th,  head_lex, res2 );
		
		RETFRAMEFUNC( head_lex );
	}
	
	PUSHCHAR( &( stkp->data ), a.c,   head_lex, res2 );
	PUSHTOKENHEAD( &( stkp->data ), th,  head_lex, res2 );
	
	TRESPASSPATH( head_lex, "ERROR: head_lex() exited via it's ending." );
	return( ret );
}

retframe space_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &space_lex;
	extrachar a;
	int res;
	
	/* Validation stuff. */
	POPTOKENHEAD( &( stkp->data ), &th,  space_lex, res );
	if( th.toktype != TOKTYPE_SPACE )
	{
		TRESPASSPATH( space_lex, "ERROR: space_lex encountered a non-_SPACE token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define space_lex_GETFAIL( err ) \
	MONADICFAILURE( space_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, space_lex_GETFAIL )
	}
	
	if( isblank( a.c ) || ( isspace( a.c ) && a.is_delimited ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else {
		
		UNGETEXTRACHAR_2( stkp, a, th,  space_lex, res, SETHAND_ENDRUN );
	}
	
	PUSHCHAR( &( stkp->data ), a.c,  space_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  space_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}

retframe newline_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &newline_lex;
	extrachar a;
	int res;
	
	/* Validation stuff. */
	POPTOKENHEAD( &( stkp->data ), &th,  newline_lex, res );
	if( th.toktype != TOKTYPE_NEWLINE )
	{
		TRESPASSPATH( newline_lex, "ERROR: newline_lex encountered a non-_NEWLINE token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define newline_lex_GETFAIL( err ) \
	MONADICFAILURE( newline_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, newline_lex_GETFAIL )
	}
	
	if( isspace( a.c ) && ( !isblank( a.c ) == !a.is_delimited ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else {
		
		UNGETEXTRACHAR_2( stkp, a, th,  newline_lex, res, SETHAND_ENDRUN );
	}
	
	PUSHCHAR( &( stkp->data ),  a.c,  newline_lex, res );
	PUSHTOKENHEAD( &( stkp->data ),  th,  newline_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}

retframe name_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &nameline_lex;
	extrachar a;
	
	/* Validation stuff. */
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  name_lex, res );
	if( th.toktype != TOKTYPE_NAME )
	{
		TRESPASSPATH( name_lex, "ERROR: name_lex encountered a non-_NAME token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define name_lex_GETFAIL( err ) \
	MONADICFAILURE( name_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, name_lex_GETFAIL )
	}
	
	if( a.c == '_' && isalpha( a.c ) ) && isdigit( a.c ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else {
		
		UNGETEXTRACHAR_2( stkp, a, th,  name_lex, res, SETHAND_ENDRUN );
	}
	
	PUSHCHAR( &( stkp->data ), a.c,  name_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  name_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}


retframe numberentry_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	POPTOKENHEAD( &( stkp->data ), &th,  numberentry_lex, res );
	if( th.toktype != TOKTYPE_NUMBER )
	{
		TRESPASSPATH( numberentry_lex, "ERROR: numberentry_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = peek_extrachar( stkp, v );
#define numberentry_lex1_GETFAIL( err ) \
	MONADICFAILURE( numberentry_lex, "peek_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberentry_lex1_GETFAIL )
	}
	
	switch( a.c )
	{
		case '0':
			{
					/* The extrachar that we peek'd isn't needed anymore, so */
					/*  we're just getting it out of the way. */
				extrachar_result ecr = get_extrachar( stkp, v );
				
#define numberentry_lex2_FETCHFAIL( err ) \
	if( 1 ) { \
		MONADICFAILURE( numberentry_lex, "get_extrachar", (err) ); \
		return( (retframe){ &end_run, (void*)0 } ); }
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
					UNGETEXTRACHAR_2( stkp, a, th,  numberentry_lex, res, RETURN_ENDRUN );
					
					break;
			}
			
			th.length += 1;
			PUSHCHAR( &( stkp->data ), a.c,  numberentry_lex, res );
			
			break;
			
		default:
				/* Don't even need to look at the next character, it's */
				/*  decimal. */
			hand = &numberdecimal_lex;
			
			/* Note: This could technically use validation, but it shouldn't */
			/*  ever be called via anything other than this file, so not */
			/*  important. */
			
			break;
	}
	
	PUSHTOKENHEAD( &( stkp->data ), th,  numberentry_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberdecimal_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberdecimal_lex;
	extrachar a;
	int res;
	
	POPTOKENHEAD( &( stkp->data ), &th,  numberdecimal_lex, res );
	if( th.toktype != TOKTYPE_NUMBER )
	{
		TRESPASSPATH( numberdecimal_lex, "ERROR: numberdecimal_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberdecimal_lex_FETCHFAIL( err ) \
	MONADICFAILURE( numberdecimal_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberdecimal_lex_FETCHFAIL )
	}
	
	if( !isdigit( a.c ) )
	{
			/* One way or another, we want to break the loop. */
		UNGETEXTRACHAR_2( stkp, a, th,  numberdecimal_lex, res, RETURN_ENDRUN );
	}
	
		/* Grow the string back onto the stack. */
	th.length += 1;
	PUSHCHAR( &( stkp->data ), a.c,  numberdecimal_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  numberdecimal_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberhexadecimal_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberhexadecimal_lex;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  numberhexadecimal_lex, res );
	if( th.toktype != TOKTYPE_NUMBER )
	{
		TRESPASSPATH( numberhexadecimal_lex, "ERROR: numberhexadecimal_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberhexadecimal_lex_FETCHFAIL( err ) \
	MONADICFAILURE( numberhexadecimal_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberhexadecimal_lex_FETCHFAIL )
	}
	
	if( !isxdigit( a.c ) )
	{
			/* One way or another, we want to break the loop. */
		UNGETEXTRACHAR_2( stkp, a, th,  numberhexadecimal_lex, res, RETURN_ENDRUN );
	}
	
		/* Grow the string back onto the stack. */
	th.length += 1;
	PUSHCHAR( &( stkp->data ), a.c,  numberhexadecimal_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  numberhexadecimal_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberoctal_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberoctal_lex;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  numberoctal_lex, res );
	if( th.toktype != TOKTYPE_NUMBER )
	{
		TRESPASSPATH( numberoctal_lex, "ERROR: numberoctal_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberoctal_lex_FETCHFAIL( err ) \
	MONADICFAILURE( numberoctal_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberoctal_lex_GETFAIL )
	}
	
	if( !(
		a.c == '0' || a.c == '1' || a.c == '2' || a.c == '3' ||
		a.c == '4' || a.c == '5' || a.c == '6' || a.c == '7'
	) )
	{
			/* One way or another, we want to break the loop. */
		UNGETEXTRACHAR_2( stkp, a, th,  numberoctal_lex, res, RETURN_ENDRUN );
	}
	
		/* Grow the string back onto the stack. */
	th.length += 1;
	PUSHCHAR( &( stkp->data ), a.c,  numberoctal_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  numberoctal_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberbinary_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberbinary_lex;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  numberbinary_lex, res );
	if( th.toktype != TOKTYPE_NUMBER )
	{
		TRESPASSPATH( numberbinary_lex, "ERROR: numberbinary_lex encountered a non-_NUMBER token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberbinary_lex_FETCHFAIL( err ) \
	MONADICFAILURE( numberbinary_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberbinary_lex_GETFAIL )
	}
	
	if( !( a.c == '0' || a.c == '1' ) )
	{
			/* One way or another, we want to break the loop. */
		UNGETEXTRACHAR_2( stkp, a, th,  numberbinary_lex, res, RETURN_ENDRUN );
	}
	
		/* Grow the string back onto the stack. */
	th.length += 1;
	PUSHCHAR( &( stkp->data ), a.c,  numberbinary_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  numberbinary_lex, res );
	
	return( (retframe){ hand, (void*)0 } );
}


retframe brackop_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &end_run;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  brackop_lex, res );
	if( !(
		th.toktype == TOKTYPE_OPCRL ||
		th.toktype == TOKTYPE_OPSQR ||
		th.toktype == TOKTYPE_OPPAR
	) )
	{
		TRESPASSPATH( brackop_lex, "ERROR: brackop_lex encountered a non- opening-bracket token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define brackop_lex_FETCHFAIL( err ) \
	MONADICFAILURE( brackop_lex, "get_extrachar", (err) ); \
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
			UNGETEXTRACHAR_2( stkp, a, th,  brackop_lex, res, RETURN_ENDRUN );
	}
	
	/* Wouldn't be bad to double-check the value of th.toktype here, but not */
	/*  important. */
	
	
		/* Grow the string back onto the stack. */
	th.length += 1;
	PUSHCHAR( &( stkp->data ), a.c,  brackop_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  brackop_lex, res );
	
	RETFRAMEFUNC( brackop_lex );
}


retframe str_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &str_lex;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  str_lex, res );
	if( !(
		th.toktype == TOKTYPE_SQSTR ||
		th.toktype == TOKTYPE_DQSTR
	) )
	{
		TRESPASSPATH( str_lex, "ERROR: str_lex encountered a non-quote token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define str_lex_FETCHFAIL( err ) \
	MONADICFAILURE( str_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, str_lex_GETFAIL )
	}
	
	if( th.toktype == TOKTYPE_SQSTR && a.c == '\"' )
	{
		TRESPASSPATH( str_lex, "ERROR: str_lex encountered character / token type mismatch." );
		
	} else if( th.toktype == TOKTYPE_DQSTR && a.c == '\'' )
	{
		TRESPASSPATH( str_lex, "ERROR: str_lex encountered character / token type mismatch." );
		
	} else if( a.c == '\\' )
	{
		hand = &escstr_lex;
	}
	
		/* Grow the string back onto the stack. */
	th.length += 1;
	PUSHCHAR( &( stkp->data ), a.c,  str_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  str_lex, res );
	
	if
	(
		( th.toktype == TOKTYPE_SQSTR && a.c == '\'' ) ||
		( th.toktype == TOKTYPE_DQSTR && a.c == '\"' )
	)
	{
		RETFRAMEFUNC( str_lex );
	}
	return( (retframe){ hand, (void*)0 } );
}
retframe escstr_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &str_lex;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  escstr_lex, res );
	if( !(
		th.toktype == TOKTYPE_SQSTR ||
		th.toktype == TOKTYPE_DQSTR
	) )
	{
		TRESPASSPATH( escstr_lex, "ERROR: escstr_lex encountered a non-quote token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define escstr_lex_FETCHFAIL( err ) \
	MONADICFAILURE( escstr_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, escstr_lex_GETFAIL )
	}
	
		/* Grow the string back onto the stack. */
	th.length += 1;
	PUSHCHAR( &( stkp->data ), a.c,  escstr_lex, res );
	PUSHTOKENHEAD( &( stkp->data ), th,  escstr_lex, res );
	
	if
	(
		( th.toktype == TOKTYPE_SQSTR && a.c == '\'' ) ||
		( th.toktype == TOKTYPE_DQSTR && a.c == '\"' )
	)
	{
		RETFRAMEFUNC( escstr_lex );
	}
	return( (retframe){ hand, (void*)0 } );
}


retframe syms_lex( stackpair *stkp, v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  syms_lex, res );
	if( th.toktype != TOKTYPE_SYM )
	{
		TRESPASSPATH( syms_lex, "ERROR: syms_lex encountered a non-_SYM token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define name_lex_GETFAIL( err ) \
	MONADICFAILURE( syms_lex, "get_extrachar", (err) ); \
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
			
				/* Grow the string back onto the stack. This stretches to */
				/*  the first statement following this switch. */
			th.length += 1;
			PUSHCHAR( &( stkp->data ), b.c,  syms_lex, res );
			break;
	}
	PUSHTOKENHEAD( &( stkp->data ), th,  syms_lex, res );
	
	RETFRAMEFUNC( syms_lex );
}
retframe symssinglet_lex( stackpair *stkp, void *arg, token_head *th, char a, extrachar b )
{
	if( !th )
	{
		BADNULL( symssinglet_lex, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	
	int res;
	UNGETEXTRACHAR_1( a,  symssinglet_lex, res );
	
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
	
		/* Restore the token header to the stack. We now have a full "leaf" */
		/*  token stored on the stack. */
	PUSHTOKENHEAD( &( stkp->data ), th,  symssinglet_lex, res );
	
	RETFRAMEFUNC( symssinglet_lex );
}
retframe symsext_lex( stackpair *stkp, void *arg, token_head *th, extrachar a, extrachar b )
{
	extrachar c;
	int res;
	
	if( !th )
	{
		BADNULL( symsext_lex, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define symsext_lex_FETCHFAIL( err ) \
	MONADICFAILURE( symsext_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETc, symsext_lex_FETCHFAIL )
	}
	
#define symsext_lex_STDERR( calleestr, err ) \
	if( !res ) { \
		FAILEDINTFUNC( ( calleestr ), symsext_lex, res ); \
		NOTELINE(); SIGNEDARG( (err) ); \
		return( (retframe){ &end_run, (void*)0 } ); } }
#define symsext_lex_DITOK( erra, errb ) \
	if( 1 ) { \
		res = unget_extrachar( c ); \
			symsext_lex_STDERR( "unget_extrachar", erra ); \
		res = push_char( &( stkp->data ),  b.c ); \
			symsext_lex_STDERR( "push_char", errb ); }
#define symsext_lex_TRITOK( erra, errb ) \
	if( 1 ) { \
		res = push_char( &( stkp->data ),  b.c ); \
			symsext_lex_STDERR( "push_char", erra ); \
		res = push_char( &( stkp->data ),  c.c ); \
			symsext_lex_STDERR( "push_char", errb ); \
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
					
					symsext_lex_DITOK( -17, -18 );
					th.toktype = TOKTYPE_SYM_PLACEDDIVISIVSHIFT;
				}
				
			} else {
				
				TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered dual contiguous > characters." );
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
					
					symsext_lex_DITOK( -22, -23 );
					th.toktype = TOKTYPE_SYM_PLACEDMULTIPLYSHIFT;
				}
				
			} else {
				
				TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered dual contiguous < characters." );
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
					
					UNGETEXTRACHAR_1( c,  symsext_lex, res );
					UNGETEXTRACHAR_1( b,  symsext_lex, res );
					
					th.toktype = TOKTYPE_SYM_QUERY;
				}
				
			} else {
				
				TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered an isolated ? character." );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			th.length += 1;
			break;
			
		default:
			TRESPASSPATH( symsext_lex, "ERROR: symsext_lex encountered an unforeseen character: 0x" );
			HEXARG( a.c );
			return( (retframe){ &end_run, (void*)0 } );
	}
	
	/* Success. */
	
		/* Store the token header back, and then the token is finished. */
	PUSHTOKENHEAD( &( stkp->data ), th,  symsext_lex, res );
	
	RETFRAMEFUNC( symsext_lex );
}


retframe at_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	int res;
	POPTOKENHEAD( &( stkp->data ), &th,  at_lex, res );
	if( th.toktype != TOKTYPE_AT )
	{
		TRESPASSPATH( at_lex, "ERROR: at_lex encountered a non-_AT token type." );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define at_FETCHFAIL( err ) \
	MONADICFAILURE( at_lex, "get_extrachar", (err) ); \
	return( (retframe){ &end_run, (void*)0 } );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, at_FETCHFAIL )
	}
	
	if( a.c == '<' )
	{
		PUSHCHAR( &( stkp->data ), a.c,  at_lex, res );
		th.toktype = TOKTYPE_SYM_ADDROF;
		th.length += 1;
		
	} else {
		
		UNGETEXTRACHAR_1( a,  at_lex, res );
		th.toktype = TOKTYPE_SYM_DEREFERENCE;
	}
	
	/* Success. */
	
	PUSHTOKENHEAD( &( stkp->data ), th,  at_lex, res );
	
	RETFRAMEFUNC( at_lex );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
