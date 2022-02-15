#ifndef METAC_INPUT_SOURCE_H
 #define METAC_INPUT_SOURCE_H
	
	/* input/source.h */
	
	#include "headers.h"
	
	typedef lib4_charresult char_result;
		/* Note: the on* args must be the names of either functions or */
		/*  function macros, that take one arg of the relevant type. Use of */
		/*  the set* macros is advised, for simplicity. */
	#define CHAR_RESULT_BODYMATCH( var, onsucc, onfail ) \
		LIB4_CHARRESULT_BODYMATCH( var, onsucc, onfail )
	#define CHAR_RESULT_EXPRMATCH( var, onsucc, onfail ) \
		LIB4_CHARRESULT_EXPRMATCH( var, onsucc, onfail )
	
	int init();
	void deinit();
	int reinit();
	
	source* build_source2( char_pascalarray *name, uintmax_t inclusionpoint, const char *opentype );
	source* build_source( char_pascalarray *name, uintmax_t inclusionpoint );
	int discard_source( source *src );
	
	char_result charin();
	int charback( char val );

#endif
