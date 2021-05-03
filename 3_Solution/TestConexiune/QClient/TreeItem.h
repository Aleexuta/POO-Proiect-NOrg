#pragma once
#ifndef TREEITEM_H
#define TREEITEM_H
#include <QStandardItem>
#include <QObject>
#include <QList>
#include <QVariant>
#include <QVector>
class TreeItem: public QObject, public QStandardItem
{
	Q_OBJECT
public:
	TreeItem(const QVector<QVariant>& data, TreeItem* parent = 0);
	~TreeItem();

	TreeItem* child(int number);//returneaza copilul cu nr numar
	int childCount() const;
	int columnCount() const;
	QVariant data(int column)const;
	bool insertChildren(int position, int count, int columns);
	void insertChildrenLoad(int  position, int id, int columns,std::string text ,QVariant& name, 
		QVariant& photoname, QVariant& color, QVariant&font, std::string date);
	TreeItem* parent();
	bool removeChildren(int position, int count);
	int childNumber()const;
	bool setData(int column, const QVariant& value);

	void moveChildrenToParent(); //move the children to his father
	void addChild(TreeItem* child);
	void removeChild(TreeItem* child);

	void moveChildrenFromParent();//move the children to his old father
	void restoreOldParent();

	void setOldParentNode(TreeItem* node);
	void sedID(int id);
	int getID();
	int getIDParent();

	bool setPhoto(QVariant photo);
	QVariant getPhoto();
	bool setColor(QVariant color);
	QVariant getColor();
	void setText(std::string text);
	std::string getText();
	bool setFont(QVariant font);
	QVariant getFont();
	bool setDate(std::string date);
	std::string getDate();

	bool isTrash();

	int getOldParent();
	void setOldParent(TreeItem* parent);
	void setParentGranny();
	bool canRecover();

	void deleteForLogout();
private:
	int IDNode;//il punem din iuser
	QList<TreeItem*> m_childItems;
	QVector<QVariant> m_itemData;
	QVariant m_photoname;
	QVariant m_color;
	QVariant m_font;
	std::string m_date;
	TreeItem* m_parentItem;
	TreeItem* m_oldParentItem;
	//bool trash;
	std::string m_text;

};
#endif
