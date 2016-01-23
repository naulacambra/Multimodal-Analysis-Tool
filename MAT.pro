#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T22:00:59
#
#-------------------------------------------------

QT += core gui opengl multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MAT
TEMPLATE = app


SOURCES += \
    Sources/main.cpp \
    Sources/log.cpp \
    Sources/menu.cpp \
    Sources/clickablelabel.cpp \
    Sources/functions.cpp \
    Sources/visualizegl.cpp \
    Sources/customslider.cpp \
    Sources/definevisualization.cpp \
    Sources/styles.cpp

HEADERS += \
    #Headers/main.h \
    Headers/log.h \
    Headers/menu.h \
    Headers/clickablelabel.h \
    Headers/functions.h \
    Headers/visualizegl.h \
    Headers/puntos.h \
    Headers/videolog.h \
    Headers/customslider.h \
    Headers/definevisualization.h \
    Headers/styles.h

FORMS += \
    #Forms/main.ui \
    Forms/loader.ui \
    Forms/menu.ui \
    Forms/visualizegl.ui \
    Forms/definevisualization.ui \

DISTFILES += \
    README.md
