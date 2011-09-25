#ifndef FENACCUEIL_HPP
#define FENACCUEIL_HPP

#include <QtNetwork>
#include <QtGui>

#include "FenDevoir.hpp"

#include "Client.hpp"
#include "ui_FenAccueil.h"

class FenAccueil : public QDialog, private Ui::FenAccueil
{
    Q_OBJECT

public:
    explicit FenAccueil(QWidget *parent = 0);
    ~FenAccueil();

public slots:
    virtual void PutMessage(QString const& message);

private slots:
    void on_connexion_clicked();
    void on_OpenFenDevoir_clicked();
    void on_auth_clicked();
    void on_ping_clicked();

    void connected();
    void disconnected();

    void displayError(const QString& error);

protected:
    void closeEvent(QCloseEvent *e);

private:
    Client* cl;
    FenDevoir* fenetre;
};

#endif // FENACCUEIL_HPP
