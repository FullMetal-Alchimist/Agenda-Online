#include "AuthentificationSystem.hpp"

AuthentificationSystem::AuthentificationSystem(QString const& UserName, QString const& Password, QObject *parent) :
    QObject(parent), m_UserName(UserName), m_Password(Password), m_State(InProgress), m_Error(NoError)
{

}
AuthentificationSystem* AuthentificationSystem::Authentificate(const QString &UserName, const QString &Password)
{
    AuthentifcationSystem* pAuth = new AuthentifcationSystem(UserName, Password);
    pAuth->m_Error = SQLServerSupervisor::GetInstance()->Authentificate(pAuth) ? NoError : pAuth->RetrieveError();
    pAuth->m_State = pAuth->Error() == NoError ? Accepted : Refused;
    if(pAuth->Error() == NoError)
    {
        pAuth->m_Classe = SQLServerSupervisor::GetInstance()->FindClasse(UserName);
        pAuth->m_ID = SQLServerSupervisor::GetInstance()->FindID(UserName);
        s_Authentifications[pAuth->m_ID] = pAuth;
    }

    return pAuth;
}
