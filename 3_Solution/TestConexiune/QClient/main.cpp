#include "QClient.h"
#include <QtWidgets/QApplication>
#include <qmessagebox.h>
#include "FirstForm.h"
#include "RegisterForm.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QClient* w = QClient::getInstance();
    FirstForm f1;
    // apare doar daca s a conectat la server
    w->Connect("6.tcp.ngrok.io",16296);
    f1.show();
	bool bQuit = false;
	//while (!bQuit)
	{
		if (w->IsConnected())
		{
			if (!w->Incoming().empty())
			{


				auto msg = w->Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerAcceptRegister:
				{
					// Server has responded to a ping request				
					QMessageBox::information(w, "Server Message", "Server Accepted Connection");
					QClient* main = QClient::getInstance();
					main->show();
				}
				break;
				case CustomMsgTypes::ServerDenyRegister:
				{
					// Server has responded to a ping request				
					QMessageBox::warning(w, "Server Message", "Server Deny Connection");
				}
				break;

				}
			}
		}
		else
		{
			QMessageBox::warning(w, "Server Message", "Server Down");
			//a se decomenta la lansare(teste)

			/*w->disconnect();
			w->deleteInstance();
			exit(EXIT_FAILURE);
			QApplication::quit();*/
			bQuit = true;
			
		}
	}

    return a.exec();
}
