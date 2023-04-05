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

/* test/input/simple_test.c */

	/* Fore size_t, though I think it's in stdlib.h and stdio.h as well. */
#include <stddef.h>
	/* For EXIT_SUCCESS and EXIT_FAILURE. */
#include <stdlib.h>
	/* For printf(). */
#include <stdio.h>

#include "../../external_dependencies/libandria4/basic/stdmonads.h"
#include "../../external_dependencies/libandria4/stdabbrevs/metaCabbrevs.h"

#include "../../src/input/char2num.h"



#if defined( __cplusplus ) && __cplusplus >= 199711L
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif



int result = EXIT_SUCCESS;

void success_func( int val, int goal )
{
	int flag = 1;
	
	printf( "listed success, %i;", val );
	if( val < 0 )
	{
		printf( " val was reported as negative, testing failure;" );
		flag = 0;
	}
	if( goal < 0 )
	{
		printf( " goal was reported as negative ( %i ), testing failure;", goal );
		flag = 0;
	}
	if( val != goal )
	{
		printf( " value and goal weren't identical, testing failure;" );
		flag = 0;
	}
	
	if( flag )
	{
		printf( "\n\tTesting pitch succeeded.\n" );
		
	} else {
		
		printf( "\n\tTesting pitch failed.\n" );
		result = EXIT_FAILURE;
	}
}
void failure_func( uintptr_t err, int goal )
{
	int flag = 1;
	
	printf( "listed failure, %c", (int)err );
	if( goal >= 0 )
	{
		printf( " goal was reported as non-negative ( %i ), listed success expected;" );
		flag = 0;
	}
	if( ( goal >= 0 ? goal : -goal ) != err )
	{
		printf( " error didn't equal absolute( goal: %i ), testing failure;", goal );
		flag = 0;
	}
	
	if( flag )
	{
		printf( "\n    Testing pitch succeeded.\n" );
		
	} else {
		
		printf( "\n    Testing pitch failed.\n" );
		result = EXIT_FAILURE;
	}
}

typedef struct
{
	char *descriptor;
	int goal;
	char input;
	
} testpitch;
int test_set( size_t pitchcount, testpitch *pitches, LIB4_INTRESULT (*func)( char c ) )
{
	size_t iter = 0;
	int goal;
	LIB4_INTRESULT testres;
#define SUCCESS_PATH( val ) success_func( val, goal );
#define FAILURE_PATH( err ) failure_func( err.val, goal );
	
	if( pitchcount < 1 )
	{
		printf( "   Alert: test_set() was given a null pitchcount: %i.\n   ", (int)pitchcount );
	}
	if( !pitches || !func )
	{
		printf( "\n   Error: test_set( %p, %p ) was given an illegal null.\n", (void*)pitches, (void*)func );
		return( -1 );
	}
	
	while( iter < pitchcount )
	{
		if( !( pitches[ iter ].descriptor ) )
		{
			printf( "\n   Error: pitch[ %i ] had a null descriptor.\n", (int)iter );
			return( -2 );
		}
		
		printf( pitches[ iter ].descriptor );
		goal = pitches[ iter ].goal;
		testres = bin2num( pitches[ iter ].input );
		LIB4_INTRESULT_BODYMATCH( testres, SUCCESS_PATH, FAILURE_PATH );
		
		++iter;
	}
	
	return( 1 );
}

typedef struct
{
	size_t pitchcount;
	testpitch *pitches;
	LIB4_INTRESULT (*func)( char c );
	
	char *funcname;
	
} testbatch;
int test_batch( testbatch *batch )
{
	if( !batch )
	{
		printf( "\n   Error: test_batch( %p ) was given an illegal null.\n", (void*)batch );
		return( -1 );
	}
	if( !( batch->funcname ) )
	{
		printf
		(
			"\n   Error: test_batch() was given a null (testbatch*)->funcname.\n"
		);
		return( -2 );
	}
	
	printf( "\n Testing %s( * ):\n", batch->funcname );
	
	if( test_set( batch->pitchcount, batch->pitches, batch->func ) > 0 )
	{
		printf( "  %s() test succeeded.\n", batch->funcname );
		
	} else {
		
		printf( "  %s() test failed.\n", batch->funcname );
		result = EXIT_FAILURE;
	}			
	
	return( 1 );
}

