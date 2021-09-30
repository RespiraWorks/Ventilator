#
# set default output directory paths
#

# =============================================================================
# setting up the output directory for library, archive and runtime targets
#
# This should be the same on all platforms just to avoid confusion
# =============================================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message(STATUS "==============================================================")
message(STATUS "Build output directories:")
message(STATUS "Write runtime files to: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "Write library files to: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
message(STATUS "Write archive files to: ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
message(STATUS "==============================================================")
