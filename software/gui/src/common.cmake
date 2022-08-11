set(this_target common)

set(${this_target}_sources
  ${PROJECT_SOURCE_DIR}/../common/generated_libs/protocols/network_protocol.pb.c
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_common.c
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_decode.c
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_encode.c
  ${PROJECT_SOURCE_DIR}/../common/libs/checksum/checksum.cpp
  )

set(${this_target}_headers
  ${PROJECT_SOURCE_DIR}/../common/generated_libs/protocols/network_protocol.pb.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_common.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_decode.h
  ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb/pb_encode.h
  ${PROJECT_SOURCE_DIR}/../common/libs/units/units.h
  ${PROJECT_SOURCE_DIR}/../common/libs/framing/frame_detector.h
  ${PROJECT_SOURCE_DIR}/../common/libs/framing/framing.h
  ${PROJECT_SOURCE_DIR}/../common/libs/framing/framing_streams.h
  ${PROJECT_SOURCE_DIR}/../common/libs/framing/framing_spec_chars.h
  ${PROJECT_SOURCE_DIR}/../common/libs/framing/serial_listeners.h
  ${PROJECT_SOURCE_DIR}/../common/libs/framing/rx_buffer.h
  ${PROJECT_SOURCE_DIR}/../common/libs/framing/soft_rx_buffer.h
  ${PROJECT_SOURCE_DIR}/../common/libs/proto_traits/proto_traits.h
  )

add_library(
  ${this_target} STATIC
  ${${this_target}_headers}
  ${${this_target}_sources}
)

target_include_directories(
  ${this_target}
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/generated_libs/protocols
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/libs/units
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/libs/checksum
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/libs/framing
  PUBLIC ${PROJECT_SOURCE_DIR}/../common/libs/proto_traits
  INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
