#ifndef METAC_INPUT_CHAR2NUM_H
 #define METAC_INPUT_CHAR2NUM_H
	
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
