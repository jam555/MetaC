#include<string.h>

#include "command_parse.h"

	/* optn and opts are the strings that are recognized as */
	/*  command-line arguments. They are allowed to be followed */
	/*  by an equals sign on the actual command line, but shouldn't */
	/*  directly contain one themselves. The index of the first */
	/*  recognized option will be recorded in the matches instance */
	/*  that corresponds to the currently searched args instance, or */
	/*  else will include -1. The position of the first encountered */
	/*  equals sign will be recorded in eqpoints, or else -1. */
	/* args, matches, and eqpoints must all have argn members; optl */
	/*  and opts must have optn members; the members of args and */
	/*  opts must be null terminated strings; the members of optl */
	/*  must be the lengths of the corresponding members of opts. */
int arg_parser
(
	int argn, const char **args,
	int *matches, int *eqpoints,
	
	size_t optn, size_t *optl, const char **opts
)
{
	size_t iter1 = 0, iter2;
	const char *eq;
	
	if
	(
		!argn || !args || !matches || !eqpoints ||
		!optn || !optl || !opts
	)
	{
		return( -1 );
	}
	
	while( iter1 < argn && ( args[ iter1 ] && 1 ) )
	{
		/* We're going to assume that a null agrs[] pointer */
		/*  means that we're ACTUALLY at the end of the */
		/*  argument list, but these "&& 1" bits are to */
		/*  reserve/bookmark space for platform adaptation. */
		
		if( args[ iter1 ] && 1 )
		{
			matches[ iter1 ] = -1;
			
				/* Will return NULL if character not found. */
			eq = strchr( args[ iter1 ], '=' );
			if( eq )
			{
				eqpoints[ iter1 ] = (int)( eq - args[ iter1 ] );
				
				iter2 = 0;
				while( iter2 < optn )
				{
					if
					(
						memcmp
						(
							(const void*)( args[ iter1 ] ),
							(const void*)( opts[ iter2 ] ),

							eqpoints[ iter1 ] < optl[ iter2 ] ?
								eqpoints[ iter1 ] :
								optl[ iter2 ]
						) == 0
					)
					{
						matches[ iter1 ] = iter2;
						
						iter2 = optn;
						
					} else {
						
						++iter2;
					}
				}

			} else {

				eqpoints[ iter1 ] = -1;
				
				iter2 = 0;
				while( iter2 < optn )
				{
					if( strcmp( args[ iter1 ], opts[ iter2 ] ) == 0 )
					{
						matches[ iter1 ] = iter2;
						
						iter2 = optn;
						
					} else {

						++iter2;
					}
				}
			}
			
		} else {
			
			/* It would be good for debug messages to be */
			/*  emitted from here. */
		}
		
		++iter1;
	}
	if( iter1 < argn )
	{
		/* Emit a debug message here. */
	}
	
	return( 1 );
}
