#ifndef ABSTRACTSERVEUR_H
#define ABSTRACTSERVEUR_H

#include <QThread>

class AbstractServeur : public QThread
{
    Q_OBJECT
public:
    explicit AbstractServeur(QObject *parent = 0);

signals:

public slots:

};

#endif // ABSTRACTSERVEUR_H
