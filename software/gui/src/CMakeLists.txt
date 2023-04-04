include(common.cmake)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)
find_package(Threads REQUIRED)

set(this_target ${PROJECT_NAME}_backend)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS
  SerialPort Quick Multimedia
  REQUIRED)

set(${this_target}_sources
  protocols/network_protocol.pb.cc
  alarm_manager.cpp
  breath_signals.cpp
  controller_history.cpp
  gui_state_container.cpp
  latching_alarm.cpp
  logger.cpp
  periodic_closure.cpp
  respira_connected_device.cpp
  time_series_graph.cpp
  )

set(${this_target}_headers
  protocols/network_protocol.pb.h
  alarm_manager.h
  breath_signals.h
  chrono.h
  connected_device.h
  controller_history.h
  gui_state_container.h
  latching_alarm.h
  logger.h
  patient_detached_alarm.h
  periodic_closure.h
  pip_exceeded_alarm.h
  pip_not_reached_alarm.h
  respira_connected_device.h
  simple_clock.h
  time_series_graph.h
  )

add_library(
  ${this_target} STATIC
  ${${this_target}_resources}
  ${${this_target}_sources}
  ${${this_target}_headers}
)

target_include_directories(
  ${this_target}
  PUBLIC ${PROJECT_SOURCE_DIR}/src
  PUBLIC ${PROJECT_SOURCE_DIR}/src/protocols
  INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

enable_coverage(${this_target})

target_link_libraries(
  ${this_target}
  PUBLIC common
  PUBLIC fmt::fmt
  PUBLIC spdlog::spdlog
  PUBLIC Threads::Threads
  PUBLIC Qt5::SerialPort
  PUBLIC Qt5::Quick
  PUBLIC Qt5::Multimedia
)
