#include "FenAccueil.hpp"

FenAccueil::FenAccueil(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    cl = new Client(this);
    fenetre = new FenDevoir(cl);
    DebugConsole->setReadOnly(true);

    connect(cl, SIGNAL(connected()), this, SLOT(connected()));
    connect(cl, SIGNAL(disconnected()), this, SLOT(disconnected()));

    connect(cl, SIGNAL(displayError(QString)), this, SLOT(displayError(QString)));

    connect(cl, SIGNAL(message(QString)), SLOT(PutMessage(QString)));

#ifndef _DEBUG
    groupBox->hide();
#endif
}
FenAccueil::~FenAccueil()
{
    delete fenetre;
}
void FenAccueil::closeEvent(QCloseEvent *e)
{
    if(cl->IsAuthentified())
        cl->SendDisconnection();

    e->accept();
}

void FenAccueil::on_connexion_clicked()
{
    connexion->setDisabled(true);
    cl->connectTo(ipAddrOfServer->text(), portOfServer->value());

    PutMessage(tr("Connexion en cours."));

#ifndef _DEBUG
    if(!cl->IsAuthentified())
        cl->SendAuthentification(UserName->text(), Password->text());
#endif
}
void FenAccueil::connected()
{
    PutMessage(tr("Connecté."));
    connexion->setDisabled(true);

#ifndef _DEBUG
    fenetre->show();
    close();
#endif
}
void FenAccueil::disconnected()
{
    PutMessage(tr("Déconnecté."));
    connexion->setDisabled(false);
}
void FenAccueil::displayError(const QString &error)
{
    PutMessage(tr("Une erreur est survenu lors de la connexion au serveur\n%1").arg(error));
    connexion->setDisabled(false);
}

void FenAccueil::PutMessage(const QString &message)
{
    qApp->alert(this);
#ifdef _DEBUG
    DebugConsole->insertPlainText(message + tr("\n"));
#else
    QMessageBox::information(this, tr("Agenda-Online"), message);
#endif
}

void FenAccueil::on_auth_clicked()
{
    if(!cl->IsAuthentified())
        cl->SendAuthentification(UserName->text(), Password->text());
}
void FenAccueil::on_ping_clicked()
{
    cl->SendPing();
}
void FenAccueil::on_OpenFenDevoir_clicked()
{
    fenetre->show();
}
