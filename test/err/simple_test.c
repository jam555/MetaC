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

/* test/err/simple_test.c */

#include <stdlib.h>
#include <stdio.h>

#include "err.h"
#include "inner_err.h"



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		static msg_styleset this_set;
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs;
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif



static msg_style this_style = (msg_style){ "\tHello, this is a printf-link style message." };
static msg_piece this_piece = (msg_piece){ { .style = &this_style }, 0 };

int main( int argn, char *argc[] )
{
	
	printf( "Simple msg_interface test.\n" );
	
	msg_interface( &this_set, 0 );
	
		/* Note: on error, exit with EXIT_FAILURE instead. */
	return( EXIT_SUCCESS );
}


#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset this_set = { &this_piece, 1 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset this_set = (msg_styleset){ &this_piece, 1 };
#endif
