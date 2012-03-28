#include "ChatServeur.hpp"

QMap<ChatAuth, ChatServeur* > ChatServeur::s_Clients = QMap<ChatAuth, ChatServeur* >();

ChatServeur::ChatServeur(QObject *parent) :
    AbstractServeur(parent), m_DefaultCanal(Canal::MainCanal()), m_Authentification(AuthentificationSystem::CreateAuthentification()), m_TaillePacket(0),
    m_Error(false), m_ThreadRunning(true)
{

}


ChatServeur* ChatServeur::Clone() const
{
    return new ChatServeur(*this);
}

void ChatServeur::run()
{
    m_Client = new QTcpSocket;

    if(!m_Client->setSocketDescriptor(m_Handle))
    {
        emit message(tr("Error : When setting Socket Descriptor : %1").arg(QString::number(m_Handle)));
        return;
    }
    while((m_ThreadRunning) || (!m_Error))
    {
        if(m_Client->waitForReadyRead(5000))
        {
            if(m_Authentification->State() == AuthentificationSystem::Accepted)
                emit message(tr("Données reçues depuis %1").arg(m_Authentification->GetUserName()));
            else
                emit message(tr("Données reçues depuis un client non authentifié."));

            ProcessData();
        }
        m_Client->waitForBytesWritten();
        msleep(750);
    }
}
void ChatServeur::ProcessData()
{
    QDataStream in(m_Client);

    if(m_Client->bytesAvailable() < (int)sizeof(quint16))
        return;

    if(m_TaillePacket == 0)
    {
        in >> m_TaillePacket;
    }
    if(m_Client->bytesAvailable() < m_TaillePacket)
        return;

    quint8 Header;
    in >> Header;

    bool Authentified = m_Authentification->State() == AuthentificationSystem::Accepted;

    switch(Header)
    {
        case CMSG_MESSAGE_AUTH:
        {
            QString userName;
            in >> userName;

            QByteArray password;
            in >> password;

            m_Authentification = AuthentificationSystem::Authentificate(userName, password);

            if(m_Authentification->Error() == AuthentificationSystem::NoError)
            {
                emit message(tr("Tentative d'authentification de %1 réussi !").arg(userName));

                Reponse(SMSG_AUTHENTIFICATION_SUCCESS);
            }
            else if(m_Authentification->Error() == AuthentificationSystem::Double_Account_Detected)
            {
                emit message(tr("Tentative d'authentification de %1 raté car double compte détectée.").arg(userName));
                Reponse(SMSG_AUTHENTIFICATION_FAILED);
                // Kick("AuthentificationSystem: another account is already connected.");
            }
            else if(m_Authentification->Error() == AuthentificationSystem::Password_Incorrect)
            {
                emit message(tr("Tentative d'authentification de %1 raté car le mot de passe est incorrecte.").arg(userName));
                Reponse(SMSG_AUTHENTIFICATION_FAILED);
            }
            else if(m_Authentification->Error() == AuthentificationSystem::UserName_Not_Available)
            {
                emit message(tr("Tentative d'authentification de %1 raté car le nom de compte n'existe pas.").arg(userName));
                Reponse(SMSG_AUTHENTIFICATION_FAILED);
            }
            else
            {
                emit message(tr("Tentative d'authentification de %1 raté, raison inconnue !").arg(userName));
                Reponse(SMSG_AUTHENTIFICATION_FAILED);
            }

            SubscribeCanal(m_Authentification->GetClasse());

            break;
        }
        case CMSG_MESSAGE_CHAT_CLASSE:
        {
            if(!Authentified)
                break;

            QString message;
            in >> message;

            SendMessageOnCanal(message);
            break;
        }
        case CMSG_MESSAGE_PRIVATE_MESSAGE:
        {
            if(!Authentified)
                break;

            QString message;

            QString destUserName;
            QString destClasse;

            in >> message;
            in >> destUserName;
            in >> destClasse;

            SendPrivateMessage(destUserName, destClasse, message);
            break;
        }
        case CMSG_MESSAGE_CHAT_CANAL:
        {
            if(!Authentified)
                break;

            QString message;
            QString destCanalName;

            in >> message;
            in >> destCanalName;

            Canal::SendByCanal(message, destCanalName, m_Authentification, m_Client);
            break;
        }
        case CMSG_PING:
        {
            if(!Authentified)
                emit message(tr("Ping reçu d'un client non authentifié"));
            else
                emit message(tr("Ping reçu de %1").arg(m_Authentification->GetUserName()));

            Reponse(SMSG_PONG);
            break;
        }
        case CMSG_PONG:
        {
            if(!Authentified)
                emit message(tr("Pong reçu d'un client non authentifié."));
            else
                emit message(tr("Pong reçu de %1").arg(m_Authentification->GetUserName()));
            break;
        }
        default:
        {
            if(!Authentified)
                emit message(tr("Header inconnu : 0x%1 venant d'un client non authentifié.").arg(QString::number(Header, 16)));
            else
                emit message(tr("Header inconnu : 0x%1 venant de %2 - %3").arg(QString::number(Header, 16), m_Authentification->GetUserName(), m_Authentification->GetClasse()));
            break;
        }
    }
    m_TaillePacket = 0;
}
void ChatServeur::Reponse(quint8 rCode)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << rCode;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_WriteOnIODevice(m_Client, paquet);
}
void ChatServeur::SendPing()
{
    Reponse(SMSG_PING);
}

void ChatServeur::SendPrivateMessage(const QString &destNom, const QString &destClasse, QString const& message)
{
    ChatAuthPtr auth = AuthentificationSystem::RetrieveFromUserNameAndClasse(destNom, destClasse);
    if(auth->Error() == AuthentificationSystem::CannotRetrieveAuth)
        return;

    ChatServeur* serveurClient = s_Clients[*auth];
    serveurClient->SendMessage(m_Authentification, message);
}
void ChatServeur::SendSystemMessage(const QString &message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_MESSAGE_CHAT_SYSTEM;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_WriteOnIODevice(m_Client, paquet);
}

void ChatServeur::SendMessageOnCanal(const QString &message)
{
    m_DefaultCanal->SendMessageOnCanal(m_Authentification, message);
}
void ChatServeur::SendMessage(AuthentificationSystem::AuthentificationSystemPtr fromSystem, const QString &message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_MESSAGE_CHAT_PRIVATE;
    out << fromSystem->GetUserName();
    out << fromSystem->GetClasse();
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    ThreadSafe_WriteOnIODevice(m_Client, paquet);
}

void ChatServeur::SubscribeCanal(const QString &nom)
{
    m_DefaultCanal = Canal::SubscribeCanal(nom, m_Authentification, m_Client);
}
void ChatServeur::SubscribeCanal(int id)
{
    m_DefaultCanal = Canal::SubscribeCanal(id, m_Authentification, m_Client);
}
