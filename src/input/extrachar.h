#ifndef METAC_INPUT_EXTRACHAR_H
 #define METAC_INPUT_EXTRACHAR_H
	
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
	
	#include "../../external_dependencies/libandria4/basic/stdmonads.h"
	#include "../headers.h"
	
	#include "source.h"
	
	
	
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
