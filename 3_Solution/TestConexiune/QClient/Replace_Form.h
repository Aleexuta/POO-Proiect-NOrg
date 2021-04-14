#pragma once

#include <QDialog>
#include "ui_Replace_Form.h"
#include <QPushButton>
class Replace_Form : public QDialog
{
	Q_OBJECT

public:
	Replace_Form(QWidget *parent = Q_NULLPTR);
	~Replace_Form();
	QString get_findtext();
	QString get_replacetext();
	bool get_all();


private:
	Ui::Replace_Form ui;
	QString m_findtext;
	QString m_replacetext;
	bool m_all;
	void save_information();
private slots:
	void on_buttonBox_rejected();
	void on_Replace_clicked();
	void on_Replace_All_clicked();
};
