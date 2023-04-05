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
#  other "makes"s needs to have the same file name: "common.make".
# Note that while the variables "basification_path" and
#  "basification_extension" are required by this file, but cannot be
#  provided by it: they form the path that moves the file reference back to
#  the project-root, and are actually needed in order to find this very
#  file. They SHOULD NOT end in a directory divider, just in case they need
#  to be used without that in the future. Don't override them in your
#  override.make, but you CAN override project_base, which is a shallow
#  wrapper around basification_path, if you need (*_extension is
#  specifically used for makefiles in subdirectories).

# Note that we assume some Bash compatible shell in this file



buildtool_path=buildtools/GNU_make/

# Note: not the same as overrides/rootoverride.make . And yes, this IS part
#  of what other variants must support.
-include overrides/override.make

project_base?=$(basification_path)



# Default C compiler
CC?=cc
# Default program to remove a file
RM?=rm -f
MAKE?=GNUmake

# Default flags for the C compiler
CFLAGS?= 
# Default flags for the C preprocessor
CPPFLAGS?= 
# Default flags for the linker
LDFLAGS?= 
# Default libraries for the linker
LDLIBS?= 
# Default flags for make
MAKEFLAGS?= 

DESTDIR?=
BUILDDIR?=$(project_base)/build/
SOURCEDIR?=$(project_base)/src/
TESTSDIR?=$(project_base)/test/
IMPORTSDIR?=$(project_base)/external_dependencies/



make=$(MAKE)
make_fileflag?=-f

csrcflag?=
coutflag?=-o
ccompileonlyflag?=-c


execdest?=$(DESTDIR)$(bindir)
headdest?=$(DESTDIR)$(datadir)
libdest?=$(DESTDIR)$(libdir)
docsdest?=$(DESTDIR)$(docsdir)


# The BASH select and case commands look interesting for menus; maybe add them
#  for something?


# The following variables define useful actions.

createdircommand?=folderbuildfunc () { mkdir $$1 ; } ; folderbuildfunc
createfilecommand?=filebuildfunc () { touch $$1 ; } ; filebuildfunc

appendcommand?=fileappendfunc () { echo "$$2" >> $$1 ; } ; fileappendfunc
truncatecommand?=filetruncatefunc () { : > $$1 ; } ; filetruncatefunc

# Only a single file at a time.
rmfilecommand?=fileremovefunc () { unlink $$1 ; } ; fileremovefunc
# We only want to remove EMPTY directories, for safety's sake.
rmdircommand?=folderremovefunc () { rm -d $$1 ; } ; folderremovefunc


# Note that both the basification path and extension get merged: together
#  they'll form the next path.
make_run?=$(make) \
	basification_path=$(basification_path)$(basification_extension) \
	$(make_fileflag) $(project_base)/$(buildtool_path)common.make
make_runfile?=$(make) \
	basification_path=$(basification_path)$(basification_extension) \
	$(make_fileflag) $(project_base)/$(buildtool_path)common.make \
	$(make_fileflag)

# First arg is the source file, second is the destination.
compileonlyCcommand?=compileonlyCcommandfunc () { $(CC) $$1 -o $$2 -c ; } ; compileonlyCcommandfunc
genericcompileCcommand?=genericcompileCcommandfunc () { arg1=$$1; shift ; $(CC) $$@ -o $$arg1 ; } ; genericcompileCcommandfunc
