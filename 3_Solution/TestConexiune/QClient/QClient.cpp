#include "QClient.h"
#include <QtGui>
#include <qmessagebox.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <qvariant.h>
#include <string>
#include <qicon.h>

#include "RegisterForm.h"
#include "LoginForm.h"
#include "FirstForm.h"
#include "NewNode.h"
#include"TreeModel.h"
#include "TreeItem.h"
#include "Common_function.h"
bool isloged = false;
bool isregistered = false;
QClient* QClient::instance = nullptr;
QClient::QClient(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    m_changed = false;
    m_path = "";
    QStringList headers;
    headers << tr("Name of notes");
    QFile file(":/default.txt");
    file.open(QIODevice::ReadOnly);
    this->model = TreeModel::getInstance(headers, file.readAll());
   // this->model = new TreeModel(headers, file.readAll());


    file.close();
    ui.treeView->setModel(model);

    ui.treeView->setColumnWidth(0,100);
    ui.treeView->setColumnWidth(1, 10);

    isloged = false;
    updateActions();
}
QClient:: ~QClient()
{
    
}
void QClient::closeEvent(QCloseEvent* event)
{
    checkSave();
    event->accept();
}
void QClient::checkSave()
{
    if (!m_changed)
        return;
    QMessageBox::StandardButton value = QMessageBox::question(this, "Save file", "You have unsaved changes.Do you want to save now?");
    if (value == QMessageBox::StandardButton::No)
        return;
    else
    {
        save();
    }
}
void QClient::save()
{
    if (!user->getType())
    {
        QString path = QFileDialog::getSaveFileName(this, "Save file");
        if (path.isEmpty())
        {
            return;
        }
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, "Error", file.errorString());
            ui.statusBar->showMessage("Error: could not save file!");
            return;
        }
        QTextStream stream(&file);
        stream << ui.textEdit->toHtml();
        file.close();

        m_path = path;
        ui.statusBar->showMessage(m_path);
        m_changed = false;
    }
    else
    {
        QModelIndex index = ui.treeView->selectionModel()->currentIndex();
        //memorie
        model->setText((ui.textEdit->toHtml()).toStdString(),index);

        //baza de data
       
        int iduser = user->getID();
        int idnode = model->getIdForIndex(index);
        nlohmann::json js;
        js["idnode"] = std::to_string(idnode);//nu ia id ul cum trebe
        js["iduser"] = std::to_string(iduser);
        std::string str= ui.textEdit->toHtml().toStdString();
        convertIntoTilda(str);
        js["text"] = str;
        std::string mes = js.dump();
        sendSaveNotesMessage(mes);
        IncomingMessages();
    }

}
void QClient::newFile()
{
    ui.textEdit->clear();
    ui.statusBar->showMessage("New File");
    m_changed = false;
    m_path = "";
}
void QClient::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File");
    if (path.isEmpty())
        return;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        return;
    }
    QTextStream stream(&file);
    ui.textEdit->setHtml(stream.readAll());
    file.close();
    m_path = path;
    ui.statusBar->showMessage(m_path);
    m_changed = false;
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
                    if (!isregistered)
                    {
                        isregistered = true;
                        QMessageBox::information(this, "Server Message", "Register Succes");
                        LoginForm* log = LoginForm::getInstance();
                        log->show();

                        RegisterForm* reg = RegisterForm::getInstance();
                        reg->close();
                        // reg->deleteInstance();
                    }
                }
                break;
                case CustomMsgTypes::ServerDenyRegister:
                {
                    if (!isregistered)
                    {
                        QMessageBox::warning(this, "Server Message", "Register Error");
                        RegisterForm* reg = RegisterForm::getInstance();
                        reg->show();
                    }}
                break;
                case CustomMsgTypes::ServerAcceptLogin:
                {
                    if (!isloged)
                    {
                        isloged = true;
                        QMessageBox::information(this, "Server Message", "Login Succes");
                        this->show();
                        LoginForm* reg = LoginForm::getInstance();
                        reg->close();
                        std::string rasp(msg.body.begin(), msg.body.end());
                        this->setUserInfo(rasp);
                        FirstForm* ff = FirstForm::getInstance();
                        ff->close();

                    }
                }
                break;
                case CustomMsgTypes::ServerDenyLogin:
                {
                    if (!isloged)
                    {
                        QMessageBox::warning(this, "Server Message", "Login Error");
                        LoginForm* reg = LoginForm::getInstance();
                        reg->show();
                        FirstForm* ff = FirstForm::getInstance();
                        ff->close();
                    }
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

void QClient::sendSaveNotesMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::SaveNode;
    char* vect = const_cast<char*>(j.c_str());
    for (int i = 0; i < j.size(); i++)
        msg << vect[i];
    Send(msg);
}

