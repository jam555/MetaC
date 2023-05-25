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

/* Note: a file named common_include.incl, and another named test.incl, */
/*  must be in the same directory as the executable. */


#include "stdlib.h"
#include "stdio.h"


#include "../../external_dependencies/libandria4/basic/pascalstring.h"

#include "../../src/basics.h"
#include "../../src/input/source.h"


void charin_onerror( uintptr_t errval, char *prestr )
{
	if( !prestr )
	{
		prestr = "";
	}
	
	printf( "%s", prestr );
	switch( errval )
	{
		case LIB4_RESULT_FAILURE_DOMAIN:
			printf( "charin() received an illegaly null argument.\n" );
			break;
		
		case LIB4_RESULT_FAILURE_UNDIFFERENTIATED:
			printf( "charin() had a generic failure.\n" );
			/* discard source failure. */
			/* EOF escaped discard source measures. */
			break;
		
		case LIBANDRIA4_RESULT_FAILURE_LOGICFAULT:
			printf( "charin() detected a refed string reference count below 0 after incrementing.\n" );
			break;
		
		case LIB4_RESULT_FAILURE_EOF:
			printf( "charin() found no sources with remaining characters, character source has run dry.\n" );
			break;
		
		default:
			printf( "Unrecognized error value!\n" );
			break;
	}
}



void runloop()
{
	int ires;
	
	
	printf( "\tRunning echo test.\n" );
	{
		/* If refresh_srcname is provided, then the pointer it points to */
		/*  MUST be NULL, as internal logic makes that assumption itself. */
		/*  Only sets refresh_srcname if the source{} instance changes, in */
		/*  which case the provided refed_pstr will ALREADY have a reference */
		/*  added for the new recipient: remember to take this into */
		/*  consideration for proper memory management. If refresh_srcname */
		/*  is provided, then prog MUST be provided; prog specifies the read */
		/*  progress that has been made through the current file, in case of */
		/*  returning from an include... */
		refed_pstr *prev = (refed_pstr*)0, *refresh = (refed_pstr*)0;
		/* Everything with a "prog" argument is probably using it wrong.  */
		uintmax_t prog;
		char_result cres;
		int loop = 1;
		char b;
		
		
#define main_CHECK_FNAME() \
	if( refresh ) { \
		printf( "\n\t\t!!! Detected new source name!\n" ); \
			printf( "\t\t Old name: %s\n", (char*)( prev->text->body ) ); \
			printf( "\t\t New progress & name: %i, %s\n", (int)prog, (char*)( refresh->text->body ) ); \
		int res = refed_pstr_decrrefs( prev ); \
		if( res <= 0 ) { \
			printf( "\t\t\trefed_pstr_decrrefs() errored with %i.\n\tExiting.\n", res ); \
			return( EXIT_FAILURE ); } \
		prev = refresh; \
		refresh = (refed_pstr*)0; \
		prog = 0; }
		
			/* Both proper success and LIB4_RESULT_FAILURE_EOF are actually */
			/*  successes. Only error out on OTHER returns. Note that this */
			/*  internally calls charback(), but DOES NOT call charin(). */
#define main_CHARPEEK_ONFAIL( err ) \
	if( err.val == LIB4_RESULT_FAILURE_EOF ) { \
		if( ires ) { \
			printf( "\t\t\tcharpeek() encountered and correctly reported EOF.\n" ); } \
		else { \
			printf( "\t\t\tcharpeek() encountered but inconsistently reported EOF.\n\tExiting.\n" ); \
			return( EXIT_FAILURE ); } } \
	else { \
		printf( "\t\t\tcharpeek() errored out with %u.\n\tExiting.\n", (unsigned int)( ( err ).val ) ); \
		return( EXIT_FAILURE ); \
	}
		cres = charpeek( &ires );
		CHAR_RESULT_BODYMATCH( cres, LIBANDRIA4_OP_SETb, main_CHARPEEK_ONFAIL )
		printf( "\t\tPeek test, peeked character: \'%c\', %i.\n", (char)b, (int)b );
		
#define main_CHARBACK_INFAIL( err ) \
	charin_onerror( ( err ).val, "\t\t\t" ); \
	return( EXIT_FAILURE );
		printf( "\t\tTesting push-back capability, pushing \'q\'.\n" );
		ires = charback( 'q' );
		if( !ires )
		{
			printf( "\t\t\tcharback() reported push failure.\n\tExiting.\n" );
			return( EXIT_FAILURE );
		}
		cres = charin( &refresh, &prog );
		CHAR_RESULT_BODYMATCH( cres, LIBANDRIA4_OP_SETb, main_CHARBACK_INFAIL )
		printf( "\t\tReading pushed character: \'%c\', %i.\n", (char)b, (int)b );
		main_CHECK_FNAME();
		
		printf( "\t\tEchoing:\n" );
		while( loop )
		{
#define main_MIDLOOP_INFAIL( err ) \
	if( ( err ).val == LIB4_RESULT_FAILURE_EOF ) { \
		loop = 0; } \
	else { \
		charin_onerror( ( err ).val, "\n\t\tLooping failure, exiting.\n" ); \
		return( EXIT_FAILURE ); }
			cres = charin( &refresh, &prog );
			CHAR_RESULT_BODYMATCH( cres, LIBANDRIA4_OP_SETb, main_MIDLOOP_INFAIL )
			putchar( b );
			main_CHECK_FNAME();
		}
		
		
			/* This appears totally useless, check to see if it can shrink charin()'s code. */
		/* ires = char_dropeof( &refresh, &prog ); */
	}
	
	
	/* Void function, so no return needed. */
}

