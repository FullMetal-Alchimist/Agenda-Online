#ifndef FENPRINCIPALE_HPP
#define FENPRINCIPALE_HPP

#include <QCalendarWidget>
#include <QStandardItem>
#include <QInputDialog>

#include "ui_FenPrincipale.h"

#include "Server.hpp"
#include "MainServeur.hpp"
#include "ChatServeur.hpp"
#include "SQLServerSupervisor.hpp"
#include "FenAddDevoir.hpp"
#include "FenAccount.hpp"


class FenPrincipale : public QWidget, private Ui::FenPrincipale
{
        Q_OBJECT

    public:
        explicit FenPrincipale(QWidget *parent = 0);
        ~FenPrincipale();

    public slots:
        virtual void PutMessage(QString const& message);
        void AddClient(QString const& nom, QString const& classe);
        void RemoveClient(QString const& nom, QString const& classe);

    private slots:
        void on_gestionAccount_clicked();
        void on_addHomework_clicked();
        void on_removeHomework_clicked();

        void AddDevoir(QString const&, QString const&, QString const&, QString const&, QDate const&);
        void AddAccount(QString const&, QString const&, QString const&);
        void RemoveAccount(QString const&);
        void RemoveAccount(int id);
        void SearchInformation(QString const&);
        void SearchInformation(int id);

    private:
        void initModel();
        void SetHeaderData();

        ServerManager* manager;
        QMap<QPair<QString, QString>, QStandardItem* > clientsMapped;
        QStandardItemModel* model;
        FenAddDevoir* addDevoirFen;
        FenAccount* accountFen;
};

#endif // FENPRINCIPALE_HPP
