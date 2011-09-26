/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 26/09/2011
**
**                      Commands.cpp:Commands
**************************************************************************/

#include "Commands.hpp"

QMutex Commands::mutex = QMutex();
Commands* Commands::_instance = NULL;

Commands::Commands() :
    QObject()
{
    LoadCommandsFromDB();
}

Commands::Command* Commands::GetInstance()
{
    if(_instance == NULL)
    {
        QMutexLocker m(&mutex);

        if(_instance == NULL)
            _instance = new Commands;
    }

    return _instance;
}

bool Commands::KillInstance()
{
    QMutexLocker m(&mutex);

    if(_instance)
        delete _instance;

    return true;
}

bool Commands::Execute(const QString &nom, QStringList const& args)
{
    QMutexLocker m(&mutex);

    if(commands.contains(nom))
    {
        commands[nom]->Execute(args);
        return true;
    }

    return false;
}
void Commands::AddCommand(const QString &nom, Command *command)
{
    QMutexLocker m(&mutex);

    commands.insert(nom, command);
}
void Commands::RemoveCommand(const QString &nom)
{
    QMutexLocker m(&mutex);

    delete commands.take(nom);
}