void QClient::sendMoveNodeTrash(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::MoveToTrashNode;
    char* vect = const_cast<char*>(j.c_str());
    for (int i = 0; i < j.size(); i++)
        msg << vect[i];
    Send(msg);
}

void QClient::sendRecoverNodeMessage(std::string j)
{
    olc::net::message<CustomMsgTypes> msg;
    msg.header.id = CustomMsgTypes::MoveFromTrashNode;
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
    this->model->setRootID(-1);
    this->makeMotherNode();
    updateActions();
    this->user->setNumberOfNodes(nr+1);
    //seteaza si load la toate nodurile
    //sendLoadAllNodesMessage(id);
    LoadAllNodes(mesaj);
    updateActions();
    IncomingMessages();

}

void QClient::setGuestInfo()
{
    this->user = new Guest;  
    this->model->setRootID(-1);
    this->makeMotherNode();
    //this->insertNewNode("../photos/trash.png", "Trash");
    updateActions();
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

void QClient::decrementNumberOfNodes()
{
    user->decrementNrNodes();
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


void QClient::LoadAllNodes(std::string j)
{
    auto js1 = nlohmann::json::parse(j);
    int pos = 1;
    LoadChildren(getRootItem(), js1, pos);
    LoadChildrenOldParent(getRootItem(), js1, pos);
    updateActions();
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
void QClient::LoadChildrenOldParent(TreeItem* root, nlohmann::basic_json<> js, int& pos)
{
    pos = 1;
    if (pos < js.size() && root!=model->getRootItem())
    {
        std::string js1 = js[pos]["idoldparent"];
        std::string noid = js[pos]["idnode"];
        while (pos < js.size())
        {

            if (js1!="NULL" && root->getID() == std::stoi(js1))
            {
                TreeItem* node = model->getNodeForId(std::stoi(noid));//nu merge functia asta
                node->setOldParentNode(root);
            }

            pos++;
            if (pos < js.size())
            {
                js1 = js[pos]["idoldparent"];
                noid = js[pos]["idnode"];
            }
        }
    }
    for (int i = 0; i < root->childCount(); i++)
    {
        LoadChildrenOldParent(root->child(i), js, pos);
    }
}
void QClient::prepareChildToInsert(TreeItem* root, nlohmann::basic_json<> js, int pos)
{
    auto st1 = js[pos];
    std::string idnode = st1["idnode"];
    std::string idparent = st1["idparent"];
    std::string name = st1["name"];
    std::string photoname = st1["photoname"];
    std::string text = st1["text"];
    std::string idoldparent = st1["idoldparent"];
    if (idnode == "1")
    {
        idparent = "-1";
    }
    convertFromTilda(text);
    root->insertChildrenLoad(0, std::stoi(idnode),NRCOL,text, QVariant(name.c_str()),QVariant(QIcon(photoname.c_str())));
    updateActions();
}

void QClient::moveNodeToTrash()
{
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    if (user->getType())//daca e user se fac modificarile si in baza de date
    {
        QClient* main = QClient::getInstance();
        int idnode = model->getIdForIndex(index);
        int iduser = user->getID();
        int idparent = model->getIdParentForIndex(index);
        nlohmann::json js;
        js["idnode"] = std::to_string(idnode);
        js["iduser"] = std::to_string(iduser);
        js["idparent"] = std::to_string(idparent);
        std::string mes = js.dump();
        sendMoveNodeTrash(mes);
        IncomingMessages();
    }

    model->moveToTrash(index);
    updateActions();
    updateActions();
}

void QClient::recoverNodeFromTrash()
{
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    //if (model->isTrash(index))//aici trebuie verificat daca old parent e trash
    //{
    //    QMessageBox::warning(this, "Recover Impossible", "This node can not be recovered because his sourse do not exist");
    //    return;
    //}
    //else
    {
        if (user->getType())
        {
            QClient* main = QClient::getInstance();
            int idnode = model->getIdForIndex(index);
            int iduser = user->getID();
            int idoldparent = model->getIdOldparentForIndex(index);
            nlohmann::json js;
            js["idnode"] = std::to_string(idnode);
            js["iduser"] = std::to_string(iduser);
            if (idoldparent < 0)
                idoldparent = 1;
            js["idoldparent"] = std::to_string(idoldparent);
            std::string mes = js.dump();
            sendRecoverNodeMessage(mes);
            IncomingMessages();

        }
        if (!model->moveFromTrash(index))
        {
            QMessageBox::warning(this, "Recover Failed", "The node can not be recovered because the location is invalid.");
        }
        updateActions();
    }
}

void QClient::makeMotherNode()
{
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    if (this->model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }
    {
        if (!model->insertRow(0, index))
            return;


        for (int column = 0; column < model->columnCount(index); ++column)
        {
            if (!model->headerData(column, Qt::Horizontal).isValid())
                model->setHeaderData(column, Qt::Horizontal, QVariant("[caini]"),
                    Qt::DisplayRole);

        }
        QModelIndex child = model->index(0, 0, index);
        model->setData(child, QVariant("Trash"), Qt::DisplayRole);
        QIcon icon("../photos/trash.png");


        model->setData(child, QVariant(icon), Qt::DecorationRole);
        model->setID(1);
    }
    {
        if (!model->insertRow(0, index))
            return;
        for (int column = 0; column < model->columnCount(index); ++column)
        {
            if (!model->headerData(column, Qt::Horizontal).isValid())
                model->setHeaderData(column, Qt::Horizontal, QVariant("[caini]"),
                    Qt::DisplayRole);

        }
        QModelIndex child = model->index(0, 0, index);
        model->setData(child, QVariant("MyNotes"), Qt::DisplayRole);
        QIcon icon("../photos/icon.png");
        model->setData(child, QVariant(icon), Qt::DecorationRole);
        model->setID(0);
    }
    ui.treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index), QItemSelectionModel::ClearAndSelect);
    updateActions();
}



void QClient::insertNewNode(const std::string photo, const std::string name)
{
    //daca e din cosul de gunoi sau cosul at nu se poate
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    if (!this->user->canNewNode())
    {
        QMessageBox::information(this, "client message", "You can't create more notes in guest mode");
        return;
    }

    if (!model->insertRow(index.row()+1, index.parent()))
        return;


    QModelIndex child = model->index(index.row()+1, 0, index.parent());
    model->setData(child, QVariant(name.c_str()), Qt::DisplayRole);
    QIcon icon(photo.c_str());
    model->setData(child, QVariant(icon), Qt::DecorationRole);

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
        IncomingMessages();
    }
   
}
void QClient::inservNewSubnode(const std::string photo, const std::string name)
{
    //daca e din cosul de gunoi sau e cosul at nu se poate

    QModelIndex index = ui.treeView->selectionModel()->currentIndex();

    if (!this->user->canNewNode())
    {
        QMessageBox::information(this, "client message", "You can't create more notes in guest mode");
        return;
    }
    if (this->model->columnCount(index) == 0) {
        if (!model->insertColumn(0, index))
            return;
    }

    if (!model->insertRow(0, index))
        return;


    for (int column = 0; column < model->columnCount(index); ++column)
    {
        if (!model->headerData(column, Qt::Horizontal).isValid())
            model->setHeaderData(column, Qt::Horizontal, QVariant("[caini]"),
                Qt::DisplayRole);

    }
    QModelIndex child = model->index(0, 0, index);
    model->setData(child, QVariant(name.c_str()), Qt::DisplayRole);
    QIcon icon(photo.c_str());
    model->setData(child, QVariant(icon), Qt::DecorationRole);

    ui.treeView->selectionModel()->setCurrentIndex(model->index(0, 0, index),QItemSelectionModel::ClearAndSelect);
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
        IncomingMessages();
    }
    
}

