#include "QClient.h"
#include <QtGui>
#include <qmessagebox.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <qvariant.h>
#include <string>

#include "RegisterForm.h"
#include "LoginForm.h"
#include "FirstForm.h"
#include "NewNode.h"
#include"TreeModel.h"
#include "TreeItem.h"
#include "Common_function.h"
QClient* QClient::instance = nullptr;
QClient::QClient(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);


    // arbore

    QStringList headers;
    headers << tr("Poza") << tr("Nume");
    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly);
    this->model = TreeModel::getInstance(headers, file.readAll());
   // this->model = new TreeModel(headers, file.readAll());
    file.close();
    ui.treeView->setModel(model);
    ui.treeView->header()->setSectionsMovable(false);
    for (int column = 0; column < this->model->columnCount(); ++column)
    {
        ui.treeView->resizeColumnToContents(column);
    }

    updateActions();
}
QClient:: ~QClient()
{
    delete user;
    model->deleteInstance();
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
                    FirstForm* ff = FirstForm::getInstance();
                    ff->close();
                }
                break;
                case CustomMsgTypes::ServerDenyLogin:
                {
                    QMessageBox::warning(this, "Server Message", "Login Error");
                    LoginForm* reg = LoginForm::getInstance();
                    reg->show();
                    FirstForm* ff = FirstForm::getInstance();
                    ff->close();
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
                case CustomMsgTypes::NewNodeAccept:
                {
                  /*  std::string rasp(msg.body.begin(), msg.body.end());
                    auto js = nlohmann::json::parse(rasp);
                    std::string st = js["max(NODE.idnode)"];
                    int id = std::stoi(st);
                    this->setNodeId(id);*/
                }
                break;
                case CustomMsgTypes::NewNodeDeny:
                {
                    //QMessageBox::warning(this, "Server Message", "The New Note was not created correcty");
                }
                break;
                case CustomMsgTypes::LoadAllNodesAccept:
                {
                    std::string rasp(msg.body.begin(), msg.body.end());
                    //LoadAllNodes(rasp);
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

void QClient::sendNewNodeMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::NewNode;
    char* vect = const_cast<char*>(j.c_str());
    for (int i = 0; i < j.size(); i++)
        msg << vect[i];
    Send(msg);

}

void QClient::sendRemoveNodeMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::RemoveNode;
    char* vect = const_cast<char*>(j.c_str());
    for (int i = 0; i < j.size(); i++)
        msg << vect[i];
    Send(msg);
}

void QClient::setUserInfo(std::string mesaj)
{


    auto js2 = nlohmann::json::parse(mesaj);
    std::string user = js2[0]["username"];
    std::string id = js2[0]["iduser"];
    int idd = std::stoi(id);
    this->user = new User;
    this->user->setUsername(user);
    this->user->setID(idd);
    //adauga idul in nodul tata

    std::string nrnode = js2[0]["nrnodes"];
    int nr = std::stoi(nrnode);
    this->user->setNumberOfNodes(nr);
    //seteaza si load la toate nodurile
    this->model->setID(0);
    //sendLoadAllNodesMessage(id);
    LoadAllNodes(mesaj);
    IncomingMessages();
}

void QClient::setGuestInfo()
{
    this->user = new Guest;
}

void QClient::setNodeId(int id)
{
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    this->model->setIDNode(id,index);
}

TreeItem* QClient::getRootItem()
{
    return model->getRootItem();
}

int QClient::getNumberOfNodes()
{
    return user->getNumberOfNodes();
}

void QClient::incrementNumberOfNodes()
{
    user->incrementNrNodes();
}

auto QClient::makeJsonNewNode(std::string name, int iduser, int idparent, int idnode,std::string namephoto)
{
    nlohmann::json js;
    js["name"] = name;
    js["iduser"] = std::to_string(iduser);
    js["idparent"] = std::to_string(idparent);
    js["photoname"] = namephoto;
    js["idnode"] = std::to_string(idnode);
    return js;
}
//posibil sters
void QClient::sendLoadAllNodesMessage(std::string id)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::LoadAllNodes;
    for (int i = 0; i < id.size(); i++)
        msg << id[i];
    Send(msg);
}

void QClient::LoadAllNodes(std::string j)
{
    auto js1 = nlohmann::json::parse(j);
    int pos = 1;
    LoadChildren(getRootItem(), js1, pos);
    updateActions();
    //for (int i = 1; i < js1.size(); i++)
    //{
    //    auto st1 = js1[i];
    //    std::string idnode = st1["idnode"];
    //    std::string idparent = st1["idparent"];
    //    std::string name = st1["name"];
    //    std::string photoname = st1["photoname"];
    //    //insereaza, functie recursiva lucrand pe st1
    //}
}

