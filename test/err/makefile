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

# This makefile is for testing the error reporting system.



basification_extension=/..



default_rule:
	echo "Error: test/err/makefile does not support a default action."



$(BUILDDIR)/srctest/err/err.o: $(SOURCEDIR)err/err.c
	$(compileCcommand) $(csrcflag) $(SOURCEDIR)err/err.c $(coutflag) $(BUILDDIR)/srctest/err/err.o $(ccompileonlyflag)

build-sourcecheck: $(BUILDDIR)/srctest/err/simple_test.exe

$(BUILDDIR)/srctest/err/simple_test.exe: $(BUILDDIR)/srctest/err/err.o simple_test.c
	$(compileCcommand) $(csrcflag) simple_test.c $(coutflag) $(BUILDDIR)/srctest/err/simple_test.o $(ccompileonlyflag)
	$(compileCcommand) $(coutflag) $(BUILDDIR)/srctest/err/build-simple_test.exe $(coutflag) $(BUILDDIR)/srctest/err/err.o $(BUILDDIR)/srctest/err/simple_test.o


clean: 
	echo "Error: test/err/makefile has not implemented the clean target."

# Clean for distribuition: stricter than just "clean"
distclean: 
	echo "Error: test/err/makefile has not implemented the distclean target."

# Clean up almost everything that can be rebuilt; usually config scripts will be
#  left alone. Some files may require special tools to reconstruct
maintainer-clean: 
	echo "Error: test/err/makefile has not implemented the maintainer-clean target."


# Test the built project: DO NOT depend on installation
check: 
	echo "Error: test/err/makefile has not implemented the check target."

# Test the source files; unit testing
sourcecheck: build-sourcecheck
	$(BUILDDIR)/srctest/err/build-simple_test.exe
