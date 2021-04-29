#include "TreeItem.h"
#include <qstringlist.h>
#include "QClient.h"
TreeItem::TreeItem(const QVector<QVariant>& data, TreeItem* parent)
{
	m_parentItem = parent;
	m_itemData = data;
	//trash = false;
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
		item->setText("Bine ai venit wai!");
	}
	return true;
}

void TreeItem::insertChildrenLoad(int position, int id,int columns,std::string text, QVariant& name, QVariant& photoname)
{
	if (position<0 || position>m_childItems.size())
		return;
	{
		QVector<QVariant> data(columns);
		TreeItem* item = new TreeItem(data, this);
		m_childItems.insert(position, item);
		item->sedID(id);
		item->setPhoto(photoname);
		item->setData(0, name);
		item->setText(text);
		item->setOldParentNode(nullptr);
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
	//decrement nr of nodes
	QClient* main = QClient::getInstance();
	main->decrementNumberOfNodes();
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

void TreeItem::moveChildrenToParent()
{
	int size = this->childCount();
	for (int i=0;i<size;i++)
	{
		m_parentItem->addChild(m_childItems.front());
		m_childItems.front()->setParentGranny();
		m_childItems.pop_front();
	}
}

void TreeItem::moveChildrenFromParent()
{
	int size = parent()->childCount();
	for (int i = 0; i < size; i++)
	{
		TreeItem* child = parent()->child(i);
		if (child->getOldParent() == IDNode)
		{
			child->restoreOldParent();
			addChild(child);
			parent()->removeChild(child);
		}
	}

}

void TreeItem::restoreOldParent()
{
	m_parentItem = m_oldParentItem;
	m_oldParentItem = nullptr;
}

void TreeItem::setOldParentNode(TreeItem* node)
{
	m_oldParentItem = node;
}

void TreeItem::sedID(int id)
{
	IDNode = id;
}

int TreeItem::getID()
{
	return IDNode;
}
int TreeItem::getIDParent()
{
	return m_parentItem->getID();
}
bool TreeItem::setPhoto(QVariant photo)
{
	m_photoname = photo;
	return true;
}

QVariant TreeItem::getPhoto()
{
	return m_photoname;
}

void TreeItem::setText(std::string text)
{
	m_text = text;
}

std::string TreeItem::getText()
{
	return m_text;
}

bool TreeItem::isTrash()
{
	if (IDNode == 1)
		return true;
	if (m_parentItem->getID() == 1)
		return true;
	return false;
}

void TreeItem::addChild(TreeItem* child)
{
	m_childItems.push_back(child);
}

void TreeItem::removeChild(TreeItem* child)
{
	int i = child->childNumber();
	m_childItems.removeAt(i);
}

int TreeItem::getOldParent()
{
	if(m_oldParentItem)
		return m_oldParentItem->getID();
	return -1;
}

void TreeItem::setOldParent(TreeItem* parent)
{
	m_oldParentItem = m_parentItem;
	m_parentItem = parent;
}

void TreeItem::setParentGranny()
{
	m_oldParentItem = m_parentItem;
	m_parentItem = m_parentItem->parent();
}

bool TreeItem::canRecover()
{
	if (!m_oldParentItem)
		return false;
	if (m_oldParentItem->getID() == 1|| m_oldParentItem->getID()<0)
		return false;
	return true;
}

