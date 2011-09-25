QT       += core gui network
TARGET = Client
TEMPLATE = app

HEADERS += \
    FenDevoir.hpp \
    FenAccueil.hpp \
    Devoir.hpp \
    Client.hpp

SOURCES += \
    main.cpp \
    FenDevoir.cpp \
    FenAccueil.cpp \
    Devoir.cpp \
    Client.cpp

FORMS += \
    FenDevoir.ui \
    FenAccueil.ui

debug:DEFINES += _DEBUG
release:DEFINES -= _DEBUG



















