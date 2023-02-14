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


# A set of common defines for GNU makefiles. Use this as a template for what
#  files for other "make" variants need to support. The equivalent file for
#  other "makes"s needs to have the same file name: "common.make"

# Note that we assume some Bash compatible shell in this file


# Note: not the same as overrides/rootoverride.make . And yes, this IS part
#  of what other variants must support.
-include overrides/override.make

buildtool_dir = buildtools/GNU_make/



# Default C compiler
CC ?= cc
# Default program to remove a file
RM ?= rm -f
MAKE ?= GNUmake

# Default flags for the C compiler
CFLAGS ?= 
# Default flags for the C preprocessor
CPPFLAGS ?= 
# Default flags for the linker
LDFLAGS ?= 
# Default libraries for the linker
LDLIBS ?= 
# Default flags for make
MAKEFLAGS ?= 

DESTDIR ?= 



make = $(MAKE)
make_fileflag ?= -f


execdest ?= $(DESTDIR)$(bindir)
headdest ?= $(DESTDIR)$(datadir)
libdest ?= $(DESTDIR)$(libdir)
docsdest ?= $(DESTDIR)$(docsdir)


# The BASH select and case commands look interesting for menus; maybe add them
#  for something?


# THe following variables define useful actions.

createdircommand ?= folderbuildfunc () { mkdir $$1 ; } ; folderbuildfunc
createfilecommand ?= filebuildfunc () { touch $$1 ; } ; filebuildfunc

appendcommand ?= fileappendfunc () { echo "$$2" >> $$1 } ; fileappendfunc
truncatecommand ?= filetruncatefunc () { : > $$1 ; } ; filetruncatefunc

# Only a single file at a time.
rmfilecommand ?= fileremovefunc () { unlink $$1 ; } ; fileremovefunc
# We only want to remove EMPTY directories, for safety's sake.
rmdircommand ?= folderremovefunc () { rm -d $$1 ; } ; folderremovefunc


make_run ?= $(make) $(make_fileflag) $(buildtool_dir)common.make
make_runfile ?= $(make) $(make_fileflag) $(buildtool_dir)common.make $(make_fileflag)
