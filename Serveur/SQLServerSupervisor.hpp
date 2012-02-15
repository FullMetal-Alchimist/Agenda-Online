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

class SQLServerSupervisor : public QObject
{
    Q_OBJECT
    enum TypeString
    {
        IP,
        Account
    };

public:

    QByteArray Hash(QString const& toHash);
    bool Authentificate(QString const& UserName, QByteArray const& Password);
    QString FindClasse(QString const& UserName);
    QList<Devoir> LoadHomeworks(QString const& Classe, QString const& Matiere = QObject::tr("all"));
    QStringList GetAllMatiereFromClasse(QString const& Classe);
    QStringList GetAllMatiere();

    bool CreateAccount(QString const& UserName, QString const& Password, QString const& classe);
    bool RemoveAccount(QString const& Nom);
    bool RemoveAccount(int ID);
    bool AddHomework(QString const& nom, QString const& sujet, QString const& matiere, QString const& classe, QDate const& date);
    bool RemoveHomework(QString const& nom);
    bool RemoveHomework(int ID);
    //bool UpdateHomework(QString const& nom, QString const& sujet, QString const& matiere, QString const& classe);


    bool AddBan(int BanType, int BanMethod, QString Reason, int Time = 1, QString IP = "", QString Account = "");

    bool RemoveBan(int ID);
    bool RemoveBan(QString String, TypeString t);

    bool GetLastIP(QString const& Nom, QString& IP);
    bool IsConnected(QString const& Nom, bool& Connected);

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
