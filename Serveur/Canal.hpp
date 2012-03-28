#ifndef CANAL_H
#define CANAL_H

#include <QMap>
#include <QString>
#include <QTcpSocket>
#include <QMutex>

#include "AuthentificationSystem.hpp"
#include "Headers.hpp"

class Canal
{
    public:
        Canal(int id, QString const& nom);

        int GetID() const;
        QString GetNom() const;

        void SendMessageOnCanal(AuthentificationSystem::AuthentificationSystemPtr fromSystem, QString const& message);
        void SendSystemCanalMessage(QString const& message);

        void AddParticipant(AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket* Socket);
        QTcpSocket* RemoveParticipant(AuthentificationSystem::AuthentificationSystemPtr System);

        void SendByCanal(const QString &message, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket *Socket);

        static Canal* CreateCanal(int id, QString const& nom);
        static Canal* SubscribeCanal(int id, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket* Socket);
        static Canal* SubscribeCanal(QString const& nom, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket* Socket);
        static void SendByCanal(QString const& message, QString const& canalName, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket* Socket);
        static Canal* MainCanal();
        static bool DeleteCanal(int id);
        static bool DeleteCanal(QString const& nom);

    private:
        QByteArray CreatePacketBaseMessage(AuthentificationSystem::AuthentificationSystemPtr pSystem, QString const& message);
        QByteArray CreatePacketSystemMessage(QString const& IdentificationName, QString const& message);

        QMap<AuthentificationSystem::AuthentificationSystemPtr, QTcpSocket* > m_Participants;
        int m_ID;
        QString m_Nom;

        mutable QMutex m_Mutex;

        static QMap<int, Canal* > s_CanalsByID;
        static QMap<QString, Canal* > s_CanalsByName;
        static Canal* s_MainCanal;

};

#endif // CANAL_H
