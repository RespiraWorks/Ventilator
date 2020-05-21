*-g++* {
  NORMAL_CFLAGS = -Wno-long-long -ansi -Wno-variadic-macros
  win32-g++ {
    NORMAL_CFLAGS += -U__STRICT_ANSI__
  }

  NORMAL_CXXFLAGS = \
    -Wnon-virtual-dtor -Wundef -Wcast-align \
    -Wchar-subscripts -Wpointer-arith \
    -Wwrite-strings -Wpacked -Wformat-security \
    -Wmissing-format-attribute -Woverloaded-virtual -Werror=reorder \
    -Werror=return-type -Werror=missing-include-dirs -Werror=conversion-null \
    -Werror=enum-compare \
    -Wall -Wextra

  NORMAL_CFLAGS += -pedantic

  CONFIG(debug, debug|release) {
    NORMAL_CXXFLAGS += -O0 -g3
    NORMAL_CXXFLAGS -= -g
  }

  QMAKE_CFLAGS   += $$NORMAL_CFLAGS
  QMAKE_CXXFLAGS += $$NORMAL_CFLAGS $$NORMAL_CXXFLAGS

}
