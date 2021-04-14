#include "Find_Form.h"

Find_Form::Find_Form(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	save_information();
}

Find_Form::~Find_Form()
{
}

bool Find_Form::case_sensitive() const
{
	return m_case_sensitive ;
}

bool Find_Form::backwards() const
{
	return m_backwards;
}

bool Find_Form::whole_words() const
{
	return m_whole_words;
}

QString Find_Form::get_text() const
{
	return m_text;
}

void Find_Form::save_information()
{
	m_text = ui.lineEdit->text();
	m_backwards = ui.Backwards->isChecked();
	m_whole_words = ui.Whole_Word->isChecked();
	m_case_sensitive = ui.Case_Sensitive->isChecked();
}

void Find_Form::on_buttonBox_rejected()
{
	reject();
}
void Find_Form::on_buttonBox_accepted()
{
	save_information();
	accept();
}

