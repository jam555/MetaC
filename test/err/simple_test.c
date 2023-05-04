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

#include "commons.incl"



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		static msg_styleset this_set;
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset this_set;
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif



static msg_style
	plain_style =
		(msg_style){ "\t\tHello, this is a plain caller message.\n" },
	char_style =
		(msg_style){ "\t\tHello, this caller message prints a character: %c ( %i ).\n" },
	int_style =
		(msg_style){ "\t\tHello, this caller message prints an integer: %i.\n" },
	float_style =
		(msg_style){ "\t\tHello, this caller message prints a float: %f.\n" },
	voidp_style =
		(msg_style){ "\t\tHello, this caller message prints a void pointer: %p.\n" },
	string_style =
		(msg_style){ "\t\tHello, this caller message prints a string: %s .\n" },
	percent_style =
		(msg_style){ "\t\tHello, this caller message prints a percent symbol: %% .\n" };
static msg_style bad_style = (msg_style){ "\tError, this should be impossible to print!" };
static msg_piece pieces[] =
	{
		{ { .style = &plain_style }, 0 },
		
		{ { .style = &char_style }, 0 },
		{ { .style = &int_style }, 0 },
		{ { .style = &float_style }, 0 },
		{ { .style = &voidp_style }, 0 },
		{ { .style = &string_style }, 0 },
		{ { .style = &percent_style }, 0 },
		
		{ { .style = &bad_style }, 0 }
	};

#define PLAIN_MESSAGE() \
	msg_interface( &this_set, 1 )

#define CHAR_MESSAGE( chara ) \
	msg_interface( &this_set, 2, (char)( chara ), (int)( chara ) )
#define INT_MESSAGE( num ) \
	msg_interface( &this_set, 3, (int)( num ) )
#define FLOAT_MESSAGE( num ) \
	msg_interface( &this_set, 4, (float)( num ) )
#define VOIDP_MESSAGE( voidp ) \
	msg_interface( &this_set, 5, (void*)( voidp ) )
#define STRING_MESSAGE( str ) \
	msg_interface( &this_set, 6, (char*)( str ) )
#define PERCENT_MESSAGE( ignore ) \
	msg_interface( &this_set, 7, (int)( ignore ) )



int main( int argn, char *argc[] )
{
	
	printf( "Simple msg_interface test.\n" );
	
	printf( "\n\tTrying a plain caller message.\n" );
		PLAIN_MESSAGE();
	
	printf( "\n\tTrying char, int, float, void pointer, and string, in that order.\n" );
		CHAR_MESSAGE( 'a' );
		INT_MESSAGE( 245 );
		FLOAT_MESSAGE( 13.78 );
		VOIDP_MESSAGE( &voidp_style );
		STRING_MESSAGE( "\'This is a test string.\'" );
	
	printf( "\n\tTrying percent, with a dummy arg.\n" );
		PERCENT_MESSAGE( 289 );
	
	printf( "\nTest concluded.\n" );
	
		/* Note: on error, exit with EXIT_FAILURE instead. */
	return( EXIT_SUCCESS );
}


#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset this_set = { pieces, sizeof( pieces ) / sizeof( msg_piece ) };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset this_set = (msg_styleset){ pieces, sizeof( pieces ) / sizeof( msg_piece ) };
#endif
