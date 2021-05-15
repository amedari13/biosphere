greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = simulation
TEMPLATE = lib
CONFIG += staticlib


SOURCES += \
    being.cpp \
    biosphere.cpp \
    capability.cpp \
    environment.cpp \
    global_scene.cpp \
    pch.cpp \
    position.cpp \
    simulation.cpp \
    species.cpp

HEADERS += \
    being.h \
    biosphere.h \
    capability.h \
    environment.h \
    framework.h \
    global_scene.h \
    pch.h \
    position.h \
    species.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
