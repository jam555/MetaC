#include "../headers.h"

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


#define BADNULL( funcname, ptr ) \
	STDMSG_BADNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, funcname, ( ptr ) )
#define BADNULL2( funcname, ptr1, ptr2 ) \
	STDMSG_BADNULL2_WRAPPER( &errs, funcname, ( ptr1 ), ( ptr2 ) )

#define MONADICFAILURE( funcname, calltext, err ) \
		STDMSG_MONADICFAILURE_WRAPPER( &errs, funcname, ( calltext ), ( err ) )
	#define NOTELINE() STDMSG_NOTELINE_WRAPPER( &errs )
	
	#define STRARG( str ) STDMSG_STRARG_WRAPPER( &errs, ( str ) )

#define FAILEDINTFUNC( calleestr, callername, val ) \
	STDMSG_FAILEDINTFUNC_WRAPPER( &errs, ( calleestr ), callername, ( val ) )

#define TRESPASSPATH( funcname, msgstr ) \
	STDMSG_TRESPASSPATH_WRAPPER( &errs, funcname, ( msgstr ) )

#define POPMACROARGS( destptr,  caller, scratch, endfunc ) \
	POP_MACROARGS( destptr,  &err, caller, scratch, endfunc )


#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )


#define RETFRAMEFUNC( stkp,  caller, scratch ) \
	RET_FRAMEFUNC( ( stkp ),  &errs, ( caller ), ( scratch ), stack_ENDRETFRAME )



LIB4_DEFINE_PASCALARRAY_STDDEFINE( tokhdptparpt_, tokhdptr_parr* );
typedef tokhdptparpt_pascalarray tokhdptparpt_parr;
tokhdptparpt_parr *macro_args = (tokhdptparpt_parr*)0;
size_t macro_args_used = 0;

int macroargs_init()
{
	if( macro_args || macro_args_used )
	{
		BADNULL2( macroargs_init, &macro_args, &macro_args_used );
		return( -2 );
	}
	
#define macroargs_init_SUCC( arr ) \
		macro_args = ( arr ); macro_args_used = 0;
#define macroargs_init_FAIL( err ) \
		MONADICFAILURE( macroargs_init, "tokhdptparpt_pascalarray_build", (err) ); \
		return( -3 );
	
	tokhdptparpt_pascalarray_result res =
		tokhdptparpt_pascalarray_build( 64 );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, macroargs_init_SUCC, macroargs_init_FAIL )
	
	return( 1 );
}
int macroargs_resize( int deltaMacros )
{
	int len = macro_args->len + deltaMacros;
	
	if( !macro_args )
	{
		BADNULL( macroargs_resize, &macro_args );
		return( -2 );
	}
	
	if( len < 0 )
	{
		BADNONULL( macroargs_resize, &len );
		return( -3 );
	}
	
#define macroargs_resize_SUCC( arr ) \
		macro_args = ( arr );
#define macroargs_resize_FAIL( err ) \
		MONADICFAILURE( macroargs_resize, "tokhdptparpt_pascalarray_rebuild", (err) ); \
		return( -4 );
	
	tokhdptparpt_pascalarray_result res =
		tokhdptparpt_pascalarray_rebuild( macro_args, len );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, macroargs_resize_SUCC, macroargs_resize_FAIL );
	
	if( macro_args_used > len )
	{
		macro_args_used = len;
	}
	
	return( 1 );
}
int macroargs_clear()
{
	if( !macro_args )
	{
		BADNULL( macroargs_clear, &macro_args );
		return( -2 );
	}
	
#define macroargs_resize_SUCC( arr ) ;
#define macroargs_resize_FAIL( err ) \
		MONADICFAILURE( macroargs_clear, "tokhdptparpt_pascalarray_destroy", (err) ); \
		return( -3 );
	
	lib4_result res = tokhdptparpt_pascalarray_destroy( macro_args );
	LIB4_RESULT_BODYMATCH( res, macroargs_resize_SUCC, macroargs_resize_FAIL );
	
	macro_args = (tokhdptparpt_parr*)0;
	macro_args_used = 0;
	
	return( 1 );
}

