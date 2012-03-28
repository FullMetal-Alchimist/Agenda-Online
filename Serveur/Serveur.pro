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
    AuthentificationSystem.hpp \
    Canal.hpp \
    Headers.hpp \
    FenAddDevoir.hpp \
    FenAccount.hpp

SOURCES += \
    SQLServerSupervisor.cpp \
    Server.cpp \
    main.cpp \
    FenPrincipale.cpp \
    Devoir.cpp \
    AbstractServeur.cpp \
    MainServeur.cpp \
    ChatServeur.cpp \
    AuthentificationSystem.cpp \
    Canal.cpp \
    FenAddDevoir.cpp \
    FenAccount.cpp

FORMS += \
    FenPrincipale.ui \
    FenAddDevoir.ui \
    FenAccount.ui
















































