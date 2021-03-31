#include "QClient.h"

#include <nlohmann/json.hpp>
#include <iostream>
QClient* QClient::instance = nullptr;
QClient::QClient(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    // facere formular
    
}
QClient:: ~QClient()
{
    delete this;
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

void QClient::sendRegisterMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::ServerRegister;
    char * vect=const_cast<char*>(j.c_str());
    for(int i=0;i<j.size();i++)
        msg << vect[i];

    Send(msg);
}
