QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH = ../simulation
LIBS += ../simulation/debug/simulation.lib

SOURCES += \
    database_work.cpp \
    main.cpp \
    mainwindow.cpp \
    resize_field_window.cpp \
    scene.cpp \
    universe_output_window.cpp

HEADERS += \
    database_work.h \
    mainwindow.h \
    resize_field_window.h \
    scene.h \
    universe_output_window.h

FORMS += \
    mainwindow.ui \
    resize_field_window.ui \
    universe_output_window.ui

TRANSLATIONS += \
    biosphere_graphical_unit_qt_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
