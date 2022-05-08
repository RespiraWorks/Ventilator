/* Copyright 2020-2021, RespiraWorks

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

#pragma once

#include <cstddef>
#include <cstdint>

namespace Debug::Variable {

// \todo should we have an `Invalid` type?
// Defines the type of variable
enum class Type {
  Int32 = 1,
  UInt32 = 2,
  Float = 3,
  FloatArray = 4,
  String = 5,
};

// Defines the possible access to variable
enum class Access {
  ReadOnly = 0,
  ReadWrite = 1,
};

static constexpr uint16_t MaxVariableCount{100};

static constexpr uint16_t InvalidID{MaxVariableCount};

/*! \class Base vars_base.h "vars_base.h"
 *  \brief Abstract base class for debug variables
 *
 * This class represents a variable that you can read/write using the debug serial port. These are
 * also variables that can be recorded in traces.
 *
 * We give each such variable a name which the debugger command line will use to access it. We can
 * also link it with a C++ variable whose value it will read or write.
 *
 * Each variable will register itself with the debug variable Registry, which will issue it a unique
 * ID. However, name uniqueness is not automatically guaranteed, and you are responsible for
 * ensuring it, lest the client side of the debugger fail to function properly.
 */
class Base {
 public:
  /*! \param type Type of the variable, so debug client knows how to interpret it.
   *  \param name Name of the variable, should be unique.
   *  \param access Access mode to this variable for the debug interface.
   *  \param units physical units associated with value represented by variable.
   *  \param help String that the interface displays describing the variable.
   *  \param fmt printf style format string, hint for debug client on how to display variable data.
   *  \post Variable will add itself to the Registry and receive a unique ID.
   */
  Base(Type type, const char *name, Access access, const char *units, const char *help,
       const char *fmt = "");

  /// \brief should write its value to buffer of sufficient size
  virtual void serialize_value(void *write_buffer) = 0;

  /// \brief retrieves its own value from buffer
  virtual void deserialize_value(const void *read_buffer) = 0;

  /// \returns number of bytes occupied by value, so caller can ensure sufficient size of buffer
  virtual size_t byte_size() const = 0;

  const char *name() const;

  /// \brief prefixes the variable name with given prefix. If prefix does not end with an '_', one
  //         is automatically added unless strict is set to true.
  /// \param strict can be set to true to prevent adding an '_' between prefix and var name
  void prepend_name(const char *prefix, bool strict = false);
  /// \brief appends the help string with given text. If text does not start with a space, one
  //         is automatically added unless strict is set to true.
  /// \param strict can be set to true to prevent adding a space between help string and text
  void append_help(const char *text, bool strict = false);

  const char *format() const;
  const char *help() const;
  const char *units() const;
  Type type() const;
  uint16_t id() const;
  Access access() const;
  bool write_allowed() const;

 private:
  uint16_t id_{InvalidID};
  const Type type_;
  const Access access_;
  char name_[50]{0};
  char units_[20]{0};
  char help_[300]{0};
  char fmt_[10]{0};

  friend class Registry;
};

/*! \class Registry vars_base.h "vars_base.h"
 *  \brief Registry for keeping track of extant debug variables
 *
 * This is a singleton for keeping track of all debug variables.
 */
// \todo deregister variables upon destruction
class Registry {
 public:
  // this is the only way to access it
  static Registry &singleton() {
    // will privately initialize on first call
    static Registry SingletonInstance;
    // will always return
    return SingletonInstance;
  }

  /// \brief adds variable to registry and issues it a unique ID
  void register_variable(Base *var);

  /*! \param vid ID of variable to be found
   *  \returns pointer to variable if found, else nullptr
   */
  Base *find(uint16_t vid);

  /*! \returns number of debug variables registered
   */
  uint16_t count() const;

 private:
  Base *var_list_[MaxVariableCount]{};
  uint16_t var_count_{0};

  // singleton assurance, because these are private
  Registry() = default;              // cannot default initialize
  Registry(Registry const &);        // cannot copy initialize
  void operator=(Registry const &);  // cannot copy assign
};

}  // namespace Debug::Variable