void QClient::updateActions()
{

    bool hasSelection = !ui.treeView->selectionModel()->selection().isEmpty();
    ui.actionDelete_Node->setEnabled(hasSelection);
    ui.actionRecover_Node->setEnabled(hasSelection);
    
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

    //verifica daca chiar vrea sa l stearga
    //verifica daca e din cosul de gunoi at face ce e mai jos
    if (user->getType())
    {
        QClient* main = QClient::getInstance();
        int idnode = model->getIdForIndex(index);
        int iduser = user->getID();
        std::string mes=model->getAllChildren(std::to_string(iduser), index);
        sendRemoveNodeMessage(mes);
        IncomingMessages();
    }

    if (model->removeRow(index.row(), index.parent()))
        updateActions();
    //daca nu e din cosul de gunoi face o mutare si un update in baza de date
}

void QClient::OpenNote()
{
    //vezi care e selectat
    //de la itemul selectat se ia textul din memorie si se trimite la editorul de text pt a l scrie pe ecran
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    std::string text = model->getText(index);
   // name += " hello";
   // QMessageBox::information(this, "test message", name.c_str());
    ui.textEdit->setText("");//merge?

    ui.textEdit->setHtml(text.c_str());

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
    QModelIndex index = ui.treeView->selectionModel()->currentIndex();
    if (!model->isTrash(index))
    {
        QMessageBox msbox;
        msbox.setText("Delete Node");
        msbox.setInformativeText("Do you want to delete the node and all subnodes permanently or move the node to Trash?");
        QAbstractButton* myDeleteButton = msbox.addButton("Delete permanently", QMessageBox::YesRole);
        QAbstractButton* myTrashButton = msbox.addButton("Move to Trash", QMessageBox::NoRole);
        QAbstractButton* myCancelButton = msbox.addButton("Cancel operation", QMessageBox::RejectRole);
        msbox.setIcon(QMessageBox::Question);
        msbox.exec();
        if (msbox.clickedButton() == myDeleteButton)
        {
            deleteNode();
        }
        if (msbox.clickedButton() == myTrashButton)
        {
            moveNodeToTrash();
        }
        if (msbox.clickedButton() == myCancelButton)
        {
            return;
        }
    }
    else
    {
        QMessageBox msbox;
        msbox.setText("Delete Node");
        msbox.setInformativeText("Do you want to delete the node permanently?");
        QAbstractButton* myDeleteButton = msbox.addButton("Delete permanently", QMessageBox::YesRole);
        QAbstractButton* myCancelButton = msbox.addButton("Cancel operation", QMessageBox::RejectRole);
        msbox.setIcon(QMessageBox::Question);
        msbox.exec();
        if (msbox.clickedButton() == myDeleteButton)
        {
            deleteNode();
        }
        if (msbox.clickedButton() == myCancelButton)
        {
            return;
        }
    }
}

