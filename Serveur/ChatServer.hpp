/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 24/09/2011
**
**                      ChatServer.hpp:ChatServer
**************************************************************************/

#ifndef CHATSERVER_HPP
#define CHATSERVER_HPP

#include <QtCore>
#include <QtNetwork>

class ChatServer : public QObject
{
    Q_OBJECT
public:
    void AddClient(QTcpSocket* socket, QString const& nom, QString const& classe);

    void SendMessageAt(QString const& nom, QString const& message, QString const& classe);
    void SendPrivateMessage(QString const& nom, QString const& message, QString const& destNom, QString const& destClass);
    void SendSystemMessage(QString const& message);

    void RemoveClient(QTcpSocket* Client);
    void RemoveClient(QPair<QString, QString> const& NameAndClass);

    bool HasClient(QTcpSocket* Client);
    bool HasClient(QPair<QString, QString> const& NameAndClass);

    static bool CreateInstance();
    static ChatServer* GetInstance();
    static bool KillInstance();

signals:

public slots:

protected:
    ChatServer();
    ~ChatServer();

    QMap<QTcpSocket*, QPair<QString, QString> > clientsBySocket;
    QMap<QPair<QString, QString>, QTcpSocket* > clientsByNameAndClass;

    static ChatServer* _instance;

};

#endif // CHATSERVER_HPP
