#include "AuthentificationSystem.hpp"
#include "SQLServerSupervisor.hpp"

QMap<int, AuthentificationSystem::AuthentificationSystemPtr> AuthentificationSystem::s_AuthentificationsByID;
QMap<QPair<QString, QString>, AuthentificationSystem::AuthentificationSystemPtr> AuthentificationSystem::s_AuthentificationsByUC;

AuthentificationSystem::AuthentificationSystem(QString const& UserName, QString const& Password) :
    m_State(InProgress), m_Error(NoError),
    m_UserName(UserName), m_Password(Password)
{

}
AuthentificationSystem::AuthentificationSystemPtr AuthentificationSystem::Authentificate(const QString &UserName, const QString &Password)
{
    AuthentificationSystem::AuthentificationSystemPtr pAuth(new AuthentificationSystem(UserName, Password));
    pAuth->m_Error = SQLServerSupervisor::GetInstance()->Authentificate(pAuth) ? NoError : pAuth->RetrieveError();
    pAuth->m_State = pAuth->Error() == NoError ? Accepted : Refused;

    if(pAuth->Error() == NoError)
    {
        pAuth->m_Classe = SQLServerSupervisor::GetInstance()->FindClasse(UserName);
        pAuth->m_ID = SQLServerSupervisor::GetInstance()->FindID(UserName);
        s_AuthentificationsByID[pAuth->m_ID] = pAuth;
        s_AuthentificationsByUC[qMakePair(pAuth->m_UserName, pAuth->m_Classe)] = pAuth;
    }

    return pAuth;
}
AuthentificationSystem::AuthentificationSystemPtr AuthentificationSystem::CreateAuthentification()
{
    AuthentificationSystem::AuthentificationSystemPtr auth(new AuthentificationSystem(QString::null, QString::null));
    auth->m_State = NoStartYet;
    auth->m_Error = NoError;

    return auth;
}
AuthentificationSystem::AuthentificationSystemPtr AuthentificationSystem::RetrieveFromUserNameAndClasse(const QString &UserName, const QString &Classe)
{
    if(s_AuthentificationsByUC.contains(qMakePair(UserName, Classe)))
    {
        return s_AuthentificationsByUC[qMakePair(UserName, Classe)];
    }
    else
    {
        AuthentificationSystem::AuthentificationSystemPtr custom = CreateAuthentification();
        custom->m_Error = CannotRetrieveAuth;
        return custom;
    }
}
AuthentificationSystem::AuthentificationSystemPtr AuthentificationSystem::RetrieveFromID(int ID)
{
    if(s_AuthentificationsByID.contains(ID))
    {
        return s_AuthentificationsByID[ID];
    }
    else
    {
        AuthentificationSystem::AuthentificationSystemPtr custom = CreateAuthentification();
        custom->m_Error = CannotRetrieveAuth;
        return custom;
    }
}

bool AuthentificationSystem::operator ==(const AuthentificationSystem& lhs) const
{
    if((&lhs) == this)
        return true;

    return m_Classe == lhs.m_Classe && m_UserName == lhs.m_UserName && m_Password == lhs.m_Password && m_ID == lhs.m_ID;
}
bool AuthentificationSystem::operator <(const AuthentificationSystem& lhs) const
{
    if((&lhs) == this)
        return false;

    return (Devoir::decodeClasse(m_Classe).first < Devoir::decodeClasse(lhs.m_Classe).first) || (Devoir::decodeClasse(m_Classe).first == Devoir::decodeClasse(lhs.m_Classe).first && Devoir::decodeClasse(m_Classe).second < Devoir::decodeClasse(lhs.m_Classe).second);
}
bool AuthentificationSystem::operator >(const AuthentificationSystem& lhs) const
{
    if((&lhs) == this)
        return false;

    return (Devoir::decodeClasse(m_Classe).first > Devoir::decodeClasse(lhs.m_Classe).first) || (Devoir::decodeClasse(m_Classe).first == Devoir::decodeClasse(lhs.m_Classe).first && Devoir::decodeClasse(m_Classe).second > Devoir::decodeClasse(lhs.m_Classe).second);;
}

QString AuthentificationSystem::GetUserName() const
{
    return m_UserName;
}
QByteArray AuthentificationSystem::GetPassword() const
{
    return m_Password.toLatin1();
}
QString AuthentificationSystem::GetClasse() const
{
    return m_Classe;
}
int AuthentificationSystem::GetID() const
{
    return m_ID;
}
AuthentificationSystem::AuthentificationState AuthentificationSystem::State() const
{
    return m_State;
}
AuthentificationSystem::AuthentificationError AuthentificationSystem::Error() const
{
    return m_Error;
}

AuthentificationSystem::AuthentificationError AuthentificationSystem::RetrieveError()
{
    QString FirstRequest = QObject::tr("SELECT COUNT(*) FROM `accounts` WHERE `UserName` = ?;");
    QString SecondRequest = QObject::tr("SELECT `Password` FROM `accounts` WHERE `UserName` = ? LIMIT 1;");
    QString ThirdRequest = QObject::tr("SELECT `Connected` FROM `accounts` WHERE `UserName` = ? LIMIT 1;");

    SQLServerSupervisor::GetInstance()->BeginCustomQuery();
    QSqlQuery* query = SQLServerSupervisor::GetInstance()->GetObjQuery();
    query->prepare(FirstRequest);
    query->bindValue(0, m_UserName);

    if(!query->exec())
    {
        qDebug() << "Query error in AuthentificationSystem.";
        return AuthentificationSystem::Unknown;
    }
    else
    {
        query->first();
        int Count = query->value(0).toInt();
        if(Count <= 0)
        {
            SQLServerSupervisor::GetInstance()->EndCustomQuery();

            return AuthentificationSystem::UserName_Not_Available;
        }

        query->clear();

        query->prepare(SecondRequest);
        query->bindValue(0, m_UserName);

        if(!query->exec())
        {
            qDebug() << "Query error in AuthentificationSystem.";
        }
        else
        {
            query->first();
            bool PasswordCorrect = query->value(0).toString() == m_Password.toLatin1();
            if(!PasswordCorrect)
            {
                SQLServerSupervisor::GetInstance()->EndCustomQuery();
                return AuthentificationSystem::Password_Incorrect;
            }
            query->clear();

#ifdef QUERY_THIRD_REQUEST
            query->prepare(ThirdRequest);
            query->bindValue(0, m_UserName);

            if(!query->exec())
            {
                qDebug() << "Query error in AuthentificationSystem.";
            }
            else
            {
                query->first();
                bool IsConnected = query->value(0).toBool();

                if(IsConnected)
                {
                    SQLServerSupervisor::GetInstance()->EndCustomQuery();
                    return AuthentificationSystem::Double_Account_Detected;
                }

            }
#endif
        }
    }
    SQLServerSupervisor::GetInstance()->EndCustomQuery();

    return AuthentificationSystem::NoError;
}

