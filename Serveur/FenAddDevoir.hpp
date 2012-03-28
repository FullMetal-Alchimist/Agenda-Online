#ifndef FENADDDEVOIR_H
#define FENADDDEVOIR_H

#include "ui_FenAddDevoir.h"

class FenAddDevoir : public QDialog, private Ui::FenAddDevoir
{
        Q_OBJECT

    public:
        explicit FenAddDevoir(QWidget *parent = 0);

    signals:
        void AddDevoir(QString const& nom, QString const& sujet, QString const& matiere, QString const& classe, QDate const& date);
        void Canceled();

    private slots:
        void on_addDevoir_clicked();
        void on_cancelDevoir_clicked();

};

#endif // FENADDDEVOIR_H
