# Define cmake options for Jabra SDK support
option(ENABLE_JABRA "Enable Jabra SDK support" OFF)
option(JABRA_NO_LIBCURL "Use no_libcurl version of Jabra SDK" OFF)
set(JABRA_APPLICATION_ID "" CACHE STRING "Jabra SDK application id")
set(JABRA_APPLICATION_GUID "" CACHE STRING "Jabra SDK application guid")
set(JABRA_APPLICATION_NAME "" CACHE STRING "Jabra SDK application name")

if (ENABLE_JABRA)
    # Import jabra sdk library from jabra-sdk-linux/library. The include files are under headers, the .so files are unter ubuntu/64-bit.
    set(JABRA_DIR ${CMAKE_CURRENT_LIST_DIR}/jabra-sdk-linux/library)

    if (EXISTS ${JABRA_DIR})
        message(STATUS "Jabra SDK found at ${JABRA_DIR}")
        set(HAVE_JABRA 1)
    else ()
        message(STATUS "Jabra SDK not found. Please clone the jabra-sdk-linux repository to ${CMAKE_CURRENT_LIST_DIR}/jabra-sdk-linux")
    endif ()


    if (HAVE_JABRA)
        set(JABRA_INCLUDE_DIRS ${JABRA_DIR}/headers)
        set(JABRA_LIB_DIR ${JABRA_DIR}/ubuntu/64-bit)

        # Find jabra sdk library file
        if (JABRA_NO_LIBCURL)
            file(GLOB JABRA_GLOB_RESULT "${JABRA_LIB_DIR}/no_libstdc++/no_libcurl/libjabra.so*")
        else ()
            file(GLOB JABRA_GLOB_RESULT "${JABRA_LIB_DIR}/libjabra.so*")
        endif ()

        list(SORT JABRA_GLOB_RESULT)
        list(GET JABRA_GLOB_RESULT -1 JABRA_LIBRARY_FILE)
        message(STATUS "Jabra SDK library found: ${JABRA_LIBRARY_FILE}")


        add_library(jabra SHARED IMPORTED)
        set_target_properties(jabra PROPERTIES
                IMPORTED_LOCATION ${JABRA_LIBRARY_FILE}
        )


        # Add jabra sdk library to linphone-daemon
        target_include_directories(linphone-daemon PRIVATE ${JABRA_INCLUDE_DIRS})
        target_link_libraries(linphone-daemon jabra)
        target_link_directories(linphone-daemon PRIVATE ${JABRA_LIB_DIR})
        target_sources(linphone-daemon PRIVATE
                jabra/JabraSdk.cpp
                jabra/JabraSdk.h)


        # Set compiler definitions
        add_definitions(-DHAVE_JABRA=1)
        add_definitions(-DJABRA_APPLICATION_ID="${JABRA_APPLICATION_ID}")
        add_definitions(-DJABRA_APPLICATION_GUID="${JABRA_APPLICATION_GUID}")
        add_definitions(-DJABRA_APPLICATION_NAME="${JABRA_APPLICATION_NAME}")


        # Install library
        # First install the actual .so file (normal vs. no_libcurl version)
        # Then install the symlinks to the .so file. cmake will not overwrite the already installed .so file.
        install(IMPORTED_RUNTIME_ARTIFACTS jabra LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
        install(DIRECTORY ${JABRA_LIB_DIR}/ DESTINATION ${CMAKE_INSTALL_LIBDIR} FILES_MATCHING PATTERN "*.so*")
    endif ()
endif ()
