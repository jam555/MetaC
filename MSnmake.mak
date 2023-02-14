# MetaC Preprocessor - A macro processor for preprocessing usage.
# Copyright (C) 2022 Jared A. Maddox
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of version 2 of the GNU General Public License as
# published by the Free Software Foundation.
# 
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
# more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the:
#	Free Software Foundation, Inc.,
#	59 Temple Place, Suite 330,
#	Boston, MA 02111-1307 USA


# The Microsoft nmake localizer for makefiles.

# This is essentially unfilled, and I'm not even sure nmake defaults to it. Most
#  of the interesting stuff is called "Preprocessing" by Microsoft, and their
#  docs aren't the best things in the world, but there are several useful things
#  that they describe.

# Supposing that the exit code of a program needs to be provided to IF, enclose
#  the command in square brackets, bearing in mind that the usual macro
#  expansions will happen, and nmake will do the needed plumbing.
!IF EXIST( overrides/rootoverride.make )
!INCLUDE overrides/rootoverride.make
!ENDIF
