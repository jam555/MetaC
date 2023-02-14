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


# The GNU make localizer for makefiles.
# To assign only things that haven't already been assigned, use '?=' instead of
#  '='

include build_tools/GNU_make/common.make

# rootoverride isn't guaranteed to exist, but if it does, then it is intended to
#  allow overrides of... STUFF in this file.
-include overrides/rootoverride.make

CC ?= gcc
MAKE ?= make



all: 
	./$(make_runfile) Makefile all

install: 

# Create the directories for installation, but don't fill them
installdir: 

uninstall: 

clean: 
	./$(make_runfile) Makefile clean

# Clean for distribuition: stricter than just "clean"
distclean: 

# Clean up almost everything that can be rebuilt; usually config scripts will be
#  left alone. Some files may require special tools to reconstruct
maintainer-clean: 

# Create a distribution file (e.g. a tar file, perhaps compressed with gzip);
#  the base-most entity in the file should be a directory named after both
#  project and version, which in turn should fully contain the project; the
#  target should depend on all relevant targets, to ensure that everything is
#  "fresh"
dist: 

# Test the built project: DO NOT depend on installation
check: 

# Test the source files; unit testing
sourcecheck: 
