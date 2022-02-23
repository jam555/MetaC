#ifndef METAC_ERR_ERR_H
 #define METAC_ERR_ERR_H
	
	
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
