#include "Serveur.hpp"

QMap<QString, Serveur* > Serveur::m_Clients = QMap<QString, Serveur* >();
QSettings Serveur::s_Settings("SOS Danbou", "Agenda-Online Server");

Serveur::Serveur(int handle, QObject *parent) :
    QThread(parent), myHandle(handle), taillePacket(0), UserName(tr("NULLCLIENT")), threadRunning(true), errorFatal(false)
{
}
Serveur::~Serveur()
{
    if(m_Clients.contains(UserName))
        m_Clients.remove(UserName);
    delete myClient;
}

void Serveur::run()
{
    myClient = new QTcpSocket;

    if(!myClient->setSocketDescriptor(myHandle))
    {
        emit message(tr("Error : When setting Socket Descriptor : %1").arg(QString::number(myHandle)));
        return;
    }
    while((threadRunning) || (!errorFatal))
    {
        if(myClient->waitForReadyRead(5000))
        {
            emit message(tr("Données reçues depuis %1").arg(UserName));
            processData();
        }
        myClient->waitForBytesWritten();
        msleep(750);

        if(myClient->state() == QTcpSocket::UnconnectedState)
        {
            if(UserName != "NULLCLIENT")
                emit removeClient(UserName, Classe);
            break;
        }
    }
}

void Serveur::processData()
{
    QDataStream in(myClient);

    if(taillePacket == 0)
    {
        if(myClient->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> taillePacket;
    }
    if(myClient->bytesAvailable() < taillePacket)
        return;

    quint8 header;
    in >> header;

    emit message(tr("Données reçues pour Header(%1) et pour taille(%2)").arg(QString::number(header), QString::number(taillePacket)));
    bool Authentified = UserName != "NULLCLIENT";
    switch(header)
    {
        case CMSG_MESSAGE_LEGER:
        {
            QString messageRecu;
            in >> messageRecu;
            QString messageSecondRecu;
            in >> messageSecondRecu;

            QByteArray encryptedPassword;
            in >> encryptedPassword;

            emit message(tr("Message léger reçu depuis un client : %1\n%2\n%3").arg(messageRecu, messageSecondRecu, encryptedPassword.toHex()));
            break;
        }
        case CMSG_MESSAGE_CUSTOM:
        {
            /** Créer un comportement de test. **/
            break;
        }
        case CMSG_PING:
        {
            if(Authentified)
                emit message(tr("Ping reçu de %1.").arg(UserName));
            else
                emit message(tr("Ping reçu d'un client non authentifié."));

            reponse(SMSG_PONG);
            break;
        }
        case CMSG_PONG:
        {
            if(Authentified)
                emit message(tr("Pong! Reçu de %1").arg(UserName));
            else
                emit message(tr("Pong! Reçu d'un client non authentifié."));
            break;
        }
        case CMSG_MESSAGE_AUTH:
        {
            QString userName;
            in >> userName;

            QByteArray password;
            in >> password;

            if(SQLServerSupervisor::GetInstance()->Authentificate(userName, password))
            {
                if(m_Clients.contains(UserName))
                {
                    Serveur* anotherInstanceOfMe = m_Clients[UserName];
                    if(s_Settings.value("KickIfConnected", 0).toInt())
                        anotherInstanceOfMe->Kick("You are already connected.");
                    else
                    {
                        Kick("You are already connected.");
                        break;
                    }
                }
                UserName = userName;
                Classe = SQLServerSupervisor::GetInstance()->FindClasse(userName);

                emit message(tr("Tentative d'authentification de %1 réussi !").arg(userName));
                emit newClient(userName, Classe);

                m_Clients[UserName] = this;

                reponse(SMSG_AUTHENTIFICATION_SUCCESS);
            }
            else
            {
                emit message(tr("Tentative d'authentification de %1 raté !").arg(userName));
                reponse(SMSG_AUTHENTIFICATION_FAILED);
            }

            break;
        }
        case CMSG_MESSAGE_HOMEWORKFOR:
        {
            if(!Authentified)
            {
                reponse(SMSG_YOU_ARE_NOT_AUTHENTIFIED);
                emit message(tr("Demande de devoir de la part d'un client non authentifié !! (Refusé)"));
                break;
            }
            QString matiere = tr("all");
            bool needOnlyMatiere;
            in >> needOnlyMatiere;

            if(needOnlyMatiere)
                in >> matiere;

            emit message(tr("Demande de devoir reçu de la part de %1 en classe de %2").arg(UserName, Classe));

            QList<Devoir> devoirs = SQLServerSupervisor::GetInstance()->LoadHomeworks(Classe, matiere);
            SendHomeworks(devoirs);
            break;
        }
        case CMSG_MESSAGE_CHAT:
        {
            if(!Authentified)
            {
                reponse(SMSG_YOU_ARE_NOT_AUTHENTIFIED);
                emit message(tr("Message envoyé d'un client inconnu."));
                break;
            }

            QString messageChat;
            in >> messageChat;

            emit message(tr("Message envoyé par Chat de la part de %1, contenu(%2)").arg(UserName, messageChat));

            SendMessageAt(UserName, messageChat, Classe);
            break;
        }
        case CMSG_MESSAGE_LISTMATIERE:
        {
            if(Authentified)
            {
                QStringList listMatiere = SQLServerSupervisor::GetInstance()->GetAllMatiereFromClasse(Classe);
                emit message(tr("Demande des matières disponibles pour la classe %1 de %2").arg(Classe, UserName));
                SendMatieres(listMatiere);
            }
            else
            {
                emit message(tr("Demande des matières disponibles d'un utilisateur inconnu."));
            }

            break;
        }
        default:
        {
            emit message(tr("Header inconnu détecté : 0x%1").arg(QString::number(header, 16).toUpper()));
            break;
        }
    }

    taillePacket = 0;
}
void Serveur::SendHomeworks(const QList<Devoir> &devoirs)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_HOMEWORK;
    out << (quint32) devoirs.size();

    for(QListIterator<Devoir> it(devoirs) ; it.hasNext() ;)
    {
        Devoir devoir = it.next();
        out << devoir;
    }
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_Write(paquet);
}

