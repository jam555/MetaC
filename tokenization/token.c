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

#include "token.h"



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
#define BADNONULL( funcname, ptr ) \
	STDMSG_BADNONNULL_WRAPPER( &errs, funcname, ( ptr ) )



#define STACKCHECK( stack,  caller, endfunc ) \
	STACK_CHECK( ( stack ),  &err, ( caller ), ( endfunc ) )

#define STACKPOP_UINT( stk, dest,  caller, scratch, endfunc ) \
	STACK_POP_UINT( ( stk ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPUSH_UINT( stk, val,  caller, scratch, endfunc ) \
	STACK_PUSH_UINT( ( stk ), ( val ),  &errs, ( caller ), ( scratch ), ( endfunc ) )
#define STACKPEEK_UINT( stk, offset, dest,  caller, scratch, endfunc ) \
	STACK_PEEK_UINT( ( stk ), ( offset ), ( dest ),  &errs, ( caller ), ( scratch ), ( endfunc ) )

#define CALLFRAMEFUNC( rethand, retval, callhand, callval,  caller ) \
	CALL_FRAMEFUNC( stkp, rethand, retval, callhand, callval,  &errs, ( caller ), res, stack_ENDRETFRAME )
#define RETFRAMEFUNC( caller ) \
	RET_FRAMEFUNC( stkp,  &errs, ( caller ), res, stack_ENDRETFRAME )




int is_stdtoken( token_head *th )
{
	if( !th )
	{
		return( -1 );
	}
	
	switch( th->toktype )
	{
		case TOKTYPE_INVALID:
		case TOKTYPE_PARSEBREAK:
		case TOKTYPE_NUMBER_UINT:
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
		case TOKTYPE_TOKENGROUP_STRMERGE:
		case TOKTYPE_TOKENGROUP_COMNTMERGE:
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
		case TOKTYPE_TOKENGROUP_WHITESPACE:
		case TOKTYPE_TOKENGROUP_DELIMITED:
		case TOKTYPE_TOKENGROUP_MACROLINK:
		case TOKTYPE_TOKENGROUP_MACROTOKEN:
		case TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION:
		case TOKTYPE_TOKENGROUP_MACRORUN:
		case TOKTYPE_TOKENGROUP_MACRODIRECTIVE:
		case TOKTYPE_TOKENGROUP_MACROCALL:
		case ???:
			return( 0 );
		
		default:
			return( 1 );
	}
}
int is_delimited( token_head *th )
{
	if( !th )
	{
		return( -1 );
	}
	
	return( !!( th->is_delimited ) );
}
int was_freshline( token_head *th )
{
	if( !th )
	{
		return( -1 );
	}
	
		/* This is a bad test, but it'll hold for now. */
	return( !( th->column ) );
}
	/* ( dest-token_head* src-token_head* -- dest-token_head* src-token_head* ) */
retframe vm_tokenhead_setflags( stackpair *stkp, void *v_ )
{
	int scratch;
	
	STACKCHECK( stkp,  vm_tokenhead_setflags );
	
	uintptr_t tmp;
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  vm_tokenhead_setflags, scratch );
	token_head *dest, *src = (token_head*)tmp;
	if( !src )
	{
		???
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), tmp,  vm_tokenhead_setflags, scratch );
	dest = (token_head*)tmp;
	if( !dest )
	{
		???
	}
	
	dest->is_delimited = src->is_delimited;
	
	RETFRAMEFUNC( vm_tokenhead_setflags );
}
	/* ( dest-token_head* src-token_head* -- dest-token_head* src-token_head* ) */
