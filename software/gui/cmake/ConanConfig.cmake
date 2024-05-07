find_program(
  CONAN
  NAMES conan
  HINT ENV PATH
  DOC "conan executable location"
  )

if(NOT CONAN)
  message(FATAL_ERROR "Could not find the command 'conan', which is required")
else()
  message(STATUS "Found conan executable at: ${CONAN}")
endif()

set(
  CONAN_ARGS
  "install" "${CMAKE_SOURCE_DIR}" "--output-folder=${CMAKE_CURRENT_BINARY_DIR}" "--build=missing" "--settings=build_type=${CMAKE_BUILD_TYPE}"
  )

execute_process(
  COMMAND ${CONAN} ${CONAN_ARGS}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND_ECHO STDOUT
  COMMAND_ERROR_IS_FATAL ANY
  )
