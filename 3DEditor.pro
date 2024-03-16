QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    engine3d.cpp \
    main.cpp \
    mainwindow.cpp \
    material.cpp \
    materiallibrary.cpp \
    object3d.cpp \
    widgetgl.cpp

HEADERS += \
    camera.h \
    engine3d.h \
    mainwindow.h \
    material.h \
    materiallibrary.h \
    object3d.h \
    transformational.h \
    widgetgl.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# OpenGL
QT += openglwidgets
LIBS += -lopengl32


# Assimp
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3dparty/assimp/release/ -lassimp-vc143-mtd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3dparty/assimp/debug/ -lassimp-vc143-mtd
else:unix: LIBS += -L$$PWD/3dparty/assimp/ -lassimp-vc143-mtd

INCLUDEPATH += $$PWD/3dparty/assimp/include
DEPENDPATH += $$PWD/3dparty/assimp/include

DISTFILES +=

RESOURCES += \
    models.qrc \
    shaders.qrc \
    textures.qrc
