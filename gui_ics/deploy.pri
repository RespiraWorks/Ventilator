include("global.pri")

unix:system(command -v rsync &> /dev/null) {
    QMAKE_COPY_DIR = "rsync -a"
}

win32 {
    propwd = $$replace(_PRO_FILE_PWD_, /, \\)
    pathSegments = $$split(propwd, \\)
    plugin = $$last(pathSegments)
    export($$plugin)
} else:unix {
    propwd = $$replace(_PRO_FILE_PWD_, \\\\, /)
    pathSegments = $$split(propwd, /)
    plugin = $$last(pathSegments)
    export($$plugin)
}

defineTest(qtcAddDeployment) {
for(deploymentfolder, DEPLOYMENTFOLDERS) {
    item = item$${deploymentfolder}
    greaterThan(QT_MAJOR_VERSION, 4) {
        itemsources = $${item}.files
    } else {
        itemsources = $${item}.sources
    }
    $$itemsources = $$eval($${deploymentfolder}.source)
    itempath = $${item}.path
    $$itempath= $$eval($${deploymentfolder}.target)
    export($$itemsources)
    export($$itempath)
    DEPLOYMENT += $$item
}

MAINPROFILEPWD = $$_PRO_FILE_PWD_

for(deploymentfolder, DEPLOYMENTFOLDERS) {
    !isEmpty($${deploymentfolder}.source):source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.source)
    else:source = $$MAINPROFILEPWD/$$eval($${deploymentfolder}.files)
    source = $$replace(source, /, \\)
    target = $$builddir/$$eval($${deploymentfolder}.target)
    unix|!win32: !isEmpty($${deploymentfolder}.source) {
        source = $$source/*
    }
    target = $$replace(target, /, \\)
    target ~= s,\\\\\\.?\\\\,\\,
    !isEqual(source,$$target) {
            unix|!win32: {
                source = $$replace(source, \\\\, /)
                target = $$replace(target, \\\\, /)
            }
            !isEmpty($${deploymentfolder}.source) {
                unix|!win32:$${deploymentfolder}.commands = @echo Copying application data... && $(MKDIR) \"$$target\" && $(COPY_DIR) $(wildcard $$source) \"$$target\"
                # robocopy will bomb the script due to the return value so we add "^& exit 0" as a work around
                else:$${deploymentfolder}.commands = @echo Copying application data... && ($(CHK_DIR_EXISTS) \"$$target\" ($(MKDIR) \"$$target\")) && (robocopy \"$$source\" \"$$target\" /s /e /nfl /njh /njs /ndl /XO) ^& exit 0

                QMAKE_CLEAN += \"$$target\"
            } else {
                unix|!win32: $${deploymentfolder}.commands = @echo Copying application data... && $(MKDIR) \"$$target\" && $(COPY_FILE) $(wildcard $$source) \"$$target\"
                else: $${deploymentfolder}.commands = @echo Copying application data... && ($(CHK_DIR_EXISTS) \"$$target\" ($(MKDIR) \"$$target\")) && $(COPY_FILE) \"$$source\" \"$$target\"

                QMAKE_CLEAN += \"$$target\"
            }
            $${deploymentfolder}.target = deploy_$$eval($${deploymentfolder}.target)
            export ($${deploymentfolder}.target)
            !isEmpty(depends) {
                $${deploymentfolder}.depends = $$depends
                export($${deploymentfolder}.depends)
            }
            export($${deploymentfolder}.commands)

            QMAKE_EXTRA_TARGETS += $$deploymentfolder
            isEmpty(depends) {
                depends = $$first(DEPLOYMENTFOLDERS)
            } else {
                depends = $$deploymentfolder
            }
            export(depends)
    }
}
!isEmpty(DEPLOYMENTFOLDERS) {
    first.depends = $(first) $$last(DEPLOYMENTFOLDERS)
    export(first.depends)
    QMAKE_EXTRA_TARGETS += first
}

win32* {
    QMAKE_DEL_FILE = $$QMAKE_DEL_FILE /S /F /Q
    export ( QMAKE_DEL_FILE )
} else {
    QMAKE_DEL_FILE = $$QMAKE_DEL_FILE -R
}

export (QMAKE_DEL_FILE)
export (QMAKE_CLEAN)
export (DEPLOYMENT)
export (QMAKE_EXTRA_TARGETS)
}
