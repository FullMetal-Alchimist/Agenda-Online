/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 26/09/2011
**
**                      Client.hpp:Client
**************************************************************************/

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QObject>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

signals:

public slots:

};

#endif // CLIENT_HPP
