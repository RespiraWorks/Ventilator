set(this_target ${PROJECT_NAME}_app)

find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(OpenGL REQUIRED)

#include(InstallConfig)

#include(BuildInfo)
execute_process(COMMAND "git" "rev-parse" "HEAD"
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
  OUTPUT_VARIABLE GIT_SHA1
  OUTPUT_STRIP_TRAILING_WHITESPACE)
add_definitions(-DGIT_VERSION="${GIT_SHA1}")

include(QtLibraryConfig)

message(STATUS "Using QtCore v. ${Qt5Widgets_VERSION_STRING}")

set(${this_target}_sources
  main.cpp
  )

#set(${this_target}_headers
#  )

qt5_add_resources(${this_target}_resources
  qml.qrc
  controls/controls.qrc
  fonts/fonts.qrc
  images/images.qrc
  modes/modes.qrc
  sounds/sounds.qrc
  )

#DISTFILES += images/Logo.png \
#controls/AlarmButton.qml

#qt5_add_translation(${this_target}_translations
#  ProjectVentilatorGUI_es_GT.ts)

add_executable(
  ${this_target}
  ${${this_target}_sources}
  ${${this_target}_resources}
#  ${${this_target}_translations}
#  ${${this_target}_headers}
)

target_include_directories(
  ${this_target}
  PRIVATE ${PROJECT_SOURCE_DIR}/app
)

target_link_libraries(
  ${this_target}
  ${PROJECT_NAME}_src
  qnanopainter
  fmt::fmt
  Threads::Threads
  OpenGL::GL
  Qt5::Core
  Qt5::SerialPort
  Qt5::Gui
  Qt5::Quick
  Qt5::Multimedia
)
