#-------------------------------------------------
#
# Project created by QtCreator 2016-01-22T22:00:59
#
#-------------------------------------------------

QT += core gui opengl multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MAT
TEMPLATE = app

isEmpty(TARGET_EXT) {
    win32 {
        TARGET_CUSTOM_EXT = .exe
    }
    macx {
        TARGET_CUSTOM_EXT = .app
    }
} else {
    TARGET_CUSTOM_EXT = $${TARGET_EXT}
}

win32 {
    DEPLOY_COMMAND = windeployqt
}
macx {
    DEPLOY_COMMAND = macdeployqt
}

CONFIG( debug, debug|release ) {
    # debug
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/debug/$${TARGET}$${TARGET_CUSTOM_EXT}))
} else {
    # release
    DEPLOY_TARGET = $$shell_quote($$shell_path($${OUT_PWD}/release/$${TARGET}$${TARGET_CUSTOM_EXT}))
}

#  # Uncomment the following line to help debug the deploy command when running qmake
#  warning($${DEPLOY_COMMAND} $${DEPLOY_TARGET})

QMAKE_POST_LINK = $${DEPLOY_COMMAND} $${DEPLOY_TARGET}

SOURCES += \
    Sources/main.cpp \
    Sources/log.cpp \
    Sources/menu.cpp \
    Sources/clickablelabel.cpp \
    Sources/functions.cpp \
    Sources/customslider.cpp \
    Sources/definevisualization.cpp \
    Sources/styles.cpp \
    Sources/MainApp.cpp

HEADERS += \
    #Headers/main.h \
    Headers/log.h \
    Headers/menu.h \
    Headers/clickablelabel.h \
    Headers/functions.h \
    Headers/puntos.h \
    Headers/videolog.h \
    Headers/customslider.h \
    Headers/definevisualization.h \
    Headers/styles.h \
    Headers/MainApp.h

FORMS += \
    #Forms/main.ui \
    Forms/loader.ui \
    Forms/menu.ui \
    Forms/definevisualization.ui \
    Forms/MainApp.ui

DISTFILES += \
    README.md
