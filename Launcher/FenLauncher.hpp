/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 25/09/2011
**
**                      FenLauncher.hpp:FenLauncher
**************************************************************************/

#ifndef FENLAUNCHER_HPP
#define FENLAUNCHER_HPP

#include <QtCore>
#include "ui_FenLauncher.h"

class FenLauncher : public QDialog, private Ui::FenLauncher
{
    Q_OBJECT

public:
    explicit FenLauncher(QWidget *parent = 0);

private slots:
    void launch(QString nomButton);
    void SetMapping();

protected:
    void changeEvent(QEvent *e);

    QSignalMapper* mapper;
};

#endif // FENLAUNCHER_HPP
