/**************************************************************************
**                  Ryan Lahfa 2009-2011 Â©
**            Ce code source a Ã©tÃ© crÃ©e et/ou Ã©ditÃ© le 24/09/2011
**
**                      FenDevoir.cpp:FenDevoir
**************************************************************************/

#include "FenDevoir.hpp"

FenDevoir::FenDevoir(Client *cl, QWidget *parent) :
    QDialog(parent), cl(cl)
{
    setupUi(this);
    connect(cl, SIGNAL(newHomework(Devoir)), this, SLOT(addHomework(Devoir)));
    connect(cl, SIGNAL(chatMessage(QString,QString)), this, SLOT(addMessage(QString,QString)));

    zoneMessage->setReadOnly(true);
    connect(message, SIGNAL(returnPressed()), this, SLOT(on_SendMessage_clicked()));

    connect(cl, SIGNAL(newListMatieres(QStringList)), this, SLOT(addMatieres(QStringList)));

    initModel();
}

void FenDevoir::initModel()
{
    model = new QStandardItemModel(this);

    SetHeaderData();

    devoirs->setModel(model);
}

void FenDevoir::on_SendHomeworkRequest_clicked()
{
    cl->SendHomeworkRequest();
    cl->Flush();
}
void FenDevoir::on_SendMessage_clicked()
{
    cl->SendMessage(message->text());

    message->clear();
    message->setFocus();
}

void FenDevoir::SetHeaderData()
{
    model->setHeaderData(0, Qt::Horizontal, tr("ID"), Qt::DisplayRole);
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"), Qt::DisplayRole);
    model->setHeaderData(2, Qt::Horizontal, tr("Sujet"), Qt::DisplayRole);
    model->setHeaderData(3, Qt::Horizontal, tr("Matière"), Qt::DisplayRole);
    model->setHeaderData(4, Qt::Horizontal, tr("Date"), Qt::DisplayRole);

    devoirs->resizeColumnsToContents();
    devoirs->resizeRowsToContents();
    devoirs->verticalHeader()->hide();
}

void FenDevoir::addHomework(const Devoir &devoir)
{
    if(currentHomeworks.contains(devoir))
        return;

    currentHomeworks.append(devoir);
    QList<QStandardItem* > items;
    QStandardItem* IDItem = new QStandardItem(QString::number(devoir.getID()));
    QStandardItem* NomItem = new QStandardItem(devoir.getName());
    QStandardItem* SujetItem = new QStandardItem(devoir.getSujet());
    QStandardItem* MatiereItem = new QStandardItem(devoir.getMatiere());
    QStandardItem* DateItem = new QStandardItem(devoir.getDate().toString());

    IDItem->setWhatsThis(tr("Un Universal Identifier (inutile pour les élèves et professeurs)"));
    NomItem->setWhatsThis(tr("Nom du Devoir"));
    SujetItem->setWhatsThis(tr("Sujet du Devoir"));
    MatiereItem->setWhatsThis(tr("Matière du Devoir"));
    DateItem->setWhatsThis(tr("Date à laquelle le devoir devra être effectué"));

    items << IDItem << NomItem << SujetItem << MatiereItem << DateItem;

    model->appendRow(items);
    model->sort(0);

    SetHeaderData();
}
void FenDevoir::addMessage(const QString &message, const QString &from)
{
    zoneMessage->insertPlainText(from + tr(": ") + message + tr("\n"));
}
void FenDevoir::addMatieres(const QStringList &matieres)
{
    if(currentMatieres.contains(matieres))
        return;
    currentMatieres << matieres;
    matieresDisponibles->addItems(matieres);
}
void FenDevoir::on_requestMatieres_clicked()
{
    cl->SendAvailableMatieresRequest();
    cl->Flush();
}

void FenDevoir::changeEvent(QEvent *e)
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
