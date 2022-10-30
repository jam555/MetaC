#ifndef METAC_INPUT_CHAR2NUM_H
 #define METAC_INPUT_CHAR2NUM_H
	
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
	
	#include "stdmonads.h"
	
	lib4_intresult bin2num( char c );
	lib4_intresult oct2num( char c );
	lib4_intresult dec2num( char c );
	lib4_intresult hexa2num( char c );
		/* Roughly the system of the ILLIAC 1, the first private */
		/*  (particularly college) owned computer. Yes, it seems odd, but it */
		/*  was very early, and the letters may have started as */
		/*  abbreviations. */
		/* Probably worth noting that this isn't actually used: I just put */
		/*  it in because it came to mind (blame Dave's Garage on Youtube). */
	lib4_intresult sexa2num( char c );
	
#endif
