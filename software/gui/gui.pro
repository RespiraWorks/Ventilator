# Structured per https://dragly.org/2014/03/13/new-project-structure-for-projects-in-qt-creator-with-unit-tests/
TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src app tests

# build.depends = app
app.depends = src
tests.depends = src
