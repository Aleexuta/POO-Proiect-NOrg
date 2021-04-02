#include "QClient.h"

#include <qmessagebox.h>
#include <nlohmann/json.hpp>
#include <iostream>

#include "RegisterForm.h"
#include "LoginForm.h"
QClient* QClient::instance = nullptr;
QClient::QClient(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    
    // facere formular
}
QClient:: ~QClient()
{

}
QClient* QClient::getInstance()
{
    if (!instance)
        instance = new QClient();
    return instance;
}

void QClient::deleteInstance()
{
    if (instance)
        delete instance;
}

void QClient::IncomingMessages()
{
    //QClient* main = QClient::getInstance();
    //afiseaza doar daca primeste aprobare de la server
    bool bQuit = false;
    while (!bQuit)
    {
        if (this->IsConnected())
        {
            if (!this->Incoming().empty())
            {
                auto msg = this->Incoming().pop_front().msg;
                switch (msg.header.id)
                {

                case CustomMsgTypes::ServerAcceptRegister:
                {
                    QMessageBox::information(this, "Server Message", "Register Succes");
                    LoginForm* log = LoginForm::getInstance();
                    log->show();
                    
                    RegisterForm* reg = RegisterForm::getInstance();
                    reg->close();
                   // reg->deleteInstance();
                }
                break;
                case CustomMsgTypes::ServerDenyRegister:
                {
                    QMessageBox::warning(this, "Server Message", "Register Error");
                    RegisterForm* reg = RegisterForm::getInstance();
                    reg->show();
                }
                break;
                case CustomMsgTypes::ServerAcceptLogin:
                {
                    QMessageBox::information(this, "Server Message", "Login Succes");
                    this->show();
                    LoginForm* reg = LoginForm::getInstance();
                    reg->close();
                    std::string rasp(msg.body.begin(), msg.body.end());
                    this->setUserInfo(rasp);
                }
                break;
                case CustomMsgTypes::ServerDenyLogin:
                {
                    QMessageBox::warning(this, "Server Message", "Login Error");
                    LoginForm* reg = LoginForm::getInstance();
                    reg->show();
                }
                break;
                case CustomMsgTypes::ServerAccept:
                {
                    // Server has responded to a ping request				
                    QMessageBox::information(this, "Server Message", "Server Accepted Connection");
                }
                break;
                case CustomMsgTypes::ServerDeny:
                {
                    // Server has responded to a ping request				
                    QMessageBox::warning(this, "Server Message", "Server Deny Connection");
                }
                break;

                }
            }
            else
            {
                bQuit = true;
            }
        }
        else
        {
            QMessageBox::warning(this, "Server Message", "Server Down");
            //a se decomenta la lansare(teste)

            this->disconnect();
            this->deleteInstance();
            exit(EXIT_FAILURE);
            QApplication::quit();
            bQuit = true;

        }
    }
}

void QClient::sendRegisterMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::ServerRegister;
    char * vect=const_cast<char*>(j.c_str());
    for(int i=0;i<j.size();i++)
        msg << vect[i];
    Send(msg);
}

void QClient::sendLoginMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::ServerLogin;
    char* vect = const_cast<char*>(j.c_str());
    for (int i = 0; i < j.size(); i++)
        msg << vect[i];
    Send(msg);
}

void QClient::setUserInfo(std::string mesaj)
{
    auto js2 = nlohmann::json::parse(mesaj);
    std::string user = js2["username"];
    std::string id = js2["iduser"];
    int idd = std::stoi(id);
    this->user = new User;
    this->user->setUsername(user);
    this->user->setID(idd);
    //sparge in json
    //pune valorile unde trebe
}
