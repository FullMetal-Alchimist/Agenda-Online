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

    for(QListIterator<Connection*> it(m_SignalsMapper) ; it.hasNext() ;)
    {
        Connection* connection = it.next();
        QObject::connect(serveur, connection->SignalName, connection->ObjectReceiver, connection->MemberName/*, connection->ConnectionType*/);
    }

    QObject::connect(serveur, SIGNAL(finished()), this, SLOT(deleteLater()));

    serveur->start();
}
void Server::connect(const char *signal, const QObject *receiver, const char *member, Qt::ConnectionType connection)
{
    m_SignalsMapper << new Connection(signal, member, receiver, connection);
}


ServerManager::ServerManager(QObject *parent) : QObject(parent)
{

}
void ServerManager::AddServeur(AbstractServeur *serveurType)
{
    Server* server = new Server(this, serveurType);

    m_TcpServers[serveurType] = server;
}
bool ServerManager::Listen(AbstractServeur *serverAbstract, const QHostAddress &hostAddress, int Port)
{
    if(m_TcpServers.contains(serverAbstract))
        return m_TcpServers[serverAbstract]->listen(hostAddress, Port);
    return false;
}
void ServerManager::Connect(AbstractServeur *serverAbstract, const char *signal, const QObject *receiver, const char *member, Qt::ConnectionType connection)
{
    if(m_TcpServers.contains(serverAbstract))
        m_TcpServers[serverAbstract]->connect(signal, receiver, member, connection);
}
