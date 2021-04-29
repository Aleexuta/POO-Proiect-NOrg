#include <QtGui>
#include <qstring.h>
#include "treeitem.h"
#include "treemodel.h"
#include "qcolor.h"
#include "qbrush.h"
TreeModel* TreeModel::instance = 0;
TreeModel::TreeModel(const QStringList& headers, const QString& data, QObject* parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach(QString header, headers)
        rootData << header;

    rootItem = new TreeItem(rootData);
    setupModelData(data.split(QString(" ")), rootItem);

}

TreeModel::~TreeModel()
{
    delete rootItem;
}

TreeModel* TreeModel::getInstance(const QStringList& headers, const QString& data, QObject* parent)
{
    if (!instance)
        instance =new TreeModel(headers, data, parent);
    return instance;
}

TreeModel* TreeModel::getInstance()
{
    if (!instance)
        return nullptr;
    return instance;
}

void TreeModel::deleteInstance()
{
    if (instance)
        delete instance; 
}

QVariant TreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        TreeItem* item = getItem(index);
        return item->data(0);
    }
    if (role == Qt::DecorationRole)
    {
        TreeItem* item = getItem(index);
        return item->getPhoto();
    }
    if (role == Qt::ForegroundRole)
    {
        TreeItem* item = getItem(index);
        return item->getColor();
    }
    if (role == Qt::FontRole)
    {
        TreeItem* item = getItem(index);
        return item->getFont();
    }
    return QVariant();
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if(parent.isValid() && parent.column() !=0)
        return QModelIndex();
    TreeItem* parentItem = getItem(parent);
    TreeItem* childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex& index) const
{   
    if(!index.isValid())
        return QModelIndex();
    TreeItem* childItem = getItem(index);
    TreeItem* parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const
{
    TreeItem* parentItem = getItem(parent);
    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent) const
{
    return rootItem->columnCount();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool TreeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{

    if (role == Qt::EditRole || role == Qt::DisplayRole)
    {
        TreeItem* item = getItem(index);
        bool result = item->setData(0, value);
        if (result)
            emit dataChanged(index, index);
        return result;
    }
    if(role==Qt::DecorationRole)
    {
        TreeItem* item = getItem(index);
        bool result = item->setPhoto(value);
        if (result)
            emit dataChanged(index, index);
        return result;
    }
    if (role == Qt::ForegroundRole)
    {
        TreeItem* item = getItem(index);
        bool result = item->setColor(value);
        if (result)
            emit dataChanged(index, index);
        return result;
    }
    if (role == Qt::FontRole)
    {
        TreeItem* item = getItem(index);
        bool result = item->setFont(value);
        if (result)
            emit dataChanged(index, index);
        return result;
    }
    return false;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role)
{
    if (role == Qt::EditRole || orientation == Qt::Horizontal)
    {
        bool result = rootItem->setData(section, value);
        if (result)
            emit headerDataChanged(orientation, section, section);
        return result;
    }
    return false;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex& parent)
{
    

    TreeItem* parentItem = getItem(parent);
    bool succes;
    beginInsertRows(parent, position, position + rows - 1);
    succes = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();
    return succes;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex& parent)
{
    TreeItem* parentItem = getItem(parent);
    bool succes = true;

    beginRemoveRows(parent, position, position + rows - 1);
    succes = parentItem->removeChildren(position, rows);
    endRemoveRows();
    return succes;
}

std::string TreeModel::getAllChildren(std::string iduser,QModelIndex& parent)
{
    std::string mesaj = "[";
    TreeItem* item = getItem(parent);
    getChildren(iduser, item, mesaj);
    mesaj[mesaj.size() - 1] = ']';
    return mesaj;
}

TreeItem* TreeModel::getRootItem()
{
    return rootItem;
}

TreeItem* TreeModel::getNodeForId(int id)
{
    return findNode(rootItem, id);
}


void TreeModel::setID(int id)
{
    TreeItem* first = rootItem->child(0);
    first->sedID(id);
}

void TreeModel::setRootID(int id)
{
    rootItem->sedID(id);
}

int TreeModel::getIdForIndex(const QModelIndex& index)
{
    return getItem(index)->getID();
}

int TreeModel::getIdParentForIndex(const QModelIndex& index)
{
    return getItem(index)->getIDParent();
}

int TreeModel::getIdOldparentForIndex(const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    return item->getOldParent();
}

void TreeModel::setIDNode(int id, const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    item->sedID(id);
}

void TreeModel::setIcon(QIcon& icon, const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    item->setIcon(icon);
    emit dataChanged(index, index);
}

std::string TreeModel::getName(const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    QVariant var = item->data(0);
    return var.toString().toStdString();
}

std::string TreeModel::getText(const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    std::string text = item->getText();
    return text;
}

void TreeModel::setText(std::string text, const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    item->setText(text);
}

void TreeModel::setDate(QDate date, QModelIndex& index)
{
    TreeItem* item = getItem(index);
    item->setDate(date.toString().toStdString());
}

bool TreeModel::isTrash(const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    return item->isTrash();
}

void TreeModel::moveToTrash(const QModelIndex& index)
{
    //change the parent for the item with the trash(id node 1)
    //set oldparent 
    TreeItem* item = getItem(index);


    //move all the children(list) to granny
    item->moveChildrenToParent();
    TreeItem* trash = getTrashNode();

   
    item->parent()->removeChild(item);
    item->setOldParent(trash);
    trash->addChild(item);
    
}

bool TreeModel::moveFromTrash(const QModelIndex& index)
{
    TreeItem* item = getItem(index);
    bool x = item->canRecover();
    if (!x)
    {   
        return false;
    }

    TreeItem* trash = getTrashNode();
    trash->removeChild(item);
    item->restoreOldParent();
    item->moveChildrenFromParent();
    item->parent()->addChild(item);
    return true;
}

void TreeModel::getChildren(std::string iduser, TreeItem* parent, std::string& full)
{
    for (int i = 0; i < parent->childCount(); i++)
    {
        getChildren(iduser,parent->child(i), full);
    }
    int idnode = parent->getID();
    full += "{\"iduser\":\"" + iduser + "\",\"idnode\":\"" + std::to_string(idnode) + "\"},";
}

TreeItem* TreeModel::getTrashNode()
{
    for (int i = 0; i < rootItem->childCount(); i++)
    {
        if (rootItem->child(i)->getID() == 1)
            return rootItem->child(i);
    }
    //return rootItem->child(rootItem->childCount() - 1);
    return rootItem;
}

TreeItem* TreeModel::findNode(TreeItem* root, int id)
{
    if (root->getID() == id)
        return root;
    int size = root->childCount();
    for (int i = 0; i < size; i++)
    {
        TreeItem* rez = findNode(root->child(i), id);
        if (rez)
            return rez;

    }
    return nullptr;
   // return this->rootItem;

}






void TreeModel::setupModelData(const QStringList& lines, TreeItem* parent)
{
    QList<TreeItem*> parents;
    QList<int> indentation;
    parents << parent;
    indentation << 0;
    int number = 0;

    while (number < lines.count())
    {
        int position = 0;
        while (position < lines[number].length())
        {
            if (lines[number].mid(position, 1) != " ")
                break;
            position++;
        }

        QString lineData = lines[number].mid(position).trimmed();
        if (!lineData.isEmpty())
        {
            QStringList columnStrings = lineData.split(" ", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];
            if (position > indentation.last())
            {
                if (parents.last()->childCount() > 0)
                {
                    parents << parents.last()->child(parents.last()->childCount() - 1);
                    indentation << position;
                }
            }
            else
            {
                while (position < indentation.last() && parents.count()>0)
                {
                    parents.pop_back();
                    indentation.pop_back();
                }
            }
            TreeItem* parent = parents.last();
            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
        }
        number++;
    }
}

TreeItem* TreeModel::getItem(const QModelIndex& index) const
{
    if (index.isValid())
    {
        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

