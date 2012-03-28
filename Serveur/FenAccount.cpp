#include "FenAccount.hpp"

FenAccount::FenAccount(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

void FenAccount::on_execute_clicked()
{
    if(set->isChecked())
    {
        // Not implemented yet.
    }
    else if(add->isChecked())
    {
        emit AddAccount(accountLine->text(), passwordLine->text(), classeLine->text());
    }
    else if(del->isChecked())
    {
        if(useIDForDelete->isChecked())
            emit DeleteAccount(IdLine->value());
        else
            emit DeleteAccount(accountLine->text());
    }
    else if(interrogate->isChecked())
    {
        if(useIDForInterrogation->isChecked())
            emit NeedInformationFromID(IdLine->value());
        else
            emit NeedInformationFromAccountName(accountLine->text());
    }
}
void FenAccount::on_set_toggled(bool e)
{
    IdLine->setEnabled(e);
}
void FenAccount::on_del_toggled(bool e)
{
    useIDForDelete->setEnabled(e);
}
void FenAccount::on_useIDForDelete_toggled(bool e)
{
    IdLine->setEnabled(e);
}
void FenAccount::on_interrogate_toggled(bool e)
{
    useIDForInterrogation->setEnabled(e);
}
void FenAccount::on_useIDForInterrogation_toggled(bool e)
{
    IdLine->setEnabled(e);
}
void FenAccount::GetInformation(int id, const QString &AccountName, const QString &PasswordEncrypted, const QString &Classe)
{
    IdLine->setValue(id);
    accountLine->setText(AccountName);
    passwordLine->setEchoMode(QLineEdit::Normal);
    passwordLine->setText(PasswordEncrypted);
    classeLine->setText(Classe);
}
