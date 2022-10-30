#ifndef METAC_METACPREPROC_BASICS_H
 #define METAC_METACPREPROC_BASICS_H
	
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
	
	/* metaCpreproc/basics.h */
	
	#include "../headers.h"
	
	#include "err/inner_err.h"
	
	
	retframe preproc_search1( stackpair *stkp, void *v );
	retframe preproc_search2( stackpair *stkp, void *table_ );
	
	retframe gentyped_search( stackpair *stkp, gentyped_parr *table, retframe (*tablehand)( stackpair*, void* ) );

#endif
