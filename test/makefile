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

# This makefile is for building and running tests.



basification_extension=../



default_rule:
	echo "Error: test/makefile does not support a default action."



build-sourcecheck:
	cd input && $(make_nestedrunfile) makefile build-sourcecheck
	cd err && $(make_nestedrunfile) makefile build-sourcecheck


clean: 
	echo "Error: test/makefile has not implemented the clean target."

# Clean for distribuition: stricter than just "clean"
distclean: 
	echo "Error: test/makefile has not implemented the distclean target."

# Clean up almost everything that can be rebuilt; usually config scripts will be
#  left alone. Some files may require special tools to reconstruct
maintainer-clean: 
	echo "Error: test/makefile has not implemented the maintainer-clean target."


# Test the built project: DO NOT depend on installation
check: 
	echo "Error: test/makefile has not implemented the check target."

# Test the source files; unit testing
sourcecheck: build-sourcecheck
	cd input && $(make_nestedrunfile) makefile sourcecheck
	cd err && $(make_nestedrunfile) makefile sourcecheck
