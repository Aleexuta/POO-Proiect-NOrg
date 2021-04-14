#include "Replace_Form.h"

Replace_Form::Replace_Form(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	save_information();
}

Replace_Form::~Replace_Form()
{
}

QString Replace_Form::get_findtext()
{
	return m_findtext;
}

QString Replace_Form::get_replacetext()
{
	return m_replacetext;
}

bool Replace_Form::get_all()
{
	return m_all;
}

void Replace_Form::save_information()
{
	m_findtext = ui.find->text();
	m_replacetext = ui.replace->text();
}

void Replace_Form::on_Replace_clicked()
{
	save_information();
	m_all = false;
	accept();
}

void Replace_Form::on_Replace_All_clicked()
{
	save_information();
	m_all = true;
	accept();
}

void Replace_Form::on_buttonBox_rejected()
{
	reject();
}