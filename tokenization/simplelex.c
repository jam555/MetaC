#include "headers.h"



retframe space_lex( stackpair*, void* );
retframe newline_lex( stackpair*, void* );

retframe name_lex( stackpair*, void* );
retframe numberentry_lex( stackpair*, void* );

#define head_lex_ERREXIT( key, ... ) \
	err_interface( &head_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
	return( (retframe){ &end_run, (void*)0 } )
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
#define head_lex_GETFAIL( val ) \
	head_lex_ERREXIT( -2,  stkp, v,  &th, &a, &ecr,  (val) );
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
		if( !res )
		{
			head_lex_ERREXIT( -4, &th, &a.c );
		}
		res2 = push_tokenhead( &( stkp->data ),  th );
		if( !res )
		{
			head_lex_ERREXIT( -5, &th );
		}
		
		RET_FRAMEFUNC( head_lex_refid, -6, 0 );
		
	} else if( a.c == '#' )
	{
		/* Preprocessor. */
		
		th.toktype = TOKTYPE_OCTO;
		
		res2 = push_char( &( stkp->data ),  a.c );
		if( !res )
		{
			head_lex_ERREXIT( -7, &th, &a.c );
		}
		res2 = push_tokenhead( &( stkp->data ),  th );
		if( !res )
		{
			head_lex_ERREXIT( -8, &th );
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
		if( !res )
		{
			head_lex_ERREXIT( -9, &th, &a.c );
		}
		res2 = push_tokenhead( &( stkp->data ),  th );
		if( !res )
		{
			head_lex_ERREXIT( -10, &th );
		}
		
		RET_FRAMEFUNC( head_lex_refid, -10, 0 );
	}
	
	res2 = push_char( &( stkp->data ),  a.c );
	if( !res )
	{
		RET_FRAMEFUNC( head_lex_refid, -11, &th, &a.c );
	}
	
	res2 = push_tokenhead( &( stkp->data ),  th );
	if( !res )
	{
		RET_FRAMEFUNC( head_lex_refid, -12, &th );
	}
	
	return( ret );
}

#define space_lex_ERREXIT( key, ... ) \
	err_interface( &space_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
	return( (retframe){ &end_run, (void*)0 } )
retframe space_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &space_lex;
	extrachar a;
	
	/* Validation stuff. */
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		space_lex_ERREXIT( -2, 0 );
	}
	if( th.toktype != TOKTYPE_SPACE )
	{
		space_lex_ERREXIT( -3, 0 );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define space_lex_GETFAIL( val ) \
	space_lex_ERREXIT( -2,  stkp, v,  &th, &a, &ecr,  (val) );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, space_lex_GETFAIL )
	}
	
	if( isblank( a.c ) || ( isspace( a.c ) && a.is_delimited ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else if( unget_extrachar( a ) )
	{
		/* Success. */
		
		if( !push_tokenhead( &( stkp->data ),  th ) )
		{
			space_lex_ERREXIT( -6, &th );
		}
		
		RET_FRAMEFUNC( space_lex_refid, -7, 0 );
		
	} else {
		
		/* Fail. */
		
		err_interface( &space_lex_refid, (lib4_failure_result){ -8 }, &th, &a.c );
		hand = &end_run;
	}
	
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		space_lex_ERREXIT( -9, &th, &a.c );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		space_lex_ERREXIT( -10, &th, &a.c );
	}
	
	return( (retframe){ hand, (void*)0 } );
}

#define newline_lex_ERREXIT( key, ... ) \
	err_interface( &newline_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
	return( (retframe){ &end_run, (void*)0 } )
retframe newline_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &newline_lex;
	extrachar a;
	
	/* Validation stuff. */
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		newline_lex_ERREXIT( -2, 0 );
	}
	if( th.toktype != TOKTYPE_NEWLINE )
	{
		newline_lex_ERREXIT( -3, 0 );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define newline_lex_GETFAIL( val ) \
	newline_lex_ERREXIT( -2,  stkp, v,  &th, &a, &ecr,  (val) );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, newline_lex_GETFAIL )
	}
	
	if( isspace( a.c ) && ( !isblank( a.c ) == !a.is_delimited ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else if( unget_extrachar( a ) )
	{
		/* Success. */
		
		if( !push_tokenhead( &( stkp->data ),  th ) )
		{
			newline_lex_ERREXIT( -6, &th );
		}
		
		RET_FRAMEFUNC( newline_lex_refid, -7, 0 );
		
	} else {
		
		/* Fail. */
		
		err_interface( &newline_lex_refid, (lib4_failure_result){ -8 }, &th, &a.c );
		hand = &end_run;
	}
	
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		newline_lex_ERREXIT( -9, &th, &a.c );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		newline_lex_ERREXIT( -10, &th, &a.c );
	}
	
	return( (retframe){ hand, (void*)0 } );
}

