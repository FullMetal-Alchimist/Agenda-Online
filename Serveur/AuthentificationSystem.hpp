#ifndef AUTHENTIFICATIONSYSTEM_H
#define AUTHENTIFICATIONSYSTEM_H

#include <QtCore>
#include "SQLServerSupervisor.hpp"

class AuthentificationSystem : public QObject
{
        Q_OBJECT
    public:
        enum AuthentificationState
        {
            Accepted,
            InProgress,
            Refused
        };
        enum AuthentificationError
        {
            UserName_Not_Available,
            Password_Incorrect,
            Double_Account_Detected,
            NoError,
            Unknown
        };

        explicit AuthentificationSystem(QString const& UserName, QString const& Password, QObject *parent = 0);

        AuthentificationState State() const;
        AuthentificationError Error() const;

        QString GetUserName() const;
        QByteArray GetPassword() const;
        QString GetClasse() const;
        int GetID() const;

        static AuthentificationSystem* Authentificate(QString const& UserName, QString const& Password);

    private:
        AuthentificationError RetrieveError();
        AuthentificationState m_State;
        AuthentificationError m_Error;

        QString m_UserName;
        QString m_Password;
        QString m_Classe;
        int m_ID;

        static QMap<int, AuthentificationSystem*> s_Authentifications;



};

#endif // AUTHENTIFICATIONSYSTEM_H