void QClient::on_actionOpen_Note_triggered()
{
    OpenNote();
}

void QClient::on_actionRecover_Node_triggered()
{
    recoverNodeFromTrash();
}

void QClient::on_actionOpen_triggered()
{
    checkSave();
    openFile();
}

void QClient::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if (pDialog.exec() == QDialog::Rejected) {
        QMessageBox::warning(this, "Warning", "Cannot Access Printer");
        return;
    }
    ui.textEdit->print(&printer);
}

void QClient::on_actionExit_triggered()
{
    checkSave();
    QApplication::quit();
}

void QClient::on_actionSave_triggered()
{
    save();
}

void QClient::on_actionCopy_triggered()
{
    ui.textEdit->copy();
}

void QClient::on_textEdit_textChanged()
{
    m_changed = true;
}

void QClient::on_actionCut_triggered()
{
    ui.textEdit->cut();
}

void QClient::on_actionPaste_triggered()
{
    ui.textEdit->paste();
}

void QClient::on_actionUndo_triggered()
{
    ui.textEdit->undo();
}

void QClient::on_actionRedo_triggered()
{
    ui.textEdit->redo();
}

void QClient::on_actionZoom_in_triggered()
{
    ui.textEdit->zoomIn(5);
}

void QClient::on_actionZoom_out_triggered()
{
    ui.textEdit->zoomOut(5);
}

