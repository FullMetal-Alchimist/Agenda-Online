#ifndef SERVEUR_HPP
#define SERVEUR_HPP

#include <QTcpSocket>
#include <QQueue>
#include <QStringList>

#include "AbstractServeur.hpp"

#include "Headers.hpp"
#include "AuthentificationSystem.hpp"

#include "SQLServerSupervisor.hpp"

#include "Devoir.hpp"

class MainServeur : public AbstractServeur
{
        Q_OBJECT
    public:
        explicit MainServeur(QObject *parent = 0);
        MainServeur(const MainServeur& copyServeur) { }

        ~MainServeur();

        virtual MainServeur* Clone() const;
    signals:
        /** Authentification Signal **/
        void newClient(QString const& nom, QString const& classe);
        void removeClient(QString const& nom, QString const& classe);
    public slots:
        /** Thread Functions **/
        virtual void run();

    private:
        /** Network Functions **/
        void ProcessData();
        void Reponse(quint8 rCode);

        void SendHomeworks(QList<Devoir> const& devoirs);
        void SendMatieres(QStringList const& matieres);

        void SendPing();

        void Kick(QString Reason);

    private:
        QTcpSocket* m_Client;
        quint16 taillePacket;

    private:
        AuthentificationSystem::AuthentificationSystemPtr m_Authentification;

    private:
        bool threadRunning;
        bool errorFatal;


};

#endif // SERVEUR_HPP
