#include "QClient.h"
//#include <QtWidgets/QApplication>
#include <fstream>

#include <qapplication.h>
#include <QtWidgets/QApplication.h>
#include <qmessagebox.h>
#include "FirstForm.h"
#include "RegisterForm.h"
#include "Common_function.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QClient* w = QClient::getInstance();

    // apare doar daca s a conectat la server


    std::fstream server("server_info.txt", std::ios::in);
    if (!server)
    {
        QMessageBox::critical(w, "ERROR", "Client down");
        return 0;
    }
    std::string serv;
    long int port;
    server >> serv;
    server >> port;
    //deschide un fisier
    w->Connect(serv, port);

    w->IncomingMessages();

    FirstForm* f1 = FirstForm::getInstance();
    f1->show();
    w->IncomingMessages();
    return a.exec();
}
