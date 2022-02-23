#ifndef METAC_ERR_INNER_ERR_H
 #define METAC_ERR_INNER_ERR_H
	
	
	/* err/inner_err.h */
	
	
	
	typedef struct msg_style msg_style;
	struct msg_style
	{
		char *layout;
			/* What was recep() for? */
		void (*recep)( msg_styleset*, int,  );
	};
	typedef struct
	{
		void *destdata;
			/* This function is intentionally based around fputc(). */
		int (*destfunc)( int, void* );
	} char_receiver;
		/* Note that these functions are to be invoked via the standard */
		/*  printf() "pointer" specifier, IN CONJUNCTION WITH the capital */
		/*  'L' "long double" data-length specifier: this is both because */
		/*  the usual 'l'/'ll'/'h'/'hh' length specifiers logically should */
		/*  just differentiate between e.g. near & far pointers (where */
		/*  relevant), and ALSO because all of the floats have COMPLEX */
		/*  internal data representations with two (and maybe more, though */
		/*  probably not) numerical components, which forms a decent-enough */
		/*  analogy for these function pointers being used in conjunction */
		/*  with a data pointer, thus forming a complex argument. */
		/* The void* is (presumably a pointer to) the data that needs to be */
		/*  printed: this pointer is to be found BEFORE the pointer to the */
		/*  custom printing function. */
		/* The first size_t is the "width" from the printf() specifier, */
		/*  while the second is the "precision" value. These should be */
		/*  either ignored, or interpreted in line with their interpretation */
		/*  for standard printf() behavior. */
		/* char_receiver{} is a set of function & data provided by the */
		/*  caller, and in conjunction with the int return is the only */
		/*  advisable means of returning information. */
		/* The intmax_t pointer is optional, and returns the number of */
		/*  printed characters. If it is non-null, then the function is */
		/*  REQUIRED to populate it via set (not addition). */
	int (*customprint_signature)( void*, ssize_t, ssize_t,  char_receiver,  intmax* );
	
	
	typedef struct msg_piece
	{
		union
		{
			msg_styleset *set;
			msg_style *style;
		} data;
		
		char is_set;
		
	} msg_piece;
	
	#define BUILD_MSGPIECE_STYLE( styledef ) ( (msg_piece){ { .style = ( styledef ) }, 0 } )
	#define BUILD_MSGPIECE_SET( setdef ) ( (msg_piece){ ( .set = ( setdef ) ), 1 } )
	
	
	LIB4_MONAD_EITHER_BUILDTYPE_DEFINITION(
		msg_piece_result,
		
		msg_piece,
		lib4_failure_uipresult
	);
	
	#define MSG_PIECE_RESULT_BUILDSUCCESS( val ) \
		LIB4_MONAD_EITHER_BUILDLEFT( \
			msg_piece_result, msg_piece, (val) \
		)
	#define MSG_PIECE_RESULT_BUILDFAILURE( val ) \
		LIB4_MONAD_EITHER_BUILDRIGHT( \
			msg_piece_result, lib4_failure_uipresult, (val) \
		)
	
		/* The *BODY* version takes statements, *EXPR* takes expressions. */
		/*  The matches must be function-style, though function macros are */
		/*  allowed. */
	#define MSG_PIECE_RESULT_BODYMATCH( var, matcha, matchb ) \
		LIB4_MONAD_EITHER_BODYMATCH( var, matcha, matchb )
	#define MSG_PIECE_RESULT_EXPRMATCH( var, matcha, matchb ) \
		LIB4_MONAD_EITHER_EXPRMATCH( var, matcha, matchb )
	
		/* Convenience wrappers. See monads.h for more details. */
	#define MSG_PIECE_RESULT_RETURNSUCCESS( val ) \
		LIB4_MONAD_EITHER_RETURNLEFT( \
			msg_piece_result, msg_piece, val )
	#define MSG_PIECE_RESULT_RETURNFAILURE( val ) \
		LIB4_MONAD_EITHER_RETURNRIGHT( \
			msg_piece_result, lib4_failure_uipresult, val )
	
	
	struct msg_styleset
	{
		msg_piece *members;
		size_t count;
	};
	
	
	
	msg_piece_result styleset_getelem( msg_styleset *set, int elem );
	
#endif
