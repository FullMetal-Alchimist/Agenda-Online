#ifndef ABSTRACTSERVEUR_H
#define ABSTRACTSERVEUR_H

#include <QThread>

class AbstractServeur : public QThread
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

            COUNT = SMSG_COMMAND_REFUSED + 1
        };
        typedef quint8 Header;
        explicit AbstractServeur(QObject *parent = 0);

        virtual AbstractServeur* Clone() const = 0;
        void setHandle(int handle);
    signals:
        void message(QString const& message);

    protected:
        int m_Handle;

};

#endif // ABSTRACTSERVEUR_H
