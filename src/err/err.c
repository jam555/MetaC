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


	/* size_t I think. */
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
	/* memcpy() or something? */
#include <string.h>
#include <ctype.h>
#include <wchar.h>
	/* For the varargs stuff. */
#include <stdarg.h>


#include "../../external_dependencies/libandria4/basic/commonerrvals.h"
#include "../../external_dependencies/libandria4/basic/commontypes.h"
#include "../../external_dependencies/libandria4/basic/simpleops.h"
#include "../../external_dependencies/libandria4/basic/nulls.h"
#include "../../external_dependencies/libandria4/basic/monads.h"
#include "../../external_dependencies/libandria4/basic/stdmonads.h"
#include "../../external_dependencies/libandria4/basic/pascalarray.h"
#include "../../external_dependencies/libandria4/basic/arraccess.h"

	/* For the sake of abbreviated names. */
#include "../../external_dependencies/libandria4/stdabbrevs/metaCabbrevs.h"


#include "err.h"
#include "inner_err.h"
#include "../input/char2num.h"



#if defined( DEBUG ) && DEBUG
	#define METAC_DEBUG 1
#else
	#define METAC_DEBUG 0
#endif



LIB4_RESULT err_chout( int character )
{
	errno = 0;
	
	if( character == EOF )
	{
		return( LIB4_RESULT_BUILDFAILURE( LIB4_RESULT_FAILURE_DOMAIN ) );
	}
	
	int res = fputc( character, stderr );
	
	if( res != character )
	{
		if( errno == 0 )
		{
			return( LIB4_ERRNO_2RESULT() );
		}
		
		if( res == EOF )
		{
			return( LIB4_RESULT_BUILDFAILURE( LIB4_RESULT_GENERIC ) );
		}
		
		return( LIB4_RESULT_BUILDFAILURE( LIB4_RESULT_FAILURE_RANGE ) );
	}
	
	return( LIB4_RESULT_BUILDSUCCESS( LIB4_RESULT_GENERIC ) );
}
LIB4_RESULT err_clearout()
{
	clearerr( stderr );
	
	return( LIB4_RESULT_BUILDSUCCESS( LIB4_RESULT_GENERIC ) );
}



typedef enum
{
	psf_null = 0,
	
	psf_leftjust = 1,
	psf_signall = 2,
	psf_signspace = 4,
	psf_elaborate = 8,
	psf_zerospad = 16
	
} printf_spec_flags;
typedef enum
{
	psl_null = 0,
	
	psl_dshort = 1,
	psl_short = 2,
	
	psl_normal = 3,
	
	psl_long = 4,
	psl_longlong = 5,
	
	psl_fraclong = 6,
	
	psl_maxt = 7,
	psl_sizet = 8,
	psl_ptrdiff = 9
	
} printf_spec_length;
typedef enum
{
	pss_null = 0,
	
	pss_percent = 1,
		/* dint & iint only differ when reading data from a source, not when */
		/*  outputing. */
	pss_dint = 2,
	pss_iint = 3,
		/* Unsigned ints in various formats. */
	pss_bin = 4,
	pss_BIN = 5,
	pss_oct = 6,
	pss_dec = 7,
	pss_hex = 8,
	pss_HEX = 9,
		/* Various formats of double. */
	pss_fix = 10,
	pss_FIX = 11,
	pss_exp = 12,
	pss_EXP = 13,
		/* Dynamically chooses between fixed point and exponential forms, */
		/*  whichever is smaller. */
	pss_dyn = 14,
	pss_DYN = 15,
		/* Double in hexadecimal form. Starts with 0x or 0X, but past that I */
		/*  have no idea. */
	pss_dex = 16,
	pss_DEX = 17,
		/* Assorted stuff. */
	pss_str = 18,
	pss_char = 19,
	pss_ptr = 20,
		/* Print nothing, store number of written characters via an integer pointer (Java printf() prints a newline). */
	pss_printed = 21
	
} printf_spec_specifier;
typedef struct printf_spec printf_spec;
struct printf_spec
{
	printf_spec_flags flags;
	
	LIB4_SSIZE width;
	LIB4_SSIZE precision;
	
	printf_spec_length length;
	
	printf_spec_specifier specifier;
};

