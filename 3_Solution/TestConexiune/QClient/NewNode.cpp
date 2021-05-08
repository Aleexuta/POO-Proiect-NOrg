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
	QClient* main = QClient::getInstance();
	if (main->getTheme() == ThemeClient::LightTheme)
		m_color.setRgb(0, 0, 0);
	else if (main->getTheme() == ThemeClient::DarkTheme)
		m_color.setRgb(255, 255, 255);
}
NewNode::~NewNode()
{
	delete ui;
}
void NewNode::on_colorSet_clicked()
{
	QColor current;
	QClient* main = QClient::getInstance();
	if (main->getTheme() == ThemeClient::LightTheme)
		current.setRgb(0, 0, 0);
	else if (main->getTheme() == ThemeClient::DarkTheme)
		current.setRgb(255, 255, 255);
		
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
	if (ui->attentionButton->isChecked())
	{
		photo = "../photos/attention.png";
	}
	if (ui->foodButton->isChecked())
	{
		photo = "../photos/food.png";
	}
	if (ui->bookButton->isChecked())
	{
		photo = "../photos/books.png";
	}
	if (ui->ideaButton->isChecked())
	{
		photo = "../photos/idea.png";
	}
	if (ui->locationButton->isChecked())
	{
		photo = "../photos/location.png";
	}
	if (ui->pinButton->isChecked())
	{
		photo = "../photos/pin.png";
	}
	if (ui->workButton->isChecked())
	{
		photo = "../photos/work.png";
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