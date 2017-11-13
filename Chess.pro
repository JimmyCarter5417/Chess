#-------------------------------------------------
#
# Project created by QtCreator 2017-11-02T19:45:41
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Chess
TEMPLATE = app


include(src/src.pri)

FORMS    += chess.ui

RESOURCES += \
    res.qrc

CONFIG += c++14

DISTFILES +=

RC_ICONS = res/pic/icon/XQF.ICO

INCLUDEPATH += src/include/