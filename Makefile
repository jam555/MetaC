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


# This is the generic root-most makefile.

# Note that includes aren't the same across all makefile styles- in particular,
#  GNU and Microsoft are COMPLETELY incompatible. So, that stuff MUST be handled
#  on the command line. The build_tools/*/common.make files provide variables to
#  handle precisely that.



all: 
	echo "Alert: The all target isn't currently implemented by Makefile."

world: all install
# This should probably be followed by using the preprocessor and/or compiler to
#  generate localized support files.


# There should eventually be a pre-build option, that builds at least the
#  preprocessor from C source to iterate to the self-compiling versions.
build:
	echo "Error: Makefile has not implemented the build target."

build-sourcecheck:


import:
	echo "Error: Makefile has not yet properly implemented external dependency import."
	echo "  User must currently import external dependencies to external_dependencies manually."
	$(createdircommand) $(IMPORTSDIR)

install: 
	echo "Error: Makefile has not implemented the install target."
# example-
# install: default
#         install gcd INSTALL_BIN
#         install libmathlib.a INSTALL_LIB

# Create the directories for installation, but don't fill them
installdir: 
	echo "Error: Makefile has not implemented the installdir target."

uninstall: 
	echo "Error: Makefile has not implemented the uninstall target."


clean: 
	echo "Error: Makefile has not implemented the clean target."

# Clean for distribuition: stricter than just "clean"
distclean: clean
	echo "Error: Makefile has not implemented the distclean target."
# Then do more stuff, like executables.

# Clean up almost everything that can be rebuilt; usually config scripts will be
#  left alone. Some files may require special tools to reconstruct
maintainer-clean: distclean
	echo "Error: Makefile has not implemented the maintainer-clean target."
# And even more stuff...


# Create a distribution file (e.g. a tar file, perhaps compressed with gzip);
#  the base-most entity in the file should be a directory named after both
#  project and version, which in turn should fully contain the project; the
#  target should depend on all relevant targets, to ensure that everything is
#  "fresh"
dist: build
	echo "Error: Makefile has not implemented the dist target."
# So, how do we build?


# Test the built project: DO NOT depend on installation
check: build
	echo "Error: Makefile has not implemented the check target."
# Tests.

# Test the source files; unit testing
sourcecheck: build-sourcecheck
	echo "Error: Makefile has not implemented the sourcecheck target."
# Then run tests.
