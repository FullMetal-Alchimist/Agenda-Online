#ifndef ABSTRACTSERVEUR_H
#define ABSTRACTSERVEUR_H

#include <QThread>
#include <QIODevice>
#include <QMutex>

class AbstractServeur : public QThread
{
        Q_OBJECT
    public:
        explicit AbstractServeur(QObject *parent = 0);

        virtual AbstractServeur* Clone() const = 0;
        void setHandle(int handle);
    signals:
        void message(QString const& message);
    protected:
        inline void ThreadSafe_WriteOnIODevice(QIODevice* ioDevice, QByteArray const& rData)
        {
            m_Mutex.lock();
            ioDevice->write(rData);
            m_Mutex.unlock();
        }

        virtual void Reponse(quint8 rCode) = 0;
        virtual void ProcessData() = 0;
        virtual void SendPing() = 0;

    protected:
        int m_Handle;
        mutable QMutex m_Mutex;

};

#endif // ABSTRACTSERVEUR_H
