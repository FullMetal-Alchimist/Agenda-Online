#ifndef SERVEUR_HPP
#define SERVEUR_HPP

#include <QtNetwork>

#include "ChatServer.hpp"
#include "SQLServerSupervisor.hpp"

class Serveur : public QThread
{
    Q_OBJECT
public:
    enum Header
    {
        CMSG_MESSAGE_LEGER = 0x1,
        CMSG_PING = 0x2,
        CMSG_PONG = 0x3,
        CMSG_MESSAGE_CUSTOM = 0x4,

        SMSG_PONG = 0x5,
        SMSG_PING = 0x6,
        SMSG_KICK = 0x7,

        /** Custom Messages Clients **/
        CMSG_MESSAGE_AUTH = 0x8,
        CMSG_MESSAGE_HOMEWORKFOR = 0x9,
        CMSG_MESSAGE_CHAT = 0xA,
        CMSG_MESSAGE_LISTMATIERE = 0xB,
        CMSG_MESSAGE_PRIVATE_MESSAGE = 0xC,
        CMSG_MESSAGE_SHOW_ME_CONNECTED = 0xD,
        CMSG_COMMAND_ADMIN = 0xE,

        SMSG_AUTHENTIFICATION_SUCCESS = 0xF,
        SMSG_AUTHENTIFICATION_FAILED = 0x10,
        SMSG_HOMEWORK = 0x11,
        SMSG_YOU_ARE_NOT_AUTHENTIFIED = 0x12,
        SMSG_MESSAGE_CHAT = 0x13,
        SMSG_LISTMATIERE = 0x14,
        SMSG_COMMAND_REPONSE = 0x15,
        SMSG_COMMAND_REFUSED = 0x16,

        TOTAL_OP
    };
    typedef quint8 Header;

    Serveur(int handle, QObject *parent = 0);
    ~Serveur();

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
    QString UserName;
    QString Classe;

    /** Low-level thread vars **/
private:
    bool threadRunning;
    bool errorFatal;

    /** Mutex and other primitive of synchronization **/
private:
    mutable QMutex mutex;

protected:
    static QMap<QString, Serveur* > m_Clients;

};

#endif // SERVEUR_HPP
