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

# This makefile is for testing the input system.
# Note that while char2num.c is currently here for testing, it should
#  actually be moved to LibAndria4, due to it's "generic-ness".



basification_extension=../



default_rule:
	echo "Error: test/input/makefile does not support a default action."



# A firewall, to ensure that sourcecheck ALWAYS runs the tests.
build-sourcecheck: $(TESTBUILDS)input/char2num_test.exe


$(TESTBUILDS)input/char2num.o: $(SOURCEDIR)input/char2num.c
	$(compileonlyCcommand) $(csrcflag) $(SOURCEDIR)input/char2num.c $(TESTBUILDS)input/char2num.o

$(TESTBUILDS)input/char2num_test.o: char2num_test.c
	$(genericcompileCcommand) $(TESTBUILDS)input/char2num_test.o $^ -c

$(TESTBUILDS)input/char2num_test.exe: $(TESTBUILDS)input/char2num.o $(TESTBUILDS)input/char2num_test.o
	$(genericcompileCcommand) $(TESTBUILDS)input/char2num_test.exe $^


clean: 
	echo "Error: test/input/makefile has not implemented the clean target."

# Clean for distribuition: stricter than just "clean"
distclean: 
	echo "Error: test/input/makefile has not implemented the distclean target."

# Clean up almost everything that can be rebuilt; usually config scripts will be
#  left alone. Some files may require special tools to reconstruct
maintainer-clean: 
	echo "Error: testinput/makefile has not implemented the maintainer-clean target."


# Test the built project: DO NOT depend on installation
check: 
	echo "Error: test/input/makefile has not implemented the check target."

# Test the source files; unit testing
sourcecheck: build-sourcecheck
	$(TESTBUILDS)input/char2num_test.exe
