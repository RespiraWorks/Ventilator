INCLUDEPATH += $$IN_PWD/global

isEmpty(builddir) {
  builddir=$$shadowed($$PWD)
}
