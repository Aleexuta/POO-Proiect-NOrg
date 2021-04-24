#pragma once

//#include <QtWidgets/QMainWindow>
#include <nlohmann/json.hpp>
#include <qmainwindow.h>
#include "ui_QClient.h"
#include <olc_net.h>
#include "IUser.h"
class TreeModel;
class TreeItem;
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
    NewNode,
    NewNodeAccept,
    NewNodeDeny,
    LoadAllNodes,
    LoadAllNodesAccept,
    RemoveNode,
    OpenNode,
    SaveNode
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
    TreeModel* model;
public:
    static QClient* getInstance();
    void deleteInstance();
    void IncomingMessages();

    void sendRegisterMessage(std::string j);
    void sendLoginMessage(std::string j);
    void sendNewNodeMessage(std::string j);
    void sendRemoveNodeMessage(std::string j);

    void setUserInfo(std::string mesaj);
    void setGuestInfo();
    void setNodeId(int id);

    TreeItem* getRootItem();
    int  getNumberOfNodes();
    void incrementNumberOfNodes();
private:
    auto makeJsonNewNode(std::string name, int iduser, int idparent, int idnode, std::string namephoto = "");
    void sendLoadAllNodesMessage(std::string id);
    void LoadAllNodes(std::string j);
    void LoadChildren(TreeItem * root,nlohmann::basic_json<> js, int &pos);
    void prepareChildToInsert(TreeItem* root, nlohmann::basic_json<> js, int pos);
public:
    void insertNewNode(const std::string photo,const std::string name);
    void inservNewSubnode(const std::string photo,const std::string name);
    void updateActions();
    void deleteNode();
public slots:
    void on_actionExit_triggered();
    void on_actionAdd_New_Node_triggered();
    void on_actionAdd_New_Subnode_triggered();
    void on_actionDelete_Node_triggered();

};