void QClient::on_actionSelect_All_triggered()
{
    ui.textEdit->selectAll();
}

void QClient::on_actionFind_triggered()
{
    Find_Form* frm = new Find_Form(this);
    if (!frm->exec())
        return;
    QTextDocument::FindFlags flags;
    if (frm->case_sensitive())
        flags = flags | QTextDocument::FindFlag::FindCaseSensitively;
    if (frm->whole_words())
        flags = flags | QTextDocument::FindFlag::FindWholeWords;
    if (frm->backwards())
        flags = flags | QTextDocument::FindFlag::FindBackward;
    bool value = ui.textEdit->find(frm->get_text(), flags);
    if (!value)
        QMessageBox::information(this, "Not found", "Was not able to find: " + frm->get_text());
}

void QClient::on_actionReplace_triggered()
{
    Replace_Form* frm = new Replace_Form(this);
    if (!frm->exec())
        return;
    if (frm->get_all())
    {
      //Replace All
        QString text = ui.textEdit->toHtml();
        text = text.replace(frm->get_findtext(), frm->get_replacetext());
        ui.textEdit->setHtml(text);
    }
    else 
    {
        //Replace one
        bool value = ui.textEdit->find(frm->get_findtext());
        QTextCursor cursor = ui.textEdit->textCursor();
        cursor.insertHtml(frm->get_replacetext());
        if (!value)
            QMessageBox::information(this, "Not found", "Was not able to find: " + frm->get_findtext());
    }
}

void QClient::on_actionBold_triggered()
{
    QFont font = ui.textEdit->currentFont();
    if (font.bold())
        font.setBold(false);
    else font.setBold(true);
    ui.textEdit->setCurrentFont(font);
}

void QClient::on_actionItalic_triggered()
{
    QFont font = ui.textEdit->currentFont();
    if (font.italic())
        font.setItalic(false);
    else font.setItalic(true);
    ui.textEdit->setCurrentFont(font);
}

void QClient::on_actionStrikeout_triggered()
{
    QFont font = ui.textEdit->currentFont();
    if (font.strikeOut())
        font.setStrikeOut(false);
    else font.setStrikeOut(true);
    ui.textEdit->setCurrentFont(font);
}

void QClient::on_actionUnderline_triggered()
{
    QFont font = ui.textEdit->currentFont();
    if (font.underline())
        font.setUnderline(false);
    else font.setUnderline(true);
    ui.textEdit->setCurrentFont(font);
}

void QClient::on_actionColor_triggered()
{
    QColor current = ui.textEdit->currentCharFormat().foreground().color();
    QColor color = QColorDialog::getColor(current, this, "Select a color");
    ui.textEdit->setTextColor(color);
}

void QClient::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui.textEdit->currentFont(), this, "Select a font");
    if (ok)
        ui.textEdit->setFont(font);
}

void QClient::on_actionNew_triggered()
{
    checkSave();
    newFile();
}

