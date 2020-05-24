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

// Defines an interface for creating debug variables.
//
// All debug variables must have a 32-bit value: int32_t, uint32_t, or float.
// They are declared using DebugVar that takes a name, help string,
// printf format string, and getter and setter lambdas:
//
//   DebugVar var("speed", "The vehicle speed", "%.3f",
//     [&] { return ...; },
//     [&](float value) { ... });
//
// All debug variables in the program are accessible via a static registry:
//
//   DebugVarRegistry::GetVar(i) is valid for all i in [0, N)
//   where N = DebugVarRegistry::GetNumVars().
//
#ifndef VARS_H
#define VARS_H

#include "debug.h"
#include <stdint.h>
#include <type_traits>

// Defines the type of variable
enum class VarType {
  INT32 = 1,
  UINT32 = 2,
  FLOAT = 3,
};

#define ARRAY_CT(a) (sizeof(a) / sizeof((a)[0]))

class UntypedDebugVar;

class DebugVarRegistry {
public:
  static uint16_t Register(UntypedDebugVar *var) {
    if (var_count < static_cast<int>(ARRAY_CT(all_vars)))
      all_vars[var_count++] = var;
    return static_cast<uint16_t>(var_count - 1);
  }

  static UntypedDebugVar *GetVar(uint16_t vid) {
    if (vid >= ARRAY_CT(all_vars))
      return nullptr;
    return all_vars[vid];
  }

  static int GetNumVars() { return var_count; }

private:
  // List of all the variables in the system.
  // Increase size as necessary
  static UntypedDebugVar *all_vars[100];
  static int var_count;
};

#undef ARRAY_CT

class UntypedDebugVar {
public:
  UntypedDebugVar(const char *name, const char *help, const char *fmt,
                  VarType type)
      : name_(name), help_(help), fmt_(fmt), type_(type),
        id_(DebugVarRegistry::Register(this)) {}

  virtual uint32_t as_uint32() = 0;
  virtual void set_uint32(uint32_t value) = 0;

  const char *name() const { return name_; }
  const char *help() const { return help_; }
  const char *fmt() const { return fmt_; }
  VarType type() const { return type_; }
  uint16_t id() const { return id_; }

private:
  const char *const name_;
  const char *const help_;
  const char *const fmt_;
  const VarType type_;
  const uint16_t id_;
};

template <typename X> VarType type_to_enum();
template <> inline constexpr VarType type_to_enum<int32_t>() {
  return VarType::INT32;
}
template <> inline constexpr VarType type_to_enum<uint32_t>() {
  return VarType::UINT32;
}
template <> inline constexpr VarType type_to_enum<float>() {
  return VarType::FLOAT;
}

template <typename GetFn, typename SetFn>
class DebugVar : public UntypedDebugVar {
public:
  using ValueT = std::invoke_result_t<GetFn>;

  // @param name Name of the variable
  // @param help String that the Python code displays describing the variable.
  // @param fmt printf style format string.  This is a hint to the Python code
  // as to how the variable data should be displayed.
  // @param get A lambda for returning the current value with signature: T get()
  // @param set A lambda for setting the value with signature: void set(T value)
  DebugVar(const char *name, const char *help, const char *fmt, GetFn get,
           SetFn set)
      : UntypedDebugVar(name, help, fmt, type_to_enum<ValueT>()), get_fn_(get),
        set_fn_(set) {}

  ValueT Get() { return get_fn_(); }
  void Set(ValueT t) { set_fn_(t); }

  uint32_t as_uint32() override {
    ValueT res = Get();
    return *reinterpret_cast<uint32_t *>(&res);
  }
  void set_uint32(uint32_t value) override {
    Set(*reinterpret_cast<ValueT *>(&value));
  }

private:
  GetFn get_fn_;
  SetFn set_fn_;
};

#endif
