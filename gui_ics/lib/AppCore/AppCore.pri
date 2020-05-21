include ("AppCore-local.pri")

LIBS *= -L$$builddir/lib

win32 {
    TARGET_VERSION_EXT = $$section(APPCORE_VERSION, ., 0, 0)
    isEqual(TARGET_VERSION_EXT, 0):unset(TARGET_VERSION_EXT)
    release: LIBS += $$sprintf( -lAppCore%1, $$TARGET_VERSION_EXT )
    else: $$sprintf( -lAppCore%1, $$TARGET_VERSION_EXT )
} else {
    LIBS += -lAppCore
}

INCLUDEPATH += $$IN_PWD
QML_IMPORT_PATH += $$IN_PWD/..
