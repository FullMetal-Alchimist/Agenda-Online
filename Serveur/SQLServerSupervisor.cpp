/**************************************************************************
**                  Ryan Lahfa 2009-2011 Â©
**            Ce code source a Ã©tÃ© crÃ©e et/ou Ã©ditÃ© le 24/09/2011
**
**                      SQLServerSupervisor.cpp:SQLServerSupervisor
**************************************************************************/

#include "SQLServerSupervisor.hpp"

SQLServerSupervisor* SQLServerSupervisor::_instance = NULL;
QMutex* SQLServerSupervisor::_StaticMutex = new QMutex;

SQLServerSupervisor::SQLServerSupervisor() :
    QObject()
{
    lock.lockForWrite();
    db = QSqlDatabase::addDatabase(tr("QMYSQL"), tr("SQL Server Supervisor"));
    query = new QSqlQuery(db);

    OpenDB();
    lock.unlock();
}
SQLServerSupervisor::~SQLServerSupervisor()
{
    delete query;
    db.close();
}

bool SQLServerSupervisor::OpenDB()
{
    db.setDatabaseName(tr("homework"));
    db.setHostName(tr("127.0.0.1"));
    db.setPassword(tr("748700"));
    db.setUserName(tr("root"));
    db.setPort(3306);


    return db.open();
}

