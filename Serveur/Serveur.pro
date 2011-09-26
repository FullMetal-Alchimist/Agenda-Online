QT       += core gui network sql

TARGET = Serveur
TEMPLATE = app

HEADERS += \
    SQLServerSupervisor.hpp \
    Serveur.hpp \
    Server.hpp \
    FenPrincipale.hpp \
    Devoir.hpp \
    ChatServer.hpp \
    Commands.hpp \
    Handles.hpp

SOURCES += \
    SQLServerSupervisor.cpp \
    Serveur.cpp \
    Server.cpp \
    main.cpp \
    FenPrincipale.cpp \
    Devoir.cpp \
    ChatServer.cpp \
    Commands.cpp \
    Handles.cpp

FORMS += \
    FenPrincipale.ui







