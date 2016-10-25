#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T12:21:58
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Paint2
TEMPLATE = app
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11 # To compile on CADE machines
DEFINES += MAGICKCORE_QUANTUM_DEPTH=16 MAGICKCORE__ENABLE=0 MAGICKCORE_HDRI_ENABLE=0 GIFEXPORT=1

SOURCES += main.cpp\
        mainwindow.cpp \
    canvas.cpp \
    frame.cpp \
    layer.cpp \
    pixelarray.cpp \
    tool.cpp \
    penciltool.cpp \
    gifexporter.cpp \
    linetool.cpp \
    previewtool.cpp \
    rectangletool.cpp \
    colorpicker.cpp \
    bucketfill.cpp \
    eraser.cpp \
    newformdialog.cpp \
    projectfile.cpp \
    color.cpp \
    ellipsetool.cpp \
    previewwindow.cpp \
    sprayertool.cpp

HEADERS  += mainwindow.h \
    canvas.h \
    frame.h \
    layer.h \
    pixelarray.h \
    tool.h \
    penciltool.h \
    toolDefinitions.h \
    gifexporter.h \
    linetool.h \
    previewtool.h \
    rectangletool.h \
    colorpicker.h \
    bucketfill.h \
    eraser.h \
    newformdialog.h \
    projectfile.h \
    color.h \
    ellipsetool.h \
    previewwindow.h \
    sprayertool.h

FORMS    += mainwindow.ui \
            newformdialog.ui

unix:INCLUDEPATH += /usr/include/ImageMagick
unix:LIBS += -lMagick++ -lMagickCore -lMagickWand

win32:!win32-g++: INCLUDEPATH += $$PWD/../magick++/windows
win32:!win32-g++: DEPENDPATH += $$PWD/../magick++/windows
win32:!win32-g++: INCLUDEPATH += $$PWD/../magick++/windows/wand
win32:!win32-g++: DEPENDPATH += $$PWD/../magick++/windows/wand
win32:!win32-g++: INCLUDEPATH += $$PWD/../magick++/windows/magick
win32:!win32-g++: DEPENDPATH += $$PWD/../magick++/windows/magick

win32:!win32-g++: LIBS += -L$$PWD/../magick++/windows -lCORE_RL_magick -lCORE_RL_wand -lCORE_RL_Magick++

win32-g++: INCLUDEPATH += $$PWD/../magick++/include
win32-g++: DEPENDPATH += $$PWD/../magick++/include
win32-g++: INCLUDEPATH += $$PWD/../magick++/include/wand
win32-g++: DEPENDPATH += $$PWD/../magick++/include/wand
win32-g++: INCLUDEPATH += $$PWD/../magick++/include/magick
win32-g++: DEPENDPATH += $$PWD/../magick++/include/magick

win32!win32-g++: LIBS += -L$$PWD/../magick++ -lMagick++-6.Q16 -lMagickWand-6.Q16 -lMagickCore-6.Q16

RESOURCES += \
    buttons.qrc
