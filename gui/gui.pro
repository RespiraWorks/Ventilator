TEMPLATE = subdirs  # This changes to the subdirs function.  You can't combine
                    # compiling code and the subdirs function in the same .pro
                    # file.

# By default, you assign a directory to the SUBDIRS variable, and qmake looks
# inside that directory for a <dirname>.pro file.
SUBDIRS = app tests

# You can also use this to specify dependencies.  In this case, we don't want
# the build target to run until after the logic and gui targets are complete.
build.depends = app
