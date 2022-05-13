include($$PWD/../../plugins.pri)

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS += -std=gnu11

TARGET = $$PLUGINS_PREFIX/Input/asap

HEADERS += decoderasapfactory.h \
           decoder_asap.h \
           asaphelper.h

SOURCES += decoderasapfactory.cpp \
           decoder_asap.cpp \
           asaphelper.cpp \
           libasap/asap.c

INCLUDEPATH += $$PWD/libasap

unix {
    target.path = $$PLUGIN_DIR/Input
    INSTALLS += target
    QMAKE_CLEAN = $$PLUGINS_PREFIX/Input/libasap.so
}
