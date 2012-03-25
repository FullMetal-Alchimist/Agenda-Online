/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 12/07/2011
**
**                      Server.cpp:Server
**************************************************************************/

#include "Server.hpp"

Server::Server(QObject* parent, AbstractServeur* Type) :
    QTcpServer(parent), m_BaseType(Type)
{
}

void Server::incomingConnection(int handle)
{
    AbstractServeur* serveur = m_BaseType->Clone();
    serveur->setHandle(handle);

    emit message(tr("New incoming connection!"));

    connect(serveur, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(serveur, SIGNAL(newClient(QString,QString)), this, SIGNAL(newClient(QString,QString)));
    connect(serveur, SIGNAL(removeClient(QString,QString)), this, SIGNAL(removeClient(QString,QString)));
    connect(serveur, SIGNAL(finished()), this, SLOT(deleteLater()));

    serveur->start();
}


ServerManager::ServerManager(QObject *parent) : QObject(parent)
{

}
void ServerManager::AddServeur(int Port, AbstractServeur *serveurType)
{
    Server* server = new Server(this, serveurType);
    server->listen(QHostAddress::Any, Port);

    connect(server, SIGNAL(message(QString)), this, SIGNAL(message(QString)));
    connect(server, SIGNAL(newClient(QString,QString)), this, SIGNAL(newClient(QString,QString)));
    connect(server, SIGNAL(removeClient(QString,QString)), this, SIGNAL(removeClient(QString,QString)));

    m_TcpServers[serveurType] = server;
}
void ServerManager::Listen(AbstractServeur *serverAbstract, const QHostAddress &hostAddress, int Port)
{
    if(m_TcpServers.contains(serverAbstract))
        m_TcpServers[serverAbstract]->listen(hostAddress, port);
}
