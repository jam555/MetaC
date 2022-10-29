
	/* .th.toktype must == TOKTYPE_CONTEXTSPECIALS */
typedef struct context_specials
{
	token_head th;
	
	tokhdptr_parr *breaks;
	token_head *start, *end;
	
} context_specials;
lib4_intresult not_contextspecials( token_head *tok_, context_specials *ctx );
	
	/* This REQUIRES a context_specials{}* to be provided to v_. It will */
	/*  return a sequence of tokens, themselves additional tokengroup{}s, */
	/*  each representing a separate argument or similar. These may have */
	/*  macro or directive invocation tokens mixed in, as most macros & */
	/*  tokens are expected to NOT be invoked at this early point in the */
	/*  parsing process. */
	/* (  -- tokengroup* ) */
retframe bracketgather_enter( stackpair *stkp, void *v_ );

	/* This calls bracketgather_enter(), use it instead. */
	/* ( token* -- token* | tokengroup* ) */
retframe bracketgather_dispatcher( stackpair *stkp, void *v )

