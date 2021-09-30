set(this_target qnanopainter)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

# Enable this to get drawing debug information
#add_definitions(-DQNANO_DEBUG)

# Enable this to let Qt include OpenGL headers
add_definitions(-DQNANO_QT_GL_INCLUDE)

# This will enable GLES3 (disable to force GLES2)
add_definitions(-DQNANO_ENABLE_GLES3)

# This will enable signalling touch events
# Can be useful when using view/widget classes directly
#add_definitions(-DQNANO_ENABLE_TOUCH_SIGNALS)

# This will enable signalling paint events
# Can be useful when using view/widget classes directly
#add_definitions(-DQNANO_ENABLE_PAINT_SIGNALS)

# When building for embedded devices you can define manually which backends are supported
set(QNANO_BUILD_GLES_BACKENDS OFF)
set(QNANO_BUILD_GL_BACKENDS ON)

# \TODO: autodetect backend

if(QNANO_BUILD_GLES_BACKENDS)
  message(STATUS "QNanoPainter: Including OpenGL ES backends")
  add_definitions(-DQNANO_BUILD_GLES_BACKENDS)

  set(${this_target}_backend_sources
    qnanopainter/libqnanopainter/private/qnanobackendgles2.cpp
    qnanopainter/libqnanopainter/private/qnanobackendgles3.cpp
    )
  set(${this_target}_backend_headers
    qnanopainter/libqnanopainter/private/qnanobackendgles2.h
    qnanopainter/libqnanopainter/private/qnanobackendgles3.h
    )
endif()

if(QNANO_BUILD_GL_BACKENDS)
  message(STATUS "QNanoPainter: Including OpenGL backends")
  add_definitions(-DQNANO_BUILD_GL_BACKENDS)

  set(${this_target}_backend_sources
    qnanopainter/libqnanopainter/private/qnanobackendgl2.cpp
    qnanopainter/libqnanopainter/private/qnanobackendgl3.cpp
    )
  set(${this_target}_backend_headers
    qnanopainter/libqnanopainter/private/qnanobackendgl2.h
    qnanopainter/libqnanopainter/private/qnanobackendgl3.h
    )
endif()

set(${this_target}_sources
  qnanopainter/libqnanopainter
  qnanopainter/libqnanopainter/qnanopainter.cpp
  qnanopainter/libqnanopainter/qnanocolor.cpp
  qnanopainter/libqnanopainter/qnanolineargradient.cpp
  qnanopainter/libqnanopainter/qnanoimagepattern.cpp
  qnanopainter/libqnanopainter/qnanoimage.cpp
  qnanopainter/libqnanopainter/qnanofont.cpp
  qnanopainter/libqnanopainter/qnanoradialgradient.cpp
  qnanopainter/libqnanopainter/qnanoboxgradient.cpp
  qnanopainter/libqnanopainter/qnanowindow.cpp
  qnanopainter/libqnanopainter/private/qnanodebug.cpp
  qnanopainter/libqnanopainter/nanovg/nanovg.c
  ${${this_target}_backend_sources}
  )

set(${this_target}_headers
  qnanopainter/libqnanopainter/private/qnanobrush.h
  qnanopainter/libqnanopainter/qnanopainter.h
  qnanopainter/libqnanopainter/qnanocolor.h
  qnanopainter/libqnanopainter/qnanolineargradient.h
  qnanopainter/libqnanopainter/qnanoimagepattern.h
  qnanopainter/libqnanopainter/qnanoimage.h
  qnanopainter/libqnanopainter/qnanofont.h
  qnanopainter/libqnanopainter/qnanoradialgradient.h
  qnanopainter/libqnanopainter/qnanoboxgradient.h
  qnanopainter/libqnanopainter/private/qnanodataelement.h
  qnanopainter/libqnanopainter/private/qnanobackend.h
  qnanopainter/libqnanopainter/private/qnanobackendfactory.h
  qnanopainter/libqnanopainter/qnanowindow.h
  qnanopainter/libqnanopainter/private/qnanodebug.h
  qnanopainter/libqnanopainter/nanovg/nanovg.h
  ${${this_target}_backend_headers}
  )

qt5_add_resources(${this_target}_resources
  qnanopainter/libqnanopainter/libqnanopainterdata.qrc)

if(Qt5Quick_FOUND)
  message(STATUS "QNanoPainter: Including QtQuick components")
  set(${this_target}_sources
    ${${this_target}_sources}
    qnanopainter/libqnanopainter/qnanoquickitem.cpp
    qnanopainter/libqnanopainter/qnanoquickitempainter.cpp
    )
  set(${this_target}_headers
    ${${this_target}_headers}
    qnanopainter/libqnanopainter/qnanoquickitem.h
    qnanopainter/libqnanopainter/qnanoquickitempainter.h
    )
  set(${this_target}_libs
    ${${this_target}_libs}
    PUBLIC Qt5::Quick
    )
endif()

if(Qt5Widgets_FOUND)
  message(STATUS "QNanoPainter: Including QtQuick components")
  set(${this_target}_sources
    ${${this_target}_sources}
    qnanopainter/libqnanopainter/qnanowidget.cpp
    )
  set(${this_target}_headers
    ${${this_target}_headers}
    qnanopainter/libqnanopainter/qnanowidget.h
    )
  set(${this_target}_libs
    ${${this_target}_libs}
    PUBLIC Qt5::Widgets
    )
endif()

add_library(
  ${this_target} STATIC
  ${${this_target}_headers}
  ${${this_target}_sources}
  ${${this_target}_resources}
)

target_include_directories(
  ${this_target}
  PUBLIC qnanopainter/libqnanopainter/private
  PUBLIC qnanopainter/libqnanopainter
  INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(
  ${this_target}
  PUBLIC Qt5::Core
  PUBLIC Qt5::Gui
  ${${this_target}_libs}
)
