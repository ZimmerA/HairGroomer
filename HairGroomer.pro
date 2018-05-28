#-------------------------------------------------
#
# Project created by QtCreator 2018-05-03T18:45:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HairGroomer
TEMPLATE = app


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
HEADERS += \
        glwidget.h \
        mainwindow.h \
        meshData.h \
        modelData.h \
        mvpmodel.h \
        mvppresenter.h \
    glMesh.h \
    glModel.h

SOURCES += \
        glwidget.cpp \
        main.cpp \
        mainwindow.cpp \
        meshData.cpp \
        modelData.cpp \
        mvpmodel.cpp \
        mvppresenter.cpp \
    glMesh.cpp \
    glModel.cpp



FORMS += \
        mainwindow.ui


INCLUDEPATH += $$PWD/libs/linalg \
               $$PWD/libs/stbi_image

DISTFILES += \
    frag.frag \
    vert.vert \
    uvmap.vert \
    uvmap.frag \
    drawbuffer.frag \
    drawbuffer.vert

RESOURCES += \
    assets.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/assimp/lib/release/ -lassimp-vc140-mt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/assimp/lib/debug/ -lassimp-vc140-mt

INCLUDEPATH += $$PWD/libs/assimp/include
DEPENDPATH += $$PWD/libs/assimp/include