retframe vm_tokenhead_setsource( stackpair *stkp, void *v_ )
{
	int scratch;
	
	STACKCHECK( stkp,  vm_tokenhead_setsource );
	
	uintptr_t tmp;
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  vm_tokenhead_setsource, scratch );
	token_head *dest, *src = (token_head*)tmp;
	if( !src )
	{
		???
	}
	if( !( src->src ) )
	{
		???
	}
	
	STACKPEEK_UINT( &( stkp->data ), sizeof( uintptr_t ), tmp,  vm_tokenhead_setsource, scratch );
	dest = (token_head*)tmp;
	if( !dest )
	{
		???
	}
	
	++( src->src->refs );
	dest->src = src->src;
	dest->line = src->line;
	dest->column = src->column;
	
	RETFRAMEFUNC( vm_tokenhead_setsource );
}
deeptoktype_result get_deeptoktype( token_head *th )
{
	if( !th )
	{
		DEEPTOKTYPE_RESULT_RETURNFAILURE( TOKTYPE_INVALID );
	}
	
	switch( th->toktype )
	{
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
			DEEPTOKTYPE_RESULT_RETURNSUCCESS(
				(deep_toktype){ th->toktype, ( (tokengroup*)th )->subtype }
			);
		case TOKTYPE_TOKENGROUP_STRMERGE:
		case TOKTYPE_TOKENGROUP_COMNTMERGE:
		case TOKTYPE_TOKENGROUP_EQUIVMERGE:
		case TOKTYPE_TOKENGROUP_DELIMITED:
		case TOKTYPE_TOKENGROUP_WHITESPACE:
				??? /* Does whitespace belong here, or with samemerge? */
			DEEPTOKTYPE_RESULT_RETURNSUCCESS(
				(deep_toktype){ TOKTYPE_TOKENBRANCH, ( (tokenbranch*)th )->subtype }
			);
		case TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION:
			if( ( (macro_token*)th )->tok )
			{
				DEEPTOKTYPE_RESULT_RETURNFAILURE(
					TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION
				);
			}
			DEEPTOKTYPE_RESULT_RETURNSUCCESS(
				(deep_toktype){ th->toktype, ( (macro_token*)th )->tok->toktype }
			);
		default:
			DEEPTOKTYPE_RESULT_RETURNSUCCESS(
				(deep_toktype){ th->toktype, TOKTYPE_INVALID }
			);
	}
	
	DEEPTOKTYPE_RESULT_RETURNFAILURE( TOKTYPE_SPACE );
}
int simplify_toktype( token_head *tok,  uintptr_t *dest )
{
#define simplify_toktype_BADRET( val ) return( -( val ) );
	deep_toktype a;
	
	deeptoktype_result res = get_deeptoktype( tok );
	DEEPTOKTYPE_RESULT_BODYMATCH( res, LIB4_OP_SETa, simplify_toktype_BADRET );
	
	switch( a.shallow_toktype )
	{
		case TOKTYPE_TOKENGROUP_SAMEMERGE:
		case TOKTYPE_TOKENBRANCH:
		case TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION:
			*dest = a.virtual_toktype;
			break;
		default:
			*dest = a.shallow_toktype;
			break;
	}
	
	return( 1 );
}
	/* ( token* -- token* char_parr* ) */
	/* v_ must point to a retframe{} to handle "unrecognized token type" */
	/*  errors. The stack will be ( token* &token2char_parr ). */
