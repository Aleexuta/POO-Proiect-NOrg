#include "QClient.h"
//#include <QtWidgets/QApplication>
#include <fstream>

#include <qapplication.h>
#include <qmessagebox.h>
#include "FirstForm.h"
#include "RegisterForm.h"
#include "Common_function.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QClient* w = QClient::getInstance();

    // apare doar daca s a conectat la server
    w->Connect("6.tcp.ngrok.io",16179);

    w->IncomingMessages();


    //deschide un fisier


        FirstForm* f1 = FirstForm::getInstance();
        f1->show();
        w->IncomingMessages();
    w->IncomingMessages();
    return a.exec();
}
