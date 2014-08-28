INCLUDEPATH += $$PWD

SOURCES += $$PWD/common.cpp \
        $$PWD/options.cpp \
        $$PWD/qompmetadataresolver.cpp \
        $$PWD/qompnetworkingfactory.cpp \
        $$PWD/qompplayer.cpp \
        $$PWD/tune.cpp \
        $$PWD/pluginmanager.cpp \
        $$PWD/pluginhost.cpp


HEADERS += $$PWD/libqomp_global.h \
        $$PWD/common.h \
        $$PWD/options.h \        
        $$PWD/qompmetadataresolver.h \
        $$PWD/qompnetworkingfactory.h \
        $$PWD/qompplayer.h \
        $$PWD/tune.h \
        $$PWD/defines.h \
        $$PWD/pluginmanager.h \
        $$PWD/pluginhost.h


android {
    HEADERS += $$PWD/qompqmlengine.h

    SOURCES += $$PWD/qompqmlengine.cpp

    RESOURCES += $$PWD/qml.qrc
}
else {
    HEADERS +=  $$PWD/qompbusylabel.h

    SOURCES += $$PWD/qompbusylabel.cpp
}

lupdate_only {
    SOURCES += $$PWD/qmlshared/*.qml
}

include(plugins/plugins.pri)
include(options/options.pri)
