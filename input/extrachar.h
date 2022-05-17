#ifndef METAC_INPUT_EXTRACHAR_H
 #define METAC_INPUT_EXTRACHAR_H
	
	#include "stdmonads.h"
	#include "headers.h"
	
	
	
	typedef struct extrachar
	{
		char c;
		/* "int was_freshline" was previously here, replace it's usage with */
		/*  "(column==0)". */
		int is_delimited;
		
		refed_pstr *file;
		uintmax_t line, column;
	};
	
	LIB4_MONAD_EITHER_BUILDTYPE_DEFINITION(
		extrachar_result,
		
		extrachar,
		lib4_failure_result
	);
	
	
	
	int popas_extrachar( stackpair *stkp, void *v,  extrachar *ec );
	
	/* These function as "enriched" equivalents of the normal C IO */
	/*  functions. They specifically handle characters that are output by an */
	/*  EARLY lexing stage, which currently handles delimiting, but in the */
	/*  future will probably be expanded to handle trigraphs as well. */
	extrachar_result get_extrachar( stackpair *stkp, void *v );
	extrachar_result peek_extrachar( stackpair *stkp, void *v );
	int unget_extrachar( extrachar ec );
	
	
	#define EXTRACHAR_BUILDSUCCESS( val ) \
		LIB4_MONAD_EITHER_BUILDLEFT( extrachar_result, extrachar, (val) )
	#define EXTRACHAR_BUILDFAILURE( val ) \
		LIB4_MONAD_EITHER_BUILDRIGHT( extrachar_result, lib4_failure_result, (val) )
	
		/* The *BODY* version takes statements, *EXPR* takes expressions. */
		/*  The matches must be function-style, though function macros are */
		/*  allowed. */
	#define EXTRACHAR_BODYMATCH( var, succ, fail ) \
		LIB4_MONAD_EITHER_BODYMATCH( var, succ, fail )
	#define EXTRACHAR_EXPRMATCH( var, succ, fail ) \
		LIB4_MONAD_EITHER_EXPRMATCH( var, succ, fail )
	
		/* Convenience wrappers. See monads.h for more details. */
	#define EXTRACHAR_RETURNSUCCESS( val ) \
		LIB4_MONAD_EITHER_RETURNLEFT( extrachar_result, extrachar, val )
	#define EXTRACHAR_RETURNFAILURE( val ) \
		LIB4_MONAD_EITHER_RETURNRIGHT( extrachar_result, lib4_failure_result, val )
	
#endif
