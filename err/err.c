	/* For the varargs stuff. */
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

	/* All the stuff below should probably be included via this, but it's */
	/*  best to make dependencies explicit. */
#include "../headers.h"

#include "../pascalarray.h"
#include "../macrotime/arraccess.h"

#include "stdmonads.h"
#include "inner_err.h"
#include "../input/lexalike.h"



lib4_result err_chout( int character )
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
			return( lib4_errno_2result() );
		}
		
		if( res == EOF )
		{
			return( LIB4_RESULT_BUILDFAILURE( LIB4_RESULT_GENERIC ) );
		}
		
		return( LIB4_RESULT_BUILDFAILURE( LIB4_RESULT_FAILURE_RANGE ) );
	}
	
	return( LIB4_RESULT_BUILDSUCCESS( LIB4_RESULT_GENERIC ) );
}
lib4_result err_clearout()
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
	
	ssize_t width;
	ssize_t precision;
	
	printf_spec_length length;
	
	printf_spec_specifier specifier;
};

int parse_printfspec( char **text,  printf_spec *ps )
{
	lib4_intresult res;
	
	if( !text || !( *text ) || !ps )
	{
		return( -1 );
	}
	if( **text != '%' )
	{
		return( -2 );
	}
	++( *text );
	
	ps->flags = psf_null;
	ps->width = 0;
	ps->precision = 0;
	ps->length = psl_null;
	ps->specifier = pss_null;
	
	while( **text != '\0' )
	{
		switch( **text )
		{
			case '-':
				ps->flags |= psf_leftjust;
				++( *text );
				break;
			case '+':
				ps->flags |= psf_signall;
				++( *text );
				break;
			case ' ':
				ps->flags |= psf_signspace;
				++( *text );
				break;
			case '#':
				ps->flags |= psf_elaborate;
				++( *text );
				break;
			case '0':
				ps->flags |= psf_zerospad;
				++( *text );
				break;
			
			default:
				goto width;
		}
	}
	
	width:
	if( **text == '\0' )
	{
		return( -3 );
	}
	if( **text = '*' )
	{
		ps->width = -1;
		++( *text );
		
	} else if( isdigit( **text ) != 0 )
	{
		ssize_t acc = 0, b = 0;
		while( **text != '\0' && isdigit( **text ) != 0 )
		{
#define parse_printfspec_FAIL1( err ) return( -4 );
			acc *= 10;
			res = dec2num( **text );
			LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, parse_printfspec_FAIL1 );
			acc += b;
			
			++( *text );
		}
		
		ps->width = acc;
		
	} else if( **text = '*' )
	{
		ps->width = -1;
		++( *text );
	}
	
		/* Precision. */
	if( **text == '.' )
	{
		++( *text );
		if( **text == '\0' )
		{
			return( -5 );
		}
		
		if( isdigit( **text ) != 0 )
		{
			ssize_t acc = 0, b = 0;
			while( **text != '\0' && isdigit( **text ) != 0 )
			{
#define parse_printfspec_FAIL2( err ) return( -6 );
				acc *= 10;
				res = dec2num( **text );
				LIB4_INTRESULT_BODYMATCH( res, LIB4_OP_SETb, parse_printfspec_FAIL2 );
				acc += b;
				
				++( *text );
			}
			
			ps->precision = a;
			
		} else if( **text == '*' )
		{
			ps->precision = -1;
			++( *text );
			
		} else {
			
			return( -7 );
		}
	}
	
		/* Length modifier. Indicates the data size of the value that was */
		/*  provided as an argument. Note that "L" is the equivalent of "l", */
		/*  but for doubles; while "l" works for both characters and */
		/*  strings, in addition to the numbers that everything other than */
		/*  "L" will work with. */
	switch( **text )
	{
		case 'h':
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
			ps->length = psl_fraclong;
			++( *text );
			break;
		case 'j':
			ps->length = psl_maxt;
			++( *text );
			break;
		case 'z':
			ps->length = psl_sizet;
			++( *text );
			break;
		case 't':
			ps->length = psl_ptrdiff;
			++( *text );
			break;
		
		default:
			ps->length = psl_normal;
			break;
	}
	if( ps->length != 0 )
	{
		++( *text );
	}
	
		/* Specifier. */
	switch( **text )
	{
		case "%":
			ps->specifier = pss_percent;
			++( *text );
			break;
		
		
			/* These only differ when reading data from a source, not when */
			/*  outputing. */
		case "d":
			ps->specifier = pss_dint;
			++( *text );
			break;
		case "i":
			ps->specifier = pss_iint;
			++( *text );
			break;
		
		
		case "b":
			ps->specifier = pss_bin;
			++( *text );
			break;
		case "B":
			ps->specifier = pss_BIN;
			++( *text );
			break;
		
		case "o":
			ps->specifier = ;pss_oct
			++( *text );
			break;
		
		case "u":
			ps->specifier = pss_dec;
			++( *text );
			break;
		
		case "x":
			ps->specifier = pss_hex;
			++( *text );
			break;
		case "X":
			ps->specifier = pss_HEX;
			++( *text );
			break;
		
		
		case "f":
			ps->specifier = pss_fix;
			++( *text );
			break;
		case "F":
			ps->specifier = pss_FIX;
			++( *text );
			break;
		
		case "e":
			ps->specifier = pss_exp;
			++( *text );
			break;
		case "E":
			ps->specifier = pss_EXP;
			++( *text );
			break;
		
		case "g":
			ps->specifier = pss_dyn;
			++( *text );
			break;
		case "G":
			ps->specifier = pss_DYN;
			++( *text );
			break;
		
		case "a":
			ps->specifier = pss_dex;
			++( *text );
			break;
		case "A":
			ps->specifier = pss_DEX;
			++( *text );
			break;
		
		
		case "s":
			ps->specifier = pss_str;
			++( *text );
			break;
		
		case "c":
			ps->specifier = pss_char;
			++( *text );
			break;
		
		case "p":
			ps->specifier = pss_ptr;
			++( *text );
			break;
		
		
		
		case "n":
			ps->specifier = pss_printed;
			++( *text );
			break;
		
		
		
		default:
			return( -8 );
	}
	
	return( 1 );
}

