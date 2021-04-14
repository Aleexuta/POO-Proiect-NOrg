#pragma once

#include <QDialog>
#include "ui_Find_Form.h"
#include <QPushButton>
class Find_Form : public QDialog
{
	Q_OBJECT

public:
	Find_Form(QWidget *parent = Q_NULLPTR);
	~Find_Form();
	bool case_sensitive()const;
	bool backwards()const;
	bool whole_words()const;
	QString get_text()const;

private:
	Ui::Find_Form ui;
	bool m_case_sensitive;
	bool m_backwards;
	bool m_whole_words;
	QString m_text;
	void save_information();


private slots:
	void on_buttonBox_accepted();
	void on_buttonBox_rejected();
};
