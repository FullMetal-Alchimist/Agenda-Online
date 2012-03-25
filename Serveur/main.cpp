#include <QtGui/QApplication>
#include "FenPrincipale.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Devoir::initSystem();
    ChatServer::CreateInstance();
    SQLServerSupervisor::GetInstance();

    FenPrincipale w;

#ifdef Q_OS_SYMBIAN
    w.showMinimized();
#else
    w.show();
#endif

    return a.exec();
}