#define name_lex_ERREXIT( key, ... ) \
	err_interface( &name_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
	return( (retframe){ &end_run, (void*)0 } )
retframe name_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &nameline_lex;
	extrachar a;
	
	/* Validation stuff. */
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		name_lex_ERREXIT( -2, 0 );
	}
	if( th.toktype != TOKTYPE_NAME )
	{
		name_lex_ERREXIT( -3, 0 );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define name_lex_GETFAIL( val ) \
	name_lex_ERREXIT( -2,  stkp, v,  &th, &a, &ecr,  (val) );
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, name_lex_GETFAIL )
	}
	
	if( a.c == '_' && isalpha( a.c ) ) && isdigit( a.c ) )
	{
		/* Accumulate. */
		
		th.length += 1;
		
	} else if( unget_extrachar( a ) )
	{
		/* Success. */
		
		if( !push_tokenhead( &( stkp->data ),  th ) )
		{
			name_lex_ERREXIT( -6, &th );
		}
		
		RET_FRAMEFUNC( name_lex_refid, -7, 0 );
		
	} else {
		
		/* Fail. */
		
		err_interface( &name_lex_refid, (lib4_failure_result){ -8 }, &th, &a.c );
		hand = &end_run;
	}
	
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		name_lex_ERREXIT( -9, &th, &a.c );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		name_lex_ERREXIT( -10, &th, &a.c );
	}
	
	return( (retframe){ hand, (void*)0 } );
}


