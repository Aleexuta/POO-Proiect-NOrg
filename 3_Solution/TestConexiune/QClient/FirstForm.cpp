#include "FirstForm.h"
#include "ui_FirstForm.h"
#include <qmessagebox.h>
#include "RegisterForm.h"
#include "LoginForm.h"
#include "QClient.h"
#include "Common_function.h"
FirstForm* FirstForm::instance = nullptr;
FirstForm* FirstForm::getInstance()
{
	if (!instance)
		instance = new FirstForm();
	return instance;
}

void FirstForm::deleteInstance()
{
	if (instance)
		delete instance;
}
FirstForm::FirstForm(QWidget* parent):
	QDialog(parent),
	ui(new Ui::FirstForm)
{
	ui->setupUi(this);
	this->setWindowIcon(QIcon("../photos/user.png"));
	this->setWindowTitle("Scribe");
}
FirstForm::~FirstForm()
{
	delete ui;
}

void FirstForm::on_closeButton_clicked()
{
	close();
}

void FirstForm::on_ConnectButton_clicked()
{
	QClient* main = QClient::getInstance();
	if (isAlreadyLogged())
	{//nu primeste mesaj inapoi
		LoginAutomatically();
		main->IncomingMessages();
	}
	else
	{
		FirstForm* f1 = FirstForm::getInstance();
		f1->show();
		main->IncomingMessages();
	}

}

void FirstForm::on_pushButton_clicked()
{
	if (ui->RegisterButton->isChecked())
	{	
		RegisterForm* reg = RegisterForm::getInstance();
		reg->show();
		//close this one
		close();
	}
	if (ui->LoginButton->isChecked())
	{
		LoginForm* log = LoginForm::getInstance();
		log->show();
		close();
	}
	if (ui->GuestButton->isChecked())
	{
		QClient* main = QClient::getInstance();
		main->setGuestInfo();
		main->show();
		close();
	}
	if (!ui->RegisterButton->isChecked() && !ui->LoginButton->isChecked() && !ui->GuestButton->isChecked())
	{
		QClient* w = QClient::getInstance();
		QMessageBox::warning(w, "Client message", "You must select a button to continue ");
	}
}
