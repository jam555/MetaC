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

#include "headers.h"

#include "err/inner_err.h"


#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		static msg_styleset errs;
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs;
#else
	#error "The file " __FILE__ " requires at least C99 or C++98."
#endif



	/* This queues a call to head_lex(), in such a way that head_lex() will */
	/*  return with a call to assemble_token() instead of to */
	/*  getANDassemble_token(). This allows functions to be built from */
	/*  simpler functions in a simpler fashion than might otherwise be */
	/*  possible. */
retframe getANDassemble_token( stackpair *stkp, void *v )
{
	if( !stkp )
	{
			/* Use a standard error format. */
		STDMSG_BADNULL_WRAPPER( &errs, getANDassemble_token, &stkp );
	}
	
		/* Setup a return into assemble_token() to get the token material */
		/*  properly packaged, and then directly head into head_lex() to get */
		/*  that material itself. */
	int res;
	CALL_FRAMEFUNC(
		stkp,
		
		&assemble_token, (void*)0,
		&head_lex, (void*)0,
		
		&errs, getANDassemble_token,
		
		res,
		
		stack_ENDRETFRAME
	);
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
