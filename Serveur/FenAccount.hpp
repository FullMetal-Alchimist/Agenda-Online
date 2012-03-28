#ifndef FENACCOUNT_H
#define FENACCOUNT_H

#include <QString>
#include "ui_FenAccount.h"

class FenAccount : public QDialog, private Ui::FenAccount
{
        Q_OBJECT

    public:
        explicit FenAccount(QWidget *parent = 0);

        void GetInformation(int id, QString const& AccountName, QString const& PasswordEncrypted, QString const& Classe);
    signals:
        void AddAccount(QString const& AccountName, QString const& PasswordValue, QString const& Classe);
        /** void ModificationPasswordAccount(QString const& AccountName, QString const& newPassword); **/
        /** void ModificationAccountName(int id, QString const& newAccountName); **/
        /** void ModificationClasse(QString const& AccountName, QString const& newClasse); **/
        void DeleteAccount(QString const& AccountName);
        void DeleteAccount(int id);
        void NeedInformationFromID(int id);
        void NeedInformationFromAccountName(QString const& name);

    private slots:
        void on_execute_clicked();
        void on_set_toggled(bool e);
        void on_del_toggled(bool e);
        void on_useIDForDelete_toggled(bool e);
        void on_useIDForInterrogation_toggled(bool e);
        void on_interrogate_toggled(bool e);

};

#endif // FENACCOUNT_H
