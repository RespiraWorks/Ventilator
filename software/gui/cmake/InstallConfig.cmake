#
# this file configures a couple of default installation directories including
# an installation path for the CMake - package files. This file should be
# included rather realy in the global project configuration.
#

include(GNUInstallDirs) #Required for CMAKE_INSTALL_XXX

if((CMAKE_HOST_SYSTEM_NAME MATCHES Linux) AND
    (CMAKE_SYSTEM_NAME MATCHES Linux))
    # --- if we are building on Linux for Linux ----
    # on the Linux host we rely on the GNUInstallDirs module to
    # determine the installation paths
    include(GNUInstallDirs)
    message(STATUS "==============================================================")
    message(STATUS "Installation directories for Linux: ")

    set(CMAKE_INSTALL_DOCDIR ${CMAKE_INSTALL_DATAROOTDIR}/doc/${PROJECT_NAME}-${PROJECT_VERSION})

elseif(CMAKE_SYSTEM_NAME MATCHES Windows)

    #if we are building for Windows the GNU installation paths do
    #not make sens (actually the module may not even exists.
    #In this case we set the installation paths manually
    #
    #Please note that on Windows we can only use relative installation
    #paths. However, we use the same variables as GNUInstallDirs would
    #use in order to simplify the CMake code.
    #
    set(CMAKE_INSTALL_LIBDIR lib)
    set(CMAKE_INSTALL_BINDIR bin)
    set(CMAKE_INSTALL_INCLUDEDIR include)
    set(CMAKE_INSTALL_DATAROOTDIR share)
    set(CMAKE_INSTALL_DOCDIR share/doc/${PROJECT_NAME}-${PROJECT_VERSION})

    message(STATUS "==============================================================")
    message(STATUS "Installation directories for Windows: ")

endif()

#
# the installation path the the cmake package file is the same
# for both platforms
#
set(CMAKE_INSTALL_PACKAGEDIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}-${PROJECT_VERSION})

if(CMAKE_INSTALL_PREFIX)
    message(STATUS "Prefix is: ${CMAKE_INSTALL_PREFIX}")
else()
    message(STATUS "Prefix is: unknown")
endif()
message(STATUS "Libraries: ${CMAKE_INSTALL_LIBDIR}")
message(STATUS "Binaries:  ${CMAKE_INSTALL_BINDIR}")
message(STATUS "Headers :  ${CMAKE_INSTALL_INCLUDEDIR}")
message(STATUS "Data:      ${CMAKE_INSTALL_DATAROOTDIR}")
message(STATUS "Docs:      ${CMAKE_INSTALL_DOCDIR}")
message(STATUS "CMake PKG: ${CMAKE_INSTALL_PACKAGEDIR}")
message(STATUS "==============================================================")
