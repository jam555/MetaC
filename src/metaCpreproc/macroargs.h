#ifndef METAC_METACPREPROC_MACROARGS_H
 #define METAC_METACPREPROC_MACROARGS_H
	
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
	
	/* metaCpreproc/macroargs.h */
	
	#include "../headers.h"
	
	#include "err/inner_err.h"
	
	
	/* These macroargs_*() functions, plus some stuff in the .c file, define */
	/*  an entire additional stack system. It likely belongs in it's own */
	/*  file pair. */
	
	int macroargs_init();
	int macroargs_resize( int deltaMacros );
	int macroargs_clear();
	
	int macroargs_pushset( tokhdptr_parr *val );
	int macroargs_popset( tokhdptr_parr **dest );
	int macroargs_peekset( size_t off,  tokhdptr_parr **dest );
	
	#define macroargs_ENDRETFRAME() return( (retframe){ &end_run, (void*)0 } )
	
		/* Old version: PEEK_MACROARGS( offset, var,  errfunc, err1, err2,  ... ) */
	#define PEEK_MACROARGS( offset, dest,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = macroargs_peekset( ( offset ),  &( dest ) ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "macroargs_peekset", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( dest ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), &( dest ) ); \
				( endfunc )(); } }
		/* Old version: POP_MACROARGS( destptr,  errfunc, err1, err2,  ... ) */
	#define POP_MACROARGS( destptr,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = macroargs_popset( destptr ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "macroargs_popset", ( caller ), ( scratch ) ); \
				( endfunc )(); } \
			if( !( *( destptr ) ) ) { \
				STDMSG_BADNULL_WRAPPER( ( stylesetptr ), ( caller ), ( destptr ) ); \
				( endfunc )(); } }
		/* Old version: PUSH_MACROARGS( val,  errfunc, err,  ... ) */
	#define PUSH_MACROARGS( val,  stylesetptr, caller, scratch, endfunc ) \
		if( 1 ) { \
			( scratch ) = macroargs_pushset( val ); \
			if( !( scratch ) ) { \
				STDMSG_FAILEDINTFUNC_WRAPPER( ( stylesetptr ), "macroargs_pushset", ( caller ), ( scratch ) ); \
				( endfunc )(); } }
	
	
		/*
				shufflecount shuffle:token*[shufflecount]
			--
				arglist*
		*/
	int pack_arglist( stackpair *stkp );
	retframe vm_pop_macroarg( stackpair *stkp, void *v );

#endif
