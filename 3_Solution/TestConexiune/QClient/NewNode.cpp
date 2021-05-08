#include "NewNode.h"
#include "ui_NewNode.h"
#include "QClient.h"
#include <qcolor.h>
NewNode::NewNode(int tip,QWidget* parent) :
	QDialog(parent),
	ui(new Ui::NewNode)
{
	ui->setupUi(this);
	m_tip = tip;
}
NewNode::~NewNode()
{
	delete ui;
}
void NewNode::on_colorSet_clicked()
{
	QColor current(0, 0, 0);
	QColor color = QColorDialog::getColor(current, this, "Select a color");
	m_color = color;
}
void NewNode::on_OKButton_clicked()
{
	//se deschide functa de creare nod
	std::string name= (ui->NameText->text()).toStdString();
	if (name == "")
		name = "ERORR NAME";
	std::string photo= "../photos/home.png";
	if (ui->homeButton->isChecked())
	{
		photo = "../photos/home.png";
	}
	if(ui->clockButton->isChecked())
	{
		photo = "../photos/clock.png";
	}
	if (ui->musicButton->isChecked())
	{
		photo = "../photos/music.png";
	}

//fa aici culoarea font si data
	QFont font = ui->fontComboBox->currentFont();
	QDate data = ui->dateEdit->date();
	//daca nu e selectata niciuna at se pune un default 
	QClient* main = QClient::getInstance();
	if (m_tip == 1)
		main->insertNewNode(photo , name,font, m_color,data);
	else
		main->inservNewSubnode(photo, name, font, m_color, data);
	
	close();
}