QByteArray SQLServerSupervisor::Hash(const QString &toHash)
{
    return QCryptographicHash::hash(QCryptographicHash::hash(SEL_BEFORE, QCryptographicHash::Md5) + toHash.toLatin1() + QCryptographicHash::hash(SEL_AFTER, QCryptographicHash::Md5), QCryptographicHash::Sha1);
}
bool SQLServerSupervisor::Authentificate(const QString &UserName, const QByteArray &Password)
{
    lock.lockForRead();

    query->prepare(tr("SELECT `passwordhashed` FROM `accounts` WHERE `username` = ? LIMIT 1;"));
    query->bindValue(0, UserName);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'execution d'une requête d'authentification (Supervisor) : %1").arg(query->lastError().text()));
        lock.unlock();
        return false;
    }
    else
    {
        query->first();
        QString PasswordHashed = query->value(0).toString();

        lock.unlock();

        return PasswordHashed == Password.toHex();
    }
}
QString SQLServerSupervisor::FindClasse(const QString &UserName)
{
    lock.lockForRead();
    query->prepare(tr("SELECT `classe` FROM `accounts` WHERE `username` = ?"));
    query->bindValue(0, UserName);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'execution d'un requête de récupèration d'informations sur la classe (Supervisor) : %1").arg(query->lastError().text()));
        lock.unlock();
        return false;
    }
    else
    {
        query->first();
        QString classe = query->value(0).toString();

        lock.unlock();
        return classe;
    }
}
QList<Devoir> SQLServerSupervisor::LoadHomeworks(const QString &Classe, const QString &Matiere)
{
    lock.lockForRead();
    QList<Devoir> devoirs;

    bool all = false;

    all = Matiere.toLower() == tr("all");

    QString requeteWithAll = tr("SELECT `ID`,`Nom`,`Sujet`,`Matière`,`Date` FROM `homework_table` WHERE `Niveau` = ?");
    QString requeteWithoutAll = tr("SELECT `ID`,`Nom`,`Sujet`,`Matière`,`Date` FROM `homework_table` WHERE `Niveau` = ? AND `Matière` = ?");
    if(all)
    {
        query->prepare(requeteWithAll);
        query->bindValue(0, Devoir::decodeClasse(Classe).first);
    }
    else
    {
        query->prepare(requeteWithoutAll);
        query->bindValue(0, Devoir::decodeClasse(Classe).first);
        query->bindValue(1, Matiere);
    }

    bool ok = query->exec();
    if(!ok)
        emit debug(tr("Erreur lors de la récupèration des devoirs : %1").arg(query->lastError().text()));

    while(query->next())
    {
        Devoir devoir((quint64)query->value(0).toInt(), query->value(1).toString(), query->value(3).toString(), query->value(2).toString(), Devoir::calculateLevel(Devoir::decodeClasse(Classe).first), query->value(4).toDate());
        devoirs.append(devoir);
    }

    lock.unlock();

    return devoirs;
}
bool SQLServerSupervisor::CreateAccount(const QString &UserName, const QString &Password, const QString &classe)
{
    lock.lockForWrite();
    QByteArray password = Hash(Password).toHex();

    query->prepare(tr("INSERT INTO `accounts` VALUES(DEFAULT, ?, ?, ?);"));
    query->bindValue(0, UserName);
    query->bindValue(1, password);
    query->bindValue(2, classe);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la création d'un compte(Supervisor)!"));
        lock.unlock();
        return false;
    }
    else
    {
        lock.unlock();
        return true;
    }
}
bool SQLServerSupervisor::AddHomework(const QString &nom, const QString &sujet, const QString &matiere, const QString &classe, const QDate &date)
{
    lock.lockForWrite();

    query->prepare("INSERT INTO `homework_table` VALUES(DEFAULT, ?, ?, ?, ?, ?);");

    query->bindValue(0, nom);
    query->bindValue(1, matiere);
    query->bindValue(2, sujet);
    query->bindValue(3, classe);
    query->bindValue(4, date);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'ajout d'un devoir."));
        lock.unlock();
        return false;
    }
    else
    {
        lock.unlock();
        return true;
    }
}
bool SQLServerSupervisor::RemoveHomework(const QString &nom)
{
    lock.lockForWrite();

    query->prepare("DELETE FROM `homework_table` WHERE `Nom` = ?;");
    query->bindValue(0, nom);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'écrasement d'un devoir."));
        lock.unlock();
        return false;
    }
    else
    {
        lock.unlock();
        return true;
    }
}
QStringList SQLServerSupervisor::GetAllMatiereFromClasse(const QString &Classe)
{
    lock.lockForRead();

    QStringList matieres;

    query->prepare(tr("SELECT `Matière` FROM `homework_table` WHERE `Niveau` = ?;"));
    query->bindValue(0, Classe);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la récupèration des matières."));
        return matieres;
    }

    while(query->next())
    {
        matieres << query->value(0).toString();
    }

    lock.unlock();

    return matieres;
}
QStringList SQLServerSupervisor::GetAllMatiere()
{
    lock.lockForRead();

    QStringList matieres;

    query->prepare(tr("SELECT `Matière` FROM `homework_table`;"));

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la récupèration des matières."));
        return matieres;
    }

    while(query->next())
    {
        matieres << query->value(0).toString();
    }

    lock.unlock();

    return matieres;
}
bool SQLServerSupervisor::RemoveAccount(int ID)
{
    lock.lockForWrite();

    query->prepare(tr("DELETE FROM `accounts` WHERE `ID` = ?"));
    query->bindValue(0, QVariant(ID));

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la supression d'un compte par ID."));
        return false;
    }


    lock.unlock();
    return true;
}
bool SQLServerSupervisor::RemoveHomework(int ID)
{
    lock.lockForWrite();

    query->prepare(tr("DELETE FROM `homework_table` WHERE `ID` = ?"));
    query->bindValue(0, QVariant(ID));

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la supression d'un devoir par ID."));
        return false;
    }


    lock.unlock();
    return true;
}

bool SQLServerSupervisor::Kill()
{
    if(_instance != NULL)
        delete _instance;

    return true;
}
SQLServerSupervisor* SQLServerSupervisor::GetInstance()
{
    if(_instance == NULL)
    {
        _StaticMutex->lock();
        if(_instance == NULL)
        {
            _instance = new SQLServerSupervisor;
            emit _instance->debug(tr("SQL Server Supervisor chargé!!!"));
            _StaticMutex->unlock();
            return _instance;
        }
    }
    return _instance;
}
