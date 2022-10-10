CONFIG += c++11

inc = $$PWD/inc
src = $$PWD/src

INCLUDEPATH += \
    $$inc \
    $$inc/serializer

HEADERS += \
    $$inc/init.h \
    $$inc/settingsDeclaration.h \
    $$inc/setting.h \
    $$inc/settingGroup.h \
    $$inc/serializer/serializer.h \

SOURCES += \
    $$src/init.cpp \
    $$src/setting.cpp \
    $$src/settingGroup.cpp \
