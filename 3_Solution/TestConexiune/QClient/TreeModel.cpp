#include <QtGui>
#include "treeitem.h"
#include "treemodel.h"
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
    if (role != Qt::DisplayRole && role != Qt::EditRole && role!=Qt::DecorationRole)
        return QVariant();

    TreeItem* item = getItem(index);
    return item->data(index.column());


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
    if (role == Qt::EditRole || Qt::DisplayRole || Qt::DecorationRole)
    {
        TreeItem* item = getItem(index);
        bool result = item->setData(index.column(), value);
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

TreeItem* TreeModel::getRootItem()
{
    return rootItem;
}

void TreeModel::setID(int id)
{
    this->rootItem->sedID(id);
}

int TreeModel::getIdForIndex(const QModelIndex& index)
{
    return getItem(index)->getID();
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

