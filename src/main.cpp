#include "aimainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AIMainWindow w;
    w.resize(1200, 800);
    w.show();
    return a.exec();
}
