#include "Table_Dialog.h"

Table_Dialog::Table_Dialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_columns_written = false;
	m_rows_written = false;
	ui.columns->setValidator(new QIntValidator(0, 100, this));
	ui.rows->setValidator(new QIntValidator(0, 100, this));
}

Table_Dialog::~Table_Dialog()
{
}

void Table_Dialog::save_information()
{
	if (!ui.columns->text().isEmpty())
	{
		m_nr_columns = ui.columns->text().toInt();
		m_columns_written = true;
	}
	if (!ui.rows->text().isEmpty())
	{
		m_nr_rows = ui.rows->text().toInt();
		m_rows_written = true;
	}
}

int Table_Dialog::get_columns()
{
	return m_nr_columns;
}

int Table_Dialog::get_rows()
{
	return m_nr_rows;
}

void Table_Dialog::on_buttonBox_rejected()
{
	reject();
}

void Table_Dialog::on_buttonBox_accepted()
{
	save_information();
	if (!m_rows_written && !m_columns_written)
	{
		QMessageBox::information(this, "Error", "You haven't written any value!");
		return;
	}
	if (!m_rows_written)
	{
		QMessageBox::information(this, "Error","You haven't written a value for the rows!");
		return;
	}
	if (!m_columns_written)
	{
		QMessageBox::information(this, "Error", "You haven't written a value for the columns!");
		return;
	}
	accept();
}
