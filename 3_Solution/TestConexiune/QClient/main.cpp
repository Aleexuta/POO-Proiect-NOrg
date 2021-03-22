#include "QClient.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QClient w;
    w.Connect("0.tcp.ngrok.io",10000);
    w.show();
    return a.exec();
}