retframe token2char_parr( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  token2char_parr );
	
	if( !( (retframe*)v_ )->handler )
	{
		TRESPASSPATH( token2char_parr, "ERROR: token2char_parr() wasn't given an error reporter!" );
			NOTELINE();
			DATAPTR( errmark );
		stack_ENDRETFRAME();
	}
	
	int scratch, res;
	uintptr_t tmp;
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  token2char_parr, scratch );
	token *tok = (token*)tmp;
	
		/* -1: th was null; otherwise 0 for "fancy token" or 1 for standard token */
	scratch = is_stdtoken( &( tok->header ) );
	switch( scratch )
	{
		case 0:
			while( scratch == 0 )
			{
				switch( tok->header->toktype )
				{
					case TOKTYPE_INVALID:
					case TOKTYPE_PARSEBREAK:
					case TOKTYPE_NUMBER_UINT:
					case TOKTYPE_TOKENGROUP_MACROLINK:
					case TOKTYPE_TOKENGROUP_MACRORUN:
					case TOKTYPE_TOKENGROUP_MACRODIRECTIVE:
					case TOKTYPE_TOKENGROUP_MACROCALL:
							/* Error: none of these are stringifiable! */
						return( 0 );
						
					case TOKTYPE_TOKENGROUP_SAMEMERGE:
						if( ( (tokengroup*)&( tok->header ) )->used != 1 )
						{
							/* Error! Invalid token count! */
							
						} else {
							
							if( !( ( (tokengroup*)&( tok->header ) )->arr ) )
							{
								/* Error! Missing pascal array! */
								???
							}
							
							tok = ( (tokengroup*)&( tok->header ) )->arr->body[ 0 ];
						}
						
						break;
						
					case TOKTYPE_TOKENGROUP_STRMERGE:
					case TOKTYPE_TOKENGROUP_COMNTMERGE:
					case TOKTYPE_TOKENGROUP_EQUIVMERGE:
					case TOKTYPE_TOKENGROUP_WHITESPACE:
					case TOKTYPE_TOKENGROUP_DELIMITED:
						{
							int acc =
								( ( (tokenbranch*)&( tok->header ) )->lead ? 1 + 8 : 0 ) +
								( ( (tokenbranch*)&( tok->header ) )->body ? 1 + 16 : 0 ) +
								( ( (tokenbranch*)&( tok->header ) )->tail ? 1 + 32 : 0 );
							
							if( acc & 3 != 1 )
							{
								/* Error! Bad branch count! */
								???
							}
							
							switch( acc & 56 )
							{
								case 8:
									tok = ( (tokenbranch*)&( tok->header ) )->lead;
									break;
									
								case 16:
									tok = ( (tokenbranch*)&( tok->header ) )->body;
									break;
									
								case 32:
									tok = ( (tokenbranch*)&( tok->header ) )->tail;
									break;
									
								default:
									/* Error! */
									???
							}
						}
						break;
						
					case TOKTYPE_TOKENGROUP_MACROTOKEN:
					case TOKTYPE_TOKENGROUP_MACROTOKEN_INDIRECTION:
						tok = ( (macro_token*)&( tok->header ) )->tok;
						break;
						
					default:
						break;
				}
				
				scratch = is_stdtoken( &( tok->header ) );
			}
			if( scratch != 1 )
			{
				/* Error! Never settled to a stringifiable token! */
			}
			
				/* Error, call the handler. */
			STACKPUSH_UINT( &( stkp->data ), (uintptr_t)&token2char_parr,  token2char_parr, scratch );
			return( *( (retframe*)v_ ) );
			
		case 1:
				/* The one and only 'good' path. */
			break;
			
		case -1:
		default:
				/* Error, full break! */
			FAILEDINTFUNC( "is_stdtoken()", token2char_parr, scratch );
				NOTELINE();
				STRARG( "is_stdtoken() was somehow handed a null pointer." );
			stack_ENDRETFRAME();
	}
	
	char_parr *cparr = char_pascalarray_build( strlen( tok->text ) + 1 );
	if( !cparr )
	{
		BADNULL( token2char_parr, &cparr );
			NOTELINE();
			STRARG( "char_pascalarray_build() failed." );
		stack_ENDRETFRAME();
	}
	
	strcpy( tok->text, cparr->body );
	
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)cparr,  token2char_parr, scratch );
	
	RETFRAMEFUNC( token2char_parr );
}
	/* ( string-token* -- token* char_parr* ) */
	/* v_ must point to a retframe{} to handle "unrecognized token type" */
	/*  errors. The stack will be ( token* &stringtoken2char_parr ). */
retframe stringtoken2char_parr( stackpair *stkp, void *v_ )
{
	STACKCHECK2( stkp, v_,  stringtoken2char_parr );
	
	if( !( (retframe*)v_ )->handler )
	{
		TRESPASSPATH( token2char_parr, "ERROR: stringtoken2char_parr() wasn't given an error reporter!" );
			NOTELINE();
			DATAPTR( errmark );
		stack_ENDRETFRAME();
	}
	
	int scratch, res;
	uintptr_t tmp;
	
	STACKPEEK_UINT( &( stkp->data ), 0, tmp,  stringtoken2char_parr, scratch )
	token *tok = (token*)tmp;
	if
	(
		tok->toktype != TOKTYPE_SQSTR &&
		tok->toktype != TOKTYPE_DQSTR
	)
	{
			/* Error! Wrong type! */
		???
		
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)&stringtoken2char_parr,  stringtoken2char_parr, scratch );
		return( *( (retframe*)v_ ) );
	}
	
		??? /* Verify this section to make sure it works right. */
	char_parr *cparr = char_pascalarray_build( tok->text->len - 1 );
	if( !cparr )
	{
		BADNULL( token2char_parr, &cparr );
			NOTELINE();
			STRARG( "char_pascalarray_build() failed." );
		stack_ENDRETFRAME();
	}
	memcpy( (void*)cparr->body, (const void*)( tok->text + 1 ), tok->text->len - 2 );
		/* Add the terminating null. */
	cparr->body[ tok->text->len - 2 ] = '\0';
	
	STACKPUSH_UINT( &( stkp->data ), (uintptr_t)cparr,  stringtoken2char_parr, scratch );
	
	RETFRAMEFUNC( stringtoken2char_parr );
}



