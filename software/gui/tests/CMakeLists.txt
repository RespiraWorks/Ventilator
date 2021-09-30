set(this_target gui_backend_tests)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Test REQUIRED)

set(${this_target}_sources
  tst_main.cpp
  )

set(${this_target}_headers
  breath_signals_test.h
  latching_alarm_test.h
  logger_test.h
  patient_detached_alarm_test.h
  )

add_executable(
  ${this_target}
  ${${this_target}_sources}
  ${${this_target}_headers}
)

target_include_directories(
  ${this_target}
  PRIVATE ${PROJECT_SOURCE_DIR}/tests
)

target_link_libraries(
  ${this_target}
  PRIVATE ${PROJECT_NAME}_backend
  PRIVATE Qt5::Test
)

# Creates command dependent on existence of target
add_custom_target(run_gui_backend_tests
  COMMAND $<TARGET_FILE:${this_target}>
  DEPENDS ${this_target})
add_test(NAME "RunBackendTests" COMMAND run_gui_backend_tests)

#############
# ALL TESTS #
#############

add_custom_target(all_tests DEPENDS
  gui_backend_tests
  #add more test targets here
  )

add_custom_target(run_tests DEPENDS
  run_gui_backend_tests
  #add more test commands here
  )
