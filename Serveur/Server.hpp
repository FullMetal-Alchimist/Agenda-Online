/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 12/07/2011
**
**                      Server.h:Server
**************************************************************************/

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "AbstractServeur.hpp"



class Server : public QTcpServer
{
        Q_OBJECT
    public:
        Server(QObject* parent, AbstractServeur* Type);
    signals:
        void message(QString const& message);
        void newClient(QString const& nom, QString const& classe);
        void removeClient(QString const& nom, QString const& classe);

    public slots:

    protected:
        void incomingConnection(int handle);

    private:
        AbstractServeur* m_BaseType;

};

class ServerManager : public QObject
{
        Q_OBJECT
    public:
        explicit ServerManager(QObject *parent = 0);

        void AddServeur(AbstractServeur* serveurType);
        void Listen(AbstractServeur* serverAbstract, QHostAddress const& hostAddress, int Port);
    signals:
        void message(QString const& message);
        void newClient(QString const& nom, QString const& classe);
        void removeClient(QString const& nom, QString const& classe);

    private:
        QMap<AbstractServeur*, Server* > m_TcpServers;
};

#endif // SERVER_H
