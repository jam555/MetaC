#include "char2num.h"


lib4_intresult bin2num( char c )
{
	if( c == '0' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( 0 );
		
	} else if( c == '1' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( 1 );
		
	} else {
		
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
	}
}
lib4_intresult oct2num( char c )
{
	if( c < '0' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else if( c > '7' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else {
		
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
	}
}
lib4_intresult dec2num( char c )
{
	if( c < '0' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else if( c > '9' )
	{
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
		
	} else {
		
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
	}
}
lib4_intresult hexa2num( char c )
{
	if( c >= '0' && c <= '9' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
		
	} else if( c >= 'a' && c <= 'f' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - 'a' ) );
		
	} else if( c >= 'A' && c <= 'F' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - 'A' ) );
		
	} else {
		
		LIB4_INTRESULT_RETURNFAILURE( (int)c );
	}
}
	/* Roughly the system of the ILLIAC 1, the first private (particularly */
	/*  college) owned computer. Yes, it seems odd, but it was very early, */
	/*  and the letters may have started as abbreviations. */
	/* Probably worth noting that this isn't actually used: I just put it in */
	/*  because it came to mind (blame Dave's Garage on Youtube). */
lib4_intresult sexa2num( char c )
{
	if( c >= '0' && c <= '9' )
	{
		LIB4_INTRESULT_RETURNSUCCESS( ( c - '0' ) );
		
	} else {
		
		switch( c )
		{
			case 'k':
			case 'K':
				LIB4_INTRESULT_RETURNSUCCESS( 10 );
			case 's':
			case 'S':
				LIB4_INTRESULT_RETURNSUCCESS( 11 );
			case 'n':
			case 'N':
				LIB4_INTRESULT_RETURNSUCCESS( 12 );
			case 'j':
			case 'J':
				LIB4_INTRESULT_RETURNSUCCESS( 13 );
			case 'f':
			case 'F':
				LIB4_INTRESULT_RETURNSUCCESS( 14 );
			case 'l':
			case 'L':
				LIB4_INTRESULT_RETURNSUCCESS( 15 );
				
			default:
				LIB4_INTRESULT_RETURNFAILURE( (int)c );
		}
	}
}
