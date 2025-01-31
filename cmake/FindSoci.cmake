###############################################################################
# CMake module to search for SOCI library
#
# WARNING: This module is experimental work in progress.
#
# This module defines:
#  SOCI_INCLUDE_DIRS        = include dirs to be used when using the soci library
#  SOCI_LIBRARIES           = full path to the soci library
#  SOCI_VERSION             = the soci version found (not yet. soci does not provide that info.)
#  SOCI_FOUND               = true if soci was found
#
# For each component you specify in find_package(), the following variables are set.
#
#  SOCI_${COMPONENT}_PLUGIN = full path to the soci plugin
#  SOCI_${COMPONENT}_FOUND
#
# Copyright (c) 2011 Michael Jansen <info@michael-jansen.biz>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#
###############################################################################
#
### Global Configuration Section
#
SET(_SOCI_ALL_PLUGINS    mysql sqlite3)

SET(_SOCI_REQUIRED_VARS  SOCI_INCLUDE_DIRS SOCI_LIBRARIES)
SET(_SOCI_VERSION "_4_0")

if(TARGET soci_core OR TARGET soci_core_static)

    if(TARGET soci_core)
      set(SOCI_CORE_TARGET soci_core)
    else()
      set(SOCI_CORE_TARGET soci_core_static)
    endif()
    set(SOCI_LIBRARIES ${SOCI_CORE_TARGET})
    get_target_property(SOCI_INCLUDE_DIRS ${SOCI_CORE_TARGET} INTERFACE_INCLUDE_DIRECTORIES)

    if(TARGET soci_sqlite3 OR TARGET soci_sqlite3_static)
      if(TARGET soci_sqlite3)
        set(SOCI_SQLITE3_TARGET soci_sqlite3)
      else()
        set(SOCI_SQLITE3_TARGET soci_sqlite3_static)
      endif()
			set(SOCI_sqlite3_PLUGIN ${SOCI_SQLITE3_TARGET})
    endif()

else()

    #
    ### FIRST STEP: Find the soci headers.
    #
    FIND_PATH(SOCI_INCLUDE_DIRS soci/soci.h
        DOC "Soci (http://soci.sourceforge.net) include directory")
    MARK_AS_ADVANCED(SOCI_INCLUDE_DIRS)

    #
    ### SECOND STEP: Find the soci core library. Respect LIB_SUFFIX
    #
    FIND_LIBRARY(SOCI_LIBRARIES
        NAMES soci_core soci_core${_SOCI_VERSION} libsoci_core libsoci_core${_SOCI_VERSION}
        PATH_SUFFIXES Frameworks lib lib64)
    MARK_AS_ADVANCED(SOCI_LIBRARIES)

    GET_FILENAME_COMPONENT(SOCI_LIBRARY_DIR ${SOCI_LIBRARIES} PATH)
    MARK_AS_ADVANCED(SOCI_LIBRARY_DIR)

    #
    ### THIRD STEP: Find all installed plugins if the library was found
    #
    IF(SOCI_INCLUDE_DIRS AND SOCI_LIBRARIES)

        MESSAGE(STATUS "Soci found: Looking for plugins")
        FOREACH(plugin IN LISTS _SOCI_ALL_PLUGINS)

            FIND_LIBRARY(
                SOCI_${plugin}_PLUGIN
                NAMES soci_${plugin} soci_${plugin}${_SOCI_VERSION} libsoci_${plugin} libsoci_${plugin}${_SOCI_VERSION}
                PATH_SUFFIXES Frameworks lib lib64)
            MARK_AS_ADVANCED(SOCI_${plugin}_PLUGIN)

            IF(SOCI_${plugin}_PLUGIN)
                MESSAGE(STATUS "    * Plugin ${plugin} found ${SOCI_${plugin}_PLUGIN}.")
                SET(SOCI_${plugin}_FOUND True)
            ELSE()
                MESSAGE(STATUS "    * Plugin ${plugin} not found.")
                SET(SOCI_${plugin}_FOUND False)
            ENDIF()

        ENDFOREACH()

        #
        ### FOURTH CHECK: Check if the required components were all found
        #
        FOREACH(component ${Soci_FIND_COMPONENTS})
            IF(NOT SOCI_${component}_FOUND)
                MESSAGE(SEND_ERROR "Required component ${component} not found. It seems that Soci was built without support of ${component}, consider rebuilding it.")
            ENDIF()
        ENDFOREACH()

    ENDIF()

endif()

#
### ADHERE TO STANDARDS
#
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Soci DEFAULT_MSG ${_SOCI_REQUIRED_VARS})
