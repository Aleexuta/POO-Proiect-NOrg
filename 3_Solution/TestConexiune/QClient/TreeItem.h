#pragma once
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
class TreeItem
{

public:
	TreeItem(const QVector<QVariant>& data, TreeItem* parent = 0);
	~TreeItem();

	TreeItem* child(int number);//returneaza copilul cu nr numar
	int childCount() const;
	int columnCount() const;
	QVariant data(int column)const;
	bool insertChildren(int position, int count, int columns);
	void insertChildrenLoad(int  position, int id, int columns ,QVariant& name, QVariant& photoname);
	TreeItem* parent();
	bool removeChildren(int position, int count);
	int childNumber()const;
	bool setData(int column, const QVariant& value);

	void sedID(int id);
	int getID();

private:
	int IDNode;//il punem din iuser
	QList<TreeItem*> m_childItems;
	QVector<QVariant> m_itemData;
	TreeItem* m_parentItem;
	

};
#endif