static int intprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	intmax_t change;
	int res;
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_dshort:
			res = fprintf( stderr,  tspec, va_arg( *vals, signed char ), &change );
			break;
		case psl_short:
			res = fprintf( stderr,  tspec, va_arg( *vals, short int ), &change );
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
	
	return( 1 );
}
static int unsignedprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_dshort:
			res = fprintf( stderr,  tspec, va_arg( *vals, unsigned char ), &change );
			break;
		case psl_short:
			res = fprintf( stderr,  tspec, va_arg( *vals, unsigned short int ), &change );
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
	
	return( 1 );
}
static int floatprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
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
	
	return( 1 );
}
static int charprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
	if( !ps || !tspec || !vals )
	{
		return( -1 );
	}
	
	switch( ps->length )
	{
		case psl_normal:
			res = fprintf( stderr,  tspec, va_arg( *vals, int ), &change );
			break;
		case psl_long:
			res = fprintf( stderr,  tspec, va_arg( *vals, wint_t ), &change );
			break;
		
		
		default:
			return( -2 );
	}
	
	if( progress )
	{
		*progress += change;
	}
	
	return( 1 );
}
static int strprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	intmax_t change;
	
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
				
				void *data = va_arg( vals, void* );
				customprint_signature printer = va_arg( vals, customprint_signature );
				
				int (*customprint_signature)( void*, size_t, size_t,  char_receiver,  intmax* );
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
	
	return( 1 );
}
static int countprint( printf_spec *ps, char *tspec, intmax_t *progress,  va_list *vals )
{
	int res;
	
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
	
	return( 1 );
}



LIB4_DEFINE_PASCALARRAY_STDDEFINE( msgstyleptr_, msg_style* );
static msgstyleptr_pascalarray std_messages;



