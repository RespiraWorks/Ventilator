include ("GreenHouseLight-local.pri")

LIBS *= -L$$builddir/lib

win32 {
    TARGET_VERSION_EXT = $$section(GREENHOUSE_VERSION, ., 0, 0)
    isEqual(TARGET_VERSION_EXT, 0):unset(TARGET_VERSION_EXT)
    release: LIBS += $$sprintf( -lGreenHouseLight%1, $$TARGET_VERSION_EXT )
    else: $$sprintf( -lGreenHouseLight%1, $$TARGET_VERSION_EXT )
} else {
    LIBS += -lGreenHouseLight
}

QT += websockets
INCLUDEPATH += $$IN_PWD
QML_IMPORT_PATH += $$IN_PWD/..
