TEMPLATE = subdirs
DEFINES += QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII

GreenHouseLight_lib.subdir = lib/GreenHouseLight
GreenHouseLight_lib.target = GreenHouseLight-lib

GreenHouse_app_core.subdir = lib/AppCore
GreenHouse_app_core.target = GreenHouse-app-core
GreenHouse_app_core.depends = GreenHouseLight-lib

GreenHouse_app.subdir = src/App
GreenHouse_app.target = GreenHouse-app
GreenHouse_app.depends = GreenHouseLight-lib GreenHouse-app-core

SUBDIRS += GreenHouseLight_lib GreenHouse_app_core GreenHouse_app

translations.files = translations/*.qm
translations.target = translations
DEPLOYMENTFOLDERS += translations
include("deploy.pri")
qtcAddDeployment()

OTHER_FILES += _clang-format README.md
exists(scripts) {
    OTHER_FILES += scripts/*
}
