#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QtNetwork>
#include "Devoir.hpp"

#define SEL_BEFORE "hnnnnnnnnnnnn, -t(bgrvf"
#define SEL_AFTER "vbg ft-if ,,,,kjinuik"

QByteArray Hash(QString const& toHash);

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);
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

        SMSG_AUTHENTIFICATION_SUCCESS = 0x9,
        SMSG_AUTHENTIFICATION_FAILED = 0xA,
        SMSG_HOMEWORK = 0xB,
        SMSG_YOU_ARE_NOT_AUTHENTIFIED = 0xC,
        SMSG_MESSAGE_CHAT = 0xD,
        SMSG_LISTMATIERE = 0xF
    };
    typedef quint8 Header;


signals:
    void disconnected();
    void connected();

    void connectionAccpeted();
    void connectionRefused();
    void notAuth();

    void message(QString const& message);

    void displayError(const QString& error);

    void newHomework(const Devoir& devoir);
    void chatMessage(QString const& message, QString const& from);
    void newListMatieres(QStringList const& matieres);
public:
    bool IsAuthentified() const
    { return Authentified;
    }
public slots:
    void connectTo(const QString& ip, const quint16& port);
    void processCode(quint8 code);

    void receiveData();
    void error(QAbstractSocket::SocketError);

    void SendPacket(int Type);

    void SendPong();
    void SendPing();
    void SendMessage(QString const& message);
    void SendAuthentification(QString const& UserName, QString const& Password);
    void SendHomeworkRequest();
    void SendAvailableMatieresRequest();
    void SendDisconnection();

    void Flush();

    static bool testConnection(const QString& ip, const quint16& port)
    {
        QTcpSocket* socket = new QTcpSocket;
        socket->connectToHost(ip, port);
        socket->waitForConnected(10000);
        if(socket->state() != QAbstractSocket::ConnectedState)
            return false;
        else
            return true;
    }

private:
    QTcpSocket* socket;
    quint16 taillePacket;

    bool Authentified;

};

#endif // CLIENT_HPP