int main( int argn, char *argc[] )
{
	testpitch
		binpitch[] =
		{
				/* Desired successes. */
			(testpitch){ "   0: ", 0, '0' },
			(testpitch){ "   1: ", 1, '1' },
			
				/* Decimal, hexadecimal, and sexagesimal. All desired */
				/*  failures. */
			(testpitch){ "   2: ", -( (int)( '2' ) ), '2' },
			(testpitch){ "   3: ", -( (int)( '3' ) ), '3' },
			(testpitch){ "   4: ", -( (int)( '4' ) ), '4' },
			(testpitch){ "   5: ", -( (int)( '5' ) ), '5' },
			(testpitch){ "   6: ", -( (int)( '6' ) ), '6' },
			(testpitch){ "   7: ", -( (int)( '7' ) ), '7' },
			(testpitch){ "   8: ", -( (int)( '8' ) ), '8' },
			(testpitch){ "   9: ", -( (int)( '9' ) ), '9' },
			(testpitch){ "   a: ", -( (int)( 'a' ) ), 'a' },
			(testpitch){ "   b: ", -( (int)( 'b' ) ), 'b' },
			(testpitch){ "   c: ", -( (int)( 'c' ) ), 'c' },
			(testpitch){ "   d: ", -( (int)( 'd' ) ), 'd' },
			(testpitch){ "   e: ", -( (int)( 'e' ) ), 'e' },
			(testpitch){ "   f: ", -( (int)( 'f' ) ), 'f' },
			(testpitch){ "   k: ", -( (int)( 'k' ) ), 'k' },
			(testpitch){ "   s: ", -( (int)( 's' ) ), 's' },
			(testpitch){ "   n: ", -( (int)( 'n' ) ), 'n' },
			(testpitch){ "   j: ", -( (int)( 'j' ) ), 'j' },
			(testpitch){ "   l: ", -( (int)( 'l' ) ), 'l' },
			(testpitch){ "   A: ", -( (int)( 'A' ) ), 'A' },
			(testpitch){ "   B: ", -( (int)( 'B' ) ), 'B' },
			(testpitch){ "   C: ", -( (int)( 'C' ) ), 'C' },
			(testpitch){ "   D: ", -( (int)( 'D' ) ), 'D' },
			(testpitch){ "   E: ", -( (int)( 'E' ) ), 'E' },
			(testpitch){ "   F: ", -( (int)( 'F' ) ), 'F' },
			(testpitch){ "   K: ", -( (int)( 'K' ) ), 'K' },
			(testpitch){ "   S: ", -( (int)( 'S' ) ), 'S' },
			(testpitch){ "   N: ", -( (int)( 'N' ) ), 'N' },
			(testpitch){ "   J: ", -( (int)( 'J' ) ), 'J' },
			(testpitch){ "   L: ", -( (int)( 'L' ) ), 'L' }
		},
		octpitch[] =
		{
				/* Desired successes. */
			(testpitch){ "   0: ", 0, '0' },
			(testpitch){ "   1: ", 1, '1' },
			(testpitch){ "   2: ", 2, '2' },
			(testpitch){ "   3: ", 3, '3' },
			(testpitch){ "   4: ", 4, '4' },
			(testpitch){ "   5: ", 5, '5' },
			(testpitch){ "   6: ", 6, '6' },
			(testpitch){ "   7: ", 7, '7' },
			
				/* Decimal, hexadecimal, and sexagesimal. All desired */
				/*  failures. */
			(testpitch){ "   8: ", -( (int)( '8' ) ), '8' },
			(testpitch){ "   9: ", -( (int)( '9' ) ), '9' },
			(testpitch){ "   a: ", -( (int)( 'a' ) ), 'a' },
			(testpitch){ "   b: ", -( (int)( 'b' ) ), 'b' },
			(testpitch){ "   c: ", -( (int)( 'c' ) ), 'c' },
			(testpitch){ "   d: ", -( (int)( 'd' ) ), 'd' },
			(testpitch){ "   e: ", -( (int)( 'e' ) ), 'e' },
			(testpitch){ "   f: ", -( (int)( 'f' ) ), 'f' },
			(testpitch){ "   k: ", -( (int)( 'k' ) ), 'k' },
			(testpitch){ "   s: ", -( (int)( 's' ) ), 's' },
			(testpitch){ "   n: ", -( (int)( 'n' ) ), 'n' },
			(testpitch){ "   j: ", -( (int)( 'j' ) ), 'j' },
			(testpitch){ "   l: ", -( (int)( 'l' ) ), 'l' },
			(testpitch){ "   A: ", -( (int)( 'A' ) ), 'A' },
			(testpitch){ "   B: ", -( (int)( 'B' ) ), 'B' },
			(testpitch){ "   C: ", -( (int)( 'C' ) ), 'C' },
			(testpitch){ "   D: ", -( (int)( 'D' ) ), 'D' },
			(testpitch){ "   E: ", -( (int)( 'E' ) ), 'E' },
			(testpitch){ "   F: ", -( (int)( 'F' ) ), 'F' },
			(testpitch){ "   K: ", -( (int)( 'K' ) ), 'K' },
			(testpitch){ "   S: ", -( (int)( 'S' ) ), 'S' },
			(testpitch){ "   N: ", -( (int)( 'N' ) ), 'N' },
			(testpitch){ "   J: ", -( (int)( 'J' ) ), 'J' },
			(testpitch){ "   L: ", -( (int)( 'L' ) ), 'L' }
		},
		decpitch[] =
		{
				/* Desired successes. */
			(testpitch){ "   0: ", 0, '0' },
			(testpitch){ "   1: ", 1, '1' },
			(testpitch){ "   2: ", 2, '2' },
			(testpitch){ "   3: ", 3, '3' },
			(testpitch){ "   4: ", 4, '4' },
			(testpitch){ "   5: ", 5, '5' },
			(testpitch){ "   6: ", 6, '6' },
			(testpitch){ "   7: ", 7, '7' },
			(testpitch){ "   8: ", 8, '8' },
			(testpitch){ "   9: ", 9, '9' },
			
				/* A mix of hexadecimal and sexagesimal. All desired */
				/*  failures. */
			(testpitch){ "   a: ", -( (int)( 'a' ) ), 'a' },
			(testpitch){ "   b: ", -( (int)( 'b' ) ), 'b' },
			(testpitch){ "   c: ", -( (int)( 'c' ) ), 'c' },
			(testpitch){ "   d: ", -( (int)( 'd' ) ), 'd' },
			(testpitch){ "   e: ", -( (int)( 'e' ) ), 'e' },
			(testpitch){ "   f: ", -( (int)( 'f' ) ), 'f' },
			(testpitch){ "   k: ", -( (int)( 'k' ) ), 'k' },
			(testpitch){ "   s: ", -( (int)( 's' ) ), 's' },
			(testpitch){ "   n: ", -( (int)( 'n' ) ), 'n' },
			(testpitch){ "   j: ", -( (int)( 'j' ) ), 'j' },
			(testpitch){ "   l: ", -( (int)( 'l' ) ), 'l' },
			(testpitch){ "   A: ", -( (int)( 'A' ) ), 'A' },
			(testpitch){ "   B: ", -( (int)( 'B' ) ), 'B' },
			(testpitch){ "   C: ", -( (int)( 'C' ) ), 'C' },
			(testpitch){ "   D: ", -( (int)( 'D' ) ), 'D' },
			(testpitch){ "   E: ", -( (int)( 'E' ) ), 'E' },
			(testpitch){ "   F: ", -( (int)( 'F' ) ), 'F' },
			(testpitch){ "   K: ", -( (int)( 'K' ) ), 'K' },
			(testpitch){ "   S: ", -( (int)( 'S' ) ), 'S' },
			(testpitch){ "   N: ", -( (int)( 'N' ) ), 'N' },
			(testpitch){ "   J: ", -( (int)( 'J' ) ), 'J' },
			(testpitch){ "   L: ", -( (int)( 'L' ) ), 'L' }
		},
		hexapitch[] =
		{
				/* Desired successes. */
			(testpitch){ "   0: ", 0, '0' },
			(testpitch){ "   1: ", 1, '1' },
			(testpitch){ "   2: ", 2, '2' },
			(testpitch){ "   3: ", 3, '3' },
			(testpitch){ "   4: ", 4, '4' },
			(testpitch){ "   5: ", 5, '5' },
			(testpitch){ "   6: ", 6, '6' },
			(testpitch){ "   7: ", 7, '7' },
			(testpitch){ "   8: ", 8, '8' },
			(testpitch){ "   9: ", 9, '9' },
			
			(testpitch){ "   a: ", 10, 'a' },
			(testpitch){ "   b: ", 11, 'b' },
			(testpitch){ "   c: ", 12, 'c' },
			(testpitch){ "   d: ", 13, 'd' },
			(testpitch){ "   e: ", 14, 'e' },
			(testpitch){ "   f: ", 15, 'f' },
			
			(testpitch){ "   A: ", 10, 'A' },
			(testpitch){ "   B: ", 11, 'B' },
			(testpitch){ "   C: ", 12, 'C' },
			(testpitch){ "   D: ", 13, 'D' },
			(testpitch){ "   E: ", 14, 'E' },
			(testpitch){ "   F: ", 15, 'F' },
			
				/* Desired failures. */
			(testpitch){ "   \'g\': ", -( (int)( 'g' ) ), 'g' },
			(testpitch){ "   \'G\': ", -( (int)( 'G' ) ), 'G' },
			
			/* Sexagesimal and hexadecimal are not the same thing, so these */
			/*  should all be failures. */
			(testpitch){ "   k: ", -( (int)( 'k' ) ), 'k' },
			(testpitch){ "   s: ", -( (int)( 's' ) ), 's' },
			(testpitch){ "   n: ", -( (int)( 'n' ) ), 'n' },
			(testpitch){ "   j: ", -( (int)( 'j' ) ), 'j' },
			(testpitch){ "   l: ", -( (int)( 'l' ) ), 'l' },
			(testpitch){ "   K: ", -( (int)( 'K' ) ), 'K' },
			(testpitch){ "   S: ", -( (int)( 'S' ) ), 'S' },
			(testpitch){ "   N: ", -( (int)( 'N' ) ), 'N' },
			(testpitch){ "   J: ", -( (int)( 'J' ) ), 'J' },
			(testpitch){ "   L: ", -( (int)( 'L' ) ), 'L' }
		},
		sexapitch[] =
		{
				/* Desired successes. */
			(testpitch){ "   0: ", 0, '0' },
			(testpitch){ "   1: ", 1, '1' },
			(testpitch){ "   2: ", 2, '2' },
			(testpitch){ "   3: ", 3, '3' },
			(testpitch){ "   4: ", 4, '4' },
			(testpitch){ "   5: ", 5, '5' },
			(testpitch){ "   6: ", 6, '6' },
			(testpitch){ "   7: ", 7, '7' },
			(testpitch){ "   8: ", 8, '8' },
			(testpitch){ "   9: ", 9, '9' },
			
			(testpitch){ "   k: ", 10, 'k' },
			(testpitch){ "   s: ", 11, 's' },
			(testpitch){ "   n: ", 12, 'n' },
			(testpitch){ "   j: ", 13, 'j' },
			(testpitch){ "   f: ", 14, 'f' },
			(testpitch){ "   l: ", 15, 'l' },
			
			(testpitch){ "   K: ", 10, 'K' },
			(testpitch){ "   S: ", 11, 'S' },
			(testpitch){ "   N: ", 12, 'N' },
			(testpitch){ "   J: ", 13, 'J' },
			(testpitch){ "   F: ", 14, 'F' },
			(testpitch){ "   L: ", 15, 'L' },
			
				/* Desired failures. */
			(testpitch){ "   \'g\': ", -( (int)( 'g' ) ), 'g' },
			(testpitch){ "   \'G\': ", -( (int)( 'G' ) ), 'G' },
			
			/* Sexagesimal and hexadecimal are not the same thing, so these */
			/*  should all be failures. */
			(testpitch){ "   a: ", -( (int)( 'a' ) ), 'a' },
			(testpitch){ "   b: ", -( (int)( 'b' ) ), 'b' },
			(testpitch){ "   c: ", -( (int)( 'c' ) ), 'c' },
			(testpitch){ "   d: ", -( (int)( 'd' ) ), 'd' },
			(testpitch){ "   e: ", -( (int)( 'e' ) ), 'e' },
			(testpitch){ "   A: ", -( (int)( 'A' ) ), 'A' },
			(testpitch){ "   B: ", -( (int)( 'B' ) ), 'B' },
			(testpitch){ "   C: ", -( (int)( 'C' ) ), 'C' },
			(testpitch){ "   D: ", -( (int)( 'D' ) ), 'D' },
			(testpitch){ "   E: ", -( (int)( 'E' ) ), 'E' }
		};
	/* Roughly the system of the ILLIAC 1, the first private (particularly */
	/*  college) owned computer. Yes, it seems odd, but it was very early, */
	/*  and the letters may have started as abbreviations. */
	/* Probably worth noting that this isn't actually used: I just put it in */
	/*  because it came to mind (blame Dave's Garage on Youtube). */
	
	testbatch batches[] =
		{
			{ sizeof( binpitch ), binpitch, &bin2num, "bin2num" },
			{ sizeof( octpitch ), octpitch, &oct2num, "oct2num" },
			{ sizeof( decpitch ), decpitch, &dec2num, "dec2num" },
			{ sizeof( hexapitch ), hexapitch, &hexa2num, "hexa2num" },
			{ sizeof( sexapitch ), sexapitch, &sexa2num, "sexa2num" }
		};
	
	int test = 1;
	size_t iter = 0;
	
	printf( "Simple *2num() test program.\n\n" );
	
	
	while( test && iter < sizeof( batches ) )
	{
		test = test_batch( &( batches[ iter ] ) );
		++iter;
		printf( "\n" );
	}
	
	
	if( result == EXIT_SUCCESS )
	{
		printf( " Testing done, all tests passed.\n" );
		
	} else {
		
		printf( " Testing done, failures encountered.\n" );
	}
	
	
	return( result );
}