void Serveur::reponse(quint8 rCode)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) rCode;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_Write(paquet);

    emit message(tr("Reponse sended with rCode = %1").arg(QString::number(rCode)));
}

void Serveur::SendPing()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_PING;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_Write(paquet);

    emit message(tr("Ping sended !"));
}

void Serveur::SendMatieres(const QStringList &matieres)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_LISTMATIERE;
    out << matieres;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_Write(paquet);

    emit message(tr("Matières demandé envoyés."));
}

void Serveur::Kick(QString Reason)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_KICK;
    out << Reason;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_Write(paquet);


    /** By Ryan Lahfa
        Bye bye my friend of network
    **/

    myClient->disconnectFromHost();
}
void Serveur::SendMessageAt(const QString &nom, const QString &message, const QString &classe)
{
    for(QMapIterator<QString, Serveur* > it(m_Clients) ; it.hasNext() ;)
    {
        it.next();
        QString classeOfClient = it.value()->Classe;

        if(classeOfClient != classe && classe != tr("All"))
            continue;

        it.value()->WriteMessage(nom, message);
    }
}
void Serveur::WriteMessage(const QString &From, const QString &Message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) Serveur::SMSG_MESSAGE_CHAT;
    out << From;
    out << Message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_Write(paquet);
}
void Serveur::SendPrivateMessage(const QString& Message, const QString &DestUser, const QString &DestClasse)
{
    if(m_Clients.contains(DestUser))
    {
        if(m_Clients[DestUser]->Classe == DestClasse)
        {
            m_Clients[DestUser]->WriteMessage(UserName, Message);
        }
    }
}
void Serveur::SendSystemMessage(const QString &message)
{
    SendMessageAt(tr("System"), message, tr("All"));
}
