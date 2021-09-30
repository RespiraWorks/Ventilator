set(this_target common)

set(${this_target}_sources
  ../common/generated_libs/network_protocol/network_protocol.pb.c
  ../common/third_party/nanopb/pb_common.c
  ../common/third_party/nanopb/pb_decode.c
  ../common/third_party/nanopb/pb_encode.c
  ../common/libs/units/units.cpp
  )

set(${this_target}_headers
  ../common/generated_libs/network_protocol/network_protocol.pb.h
  ../common/third_party/nanopb/pb.h
  ../common/third_party/nanopb/pb_common.h
  ../common/third_party/nanopb/pb_decode.h
  ../common/third_party/nanopb/pb_encode.h
  ../common/libs/units/units.h
  )

add_library(
  ${this_target} STATIC
  ${${this_target}_headers}
  ${${this_target}_sources}
)

target_include_directories(
  ${this_target}
  PRIVATE ${PROJECT_SOURCE_DIR}/../common/generated_libs/network_protocol
  PRIVATE ${PROJECT_SOURCE_DIR}/../common/third_party/nanopb
  PRIVATE ${PROJECT_SOURCE_DIR}/../common/libs/units
)
