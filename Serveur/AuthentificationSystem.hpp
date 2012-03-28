#ifndef AUTHENTIFICATIONSYSTEM_H
#define AUTHENTIFICATIONSYSTEM_H

#include <QString>
#include <QByteArray>
#include <QMap>
#include <QPair>
#include <QSharedPointer>
#include <QDebug>

class AuthentificationSystem
{
    public:
        typedef QSharedPointer<AuthentificationSystem> AuthentificationSystemPtr;
        enum AuthentificationState
        {
            Accepted,
            InProgress,
            Refused,
            NoStartYet
        };
        enum AuthentificationError
        {
            UserName_Not_Available,
            Password_Incorrect,
            Double_Account_Detected,
            CannotRetrieveAuth,
            NoError,
            Unknown
        };

        AuthentificationSystem(QString const& UserName, QString const& Password);

        AuthentificationState State() const;
        AuthentificationError Error() const;

        QString GetUserName() const;
        QByteArray GetPassword() const;
        QString GetClasse() const;
        int GetID() const;

        bool operator==(const AuthentificationSystem& lhs) const;
        bool operator<(const AuthentificationSystem& lhs) const;
        bool operator>(const AuthentificationSystem& lhs) const;

        static AuthentificationSystemPtr Authentificate(QString const& UserName, QString const& Password);
        static AuthentificationSystemPtr CreateAuthentification();

        static AuthentificationSystemPtr RetrieveFromUserNameAndClasse(QString const& UserName, QString const& Classe);
        static AuthentificationSystemPtr RetrieveFromID(int ID);

    private:
        AuthentificationError RetrieveError();
        AuthentificationState m_State;
        AuthentificationError m_Error;

        QString m_UserName;
        QString m_Password;
        QString m_Classe;
        int m_ID;

        static QMap<int, AuthentificationSystemPtr> s_AuthentificationsByID;
        static QMap<QPair<QString, QString>, AuthentificationSystemPtr> s_AuthentificationsByUC; /** UC = UserName And Classe **/
};



#endif // AUTHENTIFICATIONSYSTEM_H
