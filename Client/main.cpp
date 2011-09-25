#include <QtGui/QApplication>
#include "FenAccueil.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Devoir::initSystem();

    FenAccueil w;
    w.show();

    return a.exec();
}
