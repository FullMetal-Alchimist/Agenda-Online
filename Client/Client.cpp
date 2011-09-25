#include "Client.hpp"

Client::Client(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(connected()), this, SIGNAL(connected()));
    connect(socket, SIGNAL(disconnected()), this, SIGNAL(disconnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));

    taillePacket = 0;

}
void Client::connectTo(const QString &ip, const quint16 &port)
{
    socket->abort();
    socket->connectToHost(ip, port);
}
void Client::processCode(quint8 code)
{
    switch(code)
    {
    case SMSG_PING:
    {
        emit message(tr("Pong envoyé."));
        SendPong();
        break;
    }
    case SMSG_PONG:
    {
        emit message(tr("Pong!"));
        break;
    }
    case SMSG_AUTHENTIFICATION_SUCCESS:
    {
        Authentified = true;
        emit message(tr("Authentification réussi! Vous êtes maintenant connecté(e) et authentifié(e)."));
        break;
    }
    case SMSG_AUTHENTIFICATION_FAILED:
    {
        emit message(tr("Authentification raté! Vous êtes maintenant non authentifié(e)."));
        break;
    }
    case SMSG_YOU_ARE_NOT_AUTHENTIFIED:
    {
        emit message(tr("Impossible de demander les devoirs car nous sommes considérés comme non authentifié(e) !!"));
        break;
    }

    default:
    {
        emit message(tr("Header inconnu envoyé. (0x%1)").arg(QString::number(code, 16).toUpper()));
    }
    }
}
void Client::receiveData()
{
    QDataStream in(socket);

    if(taillePacket == 0)
    {
        if(socket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> taillePacket;
    }
    if(socket->bytesAvailable() < taillePacket)
        return;

    quint8 code;
    in >> code;

    if(code == SMSG_HOMEWORK)
    {
        QList<Devoir> devoirs;
        quint32 totalHomeworks = 0;
        in >> totalHomeworks;

        for(unsigned int i = 0 ; i < totalHomeworks ; ++i)
        {
            Devoir devoir;
            in >> devoir;
            devoirs.append(devoir);
        }

        emit message("Devoirs reçus avec succés.");
        for(QListIterator<Devoir> it(devoirs) ; it.hasNext();)
        {
            Devoir i = it.next();
            emit message(tr("Devoir n°%1 : %2 avec sujet : %3 pour %4 en classe de %5").arg(QString::number(i.getID()), i.getName(), i.getSujet(), i.getMatiere(), i.getClasse()));
            emit newHomework(i);
        }
    }
    else if(code == SMSG_MESSAGE_CHAT)
    {
        QString from;
        QString chatMessageString;

        in >> from;
        in >> chatMessageString;

        emit chatMessage(chatMessageString, from);
    }
    else if(code == SMSG_LISTMATIERE)
    {
        QStringList matieres;
        in >> matieres;

        emit newListMatieres(matieres);
    }
    else
        processCode(code);

    taillePacket = 0;
}
void Client::SendPacket(int type)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;

    if(type == 0)
        ;
    else if(type == 1)
    {
        out << (quint8) CMSG_MESSAGE_LEGER;
        out << tr("Bonjour! Je suis un paquet.");
        out << tr("Je me suis envoyé deux fois pour voir.");
        out << Hash(tr("Je suis un mot-de-passe super bien crypté."));
    }
    else if(type == 4)
    {
        out << (quint8) CMSG_MESSAGE_CUSTOM;
    }

    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
}
void Client::SendPing()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;

    out << (quint8) CMSG_PING;

    out.device()->seek(0);

    out << (quint16)(paquet.size() - sizeof(quint16));

    socket->write(paquet);
}
void Client::SendPong()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) CMSG_PONG;
    out.device()->seek(0);
    out << (quint16)(paquet.size() - sizeof(quint16));

    socket->write(paquet);
}
void Client::SendAuthentification(const QString &UserName, const QString &Password)
{
    QByteArray PasswordHashed = Hash(Password);
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) CMSG_MESSAGE_AUTH;
    out << UserName;
    out << PasswordHashed;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
}
void Client::SendHomeworkRequest()
{
    if(!IsAuthentified())
    {
        emit message(tr("Vous n'êtes pas authentifié(e) !"));
        return;
    }
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) CMSG_MESSAGE_HOMEWORKFOR;
    out << (bool) false;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
}
void Client::SendMessage(const QString &messageChat)
{
    if(!IsAuthentified())
    {
        emit message(tr("Vous n'êtes pas authentifié(e) !"));
        return;
    }

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) CMSG_MESSAGE_CHAT;
    out << messageChat;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
}
void Client::SendDisconnection()
{
    /*
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) CMSG_DISCONNECT;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
    socket->waitForBytesWritten();*/
}
void Client::Flush()
{
    socket->flush();
}
void Client::SendAvailableMatieresRequest()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) CMSG_MESSAGE_LISTMATIERE;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
}

void Client::error(QAbstractSocket::SocketError error)
{
    switch(error)
    {
    case QAbstractSocket::ConnectionRefusedError:
        emit displayError(tr("La connexion a été refusé par l'hôte dû peut-être à un temps de latence trop grand...."));
        break;
    case QAbstractSocket::RemoteHostClosedError:
        emit displayError(tr("L'hôte a coupé la connexion."));
        break;
    case QAbstractSocket::HostNotFoundError:
        emit displayError(tr("Erreur... L'agendaileur n'a pas été trouvé..."));
        break;
    default:
        emit displayError(socket->errorString());
        break;
    }
}

QByteArray Hash(const QString &toHash)
{
    return QCryptographicHash::hash(QCryptographicHash::hash(SEL_BEFORE, QCryptographicHash::Md5) + toHash.toLatin1() + QCryptographicHash::hash(SEL_AFTER, QCryptographicHash::Md5), QCryptographicHash::Sha1);
}
