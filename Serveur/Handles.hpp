/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 26/09/2011
**
**                      Handles.hpp:
**************************************************************************/

#ifndef HANDLES_HPP
#define HANDLES_HPP
#include <QMap>
#include <QStringList>

class Handler;

struct ChatCommand
{
    QString Nom;
    int Level;
    bool AllowConsole;
    void (Handler::*Func) (QStringList const& args);
    QString Help;
    ChatCommand* ChildCommands;
};

class Handler
{
public:
    ChatCommand* GetTable();

    bool CheckIfLevelIsOk(Client const& client) const;
};

#endif // HANDLES_HPP
