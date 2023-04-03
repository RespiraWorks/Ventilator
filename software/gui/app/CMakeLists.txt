set(this_target ${PROJECT_NAME}_app)

find_package(Qt6 COMPONENTS Qml REQUIRED)

message(STATUS "Using QtCore v. ${Qt6Qml_VERSION_STRING}")

set(${this_target}_sources
  main.cpp
  )

#set(${this_target}_headers
#  )

qt6_add_resources(${this_target}_resources
  qml.qrc
  controls/controls.qrc
  fonts/fonts.qrc
  images/images.qrc
  modes/modes.qrc
  sounds/sounds.qrc
  )

#qt_add_translation(${this_target}_translations
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
  ${PROJECT_NAME}_backend
  Qt6::Qml
)

#DISTFILES += images/Logo.png \
#controls/AlarmButton.qml
