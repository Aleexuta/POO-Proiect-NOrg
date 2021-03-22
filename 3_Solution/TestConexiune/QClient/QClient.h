#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QClient.h"
#include <olc_net.h>

enum class CustomMsgTypes : uint32_t
{
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

public:
    QClient(QWidget *parent = Q_NULLPTR);

private:
    Ui::QClientClass ui;
};
