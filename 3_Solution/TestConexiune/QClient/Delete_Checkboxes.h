#pragma once

#include <QDialog>
#include "ui_Delete_Checkboxes.h"
#include "QClient.h"
class Delete_Checkboxes : public QDialog
{
	Q_OBJECT

public:
	Delete_Checkboxes(QWidget *parent = Q_NULLPTR);
	~Delete_Checkboxes();
	int get_value();
private:
	Ui::Delete_Checkboxes ui;
	int m_valoare;
	void save_information();
private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};
