#include "MainServeur.hpp"

QMap<QString, MainServeur* > MainServeur::m_Clients = QMap<QString, MainServeur* >();
QSettings MainServeur::s_Settings("SOS Danbou", "Agenda-Online Server");

MainServeur::MainServeur(int handle, QObject *parent) :
    AbstractServeur(parent), taillePacket(0), UserName(tr("NULLCLIENT")), threadRunning(true), errorFatal(false)
{
}
MainServeur* MainServeur::Clone()
{
    return new MainServeur(*this);
}

MainServeur::~MainServeur()
{
    if(m_Clients.contains(UserName))
        m_Clients.remove(UserName);
    delete myClient;
    delete m_Authentification;
}

void MainServeur::run()
{
    myClient = new QTcpSocket;

    if(!myClient->setSocketDescriptor(m_Handle))
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
            if(m_Authentification->State() == AuthentificationSystem::Accepted)
                emit removeClient(m_Authentification->GetUserName(), m_Authentification->GetClasse());
            break;
        }
    }
}

void MainServeur::processData()
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
    bool Authentified = m_Authentification->State() == AuthentificationSystem::Accepted;
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
                emit message(tr("Ping reçu de %1.").arg(m_Authentification->GetUserName()));
            else
                emit message(tr("Ping reçu d'un client non authentifié."));

            reponse(SMSG_PONG);
            break;
        }
        case CMSG_PONG:
        {
            if(Authentified)
                emit message(tr("Pong! Reçu de %1").arg(m_Authentification->GetUserName()));
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

            m_Authentification = AuthentificationSystem::Authentificate(userName, password);

            if(m_Authentification->Error() == AuthentificationSystem::NoError)
            {
                emit message(tr("Tentative d'authentification de %1 réussi !").arg(userName));
                emit newClient(m_Authentification->GetUserName(), m_Authentification->GetClasse());

                reponse(SMSG_AUTHENTIFICATION_SUCCESS);
            }
            else if(m_Authentification->Error() == AuthentificationSystem::Double_Account_Detected)
            {
                emit message(tr("Tentative d'authentification de %1 raté car double compte détectée.").arg(userName));
                reponse(SMSG_AUTHENTIFICATION_FAILED);
                Kick("AuthentificationSystem: another account is already connected.");
            }
            else if(m_Authentification->Error() == AuthentificationSystem::Password_Incorrect)
            {
                emit message(tr("Tentative d'authentification de %1 raté car le mot de passe est incorrecte.").arg(userName));
                reponse(SMSG_AUTHENTIFICATION_FAILED);
            }
            else if(m_Authentification->Error() == AuthentificationSystem::UserName_Not_Available)
            {
                emit message(tr("Tentative d'authentification de %1 raté car le nom de compte n'existe pas.").arg(userName));
                reponse(SMSG_AUTHENTIFICATION_FAILED);
            }
            else
            {
                emit message(tr("Tentative d'authentification de %1 raté, raison inconnue !").arg(userName));
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

            emit message(tr("Demande de devoir reçu de la part de %1 en classe de %2").arg(m_Authentification->GetUserName(), m_Authentification->GetClasse()));

            QList<Devoir> devoirs = SQLServerSupervisor::GetInstance()->LoadHomeworks(m_Authentification, matiere);
            SendHomeworks(devoirs);
            break;
        }
        case CMSG_MESSAGE_LISTMATIERE:
        {
            if(Authentified)
            {
                QStringList listMatiere = SQLServerSupervisor::GetInstance()->GetAllMatiereFromClasse(m_Authentification);
                emit message(tr("Demande des matières disponibles pour la classe %1 de %2").arg(m_Authentification->GetClasse(), m_Authentification->GetUserName()));
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
void MainServeur::SendHomeworks(const QList<Devoir> &devoirs)
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

void MainServeur::reponse(quint8 rCode)
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

void MainServeur::SendPing()
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

void MainServeur::SendMatieres(const QStringList &matieres)
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

void MainServeur::Kick(QString Reason)
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
    yieldCurrentThread();
    sleep(1);
    if(myClient->state() != QAbstractSocket::UnconnectedState)
    {
        myClient->waitForBytesWritten();
        myClient->disconnectFromHost();
    }
}
