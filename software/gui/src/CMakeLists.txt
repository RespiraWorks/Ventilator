#include(InstallConfig)
#include(BuildInfo)

find_package(spdlog REQUIRED)
#find_package(date REQUIRED)
#find_package(Threads REQUIRED)

set(this_target ${PROJECT_NAME}_src)

include(QtLibraryConfig)

set(${this_target}_sources
  gui_state_container.cpp
  logger.cpp
  periodic_closure.cpp
  time_series_graph_painter.cpp
  )

set(${this_target}_headers
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
  time_series_graph_painter.h
  )

# Mostly for qnanopainter \TODO: make it an external lib
add_subdirectory(third_party)

add_library(
  ${this_target} STATIC
  ${${this_target}_resources}
  ${${this_target}_sources}
  ${${this_target}_headers}
)

target_include_directories(
  ${this_target}
  PUBLIC ${PROJECT_SOURCE_DIR}/src
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/generated_libs/network_protocol
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/libs/units
  PUBLIC ${PROJECT_SOURCE_DIR}/src/third_party/qnanopainter/libqnanopainter
  INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(
  ${this_target}
  PUBLIC common
  PUBLIC qnanopainter
  PUBLIC Qt5::SerialPort
  PUBLIC Qt5::Quick
  PUBLIC Qt5::Multimedia
)
