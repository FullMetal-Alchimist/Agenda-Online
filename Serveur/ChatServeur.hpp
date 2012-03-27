#ifndef CHATSERVEUR_H
#define CHATSERVEUR_H

#include "AbstractServeur.hpp"
#include "AuthentificationSystem.hpp"

typedef AuthentificationSystem ChatAuth;

class ChatServeur : public AbstractServeur
{
        Q_OBJECT
    public:
        explicit ChatServeur(QObject *parent = 0);

        virtual ChatServeur* Clone() const;
        void SendMessageAt(QString const& nom, QString const& classe);
        static void SendMessageAt(QString const& fromNom, QString const& fromClasse, QString const& destNom, QString const& destClasse);
    signals:

    public slots:
        virtual void run();


    private:

        static QMap<ChatAuth, ChatServeur* > s_Clients;

};

#endif // CHATSERVEUR_H
