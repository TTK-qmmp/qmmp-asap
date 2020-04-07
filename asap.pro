include(../../plugins.pri)
include($$PWD/common/common.pri)

TARGET = $$PLUGINS_PREFIX/Input/asap

HEADERS += decoderasapfactory.h \
           decoder_asap.h \
           asaphelper.h \
           asapmetadatamodel.h \
           asap.h

SOURCES += decoderasapfactory.cpp \
           decoder_asap.cpp \
           asaphelper.cpp \
           asapmetadatamodel.cpp \
           asap.c

unix {
    target.path = $$PLUGIN_DIR/Input
    INSTALLS += target
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libasap.so
}
