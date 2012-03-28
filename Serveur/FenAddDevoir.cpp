#include "FenAddDevoir.hpp"

FenAddDevoir::FenAddDevoir(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    calendar->setGridVisible(true);
}

void FenAddDevoir::on_addDevoir_clicked()
{
    emit AddDevoir(DevoirName->text(), sujetDevoir->toPlainText(), matiereDevoir->text(), classeDevoir->text(), calendar->selectedDate());
}
void FenAddDevoir::on_cancelDevoir_clicked()
{
    emit Canceled();
}
