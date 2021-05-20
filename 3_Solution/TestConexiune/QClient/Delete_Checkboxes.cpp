#include "Delete_Checkboxes.h"

Delete_Checkboxes::Delete_Checkboxes(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.lineEdit->setValidator(new QIntValidator(0, 100, this));
}

Delete_Checkboxes::~Delete_Checkboxes()
{
}

int Delete_Checkboxes::get_value()
{
	return m_valoare;
}

void Delete_Checkboxes::save_information()
{
	m_valoare=ui.lineEdit->text().toInt();
}

void Delete_Checkboxes::on_buttonBox_accepted()
{
	QClient* client = QClient::getInstance();
	save_information();
	if (client->m_checkboxes.size() < m_valoare)
	{
		QMessageBox::warning(this, "Invalid input", "Please insert a valid number!!");
		return;
	}
	else
	{
	accept();
}
}

void Delete_Checkboxes::on_buttonBox_rejected()
{
	reject();
}