int parse_printfspec( char **text,  printf_spec *ps )
{
#define METAC_parse_printfspec_DEBUG 0
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tInside parse_printfspec( %i );\n", (int)( __LINE__ ) );
	#endif
	
	LIB4_INTRESULT res;
	
	if( !text || !( *text ) || !ps )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\t return( -1 );\n" );
		#endif
		return( -1 );
	}
	if( **text != '%' )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\t return( -2 );\n" );
		#endif
		return( -2 );
	}
	++( *text );
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\t\tpassing specifier \'%\';\n" );
	#endif
	
	ps->flags = psf_null;
	ps->width = 0;
	ps->precision = 0;
	ps->length = psl_null;
	ps->specifier = pss_null;
	
	while( **text != '\0' )
	{
		#if defined( METAC_DEBUG ) && METAC_DEBUG
			printf
			(
				"\t\tlooping( %i ), **text == %c ( %i );\n",
					__LINE__,
					(char)( **text ),
					(int)( **text )
			);
		#endif
		
		switch( **text )
		{
			case '-':
				#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
					printf( "\t\t\t- case;\n" );
				#endif
				ps->flags |= psf_leftjust;
				++( *text );
				break;
			case '+':
				#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
					printf( "\t\t\t+ case;\n" );
				#endif
				ps->flags |= psf_signall;
				++( *text );
				break;
			case ' ':
				#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
					printf( "\t\t\t\' \' case;\n" );
				#endif
				ps->flags |= psf_signspace;
				++( *text );
				break;
			case '#':
				#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
					printf( "\t\t\t# case;\n" );
				#endif
				ps->flags |= psf_elaborate;
				++( *text );
				break;
			case '0':
				#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
					printf( "\t\t\t0 case;\n" );
				#endif
				ps->flags |= psf_zerospad;
				++( *text );
				break;
			
			default:
				#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
					printf
					(
						"\t\t\tdefault case ( %c == %i ), goto width;\n",
							(char)( **text ),
							(int)( **text )
					);
				#endif
				goto width;
		}
	}
	
	width:
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
		printf
		(
			"\t\tchecking width( %i ), **text == %c ( %i );\n",
				__LINE__,
				(char)( **text ),
				(int)( **text )
		);
	#endif
	if( **text == '\0' )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\t return( -3 );\n" );
		#endif
		return( -3 );
	}
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
		printf( "\t\tstring continues;\n" );
	#endif
	if( **text == '*' )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\t**text == *;\n" );
		#endif
		
		ps->width = -1;
		++( *text );
		
	} else if( isdigit( **text ) != 0 )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\tisdigit( %c ) != 0;\n",  (char)( **text ) );
		#endif
		
		ssize_t acc = 0, b = 0;
		while( **text != '\0' && isdigit( **text ) != 0 )
		{
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tlooping: ( %c ) != 0;\n",  (char)( **text ) );
			#endif
#define parse_printfspec_FAIL1( err ) return( -4 );
			acc *= 10;
			res = dec2num( **text );
			LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, parse_printfspec_FAIL1 );
			acc += b;
			
			++( *text );
		}
		
		ps->width = acc;
		
	}
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
		printf
		(
			"\t\tchecking precision( %i ), **text == %c ( %i );\n",
				__LINE__,
				(char)( **text ),
				(int)( **text )
		);
	#endif
		/* Precision. */
	if( **text == '.' )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\t**text == .;\n" );
		#endif
		
		++( *text );
		if( **text == '\0' )
		{
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t return( -5 );\n" );
			#endif
			return( -5 );
		}
		
		if( isdigit( **text ) != 0 )
		{
			LIB4_SSIZE acc = 0, b = 0;
			while( **text != '\0' && isdigit( **text ) != 0 )
			{
				#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
					printf( "\t\t\tlooping: ( %c ) != 0;\n",  (char)( **text ) );
				#endif
#define parse_printfspec_FAIL2( err ) return( -6 );
				acc *= 10;
				res = dec2num( **text );
				LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, parse_printfspec_FAIL2 );
				acc += b;
				
				++( *text );
			}
			
			ps->precision = acc;
			
		} else if( **text == '*' )
		{
			ps->precision = -1;
			++( *text );
			
		} else {
			
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t return( -7 );\n" );
			#endif
			return( -7 );
		}
	}
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
		printf
		(
			"\t\tchecking length modifier( %i ), **text == %c ( %i );\n",
				__LINE__,
				(char)( **text ),
				(int)( **text )
		);
	#endif
		/* Length modifier. Indicates the data size of the value that was */
		/*  provided as an argument. Note that "L" is the equivalent of "l", */
		/*  but for doubles; while "l" works for both characters and */
		/*  strings, in addition to the numbers that everything other than */
		/*  "L" will work with. */
	switch( **text )
	{
		case 'h':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\th case;\n" );
			#endif
			if( ( *text )[ 1 ] == 'h' )
			{
				ps->length = psl_dshort;
				++( *text );
				
			} else {
				
				ps->length = psl_short;
			}
			++( *text );
			break;
		case 'l':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tl case;\n" );
			#endif
			if( ( *text )[ 1 ] == 'l' )
			{
				ps->length = psl_long;
				++( *text );
				
			} else {
				
				ps->length = psl_longlong;
			}
			++( *text );
			break;
		case 'L':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tL case;\n" );
			#endif
			ps->length = psl_fraclong;
			++( *text );
			break;
		case 'j':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tj case;\n" );
			#endif
			ps->length = psl_maxt;
			++( *text );
			break;
		case 'z':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tz case;\n" );
			#endif
			ps->length = psl_sizet;
			++( *text );
			break;
		case 't':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tt case;\n" );
			#endif
			ps->length = psl_ptrdiff;
			++( *text );
			break;
		
		default:
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tdefault case;\n" );
			#endif
			ps->length = psl_normal;
			break;
	}
	if( ps->length == 0 )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\t return( -8 );\n" );
		#endif
		return( -8 );
	}
	if( ps->length != psl_normal )
	{
		#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
			printf( "\t\tps->length( %i ) != psl_normal, incrementing *text;\n", (int)( ps->length ) );
		#endif
		++( *text );
	}
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
		printf
		(
			"\t\tchecking specifier( %i ), **text == %c ( %i );\n",
				__LINE__,
				(char)( **text ),
				(int)( **text )
		);
	#endif
		/* Specifier. */
	switch( **text )
	{
		case '%':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tpercent case;\n" );
			#endif
			ps->specifier = pss_percent;
			++( *text );
			break;
		
		
			/* These only differ when reading data from a source, not when */
			/*  outputing. */
		case 'd':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\td case;\n" );
			#endif
			ps->specifier = pss_dint;
			++( *text );
			break;
		case 'i':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\ti case;\n" );
			#endif
			ps->specifier = pss_iint;
			++( *text );
			break;
		
		
		case 'b':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tb case;\n" );
			#endif
			ps->specifier = pss_bin;
			++( *text );
			break;
		case 'B':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tB case;\n" );
			#endif
			ps->specifier = pss_BIN;
			++( *text );
			break;
		
		case 'o':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\to case;\n" );
			#endif
			ps->specifier = pss_oct;
			++( *text );
			break;
		
		case 'u':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tu case;\n" );
			#endif
			ps->specifier = pss_dec;
			++( *text );
			break;
		
		case 'x':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tx case;\n" );
			#endif
			ps->specifier = pss_hex;
			++( *text );
			break;
		case 'X':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tX case;\n" );
			#endif
			ps->specifier = pss_HEX;
			++( *text );
			break;
		
		
		case 'f':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tf case;\n" );
			#endif
			ps->specifier = pss_fix;
			++( *text );
			break;
		case 'F':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tF case;\n" );
			#endif
			ps->specifier = pss_FIX;
			++( *text );
			break;
		
		case 'e':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\te case;\n" );
			#endif
			ps->specifier = pss_exp;
			++( *text );
			break;
		case 'E':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tE case;\n" );
			#endif
			ps->specifier = pss_EXP;
			++( *text );
			break;
		
		case 'g':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tg case;\n" );
			#endif
			ps->specifier = pss_dyn;
			++( *text );
			break;
		case 'G':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tG case;\n" );
			#endif
			ps->specifier = pss_DYN;
			++( *text );
			break;
		
		case 'a':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\ta case;\n" );
			#endif
			ps->specifier = pss_dex;
			++( *text );
			break;
		case 'A':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tA case;\n" );
			#endif
			ps->specifier = pss_DEX;
			++( *text );
			break;
		
		
		case 's':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\ts case;\n" );
			#endif
			ps->specifier = pss_str;
			++( *text );
			break;
		
		case 'c':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tc case;\n" );
			#endif
			ps->specifier = pss_char;
			++( *text );
			break;
		
		case 'p':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tp case;\n" );
			#endif
			ps->specifier = pss_ptr;
			++( *text );
			break;
		
		
		
		case 'n':
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t\tn case;\n" );
			#endif
			ps->specifier = pss_printed;
			++( *text );
			break;
		
		
		
		default:
			#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || METAC_parse_printfspec_DEBUG
				printf( "\t\t return( -9 );\n" );
			#endif
			return( -9 );
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting parse_printfspec( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}

static int intprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	intmax_t change;
	int res;
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || 0
		printf( "\tInside intprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_dshort:
				/* The type for va_arg() would properly be "signed char", but argument */
				/*  promotion rules mean that we need to request an int to maintain */
				/*  alignment. */
			res = fprintf( stderr,  tspec, (signed char)( va_arg( *vals, int ) ), &change );
			break;
		case psl_short:
				/* As with psl_dshort. */
			res = fprintf( stderr,  tspec, (short int)( va_arg( *vals, int ) ), &change );
			break;
		case psl_normal:
			res = fprintf( stderr,  tspec, va_arg( *vals, int ), &change );
			break;
		case psl_long:
			res = fprintf( stderr,  tspec, va_arg( *vals, long int ), &change );
			break;
		case psl_longlong:
			res = fprintf( stderr,  tspec, va_arg( *vals, long long int ), &change );
			break;
		case psl_maxt:
			res = fprintf( stderr,  tspec, va_arg( *vals, intmax_t ), &change );
			break;
		case psl_sizet:
			res = fprintf( stderr,  tspec, va_arg( *vals, size_t ), &change );
			break;
		case psl_ptrdiff:
			res = fprintf( stderr,  tspec, va_arg( *vals, ptrdiff_t ), &change );
			break;
		
		
		default:
			return( -2 );
	}
	
	if( progress )
	{
		*progress += change;
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting intprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}
static int unsignedprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || 0
		printf( "\tInside unsignedprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_dshort:
				/* The type for va_arg() would properly be "unsigned char", but */
				/*  argument promotion rules mean that we need to request an int to */
				/*  maintain alignment. */
			res = fprintf( stderr,  tspec, (unsigned char)( va_arg( *vals, int ) ), &change );
			break;
		case psl_short:
				/* As with psl_dshort. */
			res = fprintf( stderr,  tspec, (unsigned short int)( va_arg( *vals, int ) ), &change );
			break;
		case psl_normal:
			res = fprintf( stderr,  tspec, va_arg( *vals, unsigned int ), &change );
			break;
		case psl_long:
			res = fprintf( stderr,  tspec, va_arg( *vals, unsigned long int ), &change );
			break;
		case psl_longlong:
			res = fprintf( stderr,  tspec, va_arg( *vals, unsigned long long int ), &change );
			break;
		case psl_maxt:
			res = fprintf( stderr,  tspec, va_arg( *vals, uintmax_t ), &change );
			break;
		case psl_sizet:
			res = fprintf( stderr,  tspec, va_arg( *vals, size_t ), &change );
			break;
		case psl_ptrdiff:
			res = fprintf( stderr,  tspec, va_arg( *vals, ptrdiff_t ), &change );
			break;
		
		
		default:
			return( -2 );
	}
	
	if( progress )
	{
		*progress += change;
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting unsignedprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}
static int floatprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || 0
		printf( "\tInside floatprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_normal:
			res = fprintf( stderr,  tspec, va_arg( *vals, double ), &change );
			break;
		case psl_fraclong:
			res = fprintf( stderr,  tspec, va_arg( *vals, long double ), &change );
			break;
		
		
		default:
			return( -2 );
	}
	
	if( progress )
	{
		*progress += change;
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting foatprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}
static int charprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || 0
		printf( "\tInside charprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_normal:
			res = fprintf( stderr,  tspec, va_arg( *vals, int ), &change );
			break;
		case psl_long: /* Note: wint_t is specified in wchar.h */
			res = fprintf( stderr,  tspec, va_arg( *vals, wint_t ), &change );
			break;
		
		
		default:
			return( -2 );
	}
	
	if( progress )
	{
		*progress += change;
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting charprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}
static int strprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || 0
		fprintf( stderr,  "\tInside strprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_normal:
			res = fprintf( stderr,  tspec, va_arg( *vals, char* ), &change );
			break;
		case psl_long:
			res = fprintf( stderr,  tspec, va_arg( *vals, wchar_t* ), &change );
			break;
		
		
		default:
			return( -2 );
	}
	
	if( progress )
	{
		*progress += change;
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting strprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}
		/* There's no guarantee that fputc() has the right ABI... */
	static int ptrprint_tool( int val, void *file )
	{
		return( fputc( val, (FILE*)file ) );
	}
static int ptrprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || 0
		printf( "\tInside ptrprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_normal:
			res = fprintf( stderr,  tspec, va_arg( *vals, void* ), &change );
			break;
		case psl_fraclong:
			{
				/* "Extended type", so a void* followed by a */
				/*  customprint_signature (note: both of these are */
				/*  pointers!). The function pointer is given a */
				/*  char_receiver-functor and the initial void*, so */
				/*  that it can use the functor to print out the */
				/*  value associated with the void*. It should */
				/*  either succeed with only a single call, or */
				/*  return a negative that causes this function to */
				/*  give up and just return. */
				
				char_receiver cr = (char_receiver){ (void*)stderr, &ptrprint_tool };
				
				void *data = va_arg( *vals, void* );
				customprint_signature printer = va_arg( *vals, customprint_signature );
				
				res = printer( data, ps->width, ps->precision,  cr,  &change );
			}
			break;
		
		default:
			return( -2 );
	}
	
	if( progress )
	{
		*progress += change;
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting ptrprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}
static int countprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	
	#if ( defined( METAC_DEBUG ) && METAC_DEBUG ) || 0
		printf( "\tInside countprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	if( !ps || !progress || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_dshort:
			*( va_arg( *vals, signed char* ) ) = *progress;
			break;
		case psl_short:
			*( va_arg( *vals, short int* ) ) = *progress;
			break;
		case psl_normal:
			*( va_arg( *vals, int* ) ) = *progress;
			break;
		case psl_long:
			*( va_arg( *vals, long int* ) ) = *progress;
			break;
		case psl_longlong:
			*( va_arg( *vals, long long int* ) ) = *progress;
			break;
		case psl_maxt:
			*( va_arg( *vals, intmax_t* ) ) = *progress;
			break;
		case psl_sizet:
			*( va_arg( *vals, size_t* ) ) = *progress;
			break;
		case psl_ptrdiff:
			*( va_arg( *vals, ptrdiff_t* ) ) = *progress;
			break;
		
		
		default:
			return( -2 );
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tExiting countprint( %i );\n", (int)( __LINE__ ) );
	#endif
	
	return( 1 );
}



LIB4_DEFINE_PASCALARRAY_STDDEFINE( msgstyleptr_ , msg_style*  );
static msgstyleptr_pascalarray *std_messages;

LIB4_DEFINE_PASCALARRAY_STDDEFINE( dynchar_, char );


static int msg_inner( msg_style *style, va_list *vals )
{
	int ret = 1;
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tInside msg_inner( %i );\n", (int)( __LINE__ ) );
	#endif
	
#define msg_inner_ERREXIT( ret_ ) ret = ( ret_ ); goto msg_inner_exitpoint;
	if( !style || !vals )
	{
		msg_inner_ERREXIT( -1 );
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tInside msg_inner( %i );\n", (int)( __LINE__ ) );
	#endif
	
	char *txt = style->layout, *bookmark;
	int res;
	intmax_t progress = 0, scratch = 0;
	printf_spec spec;
	dynchar_pascalarray *a = (dynchar_pascalarray*)0;
	
	{
			/* TODO: figure out an actual, coherent number of elements */
			/*  to allocate. */
			/* I have no idea how large a string to allocate, so let's */
			/*  just use something fairly large for a specifier. */
		dynchar_pascalarray_result res = dynchar_pascalarray_build( 8 );
		
#define msg_inner_ERREXIT2( err ) msg_inner_ERREXIT( -2 );
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, msg_inner_ERREXIT2 );
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tInside msg_inner( %i );\n", (int)( __LINE__ ) );
	#endif
	
	
		/* Travel the provided layout. */
	while( *txt != '\0' )
	{
		#if defined( METAC_DEBUG ) && METAC_DEBUG
			printf
			(
				"\tInside msg_inner( %i ) looping: char( %c )==%i;\n",
					(int)( __LINE__ ),
					(char)( *txt ),
					(int)( *txt )
			);
		#endif
		
		if( *txt == '%' )
		{
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tEntering format specifier handling;\n" );
			#endif
			
			bookmark = txt;
				/* We only need to parse in one place. Note that this */
				/*  progresses *txt by itself, so "bookmark" is enough to */
				/*  get us the length of the specifier after this returns. */
			res = parse_printfspec( &txt,  &spec );
			if( !res )
			{
				msg_inner_ERREXIT( -3 );
			}
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tFormat specifier parsed;\n" );
			#endif
			
			
				/* Let's verify our string length... */
			if( ( txt - bookmark ) + 1 >= a->len )
			{
				dynchar_pascalarray_result res =
					dynchar_pascalarray_rebuild( a, ( txt - bookmark ) + 1 );
				
#define msg_inner_ERREXIT4( ret_ ) msg_inner_ERREXIT( -4 );
				LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, msg_inner_ERREXIT4 );
			}
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tLength verified;\n" );
			#endif
				/* Copy the specifier: we'll be needing it for these calls. */
			if
			(
				(void*)( a->body ) !=
				memcpy
				(
					(void*)( a->body ),
					(const void*)bookmark,
					
					( txt - bookmark )
				)
			)
			{
				msg_inner_ERREXIT( -5 );
			}
			a->body[ txt - bookmark ] = '\0';
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tFormat specifier copied;\n" );
			#endif
			
			
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tFormat specifier id==%i;\n", (int)( spec.specifier ) );
			#endif
				/* Let's handle the various possible options. */
			switch( spec.specifier )
			{
				case pss_null:
						/* Error! But how to report it? */
					msg_inner_ERREXIT( -6 );
				
				
				case pss_percent:
					fputc( '%',  stderr );
					progress += 1;
					break;
					
					/* dint & iint only differ when reading data from a */
					/*  source, not when outputing. */
				case pss_dint:
				case pss_iint:
					if( !intprint( &spec, ( a->body ), &scratch,  vals ) )
					{
						msg_inner_ERREXIT( -7 );
					}
					if( scratch < 0 )
					{
						msg_inner_ERREXIT( -8 );
					}
					
					progress += scratch;
					scratch = 0;
					
					break;
					
					/* Unsigned ints in various formats. */
				case pss_bin:
				case pss_BIN:
				case pss_oct:
				case pss_dec:
				case pss_hex:
				case pss_HEX:
					if( !unsignedprint( &spec, ( a->body ), &scratch,  vals ) )
					{
						msg_inner_ERREXIT( -9 );
					}
					if( scratch < 0 )
					{
						msg_inner_ERREXIT( -10 );
					}
					
					progress += scratch;
					scratch = 0;
					
					break;
					
					/* Various formats of double. */
				case pss_fix:
				case pss_FIX:
				case pss_exp:
				case pss_EXP:
				case pss_dyn:
				case pss_DYN:
						/* Dynamically chooses between fixed point and exponential forms, */
						/*  whichever is smaller. */
				case pss_dex:
				case pss_DEX:
						/* Double in hexadecimal form. Starts with 0x or 0X, but past that I */
						/*  have no idea. */
					if( !floatprint( &spec, ( a->body ), &scratch,  vals ) )
					{
						msg_inner_ERREXIT( -11 );
					}
					if( scratch < 0 )
					{
						msg_inner_ERREXIT( -12 );
					}
					
					progress += scratch;
					scratch = 0;
					
					break;
					
				case pss_str:
					if( !strprint( &spec, ( a->body ), &scratch,  vals ) )
					{
						msg_inner_ERREXIT( -13 );
					}
					if( scratch < 0 )
					{
						msg_inner_ERREXIT( -14 );
					}
					
					progress += scratch;
					scratch = 0;
					
					break;
				case pss_char:
					if( !charprint( &spec, ( a->body ), &scratch,  vals ) )
					{
						msg_inner_ERREXIT( -15 );
					}
					if( scratch < 0 )
					{
						msg_inner_ERREXIT( -16 );
					}
					
					progress += scratch;
					scratch = 0;
					
					break;
				case pss_ptr:
					if( !ptrprint( &spec, ( a->body ), &scratch,  vals ) )
					{
						msg_inner_ERREXIT( -17 );
					}
					if( scratch < 0 )
					{
						msg_inner_ERREXIT( -18 );
					}
					
					progress += scratch;
					scratch = 0;
					
					break;
					
				case pss_printed:
						/* Print nothing, store number of written characters */
						/*  via an integer pointer (Java printf() prints a */
						/*  newline). */
					if( !countprint( &spec, ( a->body ), &scratch,  vals ) )
					{
						msg_inner_ERREXIT( -19 );
					}
					if( scratch < 0 )
					{
						msg_inner_ERREXIT( -20 );
					}
					
					progress += scratch;
					scratch = 0;
					
					break;
				
				
				default:
					msg_inner_ERREXIT( -21 );
			}
			
			/* Note that parse_printfspec() will automatically progress txt */
			/*  by itself, since it both already understands the specifiers, */
			/*  and needs to travel along the thing anyways. */
			
		} else {
			
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tCalling fputc( %i );\n", __LINE__ );
			#endif
			
			/* Plain text, so just print it. */
			
			char c = *txt;
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tCalling fputc( %i );\n", __LINE__ );
			#endif
			res = fputc( *txt, stderr );
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tCalled fputc( %i ), result==EOF? %i ;\n", __LINE__, (int)( res == EOF ) );
			#endif
			if( res == EOF )
			{
				#if defined( METAC_DEBUG ) && METAC_DEBUG
					printf( "\t\t\tfputc() returned EOF;\n" );
				#endif
				
				msg_inner_ERREXIT( -22 );
			}
			
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tIncrementing variables;\n" );
			#endif
			
			progress += 1;
			
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tIncrementing txt;\n" );
			#endif
			++( txt );
			
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tBranch complete;\n" );
			#endif
		}
	}
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tInside msg_inner( %i );\n", (int)( __LINE__ ) );
	#endif
	
	
		/* Time to exit. */
	msg_inner_exitpoint:
	
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tInside msg_inner( %i ) exiting;\n", (int)( __LINE__ ) );
	#endif
	
	if( a )
	{
		LIB4_RESULT res = dynchar_pascalarray_destroy( a );
		
#define msg_inner_ERREXIT23( err ) return( -23 );
		LIB4_MONAD_EITHER_BODYMATCH( res, LIBANDRIA4_NULL_MACRO, msg_inner_ERREXIT23 );
		
		/* b.val will be the errno-contained result of the deallocation. Probably doesn't even get set, so we'll just ignore it. */
	}
	return( ret );
}

void msg_interface( msg_styleset *source, ERR_KEYTYPE first_key, ... )
{
	ERR_KEYTYPE curkey = first_key;
	va_list args;
	
	va_start( args, first_key );
	
	while( source )
	{
		if( !( source->members ) || source->count <= curkey )
		{
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tmsg_interface(): Error, log and exit!\n" );
				printf
				(
					"\t\t\tmembers: %p; count: %i; curkey: %i;\n",
						(void*)( source->members ),
						(int)( source->count ),
						(int)( curkey )
				);
			#endif
			
			/* Error, log & exit! Not that we have a method for the first */
			/*  two. */
			source = (msg_styleset*)0;
			#warning "WARNING! msg_interface() in src/err/err.c doesn't log & exit on bounds check!\n"
			
		} else if( curkey )
		{
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tmsg_interface(): non-null curkey, using caller message.\n" );
			#endif
			
			if( source->members[ curkey - 1 ].is_set )
			{
				/* It's a set of styles, so climb it. */
				
				source = source->members[ curkey - 1 ].data.set;
				curkey = va_arg( args, ERR_KEYTYPE );
				
				if( !source )
				{
					/* Error, but how to report it? Regardless, we'll soon */
					/*  be returning... */
					#warning "WARNING! msg_interface() in src/err/err.c doesn't log & exit on !source!\n"
				}
				
			} else {
				
				/* It's a "leaf" style, so execute it directly. */
				
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tEntering msg_inner( %i );\n", (int)( __LINE__ ) );
	#endif
				
				if( !msg_inner( source->members[ curkey - 1 ].data.style, &args ) )
				{
					/* Error, but how to report it? */
					#warning "WARNING! msg_interface() in src/err/err.c doesn't log & exit on bad msg_inner() result!\n"
				}
				source = (msg_styleset*)0;
			}
			
		} else if( -curkey < std_messages->len )
		{
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tmsg_interface(): Hardwired handlers.\n" );
			#endif
			
			/* Standard hardwired handlers. */
			
	#if defined( METAC_DEBUG ) && METAC_DEBUG
		printf( "\tEntering msg_inner( %i );\n", (int)( __LINE__ ) );
	#endif
			
			if( !msg_inner( std_messages->body[ -curkey ], &args ) )
			{
				/* Error, but how to report it? */
				#warning "WARNING! msg_interface() in src/err/err.c doesn't log & exit on bad msg_inner() result!\n"
			}
			source = (msg_styleset*)0;
			
		} else {
			
			#if defined( METAC_DEBUG ) && METAC_DEBUG
				printf( "\t\tmsg_interface(): Error, this should never happen!\n" );
			#endif
			/* ERROR! This should never happen. */
			#warning "WARNING! msg_interface() in src/err/err.c doesn't log & exit on reaching the unreachable!\n"
			
			source = (msg_styleset*)0;
		}
	}
	
	va_end( args );
}



static msg_style
	stdmsg_badnull = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_BADNULL ) },
	stdmsg_badnonnull = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_BADNONNULL ) },
	
	stdmsg_badnull2 = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_BADNULL2 ) },
	stdmsg_badnonnull2 = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_BADNONNULL2 ) },
	
	stdmsg_monadicfailure = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_MONADICFAILURE ) },
		stdmsg_noteline = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_NOTELINE ) },
		stdmsg_notespace = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_NOTESPACE ) },
		stdmsg_signedarg = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_SIGNEDARG ) },
		stdmsg_decarg = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_DECARG ) },
		stdmsg_hexarg = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_HEXARG ) },
		stdmsg_ldoublearg = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_LDOUBLEARG ) },
		stdmsg_chararg = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_CHARARG ) },
		stdmsg_strarg = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_STRARG ) },
		stdmsg_dataptrarg = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_DATAPTRARG ) },
	
	stdmsg_failedintfunc = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_FAILEDINTFUNC ) },
	stdmsg_faileduintfunc = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_FAILEDUINTFUNC ) },
	stdmsg_failedptrfunc = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_FAILEDPTRFUNC ) },
	
	stdmsg_trespasspath = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_TRESPASSPATH ) },
	
	stdmsg_badchar = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_BADCHAR ) },
	stdmsg_badint = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_BADINT ) },
	stdmsg_baduint = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_BADUINT ) },
	
	stdmsg_i_underflow = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_I_UNDERFLOW ) },
	stdmsg_i_overflow = { LIBANDRIA4_APPLY( LIB4_RETURN_2ND, STDMSG_I_OVERFLOW ) };

LIBANDRIA4_DEFINE_PASCALARRAY_STATICBUILD(
	msg_set,
	std_messages,
	
	msgstyleptr_,
	msg_style*,
	
	&stdmsg_badnull,
	&stdmsg_badnonnull,
	
	&stdmsg_badnull2,
	&stdmsg_badnonnull2,
	
	&stdmsg_monadicfailure,
		&stdmsg_noteline,
		&stdmsg_notespace,
		&stdmsg_signedarg,
		&stdmsg_decarg,
		&stdmsg_hexarg,
		&stdmsg_ldoublearg,
		&stdmsg_chararg,
		&stdmsg_strarg,
		&stdmsg_dataptrarg,
	
	&stdmsg_failedintfunc,
	&stdmsg_faileduintfunc,
	&stdmsg_failedptrfunc,
	
	&stdmsg_trespasspath,
	
	&stdmsg_badchar,
	&stdmsg_badint,
	&stdmsg_baduint,
	
	&stdmsg_i_underflow,
	&stdmsg_i_overflow
);
static msgstyleptr_pascalarray *std_messages = &( msg_set.std_messages );
