/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 24/09/2011
**
**                      ChatServer.cpp:ChatServer
**************************************************************************/

#include "ChatServer.hpp"
#include "Serveur.hpp"

ChatServer* ChatServer::_instance = NULL;

ChatServer::ChatServer() :
    QObject()
{
}
ChatServer::~ChatServer()
{

}

void ChatServer::AddClient(QTcpSocket *socket, const QString &nom, const QString &classe)
{
    clientsBySocket.insert(socket, QPair<QString, QString>(nom, classe));
    clientsByNameAndClass.insert(QPair<QString, QString>(nom, classe), socket);
}
void ChatServer::SendMessageAt(const QString &nom, const QString &message, const QString &classe)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) Serveur::SMSG_MESSAGE_CHAT;
    out << nom;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));


    for(QMapIterator<QTcpSocket*, QPair<QString, QString> > it(clientsBySocket) ; it.hasNext() ;)
    {
        QMap<QTcpSocket*, QPair<QString, QString> >::const_iterator i = it.next();
        QTcpSocket* client = i.key();
        QString classeOfClient = i.value().second;
        if(classeOfClient != classe && classe != tr("All"))
            continue;

        client->write(paquet);
    }
}
void ChatServer::SendSystemMessage(const QString &message)
{
    SendMessageAt(tr("System"), message, tr("All"));
}
void ChatServer::RemoveClient(const QPair<QString, QString> &NameAndClass)
{
    QTcpSocket* socket = clientsByNameAndClass.take(NameAndClass);
    clientsBySocket.remove(socket);
}
void ChatServer::RemoveClient(QTcpSocket *Client)
{
    QPair<QString, QString> NameAndClass = clientsBySocket.take(Client);
    clientsByNameAndClass.remove(NameAndClass);
}

void ChatServer::SendPrivateMessage(const QString &nom, const QString &message, const QString &destNom, const QString &destClass)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) Serveur::SMSG_MESSAGE_CHAT;
    out << nom;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    if(clientsByNameAndClass.find(QPair<QString, QString>(destNom, destClass)) != clientsByNameAndClass.end())
        clientsByNameAndClass[QPair<QString, QString>(destNom, destClass)]->write(paquet);
}

bool ChatServer::HasClient(const QPair<QString, QString> &NameAndClass)
{
    return clientsByNameAndClass.contains(NameAndClass);
}
bool ChatServer::HasClient(QTcpSocket *Client)
{
    return clientsBySocket.contains(Client);
}

bool ChatServer::CreateInstance()
{
    if(_instance == NULL)
    {
        _instance = new ChatServer;
        return true;
    }
    else
    {
        return false;
    }
}
ChatServer* ChatServer::GetInstance()
{
    if(_instance == NULL)
        CreateInstance();

    return _instance;
}
bool ChatServer::KillInstance()
{
    if(_instance != NULL)
        delete _instance;

    return true;
}

