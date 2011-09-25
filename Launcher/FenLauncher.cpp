/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 25/09/2011
**
**                      FenLauncher.cpp:FenLauncher
**************************************************************************/

#include "FenLauncher.hpp"

FenLauncher::FenLauncher(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    mapper = new QSignalMapper(this);

    connect(commandLinkButton, SIGNAL(pressed()), this, SLOT(SetMapping()));
    connect(commandLinkButton_2, SIGNAL(pressed()), this, SLOT(SetMapping()));
    connect(commandLinkButton_3, SIGNAL(pressed()), this, SLOT(SetMapping()));
    connect(commandLinkButton_4, SIGNAL(pressed()), this, SLOT(SetMapping()));

    connect(commandLinkButton, SIGNAL(released()), mapper, SLOT(map()));
    connect(commandLinkButton_2, SIGNAL(released()), mapper, SLOT(map()));
    connect(commandLinkButton_3, SIGNAL(released()), mapper, SLOT(map()));
    connect(commandLinkButton_4, SIGNAL(released()), mapper, SLOT(map()));

    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(launch(QString)));
}

void FenLauncher::SetMapping()
{
    QCommandLinkButton* button = qobject_cast<QCommandLinkButton* >(sender());
    if(button == 0)
        return;

    mapper->setMapping(button, button->text());
}
void FenLauncher::launch(QString nomButton)
{
    if(nomButton == commandLinkButton->text())
    {
        QProcess::startDetached(tr("./Clientd.exe"));
    }
    else if(nomButton == commandLinkButton_2->text())
    {
        QProcess::startDetached(tr("./Client.exe"));
    }
    else if(nomButton == commandLinkButton_3->text())
    {
        QProcess::startDetached(tr("./Serveurd.exe"));
    }
    else if(nomButton == commandLinkButton_4->text())
    {
        QProcess::startDetached(tr("./Serveur.exe"));
    }
    close();
}

void FenLauncher::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        retranslateUi(this);
        break;
    default:
        break;
    }
}
