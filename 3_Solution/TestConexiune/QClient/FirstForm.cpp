#include "FirstForm.h"
#include "ui_FirstForm.h"
#include <qmessagebox.h>
#include "RegisterForm.h"
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

void FirstForm::on_pushButton_clicked()
{
	if (ui->RegisterButton->isCheckable())
	{	
		RegisterForm* reg = RegisterForm::getInstance();
		reg->show();
		//close this one
		
	}
	if (ui->LoginButton->isCheckable())
	{
		//open login form
		//close this one
	}
	if (ui->GuestButton->isCheckable())
	{
		//close and make guest user
	}
	close();
}
