/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 24/09/2011
**
**                      FenDevoir.hpp:FenDevoir
**************************************************************************/

#ifndef FENDEVOIR_HPP
#define FENDEVOIR_HPP

#include <QStandardItem>
#include <QStandardItemModel>

#include "Client.hpp"
#include "ui_FenDevoir.h"

class FenDevoir : public QDialog, private Ui::FenDevoir
{
    Q_OBJECT

public:
    FenDevoir(Client* cl, QWidget *parent = 0);

public slots:
    void addHomework(Devoir const& devoir);
    void addMessage(QString const& message, QString const& from);
    void addMatieres(QStringList const& matieres);

private slots:
    void on_SendHomeworkRequest_clicked();
    void on_SendMessage_clicked();
    void on_requestMatieres_clicked();

private:
    void initModel();
    void SetHeaderData();

protected:
    void changeEvent(QEvent *e);

private:
    Client* cl;
    QStandardItemModel* model;
    QList<Devoir> currentHomeworks;
    QList<QStringList> currentMatieres;
};

#endif // FENDEVOIR_HPP