#define number_lex_ERREXIT( key, ... ) \
		err_interface( &number_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
retframe numberdecimal_lex( stackpair*, void* );
retframe numberhexadecimal_lex( stackpair*, void* );
retframe numberoctal_lex( stackpair*, void* );
retframe numberbinary_lex( stackpair*, void* );
retframe numberentry_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		/* TODO: Find everything like this body in the file, and remove the */
		/*  extraneous return: I was a bit sleep deprived, and didn't pay */
		/*  attention to the details of the macro(s). */
		
		number_lex_ERREXIT( -2, 0 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		number_lex_ERREXIT( -3, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = peek_extrachar( stkp, v );
#define numberentry_lex1_GETFAIL( val ) \
	name_lex_ERREXIT( -4,  stkp, v,  &th, &a, &ecr,  (val) );
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
#define numberentry_lex2_FETCHFAIL( val ) \
	if( 1 ) { number_lex_ERREXIT( -5, &( val ), &th, &a ); }
			{
					/* The extrachar that we peek'd isn't needed anymore, so */
					/*  we're just getting it out of the way. */
				extrachar_result ecr = get_extrachar( stkp, v );
				
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
					if( unget_extrachar( a ) )
					{
						/* Success. */
						
						if( !push_tokenhead( &( stkp->data ),  th ) )
						{
							number_lex_ERREXIT( -6, &th );
							return( (retframe){ &end_run, (void*)0 } );
						}
						
						RET_FRAMEFUNC( number_lex_refid, -7, 0 );
						
					} else {
						
						/* Fail. */
						
						number_lex_ERREXIT( -8, &th, &a );
					}
					
					break;
			}
			
			th.length += 1;
			if( !push_char( &( stkp->data ),  a.c ) )
			{
				number_lex_ERREXIT( -9, &hand, &th, &a );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			break;
	}
	
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		number_lex_ERREXIT( -10, &hand, &th );
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
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		number_lex_ERREXIT( -11, 0 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		number_lex_ERREXIT( -12, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberdecimal_lex_FETCHFAIL( val ) \
	if( 1 ) { number_lex_ERREXIT( -13, &( val ), &th, &a ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberdecimal_lex_FETCHFAIL )
	}
	
	if( !isdigit( a.c ) )
	{
			/* One way or another, we want to break the loop. */
		if( unget_extrachar( a ) )
		{
			/* Success. */
			
			if( !push_tokenhead( &( stkp->data ),  th ) )
			{
				number_lex_ERREXIT( -14, &th );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -15, 0 );
			
		} else {
			
			/* Fail. */
			
			number_lex_ERREXIT( -16, &th, &a );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		number_lex_ERREXIT( -17, &th, &a );
		return( (retframe){ &end_run, (void*)0 } );
	}
	th.length += 1;
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		number_lex_ERREXIT( -18, &th );
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
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		number_lex_ERREXIT( -19, 0 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		number_lex_ERREXIT( -20, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberhexadecimal_lex_FETCHFAIL( val ) \
	if( 1 ) { number_lex_ERREXIT( -21, &( val ), &th, &a ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberhexadecimal_lex_FETCHFAIL )
	}
	
	if( !isxdigit( a.c ) )
	{
			/* One way or another, we want to break the loop. */
		if( unget_extrachar( a ) )
		{
			/* Success. */
			
			if( !push_tokenhead( &( stkp->data ),  th ) )
			{
				number_lex_ERREXIT( -22, &th );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -23, 0 );
			
		} else {
			
			/* Fail. */
			
			number_lex_ERREXIT( -24, &th, &a );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		number_lex_ERREXIT( -25, &th, &a );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		number_lex_ERREXIT( -26, &th );
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
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		number_lex_ERREXIT( -27, 0 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		number_lex_ERREXIT( -28, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberoctal_lex_FETCHFAIL( val ) \
	if( 1 ) { number_lex_ERREXIT( -29, &( val ), &th, &a ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberoctal_lex_GETFAIL )
	}
	
	if( !(
		a.c == '0' || a.c == '1' || a.c == '2' || a.c == '3' ||
		a.c == '4' || a.c == '5' || a.c == '6' || a.c == '7'
	) )
	{
			/* One way or another, we want to break the loop. */
		if( unget_extrachar( a ) )
		{
			/* Success. */
			
			if( !push_tokenhead( &( stkp->data ),  th ) )
			{
				number_lex_ERREXIT( -30, &th );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -31, 0 );
			
		} else {
			
			/* Fail. */
			
			number_lex_ERREXIT( -32, &th, &a );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		number_lex_ERREXIT( -33, &th, &a );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		number_lex_ERREXIT( -34, &th );
	}
	
	return( (retframe){ hand, (void*)0 } );
}
retframe numberbinary_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We loop by default. */
	framefunc hand = &numberbinary_lex;
	extrachar a;
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		number_lex_ERREXIT( -35, 0 );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( th.toktype != TOKTYPE_NUMBER )
	{
		number_lex_ERREXIT( -36, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define numberbinary_lex_FETCHFAIL( val ) \
	if( 1 ) { number_lex_ERREXIT( -37, &( val ), &th, &a ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, numberbinary_lex_GETFAIL )
	}
	
	if( !( a.c == '0' || a.c == '1' ) )
	{
			/* One way or another, we want to break the loop. */
		if( unget_extrachar( a ) )
		{
			/* Success. */
			
			if( !push_tokenhead( &( stkp->data ),  th ) )
			{
				number_lex_ERREXIT( -38, &th );
				return( (retframe){ &end_run, (void*)0 } );
			}
			
			RET_FRAMEFUNC( number_lex_refid, -39, 0 );
			
		} else {
			
			/* Fail. */
			
			number_lex_ERREXIT( -40, &th, &a );
			return( (retframe){ &end_run, (void*)0 } );
		}
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		number_lex_ERREXIT( -41, &th, &a );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		number_lex_ERREXIT( -42, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	return( (retframe){ hand, (void*)0 } );
}


#define brackop_lex_ERREXIT( key, ... ) \
		err_interface( &brackop_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
retframe brackop_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &end_run;
	extrachar a;
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		brackop_lex_ERREXIT( -2, 0 );
	}
	if( !(
		th.toktype == TOKTYPE_OPCRL ||
		th.toktype == TOKTYPE_OPSQR ||
		th.toktype == TOKTYPE_OPPAR
	) )
	{
		brackop_lex_ERREXIT( -3, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define brackop_lex_FETCHFAIL( val ) \
	if( 1 ) { brackop_lex_ERREXIT( -4, &( val ), &th, &a ); }
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
			if( unget_extrachar( a ) )
			{
				/* Success. */
				
				if( !push_tokenhead( &( stkp->data ),  th ) )
				{
					number_lex_ERREXIT( -5, &th );
					return( (retframe){ &end_run, (void*)0 } );
				}
				
				RET_FRAMEFUNC( number_lex_refid, -6, 0 );
				
			} else {
				
				/* Fail. */
				
				number_lex_ERREXIT( -7, &th, &a );
				return( (retframe){ &end_run, (void*)0 } );
			}
	}
	
	/* Wouldn't be bad to double-check the value of th.toktype here, but not */
	/*  important. */
	
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		number_lex_ERREXIT( -8, &th, &a );
		return( (retframe){ &end_run, (void*)0 } );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		number_lex_ERREXIT( -9, &th );
		return( (retframe){ &end_run, (void*)0 } );
	}
	
	RET_FRAMEFUNC( number_lex_refid, -10, 0 );
}


#define str_lex_ERREXIT( key, ... ) \
		err_interface( &str_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
retframe escstr_lex( stackpair*, void* );
retframe str_lex( stackpair *stkp, void *v )
{
	token_head th;
		/* We exit by default. */
	framefunc hand = &str_lex;
	extrachar a;
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		str_lex_ERREXIT( -2, 0 );
	}
	if( !(
		th.toktype == TOKTYPE_SQSTR ||
		th.toktype == TOKTYPE_DQSTR
	) )
	{
		str_lex_ERREXIT( -3, &th );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define str_lex_FETCHFAIL( val ) \
	if( 1 ) { str_lex_ERREXIT( -4, &( val ), &th, &a ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, str_lex_GETFAIL )
	}
	
	if( th.toktype == TOKTYPE_SQSTR && a.c == '\"' )
	{
		err_interface( &str_lex_refid, (lib4_failure_result){ -5 }, &th );
		
	} else if( th.toktype == TOKTYPE_DQSTR && a.c == '\'' )
	{
		err_interface( &str_lex_refid, (lib4_failure_result){ -6 }, &th );
		
	} else if( a.c == '\\' )
	{
		hand = &escstr_lex;
	}
	
	th.length += 1;
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		str_lex_ERREXIT( -7, &th, &a );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		str_lex_ERREXIT( -8, &th );
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
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		str_lex_ERREXIT( -10, 0 );
	}
	if( !(
		th.toktype == TOKTYPE_SQSTR ||
		th.toktype == TOKTYPE_DQSTR
	) )
	{
		str_lex_ERREXIT( -11, &th );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define escstr_lex_FETCHFAIL( val ) \
	if( 1 ) { str_lex_ERREXIT( -12, &( val ), &th, &a ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, escstr_lex_GETFAIL )
	}
	
		/* This will probably trigger a compile error: look up proper var. */
	th.length += 1;
	if( !push_char( &( stkp->data ),  a.c ) )
	{
		str_lex_ERREXIT( -13, &th, &a );
	}
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		str_lex_ERREXIT( -14, &th );
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


#define syms_lex_ERREXIT( key, ... ) \
		err_interface( &syms_lex_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
retframe symssinglet_lex( stackpair*, void*, token_head*, char, char );
retframe symsext_lex( stackpair*, void*, token_head*, char, char );
retframe syms_lex( stackpair *stkp, v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	if( !pop_tokenhead( &( stkp->data ),  &th ) )
	{
		syms_lex_ERREXIT( -2, 0 );
	}
	if( th.toktype != TOKTYPE_SYM )
	{
		syms_lex_ERREXIT( -3, &th );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define name_lex_GETFAIL( val ) \
	syms_lex_ERREXIT( -4, &th, (val) );
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
#define syms_lex_FETCHFAIL( val ) \
	if( 1 ) { syms_lex_ERREXIT( -5, &( val ), &th, &a, &b ); }
				EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETb, syms_lex_FETCHFAIL )
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
			if( !push_char( &( stkp->data ),  b.c ) )
			{
				syms_lex_ERREXIT( -6, &th, &a, &b );
			}
			
			break;
	}
	
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		number_lex_ERREXIT( -7, &th );
	}
	
	RET_FRAMEFUNC( number_lex_refid, -8, 0 );
}
retframe symssinglet_lex( stackpair *stkp, void *arg, token_head *th, char a, extrachar b )
{
	if( !th )
	{
		syms_lex_ERREXIT( -9, &a, &b );
	}
	
	
	if( unget_extrachar( a ) )
	{
		syms_lex_ERREXIT( -10, th, &a, &b );
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
	
	
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		syms_lex_ERREXIT( -11, &th );
	}
	
	RET_FRAMEFUNC( syms_lex_refid, -12, 0 );
}
retframe symsext_lex( stackpair *stkp, void *arg, token_head *th, extrachar a, extrachar b )
{
	extrachar c;
	
	if( !th )
	{
		syms_lex_ERREXIT( -13, &a, &b );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define symsext_lex_FETCHFAIL( val ) \
	if( 1 ) { syms_lex_ERREXIT( -14, &( val ), &th, &a, &b ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETc, symsext_lex_FETCHFAIL )
	}
	
#define symsetx_lex_DITOK( erra, errb ) \
		if( !unget_extrachar( c ) ) { syms_lex_ERREXIT( ( erra ), th, &a, &b, &c ); } \
		if( !push_char( &( stkp->data ),  b.c ) ) { syms_lex_ERREXIT( ( errb ), th, &a, &b ); }
#define symsext_lex_TRITOK( erra, errb ) \
		if( !push_char( &( stkp->data ),  b.c ) ) { syms_lex_ERREXIT( ( erra ), th, &a, &b, &c ); } \
		if( !push_char( &( stkp->data ),  c.c ) ) { syms_lex_ERREXIT( ( errb ), th, &a, &c ); } \
		th.length += 1;
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
				
				syms_lex_ERREXIT( -19, th, &a, &b, &c );
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
				
				syms_lex_ERREXIT( -24, th, &a, &b, &c );
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
					
					if( !unget_extrachar( c ) )
					{
						syms_lex_ERREXIT( -27, th, &a, &b, &c );
					}
					if( !unget_extrachar( b ) )
					{
						syms_lex_ERREXIT( -28, th, &a, &b );
					}
					
					th.toktype = TOKTYPE_SYM_QUERY;
				}
				
			} else {
				
				syms_lex_ERREXIT( -29, th, &a, &b, &c );
			}
			
			th.length += 1;
			break;
			
		default:
			syms_lex_ERREXIT( -30, th, &a, &b, &c );
	}
	
	/* Success. */
	
	if( !push_tokenhead( &( stkp->data ),  th ) )
	{
		syms_lex_ERREXIT( -31, &th );
	}
	
	RET_FRAMEFUNC( syms_lex_refid, -32, 0 );
}


#define at_ERREXIT( key, ... ) \
		err_interface( &at_refid, (lib4_failure_result){ (key) }, __VA_ARGS__ ); \
		return( (retframe){ &end_run, (void*)0 } )
retframe at_lex( stackpair *stkp, void *v )
{
	token_head th;
	framefunc hand = &end_run;
	extrachar a;
	
	if( !pop_tokenhead( &( stkp->data ), &th ) )
	{
		at_ERREXIT( -2, 0 );
	}
	if( th.toktype != TOKTYPE_AT )
	{
		at_ERREXIT( -3, 0 );
	}
	
	{
		extrachar_result ecr = get_extrachar( stkp, v );
#define at_FETCHFAIL( val ) \
		if( 1 ) { at_ERREXIT( -4, &( val ), &th ); }
		EXTRACHAR_BODYMATCH( ecr, LIB4_OP_SETa, at_FETCHFAIL )
	}
	
	if( a.c == '<' )
	{
		if( !push_char( &( stkp->data ), a.c ) )
		{
			at_ERREXIT( -5, th, &a );
		}
		th.toktype = TOKTYPE_SYM_ADDROF;
		th.length += 1;
		
	} else {
		
		if( !unget_extrachar( a ) )
		{
			at_ERREXIT( -6, th, &a );
		}
		th.toktype = TOKTYPE_SYM_DEREFERENCE;
	}
	
	/* Success. */
	
	if( !push_tokenhead( &( stkp->data ), th ) )
	{
		at_ERREXIT( -7, &th );
	}
	
	RET_FRAMEFUNC( at_refid, -8, 0 );
}
