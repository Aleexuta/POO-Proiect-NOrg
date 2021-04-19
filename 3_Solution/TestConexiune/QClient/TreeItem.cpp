#include "TreeItem.h"
#include <qstringlist.h>
#include "QClient.h"
TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
{
	m_parentItem = parent;
	m_itemData = data;
}

TreeItem::~TreeItem()
{
	qDeleteAll(m_childItems);
}

TreeItem* TreeItem::child(int number)
{
	return m_childItems.value(number);
}

int TreeItem::childCount() const
{
	return m_childItems.count();
}

int TreeItem::columnCount() const
{
	return m_itemData.count();
}

QVariant TreeItem::data(int column) const
{
	return m_itemData.value(column);
}

bool TreeItem::insertChildren(int position, int count, int columns)
{
	if (position<0 || position>m_childItems.size())
		return false;
	for (int i = 0; i < count; i++)
	{
		QVector<QVariant> data(columns);
		TreeItem* item = new TreeItem(data,this);
		m_childItems.insert(position, item);
		QClient* main = QClient::getInstance();
		item->sedID(main->getNumberOfNodes()+1);
		main->incrementNumberOfNodes();
	}
	return true;
}

void TreeItem::insertChildrenLoad(int position, int id,int columns, QVariant& name, QVariant& photoname)
{
	if (position<0 || position>m_childItems.size())
		return;
	{
		QVector<QVariant> data(columns);
		TreeItem* item = new TreeItem(data, this);
		m_childItems.insert(position, item);
		item->sedID(id);
		item->setData(0, photoname);
		item->setData(1, name);
	}
}

TreeItem* TreeItem::parent()
{
	return m_parentItem;
}

bool TreeItem::removeChildren(int position, int count)
{
	if (position<0 || position + count >m_childItems.size())
		return false;
	for (int row = 0; row < count; ++row)
	{
		delete m_childItems.takeAt(position);
	}
	return true;
}

int TreeItem::childNumber() const
{
	if (m_parentItem)
		return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));
	return 0;
}

bool TreeItem::setData(int column, const QVariant& value)
{
	if (column < 0 || column >= m_itemData.size())
		return false;
	m_itemData[column] = value;
	return true;
}

void TreeItem::sedID(int id)
{
	IDNode = id;
}

int TreeItem::getID()
{
	return IDNode;
}

