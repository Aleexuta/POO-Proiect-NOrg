#include <nlohmann/json.hpp>

#include "UserForm.h"
#include "ui_UserForm.h"
#include "QClient.h"
#include "IUser.h"
#include "ChangePassForm.h"
UserForm* UserForm::instance = nullptr;
UserForm::UserForm(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::UserForm)
{

    ui->setupUi(this);


    QClient* main = QClient::getInstance();
    IUser* user = main->getUser();

    ui->label->setText(("Hello, " + user->getUsername()).c_str());

    QPixmap pixmap("../photos/panda.jpg");
    ui->photo->setPixmap(pixmap);
    ui->photo->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    ui->photo->setFixedSize(80,80);
    QRegion* region = new QRegion(0, 0, ui->photo->width(), ui->photo->height(), QRegion::Ellipse);
    ui->photo->setScaledContents(true);
    ui->photo->setMask(*region);

    connect(ui->logout, SIGNAL(clicked()), this, SLOT(on_logoutButton_clicked()));
    connect(ui->deleteAcc, SIGNAL(clicked()), this, SLOT(on_deleteAccButton_clicked()));
    connect(ui->changePass, SIGNAL(clicked()), this, SLOT(on_changePassButton_clicked()));
}
UserForm::~UserForm()
{
    delete ui;
}


UserForm* UserForm::getInstance()
{
    if (!instance)
        instance = new UserForm;
    return instance;
}
void UserForm::deleteInstance()
{
    if (instance)
        delete instance;
}

void UserForm::changePassword(std::string oldpass, std::string newpass)
{
    nlohmann::json js;
    js["oldpass"] = oldpass;
    js["newpass"] = newpass;
    QClient* main = QClient::getInstance();
    js["iduser"] = std::to_string(main->getUser()->getID());
    std::string mes = js.dump();
    main->sendChangePasswordMessage(mes);    
}

void UserForm::on_logoutButton_clicked()
{
    QClient* main = QClient::getInstance();
    main->logout();
    close();
}
void UserForm::on_deleteAccButton_clicked()
{
    //fa un string cu id-ul clietului
    QClient* main = QClient::getInstance();
    IUser * user= main->getUser();
    nlohmann::json js;
    js["iduser"] = std::to_string(user->getID());
    std::string mes = js.dump();
    main->sendDeleteAccountMessage(mes);
}
void UserForm::on_changePassButton_clicked()
{
    ChangePassForm* cpf = new ChangePassForm;
    cpf->show();

}
