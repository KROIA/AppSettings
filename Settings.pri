CONFIG += c++11

inc = $$PWD/inc/
src = $$PWD/src/

INCLUDEPATH += \
    $$inc \
    $$inc/serializer

HEADERS += \
    $$PWD/inc/init.h \
    $$PWD/inc/settingsDeclaration.h \
    $$inc/setting.h \
    $$inc/settings.h \
    $$inc/serializer/serializer.h \

SOURCES += \
    $$PWD/src/init.cpp \
    $$src/setting.cpp \
    $$src/settings.cpp \
