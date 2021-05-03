#pragma once
#ifndef CHANGEPASSFORM_H
#define CHANGEPASSFORM_H
 
#include <QDialog>
#include "ui_ChangePassForm.h"

class ChangePassForm : public QDialog
{
	Q_OBJECT

public:
	ChangePassForm(QWidget *parent=0);
	~ChangePassForm();
public slots:
	void on_CancelButton_clicked();
	void on_ConfirmButton_clicked();
private:
	Ui::Dialog* ui;
};
#endif // ! CHANGEPASSFORM_H