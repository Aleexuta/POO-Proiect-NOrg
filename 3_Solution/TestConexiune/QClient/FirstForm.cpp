#include "FirstForm.h"
#include "ui_FirstForm.h"
#include <qmessagebox.h>
#include "RegisterForm.h"
#include "LoginForm.h"
#include "QClient.h"
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
	QObject::connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
}
FirstForm::~FirstForm()
{
	delete ui;
}

void FirstForm::on_closeButton_clicked()
{
	close();
}

void FirstForm::on_pushButton_clicked()
{
	if (ui->RegisterButton->isChecked())
	{	
		RegisterForm* reg = RegisterForm::getInstance();
		reg->show();
		//close this one
		
	}
	if (ui->LoginButton->isChecked())
	{
		LoginForm* log = LoginForm::getInstance();
		log->show();
	}
	if (ui->GuestButton->isChecked())
	{
		//close and make guest user
	}
	close();
}
