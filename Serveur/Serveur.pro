QT       += core gui network sql

TARGET = Serveur
TEMPLATE = app

HEADERS += \
    SQLServerSupervisor.hpp \
    Server.hpp \
    FenPrincipale.hpp \
    Devoir.hpp \
    AbstractServeur.hpp \
    MainServeur.hpp \
    ChatServeur.hpp \
    AuthentificationSystem.hpp

SOURCES += \
    SQLServerSupervisor.cpp \
    Server.cpp \
    main.cpp \
    FenPrincipale.cpp \
    Devoir.cpp \
    AbstractServeur.cpp \
    MainServeur.cpp \
    ChatServeur.cpp \
    AuthentificationSystem.cpp

FORMS += \
    FenPrincipale.ui

































