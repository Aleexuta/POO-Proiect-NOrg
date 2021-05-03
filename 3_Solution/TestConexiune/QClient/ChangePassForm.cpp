#include <qmessagebox.h>

#include "ChangePassForm.h"
#include "UserForm.h"
#include "Common_function.h"
ChangePassForm::ChangePassForm(QWidget *parent)
	: QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
	connect(ui->Confirm, SIGNAL(clicked()), this, SLOT(on_ConfirmButton_clicked()));
	connect(ui->Cancel, SIGNAL(clicked()), this, SLOT(on_CancelButton_clicked()));
}

ChangePassForm::~ChangePassForm()
{
	delete ui;
}

void ChangePassForm::on_ConfirmButton_clicked()
{
	UserForm* user = UserForm::getInstance();
	std::string newpass = ui->New->text().toStdString();
	std::string oldpass = ui->Old->text().toStdString();
	bool correct = true;
	if (!validateString(newpass, StrType::Password))
	{
		QMessageBox::warning(this, "Invalid password", "The new password does not have a correct format!");
		correct = false;
	}
	if (!validateString(oldpass, StrType::Password))
	{
		QMessageBox::warning(this, "Invalid password", "The current password is invalid!");
		correct = false;
	}
	if (newpass != ui->ConfirmPAss->text().toStdString())
	{
		QMessageBox::warning(this, "Invalid password", "The passwords does not match!");
		correct = false;
	}
	if (newpass == oldpass)
	{
		QMessageBox::warning(this, "Invalid password", "The current password and the new one are the same!");
		correct = false;
	}
	if (correct)
	{
		user->changePassword(oldpass, newpass);
		close();
	}
}

void ChangePassForm::on_CancelButton_clicked()
{
	close();

}
