QT -= gui
QT += testlib

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../Settings.pri)

inc = $$PWD/inc/
src = $$PWD/src/


INCLUDEPATH += $$inc

HEADERS += \
    $$inc/unittest.h \
    inc/settingTest.h \
    inc/subTest.h

SOURCES += \
    $$src/main.cpp \
    $$src/unittest.cpp \
    src/settingTest.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


