############################################################################
# FindXML2.cmake
# Copyright (C) 2015-2023  Belledonne Communications, Grenoble France
#
############################################################################
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
############################################################################
#
# - Find the libxml2 include file and library
#
#  XML2_FOUND - system has libxml2
#  XML2_INCLUDE_DIRS - the libxml2 include directory
#  XML2_LIBRARIES - The libraries needed to use libxml2

if(TARGET xml2)

	set(XML2_LIBRARIES xml2)
	get_target_property(XML2_INCLUDE_DIRS xml2 INTERFACE_INCLUDE_DIRECTORIES)
	set(HAVE_LIBXML_XMLREADER_H 1)

else()

	if(APPLE AND NOT IOS)
		set(XML2_HINTS "/usr")
	endif()
	if(XML2_HINTS)
		set(XML2_LIBRARIES_HINTS "${XML2_HINTS}/lib")
	endif()

	find_path(XML2_INCLUDE_DIRS
		NAMES libxml/xmlreader.h
		HINTS "${XML2_HINTS}"
		PATH_SUFFIXES include/libxml2
	)

	if(XML2_INCLUDE_DIRS)
		set(HAVE_LIBXML_XMLREADER_H 1)
	endif()

	find_library(XML2_LIBRARIES
		NAMES xml2
		HINTS "${XML2_LIBRARIES_HINTS}"
	)

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(XML2
	DEFAULT_MSG
	XML2_INCLUDE_DIRS XML2_LIBRARIES
)

mark_as_advanced(XML2_INCLUDE_DIRS XML2_LIBRARIES)
