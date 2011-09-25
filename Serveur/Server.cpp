/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 12/07/2011
**
**                      Server.cpp:Server
**************************************************************************/

#include "Server.hpp"

Server::Server(QObject *parent) :
    QTcpServer(parent)
{
}

void Server::incomingConnection(int handle)
{
    Serveur* serveur = new Serveur(handle);

    emit message(tr("New incoming connection!"));

    connect(serveur, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(serveur, SIGNAL(newClient(QString,QString)), this, SIGNAL(newClient(QString,QString)));
    connect(serveur, SIGNAL(removeClient(QString,QString)), this, SIGNAL(removeClient(QString,QString)));
    connect(serveur, SIGNAL(finished()), this, SLOT(deleteLater()));

    serveur->start();
}
