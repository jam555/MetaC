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



/* These functions have been purged, but should only */
/*  have been referenced in preproc.c: */
	/* phase1_comonexec, bracketgather_invalidcall, */
	/*  bracketgather_badtoken, bracketgather_exit, */
	/*  bracketgather_loop_search, bracketgather_loop. */
/* These functions might have external references, if */
/*  so then those references should be massaged enough */
/*  to move to bracketgather_entry(). */
	/* bracketgather_enter, bracketgather_dispatcher. */
/* bracketgather_check() and not_contextspecials() may */
/*  also have some external references, though I THINK */
/*  it would have all been internal. */



	/* Needs to be called at program startup. */
int init_basal_gennameLUTs();
	/* Don't use this one, it should be left to internal purposes. */
int grow_basal_gennameLUTs( int deltaSize );
	/* This only deallocates the LUTs, not the contents! */
	/* Actually, you probably shouldn't use this one either. */
int deinit_basal_gennameLUTs()



int is_execable( token *tok,  generic_named **found );
	/* Note: does not necessarily 'return' the SAME token pointer. In fact, */
	/*  generally SHOULDN'T. */
	/* ( token* -- ... -- token* ) */
retframe on_execable( stackpair *stkp, void *v );
	/* Gathers up a bunch of tokens that form a pair of brackets and it's */
	/*  contents. This DOES at least attempt to execute anything that */
	/*  is_execable() can find, under the influence of was_freshline(). */
	/* Top value will be: */
		/* 0 if not a bracket entrance, */
		/* 1 if correctly formed, or */
		/* 2 if explicitly bad syntax ( e.g. pairing an opening parenthese */
		/*  with a closing squate bracket). */
	/* ( token* -- ( token* 0 ) | ( tokengroup* 1 ) | ( tokengroup* 2 ) ) */
reframe bracketgather_entry( stackpair *stkp, void *v );
