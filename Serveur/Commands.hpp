/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 26/09/2011
**
**                      Commands.hpp:Commands
**************************************************************************/

#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <QtCore>
#include "SQLServerSupervisor.hpp"

class Commands : public QObject
{
    struct Command
    {
        friend class Commands;
    private:
        void (*f)(QStringList const&);
        Command(void (*f)(QStringList const&)) : f(f)
        {

        }

    public:
        void Execute(QStringList const& args)
        {
            f(args);
        }
    };
    Q_OBJECT
public:
    static Commands* GetInstance();
    static bool KillInstance();

    bool Execute(QString const& nom, QStringList const& args);
    void AddCommand(QString const& nom, Command* command);
    void RemoveCommand(QString const& nom);

signals:

private:
    void LoadCommandsFromDB();

    Commands();
    static Commands* _instance;

    QMap<QString, Command* > commands;
    static mutable QMutex mutex;

};

#endif // COMMANDS_HPP
