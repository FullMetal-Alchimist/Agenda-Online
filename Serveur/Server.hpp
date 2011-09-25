/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 12/07/2011
**
**                      Server.h:Server
**************************************************************************/

#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "Serveur.hpp"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

signals:
    void message(QString const& message);
    void newClient(QString const& nom, QString const& classe);
    void removeClient(QString const& nom, QString const& classe);

public slots:

protected:
    void incomingConnection(int handle);

};

#endif // SERVER_H
