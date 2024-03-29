QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    gui/mainwindow.cpp \
    material/material.cpp \
    object/camera/camera.cpp \
    object/light/baselight.cpp \
    object/light/light_source/lightsource.cpp \
    object/model/polyhedron/polygon.cpp \
    object/model/polyhedron/polyhedron.cpp \
    object/model/sphere/sphere.cpp \
    picture/picture.cpp

HEADERS += \
    gui/mainwindow.h \
    material/material.h \
    object/camera/camera.h \
    object/light/baselight.h \
    object/light/light_source/lightsource.h \
    object/model/model.h \
    object/model/polyhedron/polygon.h \
    object/model/polyhedron/polyhedron.h \
    object/model/sphere/sphere.h \
    object/object.h \
    objloader/objloader.h \
    picture/picture.h

FORMS += \
    gui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += \
    gui \
    material \
    object \
    object/camera \
    object/light \
    object/light/inf_light \
    object/light/light_source \
    object/model \
    object/model/polyhedron \
    object/model/sphere \
    picture \
    objloader \
    obj

RESOURCES += \
    gui/icons.qrc
