#include "FenPrincipale.hpp"

FenPrincipale::FenPrincipale(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    manager = new ServerManager(this);

    addDevoirFen = new FenAddDevoir(this);
    accountFen = new FenAccount(this);

    ChatServeur* baseChat = new ChatServeur;
    MainServeur* baseMain = new MainServeur;

    manager->AddServeur(baseChat);
    manager->AddServeur(baseMain);

    manager->Connect(baseChat, SIGNAL(message(QString)), this, SLOT(PutMessage(QString)));
    manager->Connect(baseMain, SIGNAL(message(QString)), this, SLOT(PutMessage(QString)));
    manager->Connect(baseMain, SIGNAL(newClient(QString,QString)), this, SLOT(AddClient(QString,QString)));
    manager->Connect(baseMain, SIGNAL(removeClient(QString,QString)), this, SLOT(RemoveClient(QString,QString)));

    DebugConsole->setReadOnly(true);

    if(manager->Listen(baseChat, QHostAddress::Any, 50890) && manager->Listen(baseMain, QHostAddress::Any, 50885))
    {
        PutMessage(tr("Le serveur est mis en place ! Les clients peuvent se connecter!"));
    }
    else
        PutMessage(tr("Erreur fatal! Le serveur n'a pas été mis en place, les clients ne peuvent pas se connecter...."));

    connect(SQLServerSupervisor::GetInstance(), SIGNAL(debug(QString)), this, SLOT(PutMessage(QString)));
    connect(addDevoirFen, SIGNAL(AddDevoir(QString,QString,QString,QString,QDate)), this, SLOT(AddDevoir(QString,QString,QString,QString,QDate)));
    connect(accountFen, SIGNAL(AddAccount(QString,QString,QString)), this, SLOT(AddAccount(QString,QString,QString)));
    connect(accountFen, SIGNAL(DeleteAccount(int)), this, SLOT(RemoveAccount(int)));
    connect(accountFen, SIGNAL(DeleteAccount(QString)), this, SLOT(RemoveAccount(QString)));
    connect(accountFen, SIGNAL(NeedInformationFromAccountName(QString)), this, SLOT(SearchInformation(QString)));
    connect(accountFen, SIGNAL(NeedInformationFromID(int)), this, SLOT(SearchInformation(int)));
    connect(addDevoirFen, SIGNAL(Canceled()), this, SLOT(hide()));
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
void FenPrincipale::on_gestionAccount_clicked()
{
    accountFen->show();
}
void FenPrincipale::on_addHomework_clicked()
{
    addDevoirFen->show();
}
void FenPrincipale::AddDevoir(const QString &nom, const QString &sujet, const QString &matiere, const QString &classe, const QDate &date)
{
    if(SQLServerSupervisor::GetInstance()->AddHomework(nom, sujet, matiere, classe, date))
    {
        PutMessage(tr("L'ajout d'un devoir a été un succés !"));
    }
    else
    {
        PutMessage(tr("L'ajout d'un devoir a échoué !!"));
    }
    addDevoirFen->hide();
}
void FenPrincipale::AddAccount(const QString &userName, const QString &password, const QString &classe)
{
    if(SQLServerSupervisor::GetInstance()->CreateAccount(userName, password, classe))
    {
        PutMessage(tr("La création de compte a réussi !"));
    }
    else
    {
        PutMessage(tr("La création de compte a échoué !!"));
    }
}
void FenPrincipale::RemoveAccount(const QString &nom)
{
    if(SQLServerSupervisor::GetInstance()->RemoveAccount(nom))
    {
        PutMessage(tr("La suppression de compte a réussi !"));
    }
    else
    {
        PutMessage(tr("La suppression de compte a échoué !!"));
    }
}
void FenPrincipale::RemoveAccount(int id)
{
    if(SQLServerSupervisor::GetInstance()->RemoveAccount(id))
    {
        PutMessage(tr("La suppression de compte a réussi !"));
    }
    else
    {
        PutMessage(tr("La suppression de compte a échoué !!"));
    }
}
void FenPrincipale::SearchInformation(const QString &nom)
{
    SQLServerSupervisor::GetInstance()->BeginCustomQuery();

    QSqlQuery* query = SQLServerSupervisor::GetInstance()->GetObjQuery();

    query->prepare(tr("SELECT `ID`, `Password`, `Classe` FROM `accounts` WHERE `UserName` = ? LIMIT 1;"));
    query->bindValue(0, nom);

    if(!query->exec())
    {
        qDebug() << "Erreur lors d'une requête personnalisée.";
    }
    else
    {
        query->first();
        int ID = query->value(0).toInt();
        QString Password = query->value(1).toString();
        QString Classe = query->value(2).toString();

        accountFen->GetInformation(ID, nom, Password, Classe);
    }

    SQLServerSupervisor::GetInstance()->EndCustomQuery();
}
void FenPrincipale::SearchInformation(int ID)
{
    SQLServerSupervisor::GetInstance()->BeginCustomQuery();

    QSqlQuery* query = SQLServerSupervisor::GetInstance()->GetObjQuery();

    query->prepare(tr("SELECT `UserName`, `Password`, `Classe` FROM `accounts` WHERE `ID` = ? LIMIT 1;"));
    query->bindValue(0, ID);

    if(!query->exec())
    {
        qDebug() << "Erreur lors d'une requête personnalisée.";
    }
    else
    {
        query->first();
        QString nom = query->value(0).toString();
        QString Password = query->value(1).toString();
        QString Classe = query->value(2).toString();

        accountFen->GetInformation(ID, nom, Password, Classe);
    }

    SQLServerSupervisor::GetInstance()->EndCustomQuery();
}

void FenPrincipale::on_removeHomework_clicked()
{
    QString nom = QInputDialog::getText(this, tr("Entrez ..."), tr("Entrez le nom du devoir à supprimer."));

    if(SQLServerSupervisor::GetInstance()->RemoveHomework(nom))
    {
        PutMessage(tr("L'écrasement d'un devoir a été succés !!"));
    }
    else
    {
        PutMessage(tr("L'écrasement d'un devoir a échoué!"));
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
    model->setHeaderData(0, Qt::Horizontal, tr("Nom de l'élève"));
    model->setHeaderData(1, Qt::Horizontal, tr("Classe de l'élève"));

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

