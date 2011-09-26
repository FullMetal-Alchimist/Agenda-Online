/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 26/09/2011
**
**                      Commands.hpp:Commands
**************************************************************************/

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QObject>

class Commands : public QObject
{
    Q_OBJECT
public:
    explicit Commands(QObject *parent = 0);

signals:

public slots:

};

#endif // COMMANDS_HPP