int main( int argn, char *argc[] )
{
	printf( "\n\n" );
	printf( "Running MetaCPP source.h test.\n\n" );
	
	
	
	printf( "\tInitializing source.h\n" );
	int ires = init();
	switch( ires )
	{
		case 1: /* Success. */
			printf( "\t\tinit() succeeded.\n" );
			break;
			
		case -1: /* Unchanged */
		case -2: /* common_include.incl couldn't be opened, report standard error number. */
		default: /* Errors in pascal array build. */
			return( EXIT_FAILURE );
	}
	
	char *testfile_name = "test.incl\0";
	
	printf( "\t\tpushing %s as a source.\n", testfile_name );
	char_parrres importname_res = char_parr_strbuild( testfile_name );
	char_pascalarray *a;
	{
#define main_STRBUILDFAIL( err ) \
	printf( "\t\tchar_parr_strbuild() failed with %u.\n\tExiting.\n", (unsigned int)( err.val ) ); \
	return( EXIT_FAILURE );
		
		LIBANDRIA4_DEFINE_PASCALARRAY_RESULT_BODYMATCH(
			importname_res,
				LIBANDRIA4_OP_SETa,
				main_STRBUILDFAIL
		)
	}
	source *src = push_source( a, 0 );
	if( !src )
	{
		printf( "\t\tpush_source() failed.\n\tExiting.\n" );
		
		return( EXIT_FAILURE );
	}
	printf( "\t\tInitialization succeeded.\n\n" );
	
	runloop();
	
	
	
	printf( "\n" );
	printf( "\tReinitializing source.h\n" );
	ires = reinit();
	switch( ires )
	{
		case 1: /* Success. */
			printf( "\t\treinit() succeeded.\n" );
			break;
			
		case -1: /* Unchanged */
		case -2: /* common_include.incl couldn't be opened, report standard error number. */
		default: /* Errors in pascal array build. */
			return( EXIT_FAILURE );
	}
	
	printf( "\t\tPushing %s as a source to test discard_source().\n", testfile_name );
	source *src = push_source( a, 0 );
	if( !src )
	{
		printf( "\t\t\tpush_source() failed.\n\tExiting.\n" );
		
		return( EXIT_FAILURE );
	}
	
	ires = discard_source( src );
	switch( ires )
	{
		case -1:
			printf( "\t\t\tdiscard_source() reported a null argument.\n" );
			return( EXIT_FAILURE );
		
		case -2:
			printf( "\t\t\tdiscard_source() reported it's argument was the unfreeable root source.\n" );
			return( EXIT_FAILURE );
		
		default:
			if( ires )
			{
				printf( "\t\t\tdiscard_source() succeeded with a code of %i.\n", ires );
				break;
				
			} else {
				
				printf( "\t\t\tdiscard_source() failed with a code of %i.\n", ires );
				return( EXIT_FAILURE );
			}
	}
	
	runloop();
	
	
	
	printf( "\n" );
	printf( "\tDeinitializing.\n" );
	deinit();
	
	
	return( EXIT_SUCCESS );
}