retframe complexlex_dealloctoken = (retframe){ &smart_dealloc_token, (void*)0 };

retframe set_dealloctoken( retframe dealc_ )
{
	retframe ret = (retframe){ 0, 0 };
	
	if( dealc_.handler )
	{
		ret = complexlex_dealloctoken;
		complexlex_dealloctoken = dealc_;
		
	} else {
		
		BADNULL( set_dealloctoken, &( dealc_.handler ) );
	}
	
	return( ret );
}
retframe invoke_dealloctoken( stackpair *stkp, void *v )
{
		??? /* Isn't this... exactly the opposite of what should happen? */
	if( complexlex_dealloctoken.handler )
	{
		BADNONULL( invoke_dealloctoken, &( complexlex_dealloctoken.handler ) );
		return( (retframe){ (framefunc)&end_run, (void*)0 } );
	}
	
	return( complexlex_dealloctoken );
}
retframe smart_dealloc_token( stackpair *stkp, void *v )
{
	uintptr_t a;
	token_head *th = (token_head*)0;
		/* One way or another, this SHOULDN'T be the value actually getting */
		/*  returned. But just in case... */
	retframe ret = (retframe){ &end_run, (void*)0 };
	int res;
	
	STACKCHECK( stkp,  smart_dealloc_token, macroargs_ENDRETFRAME );
	
	STACKPEEK_UINT( &( stk->data ), 0, &a,  smart_dealloc_token, res, macroargs_ENDRETFRAME );
	th = (token_head*)a;
	
	if( th->toktype == TOKTYPE_TOKENGROUP_SAMEMERGE )
	{
		tokengroup *tgrp = (tokengroup*)th;
		
		ret =
			dealloc_tokengroup
			(
				stkp, v,
				
				tgrp
			);
		if( !ret.handler && !ret.data )
		{
			BADNULL2( smart_dealloc_token, &( ret.handler ), &( ret.data ) );
			return( (retframe){ (framefunc)&end_run, (void*)0 } );
		}
		
	} else if( th->toktype == TOKTYPE_TOKENGROUP_EQUIVMERGE )
	{
		tokenbranch *tb = (tokenbranch*)th;
		
		ret = dealloc_tokenbranch( stkp, v,  tb );
		if( !ret.handler && !ret.data )
		{
			BADNULL2( smart_dealloc_token, &( ret.handler ), &( ret.data ) );
			return( (retframe){ (framefunc)&end_run, (void*)0 } );
		}
		
	} else {
		
		/* Wasn't either of the two "complexly structured" categories of */
		/*  tokens, so that means it's one of the simple ones instead. Time */
		/*  to outsource appropriately, with a hardwired return. */
		
		return( (retframe){ &dealloc_token, (void*)0 } );
	}
	
	
	if( ret.handler )
	{
		/* We're dealing with a complexly structured token (it has child */
		/*  tokens), and we haven't finished dealing with the children, so */
		/*  we'll queue up our "standard" deallocator (which is this */
		/*  function by default) as a return route, and then call whichever */
		/*  retframe was returned to us (which will probably ALSO be this */
		/*  function...).  */
		
		/* ??? */
		CALLFRAMEFUNC(
			complexlex_dealloctoken.handler,
			complexlex_dealloctoken.data,
			
			ret.handler, ret.data,
			
			smart_dealloc_token
		);
		
	} else {
		
			/* The children have been previously deallocated, and the */
			/*  complex type was deallocated by the helper function that we */
			/*  got the current value of "ret" from, so it's time to return */
			/*  to OUR caller (or at least the "designated return route")! */
		RETFRAMEFUNC( smart_dealloc_token );
	}
}
	/* ( token*[len] len -- ) */
