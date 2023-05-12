#ifndef METAC_TOKENIZATION_SIMPLELEX_H
 #define METAC_TOKENIZATION_SIMPLELEX_H
	
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
	
	#include <ctype.h>
	#include <stdint.h>
	#include <stdlib.h>
	
	#include "../external_dependencies/libandria4/basic/pascalarray.h"
	
	#include "basics.h"
	
	
	
	#include "err/err.h"
	
	#include "input/char2num.h"
	#include "input/source.h"
	#include "input/extrachar.h"
	#include "input/lexalike.h"
	
	#include "metaCpreproc/basics.h"
	#include "metaCpreproc/components.h"
	
	#include "output/echo_tokens.h"
	
	#include "tokenization/token.h"
	#include "tokenization/simplelex.h"
	#include "tokenization/complexlex.h"
	
	#include "stack.h"
	#include "tokenmatching.h"
	
#endif
