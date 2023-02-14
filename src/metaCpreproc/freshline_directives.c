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

#include "../headers.h"

/* This file exists for the preprocessor directives that demand to be */
/*  separated from the most recent newline by nothing other than whitespace. */

retframe ???( stackpair *stkp, void *v )
{
	???
	
	typedef struct macro_link macro_link;
	struct macro_link
	{
		token_head header;
		
			/* This identifies which element in the argument list should be */
			/*  used. */
		size_t args_offset;
	};
	
	typedef struct macro_call_argval macro_call_argval;
	struct macro_call_argval
	{
			/* One and ONLY one of these should be non-null. */
		size_t args_offset;
		char_pascalarray *text;
	};
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( macrocallargval_, macro_call_argval );
	typedef macrocallargval_pascalarray macrocllaval_parr;
	typedef struct macro_call macro_call;
	struct macro_call
	{
		token_head header;
		
		macrocllaval_parr *args;
		
		???;
	};
	
	typedef struct ???1 ???1;
	struct ???1
	{
		???;
	};
	
	typedef struct ???2 ???2;
	struct ???2
	{
		charparrptr_parr *argnames;
		
		???1_parr *tree;
	};
	
	???
	
	if( !stkp )
	{
		BADNULL( ??? );
	}
	
	???
}





retframe ???( stackpair *stkp, void *v )
{
}