void QClient::LoadChildren(TreeItem* root, nlohmann::basic_json<> js, int &pos)
{
    pos = 1;
    if (pos < js.size())
    {
        std::string js1 = js[pos]["idparent"];
        std::string noid = js[pos]["idnode"];
        while (pos < js.size())
        {

             if (root->getID() == std::stoi(js1))
             {
                prepareChildToInsert(root, js, pos);
             }

            pos++;
            if (pos < js.size())
            {
                js1 = js[pos]["idparent"];
                noid = js[pos]["idnode"];
            }
        }
    }
    for (int i = 0;i<root->childCount();i++)
    {
        LoadChildren(root->child(i), js, pos);
    }
}

void QClient::prepareChildToInsert(TreeItem* root, nlohmann::basic_json<> js, int pos)
{
    auto st1 = js[pos];
    std::string idnode = st1["idnode"];
    std::string idparent = st1["idparent"];
    std::string name = st1["name"];
    std::string photoname = st1["photoname"];
    root->insertChildrenLoad(0, std::stoi(idnode),NRCOL, QVariant(name.c_str()),QVariant(photoname.c_str()));
    
}

void QClient::insertNewNode(const std::string photo, const std::string name)
{
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    if (!this->user->canNewNode())
        return;

    if (!model->insertRow(index.row() + 1, index.parent()))
        return;
    //{
    //    QModelIndex child = model->index(0, 0, index);
    //    QPixmap ph(photo.c_str());
    //    QImage img(photo.c_str());
    //   // QStandardItem* item = new QStandardItem();
    //   // item->setData(QVariant(QPixmap::fromImage(img)), Qt::DecorationRole);
    //    model->setData(child, QVariant(QPixmap::fromImage(img)), Qt::DecorationRole);
    //    if (!model->headerData(0, Qt::Horizontal).isValid())
    //        model->setHeaderData(0, Qt::Horizontal, QVariant("name"), Qt::EditRole);
    //}
   

    for (int column = 0; column < model->columnCount(index.parent()); ++column) {
        QModelIndex child = model->index(index.row() + 1, column, index.parent());
        model->setData(child, QVariant(name.c_str()), Qt::EditRole);
    }
    updateActions();

    //make json
    if (user->getType())
    {
        QClient* main = QClient::getInstance();
        int iduser = this->user->getID();
        int idparent = model->getIdForIndex(index.parent());
        nlohmann::json js = makeJsonNewNode(name, iduser, idparent, main->getNumberOfNodes(), photo);
        std::string mes = js.dump();
        sendNewNodeMessage(mes);
    }
    IncomingMessages();
}
void QClient::inservNewSubnode(const std::string photo, const std::string name)
{
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    if (!this->user->canNewNode())
        return;
    if (this->model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;

    for (int column = 0; column < model->columnCount(index); ++column) {
        QModelIndex child = model->index(0, column, index);
        model->setData(child, QVariant(name.c_str()), Qt::EditRole);
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[caini]"),
                Qt::EditRole);
    }
    ui.treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),
        QItemSelectionModel::ClearAndSelect);
    updateActions();


    //make json
    if (user->getType())
    {
        QClient* main = QClient::getInstance();
        int iduser = this->user->getID();
        int idparent = model->getIdForIndex(index);
        nlohmann::json js = makeJsonNewNode(name, iduser, idparent, main->getNumberOfNodes(),photo);
        std::string mes = js.dump();
        sendNewNodeMessage(mes);
    }
    IncomingMessages();
}

void QClient::updateActions()
{

    bool hasSelection = !ui.treeView->selectionModel()->selection().isEmpty();
    ui.actionDelete_Node->setEnabled(hasSelection);
    
    bool hasCurrent = ui.treeView->selectionModel()->currentIndex().isValid();
    ui.actionAdd_New_Node->setEnabled(hasCurrent);
    if (hasCurrent)
    {
        ui.treeView->closePersistentEditor(ui.treeView->selectionModel()->currentIndex());
        
        int row = ui.treeView->selectionModel()->currentIndex().row();
        int column = ui.treeView->selectionModel()->currentIndex().column();

        if (ui.treeView->selectionModel()->currentIndex().parent().isValid())
            statusBar()->showMessage(tr("Position:(%1,%2)").arg(row).arg(column));
        else
            statusBar()->showMessage(tr("Position: (%1,%2) in top level").arg(row).arg(column));

    }
}

void QClient::deleteNode()
{
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    QAbstractItemModel* model = ui.treeView->model();
    if (model->removeRow(index.row(), index.parent()))
        updateActions();
}


void QClient::on_actionExit_triggered()
{
    close();
}

void QClient::on_actionAdd_New_Node_triggered()
{
    NewNode *nn=new NewNode(1);
    nn->show();
}

void QClient::on_actionAdd_New_Subnode_triggered()
{
    NewNode* nn = new NewNode(2);
    nn->show();
}

void QClient::on_actionDelete_Node_triggered()
{
    deleteNode();
}


