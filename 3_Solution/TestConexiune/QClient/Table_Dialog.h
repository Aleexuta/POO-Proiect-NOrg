#pragma once

#include <QDialog>
#include "ui_Table_Dialog.h"
#include <qvalidator.h>
#include <qmessagebox.h>

class Table_Dialog : public QDialog
{
	Q_OBJECT

public:
	Table_Dialog(QWidget *parent = Q_NULLPTR);
	~Table_Dialog();
	int get_columns();
	int get_rows();

private:
	Ui::Table_Dialog ui;
	int m_nr_columns;
	int m_nr_rows;
	bool m_columns_written;
	bool m_rows_written;
	void save_information();
	
private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};
