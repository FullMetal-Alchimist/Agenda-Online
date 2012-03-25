#include <QtGui/QApplication>
#include "FenPrincipale.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Devoir::initSystem();
    SQLServerSupervisor::GetInstance();

    FenPrincipale w;

    w.show();

    return a.exec();
}
