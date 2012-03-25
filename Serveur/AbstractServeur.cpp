#include "AbstractServeur.hpp"

AbstractServeur::AbstractServeur(QObject *parent) :
    QThread(parent)
{
}

void AbstractServeur::setHandle(int handle)
{
    m_Handle = handle;
}
