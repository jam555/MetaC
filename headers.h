#ifndef METAC_TOKENIZATION_SIMPLELEX_H
 #define METAC_TOKENIZATION_SIMPLELEX_H
	
	#include <ctype.h>
	#include <stdint.h>
	#include <stdlib.h>
	#include "lib4/pascalarray.h"
	
	
		/* Defines char_pascalarray, char_pascalarray_result, and various */
		/*  functions. */
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( char_, char );
	#define BUILD_CHARPARR( string ) \
		(char_pascalarray){ sizeof( (char[]){ string } ) - 1, (char[]){ string } }
	
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( charparrptr_, char_pascalarray* );
	typedef charparrptr_pascalarray charparrptr_parr;
	
	
	
	extern void err_interface( uintptr_t source, lib4_failure_result err, ... );
	
	void err_interface( uintptr_t source, lib4_failure_result err, ... );
	
	
	
	#include "input/source.h"
	#include "input/lexalike.h"
	
	#include "metaCpreproc/basics.h"
	#include "metaCpreproc/components.h"
	
	#include "output/echo_tokens.h"
	
	#include "tokenization/token.h"
	#include "tokenization/simplelex.h"
	#include "tokenization/complexlex.h"
	
	#include "stack.h"
	#include "tokenmatching.h"
	
#endif
