CONFIG += c++11

inc = $$PWD/inc
src = $$PWD/src

INCLUDEPATH += \
    $$inc \
    $$inc/serializer

HEADERS += \
    $$PWD/inc/JsonSetting.h \
    $$PWD/inc/JsonSettingGroup.h \
    $$inc/init.h \
    $$inc/SettingsDeclaration.h \
    $$inc/Setting.h \
    $$inc/SettingGroup.h \
    $$inc/serializer/IJsonSerializable.h \

SOURCES += \
    $$PWD/src/JsonSettingGroup.cpp \
    $$src/JsonSetting.cpp \
    $$src/init.cpp \
    $$src/Setting.cpp \
    $$src/SettingGroup.cpp \
