#include "FenPrincipale.hpp"

FenPrincipale::FenPrincipale(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    serveur = new Server(this);
    DebugConsole->setReadOnly(true);

    if(serveur->listen(QHostAddress::Any, 50885))
    {
        PutMessage(tr("Le serveur est mis en place ! Les clients peuvent se connecter!"));
    }
    else
        PutMessage(tr("Erreur fatal! Le serveur n'a pas �t� mis en place, les clients ne peuvent pas se connecter....\n%1").arg(serveur->errorString()));

    connect(serveur, SIGNAL(message(QString)), this, SLOT(PutMessage(QString)));
    connect(serveur, SIGNAL(newClient(QString,QString)), this, SLOT(AddClient(QString,QString)));
    connect(serveur, SIGNAL(removeClient(QString,QString)), this, SLOT(RemoveClient(QString,QString)));
    connect(SQLServerSupervisor::GetInstance(), SIGNAL(debug(QString)), this, SLOT(PutMessage(QString)));

    initModel();
}
FenPrincipale::~FenPrincipale()
{
    SQLServerSupervisor::Kill();
}

void FenPrincipale::PutMessage(QString const& message)
{
    qApp->alert(this);
    DebugConsole->insertPlainText(message + tr("\n"));
}
void FenPrincipale::on_addAccount_clicked()
{
    QString userName = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez votre nom d'utilisateur."));
    QString password = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez le mot-de-passe."), QLineEdit::Password);

    QString classe = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez votre classe (Format : niveau partie = 4C pour 4�me3 ou 4�meC"));

    if(SQLServerSupervisor::GetInstance()->CreateAccount(userName, password, classe))
    {
        PutMessage(tr("La cr�ation de compte a r�ussi !"));
    }
    else
    {
        PutMessage(tr("La cr�ation de compte a �chou� !!"));
    }
}
void FenPrincipale::on_addHomework_clicked()
{
    QString nom,sujet,matiere,classe;
    QDate date;
    QCalendarWidget* calendar = new QCalendarWidget(this);
    calendar->setGridVisible(true);

    nom = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez le nom du devoir."));
    sujet = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez le sujet."));
    matiere = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez la mati�re."));
    classe = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez la classe."));
    calendar->show();
    date = calendar->selectedDate();
    calendar->hide();

    if(SQLServerSupervisor::GetInstance()->AddHomework(nom, sujet, matiere, classe, date))
    {
        PutMessage(tr("L'ajout d'un devoir a �t� un succ�s !"));
    }
    else
    {
        PutMessage(tr("L'ajout d'un devoir a �chou� !!"));
    }
}
void FenPrincipale::on_removeHomework_clicked()
{
    QString nom = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez le nom du devoir � supprimer."));

    if(SQLServerSupervisor::GetInstance()->RemoveHomework(nom))
    {
        PutMessage(tr("L'�crasement d'un devoir a �t� succ�s !!"));
    }
    else
    {
        PutMessage(tr("L'�crasement d'un devoir a �chou�!"));
    }
}
void FenPrincipale::initModel()
{
    model = new QStandardItemModel(this);

    SetHeaderData();
    clients->setModel(model);
}
void FenPrincipale::SetHeaderData()
{
    model->setHeaderData(0, Qt::Horizontal, tr("Nom de l'�l�ve"));
    model->setHeaderData(1, Qt::Horizontal, tr("Classe de l'�l�ve"));

    clients->resizeColumnsToContents();
    clients->resizeRowsToContents();
    clients->verticalHeader()->hide();
}

void FenPrincipale::AddClient(const QString &nom, const QString &classe)
{
    QStandardItem* ItemNom = new QStandardItem(nom);
    QStandardItem* ItemClasse = new QStandardItem(classe);

    QList<QStandardItem* > Items;
    Items << ItemNom << ItemClasse;

    model->appendRow(Items);
    clientsMapped.insert(QPair<QString, QString>(nom, classe), ItemNom);

    SetHeaderData();
}
void FenPrincipale::RemoveClient(const QString &nom, const QString &classe)
{
    QStandardItem* Item = clientsMapped[QPair<QString, QString>(nom, classe)];

    model->removeRow(Item->row());

    clientsMapped.remove(QPair<QString, QString>(nom, classe));

    SetHeaderData();
}

