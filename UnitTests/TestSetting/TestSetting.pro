QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app


include(../../settings.pri)

SOURCES +=  tst_testsetting.cpp

