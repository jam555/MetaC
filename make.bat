Rem MetaC Preprocessor - A macro processor for preprocessing usage.
Rem Copyright (C) 2022 Jared A. Maddox
Rem 
Rem This program is free software; you can redistribute it and/or modify it
Rem under the terms of version 2 of the GNU General Public License as
Rem published by the Free Software Foundation.
Rem 
Rem This program is distributed in the hope that it will be useful, but WITHOUT
Rem ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
Rem FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
Rem more details.
Rem 
Rem You should have received a copy of the GNU General Public License along
Rem with this program; if not, write to the:
Rem 	Free Software Foundation, Inc.,
Rem 	59 Temple Place, Suite 330,
Rem 	Boston, MA 02111-1307 USA


Rem This is a batch file to ensure that Microsoft's nmake runs the right
Rem  makefile. If using Microsoft tools, then just run this file instead of
Rem  making life hard for yourself.

nmake.exe /F MSnmake.mak
