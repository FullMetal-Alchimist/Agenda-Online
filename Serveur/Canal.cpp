#include "Canal.hpp"

QMap<int, Canal* > Canal::s_CanalsByID;
QMap<QString, Canal* > Canal::s_CanalsByName;
Canal* Canal::s_MainCanal = new Canal(0, "Main Canal");

Canal::Canal(int id, const QString &nom) : m_ID(id), m_Nom(nom)
{
}

int Canal::GetID() const
{
    return m_ID;
}
QString Canal::GetNom() const
{
    return m_Nom;
}

void Canal::AddParticipant(AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket *Socket)
{
    m_Participants[System] = Socket;
}
void Canal::SendMessageOnCanal(AuthentificationSystem::AuthentificationSystemPtr fromSystem, const QString &message)
{
    if(m_Participants.contains(fromSystem))
    {
        QByteArray packet = CreatePacketBaseMessage(fromSystem, message);
        for(QMapIterator<AuthentificationSystem::AuthentificationSystemPtr, QTcpSocket* > it(m_Participants) ; it.hasNext() ;)
        {
            m_Mutex.lock();
            it.next();
            it.value()->write(packet);
            it.value()->waitForBytesWritten();
            m_Mutex.unlock();
        }
    }
}
void Canal::SendSystemCanalMessage(const QString &message)
{
    QByteArray packet = CreatePacketSystemMessage("SYSTEM", message);
    for(QMapIterator<AuthentificationSystem::AuthentificationSystemPtr, QTcpSocket* > it(m_Participants) ; it.hasNext() ;)
    {
        m_Mutex.lock();
        it.next();
        it.value()->write(packet);
        it.value()->waitForBytesWritten();
        m_Mutex.unlock();
    }
}

QTcpSocket* Canal::RemoveParticipant(AuthentificationSystem::AuthentificationSystemPtr System)
{
    return m_Participants.take(System);
}
QByteArray Canal::CreatePacketBaseMessage(AuthentificationSystem::AuthentificationSystemPtr pSystem, const QString &message)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_MESSAGE_CHAT_CANAL;
    out << pSystem->GetUserName();
    out << pSystem->GetClasse();
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    return packet;
}
QByteArray Canal::CreatePacketSystemMessage(const QString &IdentificationName, const QString &message)
{
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << (quint8) SMSG_MESSAGE_CHAT_CANAL_SYSTEM;
    out << IdentificationName;
    out << message;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));

    return packet;
}


Canal* Canal::CreateCanal(int id, const QString &nom)
{
    if(s_CanalsByID.contains(id) || s_CanalsByName.contains(nom))
        return NULL;

    Canal* canal = new Canal(id, nom);

    s_CanalsByID[id] = canal;
    s_CanalsByName[nom] = canal;

    return canal;
}
bool Canal::DeleteCanal(const QString &nom)
{
    if(!s_CanalsByName.contains(nom) || nom == "Main Canal")
        return false;

    Canal* canalToDelete = s_CanalsByName[nom];
    canalToDelete->SendSystemCanalMessage("!! Canal is going to be destroyed by internal system !!");
    delete canalToDelete;
    return true;
}
bool Canal::DeleteCanal(int id)
{
    if(!s_CanalsByID.contains(id) || id == 0)
        return false;

    Canal* canalToDelete = s_CanalsByID[id];
    canalToDelete->SendSystemCanalMessage("!! Canal is going to be destroyed by internal system !!");
    delete canalToDelete;
    return true;
}
Canal* Canal::SubscribeCanal(const QString &nom, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket *Socket)
{
    if(!s_CanalsByName.contains(nom))
        return s_MainCanal;

    s_CanalsByName[nom]->AddParticipant(System, Socket);
    return s_CanalsByName[nom];
}
Canal* Canal::SubscribeCanal(int id, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket *Socket)
{
    if(!s_CanalsByID.contains(id))
        return s_MainCanal;

    s_CanalsByID[id]->AddParticipant(System, Socket);
    return s_CanalsByID[id];
}

void Canal::SendByCanal(const QString &message, const QString &canalName, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket *Socket)
{
    if(!s_CanalsByName.contains(canalName))
        return;

    s_CanalsByName[canalName]->SendByCanal(message, System, Socket);
}
void Canal::SendByCanal(const QString &message, AuthentificationSystem::AuthentificationSystemPtr System, QTcpSocket *Socket)
{
    AddParticipant(System, Socket);
    SendMessageOnCanal(System, message);
    RemoveParticipant(System);
}

Canal* Canal::MainCanal()
{
    return s_MainCanal;
}
