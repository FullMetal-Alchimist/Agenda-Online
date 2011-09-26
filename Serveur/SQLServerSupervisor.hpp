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
public:

    QByteArray Hash(QString const& toHash);
    bool Authentificate(QString const& UserName, QByteArray const& Password);
    QString FindClasse(QString const& UserName);
    QList<Devoir> LoadHomeworks(QString const& Classe, QString const& Matiere = QObject::tr("all"));
    QStringList GetAllMatiereFromClasse(QString const& Classe);
    QStringList GetAllMatiere();

    bool CreateAccount(QString const& UserName, QString const& Password, QString const& classe);
    bool AddHomework(QString const& nom, QString const& sujet, QString const& matiere, QString const& classe, QDate const& date);
    bool RemoveHomework(QString const& nom);
    //bool UpdateHomework(QString const& nom, QString const& sujet, QString const& matiere, QString const& classe);

    QMap<

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

    static mutable QMutex mutex;

    static SQLServerSupervisor* _instance;
};

#endif // SQLSERVERSUPERVISOR_HPP
