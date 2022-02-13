#include "headers.h"

	/* This queues a call to head_lex(), in such a way that head_lex() will */
	/*  return with a call to assemble_token() instead of to */
	/*  getANDassemble_token(). This allows functions to be built from */
	/*  simpler functions in a simpler fashion than might otherwise be */
	/*  possible. */
retframe getANDassemble_token( void* )
{
		/* Setup a return into assemble_token() to get the token material */
		/*  properly packaged, and then directly head into head_lex() to get */
		/*  that material itself. */
	CALL_FRAMEFUNC( &assemble_token, 0, &head_lex, 0 );
}
