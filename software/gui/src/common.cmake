set(this_target common)

set(${this_target}_sources
  ${PROJECT_SOURCE_DIR}/../common/generated_libs/network_protocol/network_protocol.pb.c
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_common.c
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_decode.c
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_encode.c
  ${PROJECT_SOURCE_DIR}/../common/libs/units/units.cpp
  )

set(${this_target}_headers
  ${PROJECT_SOURCE_DIR}/../common/generated_libs/network_protocol/network_protocol.pb.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_common.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_decode.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_encode.h
  ${PROJECT_SOURCE_DIR}/../common/libs/units/units.h
  )

add_library(
  ${this_target} STATIC
  ${${this_target}_headers}
  ${${this_target}_sources}
)

target_include_directories(
  ${this_target}
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/generated_libs/network_protocol
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/libs/units
  INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