static int msg_inner( msg_style *style, va_list vals )
{
	LIB4_DEFINE_PASCALARRAY_STDDEFINE( dynchar_, char );
	
	int ret = 1;
	
#define msg_inner_ERREXIT( ret_ ) ret = ( ret_ ); goto msg_inner_exitpoint;
	if( !style )
	{
		msg_inner_ERREXIT( -1 );
	}
	
	char *txt = style->layout, *bookmark;
	int res;
	intmax_t progress = 0, scratch = 0
	printf_spec spec;
	dynchar_pascalarray *a = (dynchar_pascalarray*)0;
	
	{
		dynchar_pascalarray_result res = dynchar_pascalarray_build( size_t len );
		
#define msg_inner_ERREXIT2( err ) msg_inner_ERREXIT( -2 );
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, msg_inner_ERREXIT2 );
	}
	
	
		/* Travel the provided layout. */
	while( txt != '\0' )
	{
		if( *txt == '%' )
		{
			bookmark = txt;
				/* We only need to parse in one place. Note that this */
				/*  progresses *txt by itself, so "bookmark" is enough to */
				/*  get us the length of the specifier after this returns. */
			res = parse_printfspec( &txt,  &spec );
			if( !res )
			{
				msg_inner_ERREXIT( -3 );
			}
			
			
				/* Let's verify our string length... */
			if( ( txt - bookmark ) + 1 >= a->len )
			{
				dynchar_pascalarray_result res =
					dynchar_pascalarray_rebuild( a, ( txt - bookmark ) + 1 );
				
#define msg_inner_ERREXIT4( ret_ ) msg_inner_ERREXIT( -4 );
				LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, msg_inner_ERREXIT4 );
			}
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
			
			
				/* Let's handle the various possible options. */
			switch( spec->specifier )
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
					if( !intprint( &spec, ( a->body ), &scratch,  &vals ) )
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
					if( !unsignedprint( &spec, ( a->body ), &scratch,  &vals ) )
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
					if( !floatprint( &spec, ( a->body ), &scratch,  &vals ) )
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
					if( !strprint( &spec, ( a->body ), &scratch,  &vals ) )
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
					if( !charprint( &spec, ( a->body ), &scratch,  &vals ) )
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
					if( !ptrprint( &spec, ( a->body ), &scratch,  &vals ) )
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
					if( !countprint( &spec, ( a->body ), &scratch,  &vals ) )
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
			
			/* Plain text, so just print it. */
			
			if( fputc( *txt, (FILE*)stderr ) == EOF )
			{
				msg_inner_ERREXIT( -22 );
			}
			progress += 1;
			++( *text );
		}
	}
	
	
		/* Time to exit. */
	msg_inner_exitpoint:
	if( a )
	{
		lib4_result res = dynchar_pascalarray_destroy( a );
		
#define msg_inner_ERREXIT23( err ) return( -23 );
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, msg_inner_ERREXIT23 );
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
			/* Error, log & exit! Not that we have a method for the first */
			/*  two. */
			source = (msg_styleset*)0;
			
		} else if( curkey )
		{
			if( source->members[ curkey - 1 ].is_set )
			{
				/* It's a set of styles, so climb it. */
				
				source = source->members[ curkey - 1 ].data.set;
				curkey = va_arg( vals, ERR_KEYTYPE );
				
				if( !source )
				{
					/* Error, but how to report it? Regardless, we'll soon */
					/*  be returning... */
				}
				
			} else {
				
				/* It's a "leaf" style, so execute it directly. */
				
				if( !msg_inner( source->members[ curkey - 1 ].data.style, args ) )
				{
					/* Error, but how to report it? */
				}
				source = (msg_styleset*)0;
			}
			
		} else if( -curkey < std_messages.len )
		{
			/* Standard hardwired handlers. */
			
			if( !msg_inner( std_messages.body[ -curkey ], va_list vals ) )
			{
				/* Error, but how to report it? */
			}
			source = (msg_styleset*)0;
			
		} else {
			
			/* ERROR! This should never happen. */
			
			source = (msg_styleset*)0;
		}
	}
	
	va_end( args );
}



static msg_style
	stdmsg_badnull = { LIB4_RETURN_2ND( STDMSG_BADNULL ) },
	stdmsg_badnonnull = { LIB4_RETURN_2ND( STDMSG_BADNONNULL ) },
	
	stdmsg_badnull2 = { LIB4_RETURN_2ND( STDMSG_BADNULL2 ) },
	stdmsg_badnonnull2 = { LIB4_RETURN_2ND( STDMSG_BADNONNULL2 ) },
	
	stdmsg_monadicfailure = { LIB4_RETURN_2ND( STDMSG_MONADICFAILURE ) },
		stdmsg_noteline = { LIB4_RETURN_2ND( STDMSG_NOTELINE ) },
		stdmsg_notespace = { LIB4_RETURN_2ND( STDMSG_NOTESPACE ) },
		stdmsg_signedarg = { LIB4_RETURN_2ND( STDMSG_SIGNEDARG ) },
		stdmsg_decarg = { LIB4_RETURN_2ND( STDMSG_DECARG ) },
		stdmsg_hexarg = { LIB4_RETURN_2ND( STDMSG_HEXARG ) },
		stdmsg_ldoublearg = { LIB4_RETURN_2ND( STDMSG_LDOUBLEARG ) },
		stdmsg_chararg = { LIB4_RETURN_2ND( STDMSG_CHARARG ) },
		stdmsg_strarg = { LIB4_RETURN_2ND( STDMSG_STRARG ) },
		stdmsg_dataptrarg = { LIB4_RETURN_2ND( STDMSG_DATAPTRARG ) },
	
	stdmsg_failedintfunc = { LIB4_RETURN_2ND( STDMSG_FAILEDINTFUNC ) },
	stdmsg_faileduintfunc = { LIB4_RETURN_2ND( STDMSG_FAILEDUINTFUNC ) },
	stdmsg_failedptrfunc = { LIB4_RETURN_2ND( STDMSG_FAILEDPTRFUNC ) },
	
	stdmsg_trespasspath = { LIB4_RETURN_2ND( STDMSG_TRESPASSPATH ) },
	
	stdmsg_badchar = { LIB4_RETURN_2ND( STDMSG_BADCHAR ) },
	stdmsg_badint = { LIB4_RETURN_2ND( STDMSG_BADINT ) },
	stdmsg_baduint = { LIB4_RETURN_2ND( STDMSG_BADUINT ) },
	
	stdmsg_i_underflow = { LIB4_RETURN_2ND( STDMSG_I_UNDERFLOW ) },
	stdmsg_i_overflow = { LIB4_RETURN_2ND( STDMSG_I_OVERFLOW ) };

static msgstyleptr_pascalarray std_messages =
	LIB4_DEFINE_PASCALARRAY_LITERAL2(
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
