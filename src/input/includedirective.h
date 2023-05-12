#ifndef METAC_INPUT_INCLUDEDIRECTIVES_H
 #define METAC_INPUT_INCLUDEDIRECTIVES_H
	
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
	
	/* input/includedirective.h */
	
	#include "../headers.h"
	#include "../basics.h"
		/* For push_source(). */
	#include "source.h"
	
	
		/* ( token*directiveName --  ) */
		??? /* Note: this MIGHT need to provide a token* on the stack. I don't */
		/*  think it SHOULD, but it might. Make certain to review that. */
		/* Will ultimately result in a call to token_queue_seekFreshline(), */
		/*  to clear the remainder of the consumed line. */
	retframe metaC_stdinclude( stackpair *stkp, void *v );
	
#endif
