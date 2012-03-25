#ifndef SERVEUR_HPP
#define SERVEUR_HPP

#include <QtNetwork>

#include "AuthentificationSystem.hpp"
#include "AbstractServeur.hpp"

class MainServeur : public AbstractServeur
{
    Q_OBJECT
public:
    MainServeur(QObject *parent = 0);
    MainServeur(MainServeur const& lhs)
    {

    }

    virtual ~MainServeur();

    virtual MainServeur* Clone();
signals:
    /** Thread Signal **/
    void message(QString const& message);

    /** SQL Signal **/

    /** Network Signal **/

    /** Authentification Signal **/
    void newClient(QString const& nom, QString const& classe);
    void removeClient(QString const& nom, QString const& classe);
public slots:
    /** Thread Functions **/
    void run();

    /** Network Functions **/
    void processData();
    void reponse(quint8 rCode);

    void SendHomeworks(QList<Devoir> const& devoirs);
    void SendMatieres(QStringList const& matieres);

    void SendPing();
    void Kick(QString Reason);
public:
    static bool Kick(QString UserName, QString Reason)
    {
        if(m_Clients.contains(UserName))
            m_Clients[UserName]->Kick(Reason);

        return true;
    }

    /** Private Functions **/
private:
    inline void ThreadSafe_Write(QByteArray const& rData)
    {
        mutex.lock();
        myClient->write(rData);
        mutex.unlock();
    }

    /** Low-level network vars **/
private:
    int myHandle;
    QTcpSocket* myClient;

    quint16 taillePacket;

    /** High-level network vars **/
private:
    AuthentificationSystem* m_Authentification;
    QQueue<QByteArray> m_MessagesQueue;

    /** Low-level thread vars **/
private:
    bool threadRunning;
    bool errorFatal;

    /** Mutex and other primitive of synchronization **/
private:
    mutable QMutex mutex;

protected:

};

#endif // SERVEUR_HPP
