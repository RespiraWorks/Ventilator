exists( g++.pri ):include( g++.pri )
exists( clang.pri ):include( clang.pri )

unix:!symbian {
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
    UI_DIR = .ui
    RCC_DIR = .qrc
} else {
    MOC_DIR = _moc
    OBJECTS_DIR = _obj
    UI_DIR = _ui
    RCC_DIR = _qrc
}

DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_URL_CAST_FROM_STRING

win32 {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    CONFIG -= debug_and_release
    QMAKE_PROJECT_DEPTH = 0
}

DEFINES += DISABLE_QT_INTERNALS_LOGGING

CONFIG += c++11
