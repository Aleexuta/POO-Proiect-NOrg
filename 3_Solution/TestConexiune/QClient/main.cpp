#include "QClient.h"
#include <QtWidgets/QApplication>
#include "FirstForm.h"
#include "RegisterForm.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QClient* w = QClient::getInstance();
    FirstForm f1;
    // apare doar daca s a conectat la server
    w->Connect("8.tcp.ngrok.io",11702);
    f1.show();
   
    return a.exec();
}
