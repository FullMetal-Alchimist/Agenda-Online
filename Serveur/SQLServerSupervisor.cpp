/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 24/09/2011
**
**                      SQLServerSupervisor.cpp:SQLServerSupervisor
**************************************************************************/

#include "SQLServerSupervisor.hpp"

SQLServerSupervisor* SQLServerSupervisor::_instance = NULL;
QMutex* SQLServerSupervisor::_StaticMutex = new QMutex;

SQLServerSupervisor::SQLServerSupervisor() :
    QObject(), lock(QMutex::Recursive)
{
    lock.lock();
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
    db.setUserName(tr("Setsuna"));
    db.setPort(3306);


    return db.open();
}

QByteArray SQLServerSupervisor::Hash(const QString &toHash)
{
    return QCryptographicHash::hash(QCryptographicHash::hash(SEL_BEFORE, QCryptographicHash::Md5) + toHash.toLatin1() + QCryptographicHash::hash(SEL_AFTER, QCryptographicHash::Md5), QCryptographicHash::Sha1);
}
bool SQLServerSupervisor::Authentificate(const QString &UserName, const QByteArray &Password)
{
    lock.lock();

    query->prepare(tr("SELECT `Password` FROM `accounts` WHERE `UserName` = ? LIMIT 1;"));
    query->bindValue(0, UserName);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'execution d'une requ�te d'authentification (Supervisor) : %1").arg(query->lastError().text()));
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
bool SQLServerSupervisor::Authentificate(AuthentificationSystem::AuthentificationSystemPtr pSystem)
{
    return Authentificate(pSystem->GetUserName(), pSystem->GetPassword());
}
QString SQLServerSupervisor::FindClasse(const QString &UserName)
{
    lock.lock();
    query->prepare(tr("SELECT `Classe` FROM `accounts` WHERE `UserName` = ?"));
    query->bindValue(0, UserName);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'execution d'un requ�te de r�cup�ration d'informations sur la classe (Supervisor) : %1").arg(query->lastError().text()));
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
int SQLServerSupervisor::FindID(const QString &UserName)
{
    lock.lock();
    query->prepare(tr("SELECT `ID` FROM `accounts` WHERE `UserName` = ?"));
    query->bindValue(0, UserName);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'execution d'un requ�te de r�cup�ration d'informations sur l'id (Supervisor) : %1").arg(query->lastError().text()));
        lock.unlock();
        return false;
    }
    else
    {
        query->first();
        int id = query->value(0).toInt();

        lock.unlock();
        return id;
    }
}
QList<Devoir> SQLServerSupervisor::LoadHomeworks(const QString &Classe, const QString &Matiere)
{
    lock.lock();
    QList<Devoir> devoirs;

    bool all = false;

    all = Matiere.toLower() == tr("all");

    QString requeteWithAll = tr("SELECT `ID`,`Nom`,`Sujet`,`Mati�re`,`Date` FROM `homework_table` WHERE `Niveau` = ?");
    QString requeteWithoutAll = tr("SELECT `ID`,`Nom`,`Sujet`,`Mati�re`,`Date` FROM `homework_table` WHERE `Niveau` = ? AND `Mati�re` = ?");
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
        emit debug(tr("Erreur lors de la r�cup�ration des devoirs : %1").arg(query->lastError().text()));

    while(query->next())
    {
        Devoir devoir((quint64)query->value(0).toInt(), query->value(1).toString(), query->value(3).toString(), query->value(2).toString(), Devoir::calculateLevel(Devoir::decodeClasse(Classe).first), query->value(4).toDate());
        devoirs.append(devoir);
    }

    lock.unlock();

    return devoirs;
}
QList<Devoir> SQLServerSupervisor::LoadHomeworks(AuthentificationSystem::AuthentificationSystemPtr pSystem, const QString &Matiere)
{
    return LoadHomeworks(pSystem->GetClasse(), Matiere);
}

bool SQLServerSupervisor::CreateAccount(const QString &UserName, const QString &Password, const QString &classe)
{
    lock.lock();
    QByteArray password = Hash(Password).toHex();

    query->prepare(tr("INSERT INTO `accounts` VALUES(DEFAULT, ?, ?, ?);"));
    query->bindValue(0, UserName);
    query->bindValue(1, password);
    query->bindValue(2, classe);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la cr�ation d'un compte(Supervisor)!"));
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
    lock.lock();

    query->prepare("INSERT INTO `homework_table` VALUES(DEFAULT, ?, ?, ?, ?, ?);");

    query->bindValue(0, nom);
    query->bindValue(1, sujet);
    query->bindValue(2, matiere);
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
    lock.lock();

    query->prepare("DELETE FROM `homework_table` WHERE `Nom` = ?;");
    query->bindValue(0, nom);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de l'�crasement d'un devoir."));
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
    lock.lock();

    QStringList matieres;

    query->prepare(tr("SELECT `Mati�re` FROM `homework_table` WHERE `Niveau` = ?;"));
    query->bindValue(0, Classe);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la r�cup�ration des mati�res."));
        return matieres;
    }

    while(query->next())
    {
        matieres << query->value(0).toString();
    }

    lock.unlock();

    return matieres;
}
QStringList SQLServerSupervisor::GetAllMatiereFromClasse(AuthentificationSystem::AuthentificationSystemPtr pSystem)
{
    return GetAllMatiereFromClasse(pSystem->GetClasse());
}

QStringList SQLServerSupervisor::GetAllMatiere()
{
    lock.lock();

    QStringList matieres;

    query->prepare(tr("SELECT `Mati�re` FROM `homework_table`;"));

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la r�cup�ration des mati�res."));
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
    lock.lock();

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
bool SQLServerSupervisor::RemoveAccount(const QString &Nom)
{
    lock.lock();

    query->prepare(tr("DELETE FROM `accounts` WHERE `UserName` = ?"));
    query->bindValue(0, Nom);

    if(!query->exec())
    {
        emit debug(tr("Erreur lors de la supression d'un compte par nom."));
        return false;
    }


    lock.unlock();
    return true;
}

bool SQLServerSupervisor::RemoveHomework(int ID)
{
    lock.lock();

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

bool SQLServerSupervisor::BeginCustomQuery()
{
    return lock.tryLock();
}

QSqlQuery* SQLServerSupervisor::GetObjQuery()
{
    return query;
}

void SQLServerSupervisor::EndCustomQuery()
{
    return lock.unlock();
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
            emit _instance->debug(tr("SQL Server Supervisor charg�!!!"));
            _StaticMutex->unlock();
            return _instance;
        }
    }
    return _instance;
}
