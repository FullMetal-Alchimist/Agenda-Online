/**************************************************************************
**                  Ryan Lahfa 2009-2011 ©
**            Ce code source a été crée et/ou édité le 25/09/2011
**
**                      main.cpp:
**************************************************************************/

#include <QtGui/QApplication>
#include "FenLauncher.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FenLauncher w;
    w.show();

    return a.exec();
}