int macroargs_pushset( tokhdptr_parr *val )
{
	if( !val )
	{
		BADNULL( macroargs_pushset, &val );
		return( -1 );
	}
	if( !macro_args )
	{
		BADNULL( macroargs_pushset, &macro_args );
		return( -2 );
	}
	
	if( macro_args_used >= macro_args->len )
	{
		int res = macroargs_resize( macro_args->len );
		
		if( res < 0 )
		{
			BADNONULL( macroargs_pushset, &res );
			return( -3 );
		}
	}
	
	macro_args->body[ macro_args_used ] = val;
	++macro_args_used;
	
	return( 1 );
}
int macroargs_popset( tokhdptr_parr **dest )
{
	if( !macro_args || !macro_args_used )
	{
		BADNULL2( macroargs_popset, &macro_args, &macro_args_used );
		return( -2 );
	}
	
	--macro_args_used;
	
	if( dest )
	{
		*dest = macro_args->body[ macro_args_used ];
		
		return( 2 );
	}
	
		/* Not per-say an error, just a warning. */
	NOTELINE(); STRARG( "WARNING: macroargs_popset() exited from it's end, because *dest was null." );
	return( 1 );
}
int macroargs_peekset( size_t off,  tokhdptr_parr **dest )
{
	if( !macro_args || !macro_args_used )
	{
		BADNULL2( macroargs_peekset, &macro_args, &macro_args_used );
		return( -2 );
	}
	if( !dest || off >= macro_args_used )
	{
		TRESPASSPATH( macroargs_peekset, "ERROR: macroargs_peekset received invalid args." );
		return( -1 );
	}
	
	*dest = macro_args->body[ macro_args_used - ( off + 1 ) ];
	
	return( 1 );
}


	/*
			shufflecount shuffle:token*[shufflecount]
		--
			arglist*
	*/
int pack_arglist( stackpair *stkp )
{
	uintptr_t shufcount;
	token *tmp;
	tokenheadptr_pascalarray *a;
	
	if( !stkp )
	{
		BADNULL( pack_arglist, &stkp );
		return( -1 );
	}
	
	int res = pop_uintptr( &( stkp->data ),  &shufcount );
	if( !res )
	{
		FAILEDINTFUNC( "pop_uintptr", pack_arglist, res );
		return( -2 );
	}
	
#define pack_arglist_FAIL( err ) \
		MONADICFAILURE( pack_arglist, "tokenheadptr_pascalarray_build", (err) ); \
		return( -3 );
	tokenheadptr_pascalarray_result res = tokenheadptr_pascalarray_build( shufcount );
	LIB4_DEFINE_PASCALARRAY_RESULT_BODYMATCH( res, LIB4_OP_SETa, pack_arglist_FAIL );
	
	while( shufcount )
	{
		--shufcount;
		res = token_queue_shufflepop( &tmp );
		if( !res )
		{
			FAILEDINTFUNC( "token_queue_shufflepop", pack_arglist, res );
			return( -4 );
		}
		if( !tmp )
		{
			BADNULL( pack_arglist, &tmp );
			return( -5 );
		}
		a->body[ shufcount ] = &( tmp->header );
	}
	
	res = push_uintptr( &( stkp->data ),  (uintptr_t)a );
	if( !res )
	{
		FAILEDINTFUNC( "push_uintptr", pack_arglist, res );
		return( -6 );
	}
	
	return( 1 );
}
retframe vm_pop_macroarg( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  vm_pop_macroarg, macroargs_ENDRETFRAME );
	
	int res;
	POPMACROARGS( (tokhdptr_parr**)0,  vm_pop_macroarg, res, macroargs_ENDRETFRAME );
	
	RETFRAMEFUNC( stkp,  vm_pop_macroarg, res );
}



#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
