#ifndef CHATSERVEUR_H
#define CHATSERVEUR_H

#include "AbstractServeur.hpp"

class ChatServeur : public AbstractServeur
{
        Q_OBJECT
    public:
        explicit ChatServeur(QObject *parent = 0);

        virtual ChatServeur* Clone() const;
        void SendMessageAt(QString const& nom, )
    signals:

    public slots:
        virtual void run();

};

#endif // CHATSERVEUR_H
