cmake_minimum_required(VERSION 3.7.0 FATAL_ERROR)

set(EXTRA_MODULES_DIR ${CMAKE_CURRENT_LIST_DIR}/cmake)
list(APPEND CMAKE_MODULE_PATH ${EXTRA_MODULES_DIR})
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

#=============================================================================
# ensure build type before calling ConanConfig
#=============================================================================
include(EnsureBuildType)

# This MUST run before the call to project()
# Otherwise the appropriate variables will not be set
include(ConanConfig)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_BINARY_DIR}/conan_toolchain.cmake)

project(ventilator_gui
    LANGUAGES CXX C
    VERSION 0.0.1
)

include(CompilerConfig)
include(OutputDirConfig)
include(CoverageReports)
include(QtMacFix)

#include(InstallConfig)

#include(BuildInfo)
execute_process(COMMAND "git" "rev-parse" "HEAD"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions(-DGIT_VERSION="${GIT_SHA1}")

add_subdirectory(src)
add_subdirectory(app)

enable_testing()
add_subdirectory(tests)

add_custom_target(everything DEPENDS
  all_tests
  ventilator_gui_app
  )
