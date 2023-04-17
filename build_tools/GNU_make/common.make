# LibAndria version 4
# A C-based general purpose utility library.
# Copyright (c) 2019 Jared A. Maddox
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
# of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
# 
# This grant of rights is subject to two conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# And:
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.



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

requiredircommand?=folderrequirefunc () { mkdir -p $$1 ; } ; folderrequirefunc
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
# First arg is the output file, all others will be parsed as compiler sees appropriate.
genericcompileCcommand?=genericcompileCcommandfunc () { arg1=$$1; shift ; $(CC) -o $$arg1 $$@ ; } ; genericcompileCcommandfunc
