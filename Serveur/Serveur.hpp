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

        /** Custom Messages Clients **/
        CMSG_MESSAGE_AUTH = 0x7,
        CMSG_MESSAGE_HOMEWORKFOR = 0x8,
        CMSG_MESSAGE_CHAT = 0x9,
        CMSG_MESSAGE_LISTMATIERE = 0xE,
        CMSG_MESSAGE_PRIVATE_MESSAGE = 0x10,
        CMSG_MESSAGE_SHOW_ME_CONNECTED = 0x11,
        CMSG_COMMAND_ADMIN = 0x12,

        SMSG_AUTHENTIFICATION_SUCCESS = 0x9,
        SMSG_AUTHENTIFICATION_FAILED = 0xA,
        SMSG_HOMEWORK = 0xB,
        SMSG_YOU_ARE_NOT_AUTHENTIFIED = 0xC,
        SMSG_MESSAGE_CHAT = 0xD,
        SMSG_LISTMATIERE = 0xF,
        SMSG_COMMAND_REPONSE = 0x13,
        SMSG_COMMAND_REFUSED = 0x14
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

    /** Private Functions **/
private:
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

};

#endif // SERVEUR_HPP
