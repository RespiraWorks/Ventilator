cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)
project(ventiltor_gui
    LANGUAGES CXX C
    VERSION 0.0.1
    )

set(EXTRA_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${EXTRA_MODULES_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

#=============================================================================
# ensure build type first
#=============================================================================

include(EnsureBuildType)

#=============================================================================
# Conan
#=============================================================================

set(CONAN_PROFILE "default" CACHE STRING "Name of conan profile to use, uses default by default")
set(CONAN "AUTO" CACHE STRING "conan options AUTO (conan must be in path), MANUAL (expects conanbuildinfo.cmake in build directory) or DISABLE")
if(${CONAN} MATCHES "AUTO")
  include(${EXTRA_MODULES_DIR}/conan.cmake)
  conan_cmake_run(CONANFILE conanfile.txt
      PROFILE ${CONAN_PROFILE}
      BASIC_SETUP NO_OUTPUT_DIRS KEEP_RPATHS
      BUILD_TYPE "None"
      BUILD outdated)
  message(STATUS "Deleting Conan-generated Findqplot.cmake to use package-provided modules")
  file(REMOVE ${CMAKE_BINARY_DIR}/Findqplot.cmake)
elseif(${CONAN} MATCHES "MANUAL")
  if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    conan_basic_setup(NO_OUTPUT_DIRS KEEP_RPATHS)
  else()
    message(FATAL_ERROR "CONAN set to MANUAL but no file named conanbuildinfo.cmake found in build directory")
  endif()
elseif(NOT ${CONAN} MATCHES "DISABLE")
  message(FATAL_ERROR "Unrecognised option for CONAN (${CONAN}), use AUTO, MANUAL or DISABLE")
endif()

include(CompilerConfig)
include(OutputDirConfig)
include(CoverageReports)

include(common.cmake)
add_subdirectory(src)
add_subdirectory(app)

#include(FindGTestFix)
#if(GTest_FOUND)
#  add_subdirectory(tests)
#  add_custom_target(everything DEPENDS
#    all_tests
#    acquire
#    daquiri
#    )
#  create_coverage_targets(
#    coverage
#    run_unit_tests
#    ${PROJECT_BINARY_DIR}
#    ${CMAKE_SOURCE_DIR}/source/
#    ${PROJECT_BINARY_DIR}/tests/coverage
#  )
#endif()
