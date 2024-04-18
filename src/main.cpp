#include "aimainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AIMainWindow w;
    w.show();
    return a.exec();
}
