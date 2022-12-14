#ifndef METAC_INPUT_SOURCE_H
 #define METAC_INPUT_SOURCE_H
	
/*
MetaC Preprocessor - A macro processor for preprocessing usage.
Copyright (C) 2022 Jared A. Maddox

This program is free software; you can redistribute it and/or modify it
under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the:
	Free Software Foundation, Inc.,
	59 Temple Place, Suite 330,
	Boston, MA 02111-1307 USA
*/
	
	/* input/source.h */
	
	#include "headers.h"
	
	
	
	typedef struct source source;
	
	typedef struct refed_pstr
	{
		uintptr_t refs;
		char_pascalarray *text;
		
	} refed_pstr;
		/* For both, positive is success, negative is failure. Note that the */
		/*  argument must be non-null, and ->refs must be a POSITIVE number. */
	int refed_pstr_incrrefs( refed_pstr* );
	int refed_pstr_decrrefs( refed_pstr* );
	
	
	
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
	
	
	retframe metaC_stdinclude( stackpair *stkp, void *v );
	
	
		/* If refresh_srcname is provided, then the pointer it points to */
		/*  MUST be NULL, as internal logic makes that assumption itself. */
		/*  Only sets refresh_srcname if the source{} instance changes, in */
		/*  which case the provided refed_pstr will ALREADY have a reference */
		/*  added for the new recipient: remember to take this into */
		/*  consideration for proper memory management. If refresh_srcname */
		/*  is provided, then prog MUST be provided; prog specifies the read */
		/*  progress that has been made through the current file, in case of */
		/*  returning from an include... */
	char_result charin( refed_pstr **refresh_srcname, uintmax_t *prog );
		/* Both proper success and LIB4_RESULT_FAILURE_EOF are actually */
		/*  successes. Only error out on OTHER returns. Note that this */
		/*  internally calls charback(), but DOES NOT call charin(). */
	char_result charpeek( int *isEOF );
		/* Only "1" is a good return value. "If" you provide */
		/*  refresh_srcname, then you must also provide prog: these function */
		/*  as described for charin(). */
	int char_dropeof( refed_pstr **refresh_srcname, uintmax_t *prog );
	int charback( char val );

#endif
