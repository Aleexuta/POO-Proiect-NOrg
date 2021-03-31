#include <map>
#include <qmessagebox.h>
#include <nlohmann/json.hpp>
#include <qstring.h>

#include "RegisterForm.h"
#include "ui_RegisterForm.h"
#include "QClient.h"

RegisterForm* RegisterForm::instance = 0;
RegisterForm::RegisterForm(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::RegisterForm)
{
	ui->setupUi(this);
	QObject::connect(ui->RegisterButton, SIGNAL(clicked()), this, SLOT(on_RegisterButton_clicked()));
}
RegisterForm::~RegisterForm()
{
	delete ui;
}
RegisterForm* RegisterForm::getInstance()
{
	if (!instance)
		instance = new RegisterForm();
	return instance;
}

void RegisterForm::deleteInstance()
{
	if (instance)
		delete instance;
}

void RegisterForm::on_RegisterButton_clicked()
{
	nlohmann::json j;
	j["username"] = (ui->UsernameText->text()).toStdString();
	j["firstname"] = (ui->FirstnameText->text()).toStdString();
	j["lastname"] = (ui->LastnameText->text()).toStdString();
	j["email"] = (ui->EmailText->text()).toStdString();
	j["password"] = (ui->PasswordText->text()).toStdString();
	//se afiseaza fereastra principala din aplicatie
	QClient* main = QClient::getInstance();
	std::string str = j.dump();

	//QMessageBox::information(main, "debug", QString::fromStdString(j["email"]));

	main->sendRegisterMessage(str);
	//afiseaza doar daca primeste aprobare de la server


	//main->show();
	//close();
}