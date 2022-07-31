#include <stdlib.h>

#include "headers.h"

#include "err/inner_err.h"
#include "command_parse.h"
#include "lexalike.h"



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


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )



master_context globals =
{
	&std_stacks,

	0,
	(char**)0,
	
	-1
};


retframe common_entrance_func( stackpair *stkp, void *v );

#define main_ERREXIT() return( ??? );
int main( int argn, char** args )
{
	int res, scratch;
	retframe entrance_func = &common_entrance_func;
	
	if( !argn || !args )
	{
		BADNULL2( main, &argn, &args );
		???
	}
	STACK_CHECK( globals.stkp,  &errs, main, abort );
	
	???
	
	res = init_stack( &( globals.stkp->ret ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &ret )", main, res );
		abort();
	}
	res = init_stack( &( globals.stkp->data ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &data )", main, res );
		abort();
	}
	
	res = init();
	if( !res )
	{
		FAILEDINTFUNC( "init()", main, res );
		abort();
	}
	
	res = token_queue_init();
	if( !res )
	{
		FAILEDINTFUNC( "token_queue_init()", main, res );
		abort();
	}
	
	res = searchstack_init();
	if( !res )
	{
		FAILEDINTFUNC( "searchstack_init()", main, res );
		abort();
	}
	
	res = macroargs_init();
	if( !res )
	{
		FAILEDINTFUNC( "macroargs_init()", main, res );
		abort();
	}
	
	
	???
	
	
	res = enable_loop( globals.stkp );
	if( !res )
	{
		FAILEDINTFUNC( "enable_loop()", main, res );
		???
		abort();
	}
	
	globals.argn = argn;
	globals.args = args;
	STACK_PUSH_UINT(
		( globals.stkp->data ),
		&globals,
		
		&errs,
		main,
		scratch,
		main_ERREXIT
	);
	res = run_loop( &entrance_func,  globals.stkp );
	if( !res )
	{
		FAILEDINTFUNC( "run_loop()", main, res );
		???
		abort();
	}
	
	
	???
	
	
	res = macroargs_clear();
	if( !res )
	{
		FAILEDINTFUNC( "macroargs_clear()", main, res );
		abort();
	}
	
	res = int searchstack_deinit();
	if( !res )
	{
		FAILEDINTFUNC( "searchstack_deinit()", main, res );
		abort();
	}
	
	res = token_queue_deinit();
	if( !res )
	{
		FAILEDINTFUNC( "token_queue_deinit()", main, res );
		abort();
	}
	
	deinit();
	
	res = clear_stack( &( globals.stkp->data ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &data )", main, res );
		abort();
	}
	res = clear_stack( &( globals.stkp->ret ) );
	if( !res )
	{
		FAILEDINTFUNC( "init_stack( &ret )", main, res );
		abort();
	}
	
	???
}



retframe common_???_func( stackpair *stkp, void *v )
{
	???
	
	retframe getANDassemble_token( stackpair *stkp, void *v );
	
	if( ->toktype == TOKTYPE_NAME )
	{
		struct genericnamed
		{
			char_pascalarray *name;
			void *ref;
			uintptr_t reftype;
		};
			/* tok->text must point to an actual string of text. */
		genericnamed* bsearch1_gennamearr( genname_parr *parr, token *tok );
		
	} else {
		
		struct generictyped
		{
			uintptr_t toktype;
			void *ref;
				/* ->reftype uses the same values as in struct genericnamed. */
			uintptr_t reftype;
		};
		generictyped* gentyped_bsearch( gentyped_parr *parr, token *tok );
	}
	
	???
}

#define common_entrance_func_ENDRETFRAME() return( (retframe){ &end_run, (void*)0 } )
retframe common_entrance_func( stackpair *stkp, void *v )
{
	master_context *glob;
	int scratch;
	
	{
		uintptr_t glob_;
		STACK_PEEK_UINT(
			( stkp->data ),
			0,
			&glob_,

			&errs,
			common_entrance_func,
			scratch,
			common_entrance_func_ENDRETFRAME
		);
		glob = (master_context*)glob_;
	}
	if( !glob )
	{
		BADNULL( common_entrance_func, &glob );
	}
	
	size_t iter = 0;
	int argn = glob->argn, *matches, *eqpoints;
	char **args = glob->args;
	matches = (int*)malloc( sizeof( int ) * argn );
	if( !matches )
	{
		/* malloc error, report and fail! */
	}
	eqpoints = (int*)malloc( sizeof( int ) * argn );
	if( !eqpoints )
	{
		free( matches );
		matches = (int*)0;
		
		/* malloc error, report and fail! */
	}
	if
	(
		!arg_parser
		(
			argn, args, matches, eqpoints,

			commandline_options_count,
			commandline_options_lengths,
			commandline_options
		)
	)
	{
		/* Error, log & fail. */
	}
	while( iter < argn )
	{
		switch( matches[ iter ] )
		{
			case common_entrance_func_CLANG:
			case common_entrance_func_CppLANG:
			case common_entrance_func_FORTRANLANG:
			case common_entrance_func_METACLANG:
			case common_entrance_func_OBJCLANG:
			case common_entrance_func_TEXT:
				if( glob->lang != -1 )
				{
					if( glob->lang == matches[ iter ] )
					{
						/* Warning: language was specified twice! */
						
					} else {
						
						/* Error: conflicting language specifications! */
					}
					
				} else {
					
					glob->lang = matches[ iter ];
				}
				
				if( !( eqpoints[ iter ] ) )
				{
					/* Error: We don't currently support any language variations. */
				}
				
				break;
			case -1:
				break;
			default:
				/* Error: unrecognized match value! */
		}
		
		???
		
		++iter;
	}
	
	if( glob->lang == -1 )
	{
		/* Error: No language was chosen. */
	}
	
	switch( glob->lang )
	{
		case common_entrance_func_CLANG:
		case common_entrance_func_CppLANG:
		case common_entrance_func_FORTRANLANG:
		case common_entrance_func_METACLANG:
		case common_entrance_func_OBJCLANG:
		case common_entrance_func_TEXT:
			/* One of these per case. We push the appropriate */
			/*  lookup table to the appropriate stack. */
			
	int lexparse1_pushsearchtable
	(
		genname_parrparr **keys,
		size_t *keys_used,
		
		genname_parr *pushable
	);
			
			break;
		default:
			/* Error: unrecognized match value! */
	}
	
	???
}


#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPOP_UINT( stk, dest,  caller, scratch ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )
#define STACKPUSH_UINT( stk, val,  caller, scratch ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), lexalike_ENDRETFRAME )



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
