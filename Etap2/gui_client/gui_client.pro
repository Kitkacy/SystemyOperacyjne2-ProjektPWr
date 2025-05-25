# Create a simple Qt chat client application
TEMPLATE = app
TARGET = gui_client
QT += core gui network widgets

# Set C++ standard
CONFIG += c++17

# Source files
SOURCES += main.cpp chatwindow.cpp
HEADERS += chatwindow.h

# Destination directory
DESTDIR = $$PWD/

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
