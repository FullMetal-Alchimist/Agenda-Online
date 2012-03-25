#include "ChatServeur.hpp"

ChatServeur::ChatServeur(QObject *parent) :
    AbstractServeur(parent)
{
}


ChatServeur* ChatServeur::Clone() const
{
    return new ChatServeur(*this);
}
