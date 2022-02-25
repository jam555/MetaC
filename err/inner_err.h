#ifndef METAC_ERR_INNER_ERR_H
 #define METAC_ERR_INNER_ERR_H
	
	
	/* err/inner_err.h */
	
	
	
	typedef struct msg_style msg_style;
	struct msg_style
	{
		char *layout;
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
	
	
	
	#define STDMSG_BADNULL_WRAPPER( stylesetptr, funcname, objaddr ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_BADNULL ), \
			(char*)( __FILE__ ), (uintmax_t)&( funcname ), (unsigned)( __LINE__ ), \
			(uintmax_t)( objaddr ) \
		)
	#define STDMSG_BADNONNULL_WRAPPER( stylesetptr, funcname, objaddr ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_BADNONNULL ), \
			(char*)( __FILE__ ), (uintmax_t)&( funcname ), (unsigned)( __LINE__ ), \
			(uintmax_t)( objaddr ) \
		)
	
	#define STDMSG_BADNULL2_WRAPPER( stylesetptr, funcname, obj1addr, obj2addr ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_BADNULL2 ), \
			(char*)( __FILE__ ), (uintmax_t)&( funcname ), (unsigned)( __LINE__ ), \
			(uintmax_t)( obj1addr ), (uintmax_t)( obj2addr ) \
		)
	#define STDMSG_BADNONNULL2_WRAPPER( stylesetptr, funcname, obj1addr, obj2addr ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_BADNONNULL2 ), \
			(char*)( __FILE__ ), (uintmax_t)&( funcname ), (unsigned)( __LINE__ ), \
			(uintmax_t)( obj1addr ), (uintmax_t)( obj2addr ) \
		)
	
	#define STDMSG_MONADICFAILURE_WRAPPER( stylesetptr, funcname, failmsg, failval ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( MONADICFAILURE ), \
			(char*)( __FILE__ ), (uintmax_t)&( funcname ), (unsigned)( __LINE__ ), \
			(char*)( failmsg ), (intmax_t)( failval ) \
		)
		#define STDMSG_NOTELINE_WRAPPER( stylesetptr ) \
			msg_interface( ( stylesetptr ), LIB4_RETURN_1ST( STDMSG_NOTELINE ) )
		#define STDMSG_NOTESPACE_WRAPPER( stylesetptr ) \
			msg_interface( ( stylesetptr ), LIB4_RETURN_1ST( STDMSG_NOTESPACE ) )
		#define STDMSG_SIGNEDARG_WRAPPER( stylesetptr, val ) \
			msg_interface( \
				( stylesetptr ), LIB4_RETURN_1ST( STDMSG_SIGNEDARG ), \
				(intmax_t)( val ) \
			)
		#define STDMSG_DECARG_WRAPPER( stylesetptr, val ) \
			msg_interface( \
				( stylesetptr ), LIB4_RETURN_1ST( STDMSG_DECARG ), \
				(uintmax_t)( val ) \
			)
		#define STDMSG_HEXARG_WRAPPER( stylesetptr, val ) \
			msg_interface( \
				( stylesetptr ), LIB4_RETURN_1ST( STDMSG_HEXARG ), \
				(uintmax_t)( val ) \
			)
		#define STDMSG_LDOUBLEARG_WRAPPER( stylesetptr, val ) \
			msg_interface( \
				( stylesetptr ), LIB4_RETURN_1ST( STDMSG_LDOUBLEARG ), \
				(long double)( val ) \
			)
		#define STDMSG_CHARARG_WRAPPER( stylesetptr, val ) \
			msg_interface( \
				( stylesetptr ), LIB4_RETURN_1ST( STDMSG_CHARARG ), \
				(int)( val ) \
			)
		#define STDMSG_STRARG_WRAPPER( stylesetptr, val ) \
			msg_interface( \
				( stylesetptr ), LIB4_RETURN_1ST( STDMSG_STRARG ), \
				(char*)( val ) \
			)
		#define STDMSG_DATAPTRARG_WRAPPER( stylesetptr, val ) \
			msg_interface( \
				( stylesetptr ), LIB4_RETURN_1ST( STDMSG_DATAPTRARG ), \
				(void*)( val ) \
			)
	
	#define STDMSG_FAILEDINTFUNC_WRAPPER( stylesetptr, calleestr, callername, val ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_FAILEDINTFUNC ), \
			(char*)( calleestr ), \
			(char*)( __FILE__ ), (uintmax_t)&( callername ), (unsigned)( __LINE__ ), \
			(intmax_t)( val ) \
		)
	#define STDMSG_FAILEDUINTFUNC_WRAPPER( stylesetptr, calleestr, callername, val ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_FAILEDUINTFUNC ), \
			(char*)( calleestr ), \
			(char*)( __FILE__ ), (uintmax_t)&( callername ), (unsigned)( __LINE__ ), \
			(uintmax_t)( val ) \
		)
	#define STDMSG_FAILEDPTRFUNC_WRAPPER( stylesetptr, calleestr, callername, val ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_FAILEDPTRFUNC ), \
			(char*)( calleestr ), \
			(char*)( __FILE__ ), (uintmax_t)&( callername ), (unsigned)( __LINE__ ), \
			(uintmax_t)( val ) \
		)
	
	#define STDMSG_TRESPASSPATH_WRAPPER( stylesetptr, funcname, msgstr ) \
		msg_interface( \
			( stylesetptr ), LIB4_RETURN_1ST( STDMSG_TRESPASSPATH ), \
			(char*)( __FILE__ ), (uintmax_t)&( callername ), (unsigned)( __LINE__ ), \
			(char*)( msgstr ) \
		)
	
	
		/* If you want to use these, then the trivial way is to include */
		/*  lib4's "macrotime/arraccess.h", and use the LIB4_RETURN_1ST() */
		/*  macro to extract the index number. Then you just use it as the */
		/*  second argument to msg_interface(), and follow it with the */
		/*  relevant arguments. */
	#define STDMSG_BADNULL 0, "\nError in %s:%jx(), file line %x : %jx == null.", dummyarg
	#define STDMSG_BADNONNULL -1, "\nError in %s:%jx(), file line %x : %jx != null.", dummyarg
	
	#define STDMSG_BADNULL2 -2, "\nError in %s:%jx(), file line %x : %jx or %jx == null.", dummyarg
	#define STDMSG_BADNONNULL2 -3, "\nError in %s:%jx(), file line %x : %jx or %jx != null.", dummyarg
	
	#define STDMSG_MONADICFAILURE -4, "\nError in %s:%jx(), file line %x : %s call failed with %jd.", dummyarg
		#define STDMSG_NOTELINE -5, "\n\t", dummyarg
		#define STDMSG_NOTESPACE -6, " \t", dummyarg
		#define STDMSG_SIGNEDARG -7, "Arg %s: %jd.", dummyarg
		#define STDMSG_DECARG -8, "Arg %s: %ju.", dummyarg
		#define STDMSG_HEXARG -9, "Arg %s: %jx.", dummyarg
		#define STDMSG_LDOUBLEARG -10, "Arg %s: %Lg.", dummyarg
		#define STDMSG_CHARARG -11, "Arg %s: %c.", dummyarg
		#define STDMSG_STRARG -12, "Arg %s: %s.", dummyarg
		#define STDMSG_DATAPTRARG -13, "Arg %s: %p.", dummyarg
	
	#define STDMSG_FAILEDINTFUNC -14, "\nCall to %s() in %s:%jx(), file line %x, failed : returned( %jd ). Args:", dummyarg
	#define STDMSG_FAILEDUINTFUNC -15, "\nCall to %s() in %s:%jx(), file line %x, failed : returned( %ju ). Args:", dummyarg
	#define STDMSG_FAILEDPTRFUNC -16, "\nCall to %s() in %s:%jx(), file line %x, failed : returned( %p ). Args:", dummyarg
	
	#define STDMSG_TRESPASSPATH -17, "\nInvalid execution path in %s:%jx(), file line %x: %s", dummyarg
	
#endif
