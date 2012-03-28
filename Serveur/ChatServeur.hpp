#ifndef CHATSERVEUR_H
#define CHATSERVEUR_H

#include "AbstractServeur.hpp"
#include "AuthentificationSystem.hpp"
#include "Canal.hpp"
#include "Headers.hpp"

typedef AuthentificationSystem ChatAuth;
typedef ChatAuth::AuthentificationSystemPtr ChatAuthPtr;


class ChatServeur : public AbstractServeur
{
        Q_OBJECT
    public:
        explicit ChatServeur(QObject *parent = 0);
        ChatServeur(ChatServeur const& lhs) { }

        virtual ChatServeur* Clone() const;

        void SubscribeCanal(QString const& nom);
        void SubscribeCanal(int id);

        void SendMessageOnCanal(QString const& message);
        void SendPrivateMessage(QString const& destNom, QString const& destClasse, QString const& message);
        void SendMessage(AuthentificationSystem::AuthentificationSystemPtr fromSystem, QString const& message);
        void SendSystemMessage(QString const& message);

    private slots:
        virtual void run();
    private:
        void ProcessData();
        void Reponse(quint8 rCode);

        void SendPing();

    private:
        Canal* m_DefaultCanal;
        AuthentificationSystem::AuthentificationSystemPtr m_Authentification;

    private:
        QTcpSocket* m_Client;
        quint8 m_TaillePacket;

    private:
        bool m_Error;
        bool m_ThreadRunning;

    private:
        static QMap<ChatAuth, ChatServeur* > s_Clients;

};

#endif // CHATSERVEUR_H
