#pragma once

#include <QtWidgets/QMainWindow>
#include <nlohmann/json.hpp>


#include "ui_QClient.h"
#include <olc_net.h>
#include "IUser.h"

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerAcceptRegister,
    ServerDenyRegister,
    ServerAcceptLogin,
    ServerDenyLogin,
    ServerDeny,
	ServerRegister,
    ServerLogin,
    CreateNode,
    OpenNode,
    SaveNode,
    LoadNodes
};

class QClient : public QMainWindow, public olc::net::client_interface<CustomMsgTypes>
{
    Q_OBJECT

private:
    static QClient* instance;
     QClient(QWidget *parent = Q_NULLPTR);
    ~QClient();
private:
    Ui::QClientClass ui;
    IUser* user;

public:
    static QClient* getInstance();
    void deleteInstance();

    void sendRegisterMessage(std::string j);
};
