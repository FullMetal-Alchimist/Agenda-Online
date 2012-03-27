/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 24/09/2011
**
**                      SQLServerSupervisor.hpp:SQLServerSupervisor
**************************************************************************/

#ifndef SQLSERVERSUPERVISOR_HPP
#define SQLSERVERSUPERVISOR_HPP

#include <QtCore>
#include <QtSql>

#include "Devoir.hpp"

#define SEL_BEFORE "hnnnnnnnnnnnn, -t(bgrvf"
#define SEL_AFTER "vbg ft-if ,,,,kjinuik"

class AuthentificationSystem;

class SQLServerSupervisor : public QObject
{
    Q_OBJECT
    enum TypeString
    {
        IP,
        Account
    };

public:

    bool BeginCustomQuery();
    QSqlQuery* GetObjQuery();
    bool EndCustomQuery();

    QByteArray Hash(QString const& toHash);

    bool Authentificate(QString const& UserName, QByteArray const& Password) const;
    bool Authentificate(AuthentificationSystem* pSystem) const;

    QString FindClasse(QString const& UserName) const;
    int FindID(QString const& UserName) const;

    QList<Devoir> LoadHomeworks(QString const& Classe, QString const& Matiere = QObject::tr("all"));
    QList<Devoir> LoadHomeworks(AuthentificationSystem* pSystem, QString const& Matiere = QObject::tr("all"));

    QStringList GetAllMatiereFromClasse(QString const& Classe);
    QStringList GetAllMatiereFromClasse(AuthentificationSystem* pSystem);

    QStringList GetAllMatiere();

    bool CreateAccount(QString const& UserName, QString const& Password, QString const& classe);
    bool RemoveAccount(QString const& Nom);
    bool RemoveAccount(int ID);
    bool AddHomework(QString const& nom, QString const& sujet, QString const& matiere, QString const& classe, QDate const& date);
    bool RemoveHomework(QString const& nom);
    bool RemoveHomework(int ID);

    static SQLServerSupervisor* GetInstance();
    static bool Kill();

signals:
    void debug(QString const& messageDebug);

public slots:

protected:
    bool OpenDB();
    SQLServerSupervisor();
    ~SQLServerSupervisor();

    QSqlDatabase db;
    QSqlQuery* query;

    QReadWriteLock lock;

    static SQLServerSupervisor* _instance;
    static QMutex* _StaticMutex;
};

#endif // SQLSERVERSUPERVISOR_HPP
