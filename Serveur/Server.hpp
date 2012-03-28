/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 12/07/2011
**
**                      Server.h:Server
**************************************************************************/

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QSignalMapper>
#include "AbstractServeur.hpp"

struct Connection
{
        Connection(const char* Signal, const char* Member, const QObject* obj, Qt::ConnectionType connection) : ObjectReceiver(obj)
        {
            SignalName = new char[qstrlen(Signal) + 1];
            MemberName = new char[qstrlen(Member) + 1];

            qstrcpy(SignalName, Signal);
            qstrcpy(MemberName, Member);

            ObjectReceiver = obj;
            ConnectionType = connection;
        }

        char* SignalName;
        char* MemberName;
        const QObject* ObjectReceiver;
        Qt::ConnectionType ConnectionType;
};

class Server : public QTcpServer
{
        Q_OBJECT
    public:
        Server(QObject* parent, AbstractServeur* Type);

    public slots:
        void connect(const char *signal, const QObject *receiver, const char *member, Qt::ConnectionType);

    protected:
        void incomingConnection(int handle);

    private:
        AbstractServeur* m_BaseType;

        QList<Connection* > m_SignalsMapper;

};

class ServerManager : public QObject
{
        Q_OBJECT
    public:
        explicit ServerManager(QObject *parent = 0);

        void AddServeur(AbstractServeur* serveurType);
        bool Listen(AbstractServeur* serverAbstract, QHostAddress const& hostAddress, int Port);
        void Connect(AbstractServeur* serverAbstract, const char* signal, const QObject* receiver, const char* member, Qt::ConnectionType connection = Qt::AutoConnection);

    private:
        QMap<AbstractServeur*, Server* > m_TcpServers;
};

#endif // SERVER_H
