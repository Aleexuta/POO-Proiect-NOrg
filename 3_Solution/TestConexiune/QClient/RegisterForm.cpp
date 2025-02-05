
#include <qmessagebox.h>
#include <nlohmann/json.hpp>
#include <qstring.h>
#include <qmessagebox.h>

#include "RegisterForm.h"
#include "ui_RegisterForm.h"
#include "QClient.h"
#include "FirstForm.h"
#include "Common_function.h"
RegisterForm* RegisterForm::instance = 0;
RegisterForm::RegisterForm(QWidget* parent):
	QDialog(parent),
	ui(new Ui::RegisterForm)
{
	ui->setupUi(this);
	this->freeText();
	this->setWindowIcon(QIcon("../photos/user.png"));
	this->setWindowTitle("Register");
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

void RegisterForm::freeText()
{
	ui->UsernameText->setText("eg.username");
	ui->FirstnameText->setText("eg.Ana Maria");
	ui->LastnameText->setText("eg.Popa");
	ui->EmailText->setText("eg.email@gmail.com");
	ui->PasswordText->setText("password");
	ui->ConfirmText->setText("password");
}

void RegisterForm::on_CloseButton_clicked()
{
	close();
	FirstForm* ff = FirstForm::getInstance();
	ff->show();
}

void RegisterForm::on_RegisterButton_clicked()
{
	nlohmann::json j;

	bool incorect = false;


	j["username"] = (ui->UsernameText->text()).toStdString();
	j["firstname"] = (ui->FirstnameText->text()).toStdString();
	j["lastname"] = (ui->LastnameText->text()).toStdString();
	j["email"] = (ui->EmailText->text()).toStdString();
	std::string ps = (ui->PasswordText->text()).toStdString();
	std::string ps2 = (ui->ConfirmText->text()).toStdString();
	QClient* main = QClient::getInstance();
	if (!validateString(j["email"], StrType::Email))
	{
		QMessageBox::warning(main, "client message", "The email has not a correct format");
		incorect = true;
	}
	if (!validateString(j["username"], StrType::Username))
	{
		QMessageBox::warning(main, "client message", "The username has not a correct format");
		incorect = true;
	}
	if (!validateString(ps, StrType::Password))
	{
		QMessageBox::warning(main, "client message", "The password has not a correct format");
		incorect = true;
	}
	if (ps != ps2)
	{
		QMessageBox::warning(main, "client message", "The passwords does not match");
		incorect = true;
	}
	//makeSecretPassword(ps);
	j["password"] = ps;
	if (!incorect)
	{
		std::string str = j.dump();
		main->sendRegisterMessage(str);
		//asteapta un mesaj de confirmare, la primirea acestuia se afiseaza urmatoarea fereastra
		main->IncomingMessages();
		//close();
		//QMessageBox::information(main, "client message", "totul este corect");
	}
}