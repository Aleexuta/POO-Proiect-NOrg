#include "QClient.h"
//#include <QtWidgets/QApplication>


#include <qapplication.h>
#include <qmessagebox.h>
#include "FirstForm.h"
#include "RegisterForm.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QClient* w = QClient::getInstance();

    // apare doar daca s a conectat la server
    w->Connect("0.tcp.ngrok.io",15394);

    w->IncomingMessages();
    FirstForm* f1 = FirstForm::getInstance();
    f1->show();
    w->IncomingMessages();
  //  f1->deleteInstance();
   // RegisterForm* reg = RegisterForm::getInstance();
   // reg->deleteInstance();
    return a.exec();
}
