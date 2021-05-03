
#include <qmessagebox.h>
#include <nlohmann/json.hpp>
#include <qstring.h>

#include "ui_LoginForm.h"
#include "QClient.h"
#include "LoginForm.h"
#include "FirstForm.h"
#include "Common_function.h"
LoginForm* LoginForm::instance = nullptr;
LoginForm::LoginForm(QWidget* parent) :
	QDialog(parent),
	ui(new Ui::LoginForm)
{
	ui->setupUi(this);
	this->freeText();
}
LoginForm::~LoginForm()
{
	delete ui;
}

LoginForm* LoginForm::getInstance()
{
	if (!instance)
		instance = new LoginForm();
	return instance;
}

void LoginForm::deleteInstance()
{
	if (instance)
		delete instance;
}

void LoginForm::freeText()
{
	ui->emailText->setText("eg.email@gmail.com");
	ui->PasswordText->setText("password");
}

void LoginForm::on_closeButton_clicked()
{
	close();
	FirstForm* ff = FirstForm::getInstance();
	ff->show();
}
void LoginForm::on_LoginButton_clicked()
{
	nlohmann::json j;

	bool incorect = false;
	j["email"] = (ui->emailText->text()).toStdString();
	std::string ps = (ui->PasswordText->text()).toStdString();

	QClient* main = QClient::getInstance();
	if (!validateString(j["email"], StrType::Email))
	{
		QMessageBox::warning(main, "Client message", "The email has not a correct format");
		incorect = true;
	}
	if (!validateString(ps, StrType::Password))
	{
		QMessageBox::warning(main, "Client message", "The password has not a correct format");
		incorect = true;
	}
	//makeSecretPassword(ps);
	j["password"] = ps;
	if (!incorect)
	{
		std::string str = j.dump();
		main->sendLoginMessage(str);
		//asteapta un mesaj de confirmare, la primirea acestuia se afiseaza urmatoarea fereastra
		main->IncomingMessages();
	}
	if (ui->KeepLogged->isChecked())
	{
		//create a file with email and password
		createLoginFile(j["email"], j["password"]);
		main->IncomingMessages();
	}
}