#ifndef METAC_ERR_ERR_H
 #define METAC_ERR_ERR_H
	
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
	
	
	/* err/err.h */
	
	
	
		/* "limits" is included per spec by "stdint", but being descriptive */
		/*  like this is philosophically better. */
	#include <limits.h>
	#include <stdint.h>
	
	#if INTPTR_MAX >= INT_MAX
		#define ERR_KEYTYPE intptr_t
	#else
		#define ERR_KEYTYPE int
	#endif
	
	
	
	typedef struct msg_styleset msg_styleset;
	
	
	
	/*
			void err_interface( uintptr_t source, lib4_failure_result err, ... );
		has been "replaced" with
			void msg_interface( msg_styleset *source, ERR_KEYTYPE first_key, ... );
	*/
	void msg_interface( msg_styleset *source, ERR_KEYTYPE first_key, ... );
	
#endif
