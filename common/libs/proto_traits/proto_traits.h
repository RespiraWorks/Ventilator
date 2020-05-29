/* Copyright 2020, RespiraWorks

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef PROTO_TRAITS_H
#define PROTO_TRAITS_H

#include "network_protocol.pb.h"

// Traits classes for our protocol buffers.
//
// nanopb declares constants for each proto message, e.g. its max size
// (FooProto_size), its pb_msgdesc_t struct (FooProto_msg), and zero/default
// initializers (FooProto_init_zero/FooProto_init_default).
//
// These work ok so long as FooProto is a concrete class.  But suppose it's a
// template; how do you access T_init_zero, for any proto T?
//
// That's where these traits classes come in.  Simply define a MAKE_TRAITS
// below and then you can, for instance:
//
//   template <typename Proto>
//   bool encode(pb_ostream_t* stream, const Proto& proto) {
//     return pb_encode(&stream, ProtoTraits<Proto>::MsgDesc, &proto);
//   }

template <typename Proto> struct ProtoTraits {
  // static inline constexpr uint32_t MaxSize = Proto_size;
  // static inline constexpr Proto InitZero = Proto_init_zero;
  // static inline constexpr Proto InitDefault = Proto_init_default;
  // static inline const pb_msgdesc_t *MsgDesc = &Proto_msg;
};

#define MAKE_TRAITS(T)                                                         \
  template <> struct ProtoTraits<T> {                                          \
    using Type = T;                                                            \
    static inline constexpr uint32_t MaxSize = T##_size;                       \
    static inline constexpr T InitZero = T##_init_zero;                        \
    static inline constexpr T InitDefault = T##_init_default;                  \
    static inline const pb_msgdesc_t *MsgDesc = &T##_msg;                      \
  }

MAKE_TRAITS(ControllerStatus);
MAKE_TRAITS(GuiStatus);

#undef MAKE_TRAITS

#endif // PROTO_TRAITS_H