retframe bulk_dealloc_token( stackpair *stkp, void *v )
{
	STACKCHECK( stkp,  smart_dealloc_token, macroargs_ENDRETFRAME );
	
	int res;
	uintptr_t len, tmp;
	
	STACKPOP_UINT( &( stk->data ), len,  bulk_dealloc_token, res, macroargs_ENDRETFRAME );
	if( !len )
	{
			/* Done. */
		RETFRAMEFUNC( bulk_dealloc_token );
	}
	
	--len;
	
	if( len )
	{
		STACKPOP_UINT( &( stk->data ), tmp,  bulk_dealloc_token, res, macroargs_ENDRETFRAME );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)len,  token2char_parr, res );
		STACKPUSH_UINT( &( stkp->data ), (uintptr_t)tmp,  token2char_parr, res );
		
			/* ( token*[len] len token* -- token*[len] len ) */
		CALLFRAMEFUNC( bulk_dealloc_token, (void*)0, invoke_dealloctoken, (void*)0,  caller );
		
	} else {
		
			/* ( token* --  ) */
		return( (retframe){ &invoke_dealloctoken, (void*)0 } );
	}
}



/* If the result is the failure case, then one or both arguments were provided as null. */
/* If the result is the success case: */
	/* Negatives: a < b */
	/* Positives: a > b */
	/* 0: Match (ignores stuff like line and source file). */
	/* 1: Ended on toktype. */
	/* 2: Ended on length. */
lib4_result equal_tokenheads( token_head *a, token_head *b )
{
	if( a && b )
	{
		if( a->toktype < b->toktype )
		{
			LIB4_INTRESULT_RETURNSUCCESS( -1 );
		}
		if( a->toktype > b->toktype )
		{
			LIB4_INTRESULT_RETURNSUCCESS( 1 );
		}
		
		if( a->length < b->length )
		{
			LIB4_INTRESULT_RETURNSUCCESS( -2 );
		}
		if( a->length > b->length )
		{
			LIB4_INTRESULT_RETURNSUCCESS( 2 );
		}
		
		LIB4_INTRESULT_RETURNSUCCESS( 0 );
	}
	
	LIB4_INTRESULT_RETURNFAILURE( (lib4_failure_uipresult){ LIB4_RESULT_FAILURE_DOMAIN } );
}
/* This inherits the results of equal_tokenheads(), and adds... */
	/* 3+: Mismatch at character[ abs( result ) - 3 ]. */
lib4_result equal_tokens( token_head *a_, token_head *b_ )
{
	if( a_ && b_ )
	{
		lib4_result res = equal_tokenheads( a_, b_ );
		int c;
		LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETc, LIB4_INTRESULT_RETURNFAILURE );
		if( c != 0 )
		{
			LIB4_INTRESULT_RETURNSUCCESS( c );
		}
		
		token *a = (token*)a_, *b = (token*)b_;
		size_t iter = 0;
		
		while( a_->length < iter )
		{
			if( a->text[ iter ] < b->text[ iter ] )
			{
				LIB4_INTRESULT_RETURNSUCCESS( -( 3 + iter ) );
			}
			if( a->text[ iter ] > b->text[ iter ] )
			{
				LIB4_INTRESULT_RETURNSUCCESS( ( 3 + iter ) );
			}
			
			++iter;
		}
		
		LIB4_INTRESULT_RETURNSUCCESS( 0 );
	}
	
	LIB4_INTRESULT_RETURNFAILURE( (lib4_failure_uipresult){ LIB4_RESULT_FAILURE_DOMAIN } );
}

int isin_tokhdptr_parr( token *tok, tokhdptr_par *parr,  size_t *loc )
{
	if( tok && parr )
	{
		token_head *th;
		size_t iter = 0;
		lib4_result res;
		int a = 1, b = 0;
		
		while( parr->len > iter )
		{
			th = parr->body[ iter ];
			
			res = equal_tokens( &( tok->header ), th );
#define isin_tokhdptr_parr_ONFAIL( err ) b = 1
			LIB4_MONAD_EITHER_BODYMATCH( res, LIB4_OP_SETa, isin_tokhdptr_parr_ONFAIL );
			if( b )
			{
				/* Error! */
				
				if( loc )
				{
					*loc = iter;
				}
				
				return( -2 );
			}
			if( a == 0 )
			{
				if( loc )
				{
					*loc = iter;
				}
				
				return( 1 );
			}
			
			++iter;
		}
		
		return( 0 );
	}
	
	return( -1 );
}




#if defined( __cplusplus ) && __cplusplus >= 199711L
	namespace
	{
		msg_styleset errs = { 0 };
	};
#elif defined( __STDC__ ) && __STDC_VERSION__ >= 199901L
	static msg_styleset errs = (msg_styleset){ 0, 0 };
#endif
